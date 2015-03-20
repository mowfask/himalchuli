#include "motor.h"

//Current state of outputs:
//0:    low
//1:    high
uint8_t M1_state;
uint8_t M2_state;

void motor_init()
{
    //Motors as outputs
    setbit(DDR_M1, DDM1);
    setbit(DDR_M2, DDM2);

    M1_state = 0;
    M2_state = 0;
}

void M1_low()
{
    clearbit(PORT_M1, PM1);
    M1_state = 0;
}

void M1_high()
{
    setbit(PORT_M1, PM1);
    M1_state = 1;
}

void M1_toggle()
{
    if(M1_state == 1)
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
    clearbit(PORT_M2, PM2);
    M2_state = 0;
}

void M2_high()
{
    setbit(PORT_M2, PM2);
    M2_state = 1;
}

void M2_toggle()
{
    if(M2_state == 1)
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
