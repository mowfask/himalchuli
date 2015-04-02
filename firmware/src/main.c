#include <avr/wdt.h>
#include "common.h"
#include "motor.h"
#include "uart.h"
#include "sensors.h"
#include "panel.h"

#define MODE_AUTO   0x00
#define MODE_MAN    0x01
uint8_t mode;               //automatic/manual

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

void process_buttons()
{
    if(testbit(released_buttons, 0) == 0)
    {
        //motor rotation not pressed
    }

    if(testbit(pressed_buttons, 0))
    {
        //motor rotation pressed
    }

    //process panel buttons
    if(testbit(pressed_buttons, 1))
    {
        //manual enable pressed
        mode = MODE_MAN;
        if(testbit(pressed_buttons, 2))
        {
            //manual up pressed
            motor_up();
        }
        else if(testbit(pressed_buttons, 3))
        {
            //manual down pressed
            motor_down();
        }
        else if(testbit(released_buttons, 2) == 0 &&
                testbit(released_buttons, 3) == 0)
        {
            //both manual down and manual up released
            motor_stop();
        }
    }
    else if(testbit(released_buttons, 1))
    {
        //manual enable not pressed
        if(mode == MODE_MAN)
        {
            motor_stop();
            mode = MODE_AUTO;

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

    debounce_buttons();
    process_buttons();


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
