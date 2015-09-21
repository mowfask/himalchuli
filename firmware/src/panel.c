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


