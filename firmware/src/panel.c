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

#include "panel.h"

void panel_init()
{
    setbit(DDR_SLED, DDSLED);
    setbit(DDR_DLED, DDDLED);

    clearbit(DDR_MEB, DDMEB);
    clearbit(DDR_MUB, DDMUB);
    clearbit(DDR_MDB, DDMDB);

    //pullups for panel input switches
    setbit(PORT_MEB, PMEB);
    setbit(PORT_MUB, PMUB);
    setbit(PORT_MDB, PMDB);
}

void status_LED_on()
{
    setbit(PORT_SLED, PSLED);
}

void status_LED_off()
{
    clearbit(PORT_SLED, PSLED);
}

void detection_LED_on()
{
    setbit(PORT_DLED, PDLED);
}

void detection_LED_off()
{
    clearbit(PORT_DLED, PDLED);
}

uint8_t manual_enable_pressed()
{
    return(1-testbit(PIN_MEB, PMEB));
}

uint8_t manual_up_pressed()
{
    return(1-testbit(PIN_MUB, PMUB));
}

uint8_t manual_down_pressed()
{
    return(1-testbit(PIN_MDB, PMDB));
}


