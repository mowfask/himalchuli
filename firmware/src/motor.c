#include "motor.h"

//Current state of outputs:
//0:    low
//1:    high
uint8_t M1_state;
uint8_t M2_state;

uint8_t motor_direction;    //0: stopped, 1: right, 2: left

uint16_t motor_timer;       //Counting how long motor is running in one
                            //direction. motor_reset_timer() resets this.
                            //It is used to count how long the motor is
                            //on without turning.

void motor_init()
{
    //Motors as outputs
    setbit(DDR_M1, DDM1);
    setbit(DDR_M2, DDM2);

    M1_state = 0;
    M2_state = 0;
    motor_direction = 0;
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

uint8_t motor_left()
{
    if(motor_direction != MOTOR_LEFT)
    {
        //reset timer at change of direction
        motor_timer = timer;
    }
    if(timer-motor_timer > MOTOR_TIMER_MAX)
    {
        motor_stop();
        return(MOTOR_ERROR);
    }
    //else
    M2_low();
    //_delay_ms(40);
    M1_high();
    motor_direction = MOTOR_LEFT;
    return(MOTOR_OK);
}

uint8_t motor_right()
{
    if(motor_direction != MOTOR_RIGTH)
    {
        //reset timer at change of direction
        motor_timer = timer;
    }
    if(timer-motor_timer > MOTOR_TIMER_MAX)
    {
        motor_stop();
        return(MOTOR_ERROR);
    }
    M1_low();
    //_delay_ms(40);
    M2_high();
    motor_direction = MOTOR_RIGTH;
    return(MOTOR_OK);
}

uint8_t motor_stop()
{
    M1_low();
    M2_low();
    motor_direction = MOTOR_STOPPED;
    return(MOTOR_OK);
}

uint8_t motor_get_direction()
{
    return(motor_direction);
}

void motor_reset_timer()
{
    motor_timer = timer;
}
