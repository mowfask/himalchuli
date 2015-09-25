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

