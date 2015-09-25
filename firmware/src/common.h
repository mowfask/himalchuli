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

#ifndef COMMON_H
#define COMMON_H

#define F_CPU 1000000UL
#define BAUD 2400UL

#include <util/delay.h>
#include <util/setbaud.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>

//Bit operations
#define setbit(byte, bit) ((byte) |= ((1) << (bit)))
#define clearbit(byte, bit) ((byte) &= ~((1) << (bit)))
#define togglebit(byte, bit) ((byte) ^= ((1) << (bit)))
#define testbit(byte, bit) (((byte) >> (bit)) & (1))    //returns 1 or 0

#define setmask(byte, mask) ((byte) |= (mask))
#define clearmask(byte, mask) ((byte) &= ~(mask))
#define togglemask(byte, mask) ((byte ^= (mask))
#define testmask(byte, mask) ((byte) & (mask) == 0 ? 0 : 1) //not elegant, but
                                                            //returns 1 or 0
extern uint16_t timer;      //Timer counting in 8ms steps. This is enough for
                            //about 8.5 minutes.

#endif
