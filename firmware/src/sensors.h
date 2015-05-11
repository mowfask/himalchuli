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

void sensors_init();
inline uint8_t motor_rotation_pressed();

#endif
