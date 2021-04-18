#include "Buttons/buttons.h"
#include "TFT/tft_settings_menu.h"
#include <jled.h>
#include "debug.h"
#include "TFT/tft_main.h"
#include "main.h"

#define ENABLE_LED 1

#define SHORT_BREATHE_DURATION 600
#define LONG_BREATHE_DURATION 1000

OneButton button1(PIN_IN_BUTTON1, true, true);
OneButton button2(PIN_IN_BUTTON2, true, true);
#if (PCB > 130)
OneButton button3(PIN_IN_BUTTON_PWR, true, false);
#endif

Settings *Buttons::settings;
SharedData *Buttons::shrd;
BluetoothHandler *Buttons::blh;

bool poweringStartInProgress = false;

#if ENABLE_LED
auto led1 = JLed(jled::Esp32Hal(PIN_OUT_LED_BUTTON1, 2)).MaxBrightness(255).LowActive();
#endif

Buttons::Buttons()
{
}

void Buttons::setup(SharedData *shrd_p, BluetoothHandler *blh_p, Settings *settings_p)
{

    shrd = shrd_p;
    blh = blh_p;
    settings = settings_p;

    button1.attachClick(processButton1Click);
    button1.attachLongPressStart(processButton1LpStart);
    button1.attachDuringLongPress(processButton1LpDuring);
    button1.attachLongPressStop(processButton1LpStop);
    button1.setDebounceTicks(50);
    button1.setPressTicks(BUTTON_LONG_PRESS_TICK);

    button2.attachClick(processButton2Click);
    button2.attachLongPressStart(processButton2LpStart);
    button2.attachDuringLongPress(processButton2LpDuring);
    button2.attachLongPressStop(processButton2LpStop);
    button2.setDebounceTicks(50);
    button2.setPressTicks(BUTTON_LONG_PRESS_TICK);

#if (PCB > 130)
    button3.attachClick(processButton3Click);
    button3.attachLongPressStart(processButton3LpStart);
    button3.attachDuringLongPress(processButton3LpDuring);
    button3.attachLongPressStop(processButton3LpStop);
    button3.setDebounceTicks(50);
    button3.setPressTicks(BUTTON_LONG_PRESS_TICK);

    // lock power off while power button isn't released a first time
    if (digitalRead(PIN_IN_BUTTON_PWR) == 0)
    {
        poweringStartInProgress = true;
    }
#endif

#if ENABLE_LED
    led1.Stop();
#endif

    //led1.Breathe(SHORT_BREATHE_DURATION).Forever();
}

void Buttons::processButton1Click()
{

    if (shrd->inSettingsMenu == SETTINGS_MENU_STATE_IN)
    {
        settings_menu_btn_click(0, 1);
    }
    else
    {
        if (shrd->button1ClickStatus == ACTION_OFF)
        {
            shrd->button1ClickStatus = ACTION_ON;
        }
        else
        {
            shrd->button1ClickStatus = ACTION_OFF;
        }

        processAuxEvent(1, false);
        processSpeedLimiterEvent(1, false);
        processLockEvent(1, false);
        processModeEvent(1, false);
        processEcoEvent(1, false);
    }

    Serial.println("processButton1Click : " + (String)shrd->button1ClickStatus);

    char print_buffer[500];
    sprintf(print_buffer, "processButton1Click : %d", shrd->button1ClickStatus);
    blh->notifyBleLogs(print_buffer);
}

void Buttons::processButton1LpStart()
{
    shrd->button1LpDuration = button1.getPressedTicks();
    Serial.print("processButton1LpStart : ");
    Serial.println(shrd->button1LpDuration);

    char print_buffer[500];
    sprintf(print_buffer, "processButton1LpStart : %d", shrd->button1LpDuration);
    blh->notifyBleLogs(print_buffer);

    if (shrd->inSettingsMenu == SETTINGS_MENU_STATE_IN)
    {
        settings_menu_btn_click(1, 1);
    }
    else
    {
        processNitroEvent(1, ACTION_ON);
    }
}

void Buttons::processButton1LpDuring()
{
    shrd->button1LpDuration = button1.getPressedTicks();

    if ((shrd->button1LpDuration > settings->get_Button_long_press_duration() * 1000) && (!shrd->button1LpProcessed) && (!shrd->inSettingsMenu))
    {

        char print_buffer[500];
        sprintf(print_buffer, "processButton1LpDuring : %d =>> process", shrd->button1LpDuration);
        blh->notifyBleLogs(print_buffer);

        processAuxEvent(1, true);
        processSpeedLimiterEvent(1, true);
        processLockEvent(1, true);
        processModeEvent(1, true);
        processEcoEvent(1, true);
        shrd->button1LpProcessed = true;
    }
}

