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

#include "sensors.h"

void sensors_init()
{
    //motor rotation counter
    clearbit(DDR_MRC, DDMRC);
    //enable pullup
    setbit(PORT_MRC, PMRC);

    //distance senseor supply as output
    setbit(DDR_DSS, DDDSS);
    //default off (inverted)
    setbit(PORT_DSS, PDSS);

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

uint8_t motor_rotation_pressed()
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
    setbit(ADCSRA, ADIF);   //"ADIF is cleared by writing a logical one to the
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

static void distance_supply_on()
{
    //inverted by transistor
    clearbit(PORT_DSS, PDSS);
}

static void distance_supply_off()
{
    //inverted by transistor
    setbit(PORT_DSS, PDSS);
}

uint8_t distance_sense()
{
    /*Assumes to be called every 8ms. To reduce wear on infrared LED, measure
     *only every DIST_TIMER_MAX calls. On all the other calls, return old
     *value.
     */
    static uint16_t dist_timer = 0;
    static uint8_t current_distance = 0;

    if(dist_timer == 0)
    {
        distance_supply_on();
        _delay_us(400); //Rise time for infrared LED
        //select channel 2 for ADC
        clearbit(ADMUX, MUX0);
        setbit(ADMUX, MUX1);
        clearbit(ADMUX, MUX2);
        clearbit(ADMUX, MUX3);

        current_distance = 255-adc_conversion();
        distance_supply_off();
        dist_timer = DIST_TIMER_MAX;
    }

    dist_timer--;
    return(current_distance);
}
