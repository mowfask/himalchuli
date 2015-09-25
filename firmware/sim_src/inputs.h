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

