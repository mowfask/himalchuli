#include <stdio.h>
#include <simavr/sim_avr.h>
#include <simavr/avr_ioport.h>
#include <simavr/sim_elf.h>

#ifndef FIRMW_PATH
#error Firmware path not defined!
#endif
#ifndef MMCU
#error MMCU not defined!
#endif

avr_t *avr = NULL;

int main(int argc, char *argv[])
{
    const char myirq_name[] = "myirq";
    const char *myirq_names[] = {myirq_name, NULL};

    elf_firmware_t f;
    elf_read_firmware(FIRMW_PATH, &f);
    avr = avr_make_mcu_by_name(MMCU);
    avr_init(avr);
    avr_load_firmware(avr, &f);

    avr_irq_t *myirq;
    //allocate from irq_pool, returns 1 irq starting at 0. The 0 is what
    //myirq->irq is set to.
    myirq = avr_alloc_irq(&(avr->irq_pool), 0, 1, myirq_names);

    //"Input" irq, e.g. button (src) wants to inform io-port (dst) about
    //pin change.
    avr_connect_irq(myirq,
                    avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('C'), 0));

    //Tell everything chained to PC0 that PC0 is low.
    avr_raise_irq(myirq, 0);

    //What has been done should be equivalent to this:
    avr_raise_irq(avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('C'),0), 0);

    return 0;
}

