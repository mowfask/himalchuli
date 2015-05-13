#include <avr/wdt.h>
#include "common.h"
#include "motor.h"
#include "uart.h"
#include "sensors.h"
#include "panel.h"

#define STATE_AUTO  0x00
#define STATE_MAN   0x01
uint8_t state;          //automatic/manual

int16_t height;         //actual height of door in motor rotations

//Number of buttons and their bit positions
#define NBR_BUTTONS 4   //number of buttons
#define BUTTONS_MR  0   //motor rotation
#define BUTTONS_ME  1   //manual enable
#define BUTTONS_MU  2   //manual up
#define BUTTONS_MD  3   //manual down

//functions to check button states
uint8_t (*const button_funcs[NBR_BUTTONS])() = {motor_rotation_pressed,
                                                manual_enable_pressed,
                                                manual_up_pressed,
                                                manual_down_pressed};

uint8_t buttons_deb;    //debounced buttons, 1 means pressed

void debounce_buttons()
//debounce buttons by changing bits of buttons_deb maximally every 3rd call
//(about 25ms)
{
    static uint8_t button_count[NBR_BUTTONS];
    uint8_t i;

    //Loop over all buttons. If its counter is 0, test whether it changed
    //state.
    for(i = 0; i < NBR_BUTTONS; ++i)
    {
        if(button_count[i] > 0)
        {
            button_count[i]--;
        }
        else if(testbit(buttons_deb, i) != button_funcs[i]())
        {
            //the counter has expired and the button has toggled, toggle the
            //appropriate bit in buttons_deb
            togglebit(buttons_deb, i);
            //reset counter
            button_count[i] = 3;
        }
    }
}

static void next_state()
{
    state = testbit(buttons_deb, BUTTONS_ME) ? STATE_MAN : STATE_AUTO;
}

void process_buttons()
{
    static uint8_t prev_rotation = 0;

    if(testbit(buttons_deb, BUTTONS_MR) && !prev_rotation)
    {
        //motor rotation button was pressed (went from 0 to 1)
        switch(motor_get_direction())
        {
            case MOTOR_UP:
                height++;
                break;
            default:        //If the motor is not running, the winch is
                height--;   //probably unwinding and the door is closing.
                break;
        }
    }

    prev_rotation = testbit(buttons_deb, BUTTONS_MR);


    //process panel buttons
    if(state == STATE_MAN)
    {
        uint8_t up_pressed = testbit(buttons_deb, BUTTONS_MU);
        uint8_t down_pressed = testbit(buttons_deb, BUTTONS_MD);
        if(up_pressed && down_pressed)
        {
            motor_stop();
            height = 0;
        }
        else if(up_pressed)
        {
            motor_up();
        }
        else if(down_pressed)
        {
            motor_down();
        }
        else
        {
            motor_stop();
        }
    }
}

ISR(TIMER0_OVF_vect)
{
    static uint8_t ovf_counter;

    if(ovf_counter++ < 3)   //return 3 out of 4 times, act only every 8ms
    {
        return;
    }
    //else
    ovf_counter = 0;

    debounce_buttons();
    next_state();
    process_buttons();

    if(state == STATE_AUTO)
    {
        /*If distance is smaller than threshold (calf is there): close door.
         *If distance is bigger than threshold for 5 seconds: open door.
         *
         *Conditions of a problem while opening or closing:
         *  The rotation button is not pressed for 4 seconds or the current
         *  exeeds the current threshold for 4 seconds.
         *In case that happens:
         *  Stop the motor and wait for 10 seconds, then try again. If the
         *  problem occurs again, go into error state.
         */
    }
}

void timer_init()
{
    //factor 8 clock prescaler -> act on every 4th interrupt
    clearbit(TCCR0, CS00);
    setbit(TCCR0, CS01);
    clearbit(TCCR0, CS02);
    //enable interrupt on overflow
    setbit(TIMSK, TOIE0);
}

static void init(void) {
    uart_init();
    motor_init();
    panel_init();
    sensors_init();
    timer_init();

    //everything is set up, globally enable interrupts
    sei();
}

int main(void)
{
    init();

    char c;

    puts("Calfdoor v. 0.2");

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
            case 'x':
                //software reset: start watchdog and wait...
                wdt_enable(WDTO_15MS);  //15ms
                while(1)
                {
                }
        }
    }
}