void Buttons::processButton1LpStop()
{
    Serial.print("processButton1LpStop : ");
    Serial.println(shrd->button1LpDuration);

    char print_buffer[500];
    sprintf(print_buffer, "processButton1LpStop : %d", shrd->button1LpDuration);
    blh->notifyBleLogs(print_buffer);

    shrd->button1LpProcessed = false;
    shrd->button1LpDuration = 0;

    processNitroEvent(1, ACTION_OFF);
}

////////////////////////////////////////////

void Buttons::processButton2Click()
{

    if (shrd->inSettingsMenu == SETTINGS_MENU_STATE_IN)
    {
        settings_menu_btn_click(0, 2);
    }
    else
    {
        if (shrd->button2ClickStatus == ACTION_OFF)
        {
            shrd->button2ClickStatus = ACTION_ON;
        }
        else
        {
            shrd->button2ClickStatus = ACTION_OFF;
        }

        processAuxEvent(2, false);
        processSpeedLimiterEvent(2, false);
        processLockEvent(2, false);
        processModeEvent(2, false);
        processEcoEvent(2, false);
    }

    Serial.print("processButton2Click : ");
    Serial.println(shrd->button2ClickStatus);

    char print_buffer[500];
    sprintf(print_buffer, "processButton2Click : %d", shrd->button2ClickStatus);
    blh->notifyBleLogs(print_buffer);
}

void Buttons::processButton2LpStart()
{
    shrd->button2LpDuration = button2.getPressedTicks();
    Serial.print("processButton2LpStart : ");
    Serial.println(shrd->button2LpDuration);

    if (shrd->inSettingsMenu == SETTINGS_MENU_STATE_IN)
    {
        settings_menu_btn_click(1, 2);
    }

    char print_buffer[500];
    sprintf(print_buffer, "processButton2LpStart : %d", shrd->button2LpDuration);
    blh->notifyBleLogs(print_buffer);
}

void Buttons::processButton2LpDuring()
{
    shrd->button2LpDuration = button2.getPressedTicks();

    if ((shrd->button2LpDuration > settings->get_Button_long_press_duration() * 1000) && (!shrd->button2LpProcessed))
    {
        // button 2 long press reserved for settings menu access
        /*
        char print_buffer[500];
        sprintf(print_buffer, "processButton2LpDuring : %d ==> process", shrd->button2LpDuration);
        blh->notifyBleLogs(print_buffer);

        processAuxEvent(2, true);
        processSpeedLimiterEvent(2, true);
        processLockEvent(2, true);
        processModeEvent(2, true);
        */

        shrd->button2LpProcessed = true;

        // Enter settings panel
#if ENABLE_TFT
        if (shrd->inSettingsMenu == SETTINGS_MENU_STATE_OUT)
        {
            shrd->inSettingsMenu = SETTINGS_MENU_STATE_ENTERING;

            Serial.println("processButton2LpDuring ===> SETTINGS_MENU_STATE_ENTERING");
        }
#endif
    }
}

void Buttons::processButton2LpStop()
{
    Serial.print("processButton2LpStop : ");
    Serial.println(shrd->button2LpDuration);

    char print_buffer[500];
    sprintf(print_buffer, "processButton2LpStop : %d", shrd->button2LpDuration);
    blh->notifyBleLogs(print_buffer);

    shrd->button2LpProcessed = false;
    shrd->button2LpDuration = 0;
}

////////////////////////////////////////////

#if (PCB > 130)
void Buttons::processButton3Click()
{
    Serial.println("processButton3Click");
}

void Buttons::processButton3LpStart()
{
    Serial.println("processButton3LpStart");
}

void Buttons::processButton3LpDuring()
{
    // do not enable the power off while power button hasn't release at least once
    if (!poweringStartInProgress)
    {
        //Serial.println("processButton3LpDuring");

        shrd->button3LpDuration = button3.getPressedTicks();

        if (shrd->button3LpDuration > settings->get_Button_long_press_duration() * 1000)
        {
            powerOff();
        }
    }
}

void Buttons::processButton3LpStop()
{
    Serial.println("processButton3LpStop");

    poweringStartInProgress = false;
}
#endif

//////////////////////////

void Buttons::processNitroEvent(uint8_t buttonId, uint8_t action)
{

    // process AUX order -- button 1
    if ((buttonId == 1) && (settings->get_Button_1_long_press_action() == settings->LIST_Button_1_long_press_action_Nitro_boost_continuous))
    {
        if (action == ACTION_ON)
        {
            led1.Breathe(SHORT_BREATHE_DURATION).Forever();
            Serial.println("processNitroEvent => ok / ACTION_ON");

            char print_buffer[500];
            sprintf(print_buffer, "processNitroEvent : %d", shrd->auxOrder);
            blh->notifyBleLogs(print_buffer);
        }
        else
        {
            led1.Breathe(SHORT_BREATHE_DURATION).Stop();
            Serial.println("processNitroEvent => ok / ACTION_OFF");

            char print_buffer[500];
            sprintf(print_buffer, "processNitroEvent : %d", shrd->auxOrder);
            blh->notifyBleLogs(print_buffer);
        }
    }
}

