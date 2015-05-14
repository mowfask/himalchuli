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

#define MOTOR_OK            0
#define MOTOR_ERROR         1

#define MOTOR_TIMER_MAX     625     //If motor is running for 5 seconds (625
                                    //ticks of 8ms) without feedback, return
                                    //MOTOR_ERROR from motor_left() and
                                    //motor_rigth()

#define MOTOR_STOPPED   0
#define MOTOR_RIGTH     1
#define MOTOR_LEFT      2


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

//Return MOTOR_OK or MOTOR_ERROR. In the second case, the motor will be
//stopped.
uint8_t motor_left();
uint8_t motor_right();
uint8_t motor_stop();

uint8_t motor_get_direction();
void motor_reset_timer();

//If you switch these, switch those below as well
#define motor_up    motor_right
#define motor_down  motor_left

#define MOTOR_UP    MOTOR_RIGTH
#define MOTOR_DOWN  MOTOR_LEFT

#endif
