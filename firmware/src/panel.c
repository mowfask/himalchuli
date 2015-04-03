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

inline void status_LED_on()
{
    setbit(PORT_SLED, PSLED);
}

inline void status_LED_off()
{
    clearbit(PORT_SLED, PSLED);
}

inline void detection_LED_on()
{
    setbit(PORT_DLED, PDLED);
}

inline void detection_LED_off()
{
    clearbit(PORT_DLED, PDLED);
}

inline uint8_t manual_enable_pressed()
{
    return(testbit(PIN_MEB, PMEB));
}

inline uint8_t manual_up_pressed()
{
    return(testbit(PIN_MUB, PMUB));
}

inline uint8_t manual_down_pressed()
{
    return(testbit(PIN_MDB, PMDB));
}


