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
#include <unistd.h>
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

    ui_init();
    sleep(200);  //ui thread will terminate after 20s
    ui_deinit();

    elf_read_firmware(FIRMW_PATH, &f);
    avr = avr_make_mcu_by_name(MMCU);
    avr_init(avr);
    avr_load_firmware(avr, &f);

    return 0;
}

