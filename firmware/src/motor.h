#ifndef MOTOR_H
#define MOTOR_H

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


void motor_init();

/*Always use the following functions to set the levels of the output
 *connector!!
 */

void M1_low();
void M1_high();
void M1_toggle();

void M2_low();
void M2_high();
void M2_toggle();

void motor_left();
void motor_right();
void motor_stop();

#endif
