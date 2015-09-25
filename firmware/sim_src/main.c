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

