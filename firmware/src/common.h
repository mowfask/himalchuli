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
