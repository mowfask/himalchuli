#include <avr/wdt.h>
#include "common.h"
#include "motor.h"
#include "uart.h"
#include "sensors.h"
#include "panel.h"

#define PROBLEM_TIMER_MAX   2500    //If there are PROBLEMS_FOR_ERROR problems
                                    //happening within this period of time, go
                                    //into error state. Note that this period
                                    //is counted from the first problem that
                                    //occurred.
#define PROBLEMS_FOR_ERROR  2       //how often a problem has to occur to go
                                    //into error state
#define PROBLEM_TRY_AGAIN   625     //how long after a problem should we try
                                    //again? Should be smaller than
                                    //PROBLEM_TIMER_MAX.
#define DOOR_MIN_CLOSED     625     //If door is (being) closed, don't open
                                    //it for at least 5 seconds

#define DIST_THRES  200 //at which distance a calf is assumed to be there
#define HEIGHT_OPEN 200 //height at which door is assumed to be open

#define STATE_AUTO  0x00
#define STATE_MAN   0x01
#define STATE_ERROR 0x02

uint8_t state;          //automatic/manual/error

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

uint8_t buttons_deb;        //debounced buttons, 1 means pressed
uint8_t buttons_deb_diff;   //debounced buttons change, 1 means button changed
                            //state after last debouncing

uint16_t timer;             //Timer counting in 8ms steps. This is enough for
                            //about 8.5 minutes. Declaration in common.h

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
            continue;   //don't reset counter
        }
        else if(testbit(buttons_deb, i) != button_funcs[i]())
        {
            //the counter has expired and the button has toggled, toggle the
            //appropriate bit in buttons_deb
            togglebit(buttons_deb, i);
            //set the appropriate bit in buttons_deb_diff
            setbit(buttons_deb_diff, i);

        }
        else
        {
            //button didn't toggle
            clearbit(buttons_deb_diff, i);
        }
        //reset counter
        button_count[i] = 3;
    }
}

static void next_state()
{
    if(state == STATE_ERROR)
    {
        //can go to manual from error, so pressig the manual enable button
        //once gets the state out of error.
        state = testbit(buttons_deb, BUTTONS_ME) ? STATE_MAN : STATE_ERROR;
    }
    else
    {
        state = testbit(buttons_deb, BUTTONS_ME) ? STATE_MAN : STATE_AUTO;
    }
}

void process_buttons()
{
    static uint8_t prev_rotation = 0;

    if(testbit(buttons_deb, BUTTONS_MR) && !prev_rotation)
    {
        //motor rotation button was pressed (went from 0 to 1)
        motor_reset_timer();    //motor is turning, no problem
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
    uint8_t distance;
    uint8_t motor_state;

    static uint16_t problem_wait_cd = 0;    //countdown for waiting some time
                                            //after a problem before trying
                                            //again
    static uint16_t open_timer;     //for waiting 5 seconds before opening
                                    //door
    static uint16_t problem_timer = 0;  //to measure time between problems. If
                                        //too many occur, go into error state.
    static uint8_t problem_counter = 0; //how many problems have occurred

    if(ovf_counter++ < 3)   //return 3 out of 4 times, act only every 8ms
    {
        return;
    }
    //else
    ovf_counter = 0;

    timer++;

    //if we're waiting before trying again after a problem, don't to anything.
    if(problem_wait_cd != 0)
    {
        problem_wait_cd--;
        return;
    }

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

        motor_state = MOTOR_OK;

        distance = distance_sense();
        if(distance < DIST_THRES) //calf is there
        {
            //Reset timer
            open_timer = timer;
        }

        if(timer-open_timer <= DOOR_MIN_CLOSED)
        //calf is there or was there less than 5 seconds ago
        {
            if(height > 0)          //if the door isn't down yet, close it
            {
                motor_state = motor_down();
            }
            else
            {
                //motor_state stays MOTOR_OK
                motor_stop();
            }
        }
        else    //no calf there for at least 5 seconds
        {
            if(height < HEIGHT_OPEN)    //if the door isn't up yet, open it
            {
                motor_state = motor_up();
            }
            else
            {
                //motor_state stays MOTOR_OK
                motor_stop();
            }
        }

        if(problem_counter > 0) //if there has been a problem
        {
            if(timer-problem_timer > PROBLEM_TIMER_MAX)
            {
                //problem occurred too long ago, dismiss it.
                problem_counter = 0;
            }
        }

        if(motor_state == MOTOR_ERROR)
        {
            //Motor hasn't turned although on for five seconds. Motor has
            //stopped
            if(problem_counter == 0)
            {
                //There hasn't been a problem for at least PROBLEM_TIMER_MAX
                problem_timer = timer;
            }
            if(++problem_counter == PROBLEMS_FOR_ERROR)
            {
                state = STATE_ERROR;
                return; //we'll handle the error state in the next run (8ms
                        //from now)
            }
            else
            {
                //We wait PROBLEM_TRY_AGAIN before continuing
                problem_wait_cd = PROBLEM_TRY_AGAIN;
                return;
            }
        }
    }
    else if(state == STATE_ERROR)
    {
        //maybe blink some LED?
        return;
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
