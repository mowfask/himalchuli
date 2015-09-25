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

#include <stdio.h>
#include <simavr/avr_ioport.h>
#include <simavr/sim_elf.h>

#include "common.h"
#include "ui.h"

#ifndef FIRMW_PATH
#error Firmware path not defined!
#endif
#ifndef MMCU
#error MMCU not defined!
#endif

avr_t *avr = NULL;

int main(int argc, char *argv[])
{
    elf_firmware_t f;
    int cpu_state = cpu_Running;

    elf_read_firmware(FIRMW_PATH, &f);
    avr = avr_make_mcu_by_name(MMCU);
    avr_init(avr);
    avr_load_firmware(avr, &f);

    ui_init();

    while(cpu_state != cpu_Done && cpu_state != cpu_Crashed)
    {
        cpu_state = avr_run(avr);
    }

    ui_deinit();

    switch(cpu_state)
    {
        case cpu_Done:
            printf("CPU state: Done.\n");
            break;
        case cpu_Crashed:
            printf("CPU state: Crashed.\n");
            break;
        default:
            printf("CPU state: Unknown.\n");
    }


    return 0;
}

