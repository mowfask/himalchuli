#include "common.h"
#include "motor.h"


//Pin definitions


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
    motor_init();
    uart_init();

    //everything is set up, globally enable interrupts
    //sei();
}

int main(void)
{
    init();

    char c;

    puts("Calfdoor v. 0.1");

    while(1)
    {
        c = getchar();
        switch(c)
        {
            case 'l':
                puts("Motor left");
                motor_left();
                break;
            case 'r':
                puts("Motor right");
                motor_right();
                break;
            case 's':
                puts("Motor stop");
                motor_stop();
                break;
            case '1':
                puts("Toggle M1");
                M1_toggle();
                break;
            case '2':
                puts("Toggle M2");
                M2_toggle();
                break;
        }
    }
}