void Buttons::processAuxEvent(uint8_t buttonId, bool isLongPress)
{

    // process AUX order -- button 1
    if (((buttonId == 1) && (!isLongPress) && (settings->get_Button_1_short_press_action() == settings->LIST_Button_1_short_press_action_Aux_on_off)) ||
        ((buttonId == 1) && (isLongPress) && (settings->get_Button_1_long_press_action() == settings->LIST_Button_1_long_press_action_Aux_on_off)) ||
        ((buttonId == 2) && (!isLongPress) && (settings->get_Button_2_short_press_action() == settings->LIST_Button_2_short_press_action_Aux_on_off)))
    {
        if (shrd->auxOrder == 0)
        {
            shrd->auxOrder = 1;
            led1.Breathe(SHORT_BREATHE_DURATION).Repeat(2);
        }
        else
        {
            shrd->auxOrder = 0;
            led1.Breathe(SHORT_BREATHE_DURATION).Repeat(1);
        }
        blh->notifyCommandsFeedback();

        Serial.print("processAuxEvent => ok / ");
        Serial.println(shrd->auxOrder);

        char print_buffer[500];
        sprintf(print_buffer, "processAuxEvent : %d", shrd->auxOrder);
        blh->notifyBleLogs(print_buffer);
    }
}

void Buttons::processSpeedLimiterEvent(uint8_t buttonId, bool isLongPress)
{

    // process SpeedLimiter
    if (((buttonId == 1) && (!isLongPress) && (settings->get_Button_1_short_press_action() == settings->LIST_Button_1_short_press_action_Speed_limit_on_off)) ||
        ((buttonId == 1) && (isLongPress) && (settings->get_Button_1_long_press_action() == settings->LIST_Button_1_long_press_action_Speed_limitation_on_off)) ||
        ((buttonId == 2) && (!isLongPress) && (settings->get_Button_2_short_press_action() == settings->LIST_Button_2_short_press_action_Speed_limitation_on_off)))
    {
        if (shrd->speedLimiter == 0)
        {
            shrd->speedLimiter = 1;
            led1.Breathe(SHORT_BREATHE_DURATION).Repeat(2);
        }
        else
        {
            shrd->speedLimiter = 0;
            led1.Breathe(SHORT_BREATHE_DURATION).Repeat(1);
        }
        //blh->notifySpeedLimiterStatus(shrd->speedLimiter);
        blh->notifyCommandsFeedback();

        Serial.print("notifySpeedLimiterStatus => ok / ");
        Serial.println(shrd->speedLimiter);

        char print_buffer[500];
        sprintf(print_buffer, "notifySpeedLimiterStatus : %d", shrd->speedLimiter);
        blh->notifyBleLogs(print_buffer);
    }
}

void Buttons::processLockEvent(uint8_t buttonId, bool isLongPress)
{

    // process SpeedLimiter
    if ((buttonId == 1) && (!isLongPress) && (settings->get_Button_1_long_press_action() == settings->LIST_Button_1_long_press_action_Antitheft_manual_lock_on))
    {
        blh->setBleLock(true);
        blh->notifyBleLock();

        led1.Breathe(SHORT_BREATHE_DURATION).Forever();

        Serial.println("processLockEvent => ok / ON");

        char print_buffer[500];
        sprintf(print_buffer, "processLockEvent");
        blh->notifyBleLogs(print_buffer);
    }
}

