#include "sensors.h"

void sensors_init()
{
	clearbit(DDR_MRC, DDMRC);
	//enable pullup
	setbit(PORT_MRC, PMRC);
}

inline uint8_t motor_rotation_pressed()
{
	return(testbit(PIN_MRC, PMRC));
}
