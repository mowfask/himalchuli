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

#define PORT_M1  PORTB
#define PM1      PB0
#define DDR_M1   DDRB
#define DDM1     DDB0


void panel_init();

inline void status_LED_on();
inline void status_LED_off();
inline void detection_LED_on();
inline void detection_LED_off();

inline uint8_t manual_enable_pressed();
inline uint8_t manual_up_pressed();
inline uint8_t manual_down_pressed();

#endif
