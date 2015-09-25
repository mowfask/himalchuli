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

#ifndef UI_H
#define UI_H

#include "common.h"
#include "inputs.h"     //for button_tp

typedef struct ui_outputs
{
    enum {motor_off=0, motor_left, motor_right} motor;
    enum {ledg_off=0, ledg_on}                  led_green;
    enum {ledr_off=0, ledr_on}                  led_red;
} ui_outputs;

typedef struct ui_inputs
{
    button_tp manual_enable;
    button_tp manual_up;
    button_tp manual_down;
    button_tp motor_rot;
    uint8_t current;
    uint8_t distance;
} ui_inputs;

void ui_init(void);
void ui_deinit(void);
void ui_setoutp(ui_outputs);
ui_inputs ui_getinp(void);

#endif

