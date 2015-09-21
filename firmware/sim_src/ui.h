#ifndef UI_H
#define UI_H

#include "common.h"

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

