#include <avr/wdt.h>
#include "common.h"
#include "motor.h"
#include "uart.h"
#include "sensors.h"
#include "panel.h"

#define STATE_AUTO   0x00
#define STATE_MAN    0x01
uint8_t state;               //automatic/manual

int16_t height;             //actual height of door in motor rotations

uint8_t pressed_buttons;    //1 means pressed for longer than ~20ms
uint8_t released_buttons;   //0 means not pressed within ~20ms

void debounce_buttons()
//debounces by setting *_buttons only if buttons were/weren't pressed 3 calls
//in a row
{
    uint8_t buttons;
    static uint8_t button_hist[3];
    uint8_t i;

    buttons = motor_rotation_pressed();
    buttons |= manual_enable_pressed() << 1;
    buttons |= manual_up_pressed() << 2;
    buttons |= manual_down_pressed() << 3;

    button_hist[0] = button_hist[1];
    button_hist[1] = button_hist[2];
    button_hist[2] = buttons;

    //find buttons which were 0 the last 3 times
    for(i = 0; i < 2; i++)
    {
        buttons |= button_hist[i];
    }
    //only where bit == 0, the last 4 times were 0
    released_buttons = buttons;

    //find buttons which were 1 the last 3 times
    buttons = button_hist[2];
    for(i = 0; i < 2; i++)
    {
        buttons &= button_hist[i];
    }
    pressed_buttons = buttons;
    //only where bit == 1, the last 4 times were 1
}

void next_state()
{
    if(testbit(pressed_buttons, 1))
    {
        //manual enable pressed
        state = STATE_MAN;
    }
    else if(testbit(released_buttons, 1) == 0)
    {
        //manual enable not pressed
        state = STATE_AUTO;
    }
}

void process_buttons()
{
    static uint8_t prev_rotation_pressed;

    if(testbit(released_buttons, 0) == 0)
    {
        //motor rotation not pressed
        prev_rotation_pressed = 0;
    }

    if(testbit(pressed_buttons, 0))
    {
        //motor rotation pressed
        if(!prev_rotation_pressed)
        {
            switch(motor_get_direction())
            {
                case 1:
                    height++;
                    break;
                case 2:
                    height--;
                    break;
            }
        }
        prev_rotation_pressed = 1;
    }

    //process panel buttons
    if(state == STATE_MAN)
    {
        uint8_t up_pressed = testbit(pressed_buttons, 2);
        uint8_t down_pressed = testbit(pressed_buttons, 3);
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
    uint8_t feeding_period;     //wether feeding or waiting for feeding

    if(ovf_counter++ < 3)   //return 3 out of 4 times, act only every 8ms
    {
        return;
    }

    debounce_buttons();
    next_state();
    process_buttons();

    if(state == STATE_AUTO)
    {
        if(feeding_period)
        {

        }
    }

    ovf_counter = 0;
    return;
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
