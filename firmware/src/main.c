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
    uint8_t pwm_state = 0;
    uint8_t pwm_duty = 0;

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
            case 'p':
                if(pwm_state == 0)
                {
                    puts("PWM: on");
                    pwm_state = 1;
                    motor_pwm_start(0, pwm_duty);
                }
                else
                {
                    puts("PMW: off");
                    pwm_state = 0;
                    motor_pwm_stop();
                }
                break;
            case '+':
                if(pwm_state == 0)
                {
                    puts("PWM is off!");
                }
                else
                {
                    pwm_duty += 5;
                    motor_pwm_set(pwm_duty);
                }
                break;
            case '-':
                if(pwm_state == 0)
                {
                    puts("PWM is off!");
                }
                else
                {
                    pwm_duty -= 5;
                    motor_pwm_set(pwm_duty);
                }
                break;
        }
    }
}
