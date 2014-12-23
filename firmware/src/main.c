#include "common.h"
#include "motor.h"
#include "uart.h"


//Pin definitions

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
