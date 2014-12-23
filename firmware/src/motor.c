#include "motor.h"

//Current state of outputs:
uint8_t M1_state;
uint8_t M2_state;

void motor_init()
{
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
}

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