void Buttons::processModeEvent(uint8_t buttonId, bool isLongPress)
{

    // process mode switch 1/2/3
    if (((buttonId == 1) && (!isLongPress) && (settings->get_Button_1_short_press_action() == settings->LIST_Button_1_short_press_action_Mode_switch_1_2_3)) ||
        ((buttonId == 1) && (isLongPress) && (settings->get_Button_1_long_press_action() == settings->LIST_Button_1_long_press_action_Mode_switch_1_2_3)) ||
        ((buttonId == 2) && (!isLongPress) && (settings->get_Button_2_short_press_action() == settings->LIST_Button_2_short_press_action_Mode_switch_1_2_3)))
    {
        if (shrd->modeOrder == 1)
        {
            shrd->modeOrder = 2;
            led1.Breathe(SHORT_BREATHE_DURATION).Repeat(2);
        }
        else if (shrd->modeOrder == 2)
        {
            shrd->modeOrder = 3;
            led1.Breathe(SHORT_BREATHE_DURATION).Repeat(3);
        }
        else if (shrd->modeOrder == 3)
        {
            shrd->modeOrder = 1;
            led1.Breathe(SHORT_BREATHE_DURATION).Repeat(1);
        }

#if DEBUG_DISPLAY_MODE
        Serial.println("Buttons::processModeEvent - modeLcd = " + (String)shrd->modeOrder);
#endif

        blh->notifyCommandsFeedback();

        Serial.println("processModeEvent => new mode = " + (String)shrd->modeOrder);
    }

    // process mode switch 2/3
    if (((buttonId == 1) && (!isLongPress) && (settings->get_Button_1_short_press_action() == settings->LIST_Button_1_short_press_action_Mode_switch_2_3)) ||
        ((buttonId == 1) && (isLongPress) && (settings->get_Button_1_long_press_action() == settings->LIST_Button_1_long_press_action_Mode_switch_2_3)) ||
        ((buttonId == 2) && (!isLongPress) && (settings->get_Button_2_short_press_action() == settings->LIST_Button_2_short_press_action_Mode_switch_2_3)))
    {
        if ((shrd->modeOrder == 1) || (shrd->modeOrder == 3))
        {

            shrd->modeOrder = 2;
            led1.Breathe(SHORT_BREATHE_DURATION).Repeat(2);
        }
        else if (shrd->modeOrder == 2)
        {
            shrd->modeOrder = 3;
            led1.Breathe(SHORT_BREATHE_DURATION).Repeat(3);
        }

#if DEBUG_DISPLAY_MODE
        Serial.println("Buttons::processModeEvent - modeLcd = " + (String)shrd->modeOrder);
#endif

        blh->notifyCommandsFeedback();

        Serial.println("processModeEvent => new mode = " + (String)shrd->modeOrder);
    }
}

void Buttons::processEcoEvent(uint8_t buttonId, bool isLongPress)
{

    // process mode switch 1/2/3
    if (((buttonId == 1) && (!isLongPress) && (settings->get_Button_1_short_press_action() == settings->LIST_Button_1_short_press_action_Eco_switch_none_med_max)) ||
        ((buttonId == 1) && (isLongPress) && (settings->get_Button_1_long_press_action() == settings->LIST_Button_1_long_press_action_Eco_switch_none_med_max)) ||
        ((buttonId == 2) && (!isLongPress) && (settings->get_Button_2_short_press_action() == settings->LIST_Button_2_short_press_action_Eco_switch_none_med_max)))
    {
        if (shrd->ecoOrder == 2)
        {
            shrd->ecoOrder = 1;
            led1.Breathe(SHORT_BREATHE_DURATION).Repeat(2);
        }
        else if (shrd->ecoOrder == 1)
        {
            shrd->ecoOrder = 0;
            led1.Breathe(SHORT_BREATHE_DURATION).Repeat(1);
        }
        else if (shrd->ecoOrder == 0)
        {
            shrd->ecoOrder = 2;
            led1.Breathe(SHORT_BREATHE_DURATION).Repeat(3);
        }

#if DEBUG_DISPLAY_ECO
        Serial.println("Buttons::processEcoEvent - ecoOrder = " + (String)shrd->ecoOrder);
#endif

        blh->notifyCommandsFeedback();

        Serial.println("processEcoEvent => new ecoOrder = " + (String)shrd->ecoOrder);
    }

    // process mode switch 2/3
    if (((buttonId == 1) && (!isLongPress) && (settings->get_Button_1_short_press_action() == settings->LIST_Button_1_short_press_action_Eco_switch_none_med)) ||
        ((buttonId == 1) && (isLongPress) && (settings->get_Button_1_long_press_action() == settings->LIST_Button_1_long_press_action_Eco_switch_none_med)) ||
        ((buttonId == 2) && (!isLongPress) && (settings->get_Button_2_short_press_action() == settings->LIST_Button_2_short_press_action_Eco_switch_none_med)))
    {
        if ((shrd->ecoOrder == 0) || (shrd->ecoOrder == 2))
        {
            shrd->ecoOrder = 1;
            led1.Breathe(SHORT_BREATHE_DURATION).Repeat(2);
        }
        else if (shrd->ecoOrder == 1)
        {
            shrd->ecoOrder = 2;
            led1.Breathe(SHORT_BREATHE_DURATION).Repeat(3);
        }

#if DEBUG_DISPLAY_ECO
        Serial.println("Buttons::processEcoEvent - ecoOrder = " + (String)shrd->ecoOrder);
#endif

        blh->notifyCommandsFeedback();

        Serial.println("processEcoEvent => new ecoOrder = " + (String)shrd->ecoOrder);
    }
}

void Buttons::ledsOff()
{
  led1.Stop();
  led1.On().Forever();
}

void Buttons::processTicks()
{
    button1.tick();
    button2.tick();
#if (PCB >= 142)
    button3.tick();
#endif

    led1.Update();
}