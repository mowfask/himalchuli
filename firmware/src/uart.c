/*
 * This file is part of himalchuli, a controller for a calf protection
 * mechanism.
 *
 * Copyright (C) 2015 J. Deitmerg <mowfask@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "uart.h"

//Like the glibc example
int uart_putchar(char c, FILE* stream)
{
    if (c == '\n')
    {
        uart_putchar('\r', stream);
    }
    loop_until_bit_is_set(UCSRA, UDRE);
    UDR = c;
    return 0;
}

int uart_getchar(FILE *stream)
{
    loop_until_bit_is_set(UCSRA, RXC);
    return UDR;
}

static FILE uart_stream = FDEV_SETUP_STREAM(uart_putchar, uart_getchar,
                                            _FDEV_SETUP_RW);

void uart_init(void)
{
    //also pretty much what the glibc pages tell you to do
    UBRRH = UBRRH_VALUE;
    UBRRL = UBRRL_VALUE;
    #if USE_2X
        UCSRA |= (1 << U2X);
    #else
        UCSRA &= ~(1 << U2X);
    #endif

    UCSRB |= (1<<TXEN);     //enable UART TX
    UCSRC = (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);   // asynchronous 8N1
    UCSRB |= (1<<RXEN);     //enable UART RX

    stdout = &uart_stream;
    stdin = &uart_stream;
}
