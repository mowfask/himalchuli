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

#ifndef INPUTS_H
#define INPUTS_H

#include "common.h"

//Manual push buttons: ports and portnumbers
//Compare to src/panel.h
#define BTN_ME_PORT     'D'
#define BTN_ME_NUM      6
#define BTN_MU_PORT     'D'
#define BTN_MU_NUM      5
#define BTN_MD_PORT     'B'
#define BTN_MD_NUM      1
//Compare to src/sensors.h
#define BTN_MROT_PORT   'D'
#define BTN_MROT_NUM    2

typedef enum button_tp {btn_released=0, btn_pressed} button_tp;
typedef enum digital_btn {btn_manual_enable=0, btn_manual_up,
                          btn_manual_down, btn_motor_rot} digital_btn;

void btn_handle(digital_btn, button_tp);
void btn_press(digital_btn);
void btn_release(digital_btn);

#endif

