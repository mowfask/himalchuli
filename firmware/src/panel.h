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

#ifndef PANEL_H
#define PANEL_H

#include "common.h"

//status LED
#define PORT_SLED   PORTB
#define PSLED       PB6
#define DDR_SLED    DDRB
#define DDSLED      DDB6

//detection LED
#define PORT_DLED   PORTB
#define PDLED       PB7
#define DDR_DLED    DDRB
#define DDDLED      DDB7

//manual enable button
#define PIN_MEB     PIND
#define PORT_MEB    PORTD
#define PMEB        PD6
#define DDR_MEB     DDRD
#define DDMEB       DDD6

//manual up button
#define PIN_MUB     PIND
#define PORT_MUB    PORTD
#define PMUB        PD5
#define DDR_MUB     DDRD
#define DDMUB       DDD5

//manual down button
#define PIN_MDB     PINB
#define PORT_MDB    PORTB
#define PMDB        PB1
#define DDR_MDB     DDRB
#define DDMDB       DDB1


void panel_init();

void status_LED_on();
void status_LED_off();
void detection_LED_on();
void detection_LED_off();

uint8_t manual_enable_pressed();
uint8_t manual_up_pressed();
uint8_t manual_down_pressed();

#endif
