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

//feeder sense
#define PIN_FS      PINC
#define PORT_FS     PORTC
#define PFS         PC2
#define DDR_FS      DDRC
#define DDFS        DDC2

void sensors_init();
inline uint8_t motor_rotation_pressed();

#endif
