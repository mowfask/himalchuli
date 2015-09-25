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

#include "inputs.h"
#include <simavr/avr_ioport.h>

void btn_handle(digital_btn button, button_tp state)
//raise IRQ for pushed button.
//state may only be 1 or 0, as it's send to IO IRQ handler
{
    uint32_t ioport_irq;
    int ioport_num;

    switch(button)
    {
        case btn_manual_enable:
            ioport_irq = AVR_IOCTL_IOPORT_GETIRQ(BTN_ME_PORT);
            ioport_num = BTN_ME_NUM;
            break;
        case btn_manual_up:
            ioport_irq = AVR_IOCTL_IOPORT_GETIRQ(BTN_MU_PORT);
            ioport_num = BTN_MU_NUM;
            break;
        case btn_manual_down:
            ioport_irq = AVR_IOCTL_IOPORT_GETIRQ(BTN_MD_PORT);
            ioport_num = BTN_MD_NUM;
            break;
        case btn_motor_rot:
            ioport_irq = AVR_IOCTL_IOPORT_GETIRQ(BTN_MROT_PORT);
            ioport_num = BTN_MROT_NUM;
            break;
        default:
            return;
    }

    avr_raise_irq(avr_io_getirq(avr, ioport_irq, ioport_num), state);
}

void btn_press(digital_btn button)
{
    btn_handle(button, 1);
}

void btn_release(digital_btn button)
{
    btn_handle(button, 0);
}

