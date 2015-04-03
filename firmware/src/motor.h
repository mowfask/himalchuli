#ifndef MOTOR_H
#define MOTOR_H

#include "common.h"

//Pin definitions
//Motor connector 1
#define PORT_M1  PORTB
#define PM1      PB0
#define DDR_M1   DDRB
#define DDM1     DDB0

//Motor connector 2
#define PORT_M2  PORTD
#define PM2      PD7
#define DDR_M2   DDRD
#define DDM2     DDD7


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

#define motor_up    motor_right
#define motor_down  motor_left

#endif
