
#include <menu.h>
#include <menuIO/serialIO.h>
#include <menuIO/TFT_eSPIOut.h>
#include <menuIO/esp8266Out.h> //must include this even if not doing web output...
#include "Settings.h"
#include "SharedData.h"

#ifndef _Settings_menu_h
#define _Settings_menu_h

void settings_menu_setup();
void settings_menu_loop();
void settings_menu_btn_click(uint8_t pressType, uint8_t btnNum);
void settings_menu_reset_idle();
void settings_menu_save_to_settings();
void settings_menu_set_settings(Settings *set);
void settings_menu_set_shared_datas(SharedData *set);

#endif