#include <avr/io.h>
#include "common.h"


//Pin definitions
//Upper motor connector
#define PORT_M1 PORTB
#define PIOM1   PB2
#define DDR_M1  DDRB
#define DDM1    DDB2

//Lower motor connector
#define PORT_M2 PORTB
#define PIOM2   PB1
#define DDR_M2  DDRB
#define DDM2    DDB1


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

void init(void) {
    //Motors as outputs
    setbit(DDR_M1, DDM1);
    setbit(DDR_M2, DDM2);

    uart_init();

    //everything is set up, globally enable interrupts
    //sei();
}

void M1_low()
{
    //Set upper connector to 0 volts
    //There are two mosfets between the pin and the connector
    clearbit(PORT_M1, PIOM1);
}

void M1_high()
{
    //Set upper connector to 12 volts
    //There are two mosfets between the pin and the connector
    setbit(PORT_M1, PIOM1);
}

void M2_low()
{
    //Set lower connector to 0 volts
    //There are two mosfets between the pin and the connector
    clearbit(PORT_M2, PIOM2);
}

void M2_high()
{
    //Set lower connector to 12 volts
    //There are two mosfets between the pin and the connector
    setbit(PORT_M2, PIOM2);
}

void motor_left()
{
    M2_low();
    _delay_ms(40);
    M1_high();
}

void motor_right()
{
    M1_low();
    _delay_ms(40);
    M2_high();
}

void motor_stop()
{
    M1_low();
    M2_low();
}

int main(void)
{
    init();

    char c;

    while(1)
    {
        c = getchar();
        putchar(c);
    }
}
