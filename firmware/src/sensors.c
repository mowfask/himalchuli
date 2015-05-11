#include "sensors.h"

void sensors_init()
{
	clearbit(DDR_MRC, DDMRC);
	//enable pullup
	setbit(PORT_MRC, PMRC);

	//ADC channels as inputs
	clearbit(DDR_CS, DDCS);
	clearbit(DDR_DS, DDDS);

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

uint8_t distance_sense()
{
	/*Assumes to be called every 8ms. To reduce wear on infrared LED, measure
	 *only every 5 seconds, so every 625 calls. On all the other calls, return
	 *old value.
	 */
	static uint16_t callcount = 0;
	static uint8_t current_distance = 0;

	if(callcount == 0)
	{
		//select channel 2 for ADC
		clearbit(ADMUX, MUX0);
		setbit(ADMUX, MUX1);
		clearbit(ADMUX, MUX2);
		clearbit(ADMUX, MUX3);

		current_distance = adc_conversion();
		callcount = 625;
	}

	callcount--;
	return(current_distance);
}
