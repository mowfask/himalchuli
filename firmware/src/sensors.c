#include "sensors.h"

void sensors_init()
{
	clearbit(DDR_MRC, DDMRC);
	//enable pullup
	setbit(PORT_MRC, PMRC);

	//set up ADC
	//AVCC reference voltage
	setbit(ADMUX, REFS0);
	clearbit(ADMUX, REFS1);
	//left adjusted result (we use only high byte anyway)
	setbit(ADMUX, ADLAR);
	//125kHz sampling -> prescaler of 8 from 1MHz clock
	setbit(ADCSRA, ADPS0);
	setbit(ADCSRA, ADPS1);
	clearbit(ADCSRA, ADPS2);

	//enable ADC (but don't start it yet)
	setbit(ADCSRA, ADEN);
}

inline uint8_t motor_rotation_pressed()
{
	return(1-testbit(PIN_MRC, PMRC));
}

static uint8_t adc_conversion()
{
	//start ADC
	setbit(ADCSRA, ADSC);
	//wait for result
	loop_until_bit_is_set(ADCSRA, ADIF);
	//reset ADIF
	setbit(ADCSRA, ADIF);	//"ADIF is cleared by writing a logical one to the
							//flag"
	return(ADCH);
}

uint8_t current_sense()
{
	//select channel 1 for ADC
	setbit(ADMUX, MUX0);
	clearbit(ADMUX, MUX1);
	clearbit(ADMUX, MUX2);
	clearbit(ADMUX, MUX3);

	return(adc_conversion());
}

uint8_t feeder_sense()
{
	//select channel 2 for ADC
	clearbit(ADMUX, MUX0);
	setbit(ADMUX, MUX1);
	clearbit(ADMUX, MUX2);
	clearbit(ADMUX, MUX3);

	return(adc_conversion());
}
