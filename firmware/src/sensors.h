#ifndef SENSORS_H
#define SENSORS_H

#include "common.h"

//motor rotation counter
#define PIN_MRC     PIND
#define PORT_MRC    PORTD
#define PMRC        PD2
#define DDR_MRC     DDRD
#define DDMRC       DDD2

void sensors_init();
inline uint8_t motor_rotation_pressed();

#endif
