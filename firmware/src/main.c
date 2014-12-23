#include <avr/io.h>
#include "common.h"


//Pin definitions
//Upper motor connector high (12V) side
#define PORT_M1H PORTB
#define PM1H     PB2
#define DDR_M1H  DDRB
#define DDM1H    DDB2

//Upper motor connector low (GND) side
#define PORT_M1L PORTD
#define PM1L     PD7
#define DDR_M1L  DDRD
#define DDM1L    DDD7

//Lower motor connector high (12V) side
#define PORT_M2H PORTB
#define PM2H     PB1
#define DDR_M2H  DDRB
#define DDM2H    DDB1

//Lower motor connector low (GND) side
#define PORT_M2L PORTB
#define PM2L     PB0
#define DDR_M2L  DDRB
#define DDM2L    DDB0


//Current state of outputs:
uint8_t M1_state;
uint8_t M2_state;


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
    setbit(DDR_M1H, DDM1H);
    setbit(DDR_M1L, DDM1L);
    setbit(DDR_M2H, DDM2H);
    setbit(DDR_M2L, DDM2L);

    setbit(PORT_M1L, PM1L);     //Mosfet on
    clearbit(PORT_M1H, PM1H);   //Mosfet off
    M1_state = 0;

    setbit(PORT_M2L, PM2L);     //Mosfet on
    clearbit(PORT_M2H, PM2H);   //Mosfet off
    M2_state = 0;

    uart_init();

    //everything is set up, globally enable interrupts
    //sei();
}

/*Always use the following functions to set the levels of the output
 *connector!!
 */

void M1_low()
{
    //Set upper connector to 0 volts
    //First: make sure it's not set to 12V
    clearbit(PORT_M1H, PM1H);
    //Wait for the motor to stop
    _delay_ms(40);
    //Then connect it to ground
    setbit(PORT_M1L, PM1L);
    M1_state = 0;
}

void M1_high()
{
    //Set upper connector to 12 volts
    //First: make sure it's not set to GND
    clearbit(PORT_M1L, PM1L);
    //Wait for the motor to stop
    _delay_ms(40);
    //Then connect it to 12V
    setbit(PORT_M1H, PM1H);
    M1_state = 1;
}

void M1_toggle()
{
    if(M1_state)
    {
        M1_low();
    }
    else
    {
        M1_high();
    }
}

void M2_low()
{
    //Set lower connector to 0 volts
    //First: make sure it's not set to 12V
    clearbit(PORT_M2H, PM2H);
    //Wait for the motor to stop
    _delay_ms(40);
    //Then connect it to ground
    setbit(PORT_M2L, PM2L);
    M2_state = 0;
}

void M2_high()
{
    //Set lower connector to 12 volts
    //First: make sure it's not set to GND
    clearbit(PORT_M2L, PM2L);
    //Wait for the motor to stop
    _delay_ms(40);
    //Then connect it to 12V
    setbit(PORT_M2H, PM2H);
    M2_state = 1;
}

void M2_toggle()
{
    if(M2_state)
    {
        M2_low();
    }
    else
    {
        M2_high();
    }
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
