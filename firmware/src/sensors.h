/*
 * This file is part of himalchuli, a controller for a calf protection
 * mechanism.
 *
 * Copyright (C) 2015 J. Deitmerg <mowfask@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SENSORS_H
#define SENSORS_H

#include "common.h"

//motor rotation counter
#define PIN_MRC     PIND
#define PORT_MRC    PORTD
#define PMRC        PD2
#define DDR_MRC     DDRD
#define DDMRC       DDD2

//current sense
#define PIN_CS      PINC
#define PORT_CS     PORTC
#define PCS         PC1
#define DDR_CS      DDRC
#define DDCS        DDC1

//distance sense
#define PIN_DS      PINC
#define PORT_DS     PORTC
#define PDS         PC2
#define DDR_DS      DDRC
#define DDDS        DDC2

//distance sense supply
#define PIN_DSS     PINB
#define PORT_DSS    PORTB
#define PDSS        PB2
#define DDR_DSS     DDRB
#define DDDSS       DDB2

#define DIST_TIMER_MAX  625 //measure distance only every 5 seconds (625 ticks
                            //of 8ms)

void sensors_init();
uint8_t motor_rotation_pressed();
uint8_t current_sense();
uint8_t distance_sense();

#endif
