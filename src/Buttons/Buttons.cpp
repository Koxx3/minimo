#include "Buttons/buttons.h"
#include "TFT/tft_settings_menu.h"
#include <jled.h>

#define SINGLE_LED 0

#define SHORT_BREATHE_DURATION 600
#define LONG_BREATHE_DURATION 1000

OneButton button1(PIN_IN_BUTTON1, true, true);
OneButton button2(PIN_IN_BUTTON2, true, true);

Settings *Buttons::settings;
SharedData *Buttons::shrd;
BluetoothHandler *Buttons::blh;

#if ENABLE_LED
auto led1 = JLed(jled::Esp32Hal(PIN_OUT_LED_BUTTON1, 7)).MaxBrightness(255).LowActive();
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

#if ENABLE_LED
    led1.Stop();
#endif
}

void Buttons::processButton1Click()
{

    if (shrd->inSettingsMenu)
    {
        settings_menu_btn_click(0, 1);
    }
    else
    {
        if (shrd->button1ClickStatus == ACTION_OFF)
        {
            shrd->button1ClickStatus = ACTION_ON;
#if ENABLE_LED
            led1.Breathe(SHORT_BREATHE_DURATION).Repeat(2);
#endif
        }
        else
        {
            shrd->button1ClickStatus = ACTION_OFF;
#if ENABLE_LED
            led1.Breathe(SHORT_BREATHE_DURATION).Repeat(1);
#endif
        }
    }

    processAuxEvent(1, false);
    processSpeedLimiterEvent(1, false);
    processLockEvent(1, false);
    processModeEvent(1, false);

    Serial.print("processButton1Click : ");
    Serial.println(shrd->button1ClickStatus);

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

    if (shrd->inSettingsMenu)
    {
        settings_menu_btn_click(1, 1);
    }
}

void Buttons::processButton1LpDuring()
{
    shrd->button1LpDuration = button1.getPressedTicks();

    if ((shrd->button1LpDuration > settings->getS3F().Button_long_press_duration * 1000) && (!shrd->button1LpProcessed))
    {

        char print_buffer[500];
        sprintf(print_buffer, "processButton1LpDuring : %d =>> process", shrd->button1LpDuration);
        blh->notifyBleLogs(print_buffer);

        processAuxEvent(1, true);
        processSpeedLimiterEvent(1, true);
        processLockEvent(1, true);
        processModeEvent(1, true);
        shrd->button1LpProcessed = true;

#if ENABLE_LED
        led1.Breathe(LONG_BREATHE_DURATION).Repeat(4);
#endif
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
}

////////////////////////////////////////////

void Buttons::processButton2Click()
{

    if (shrd->inSettingsMenu)
    {
        settings_menu_btn_click(0, 2);
    }
    else
    {
        if (shrd->button2ClickStatus == ACTION_OFF)
        {
            shrd->button2ClickStatus = ACTION_ON;
#if ENABLE_LED
            led1.Breathe(SHORT_BREATHE_DURATION).Repeat(2);
#endif
        }
        else
        {
            shrd->button2ClickStatus = ACTION_OFF;
#if ENABLE_LED
            led1.Breathe(SHORT_BREATHE_DURATION).Repeat(1);
#endif
        }
    }

    processAuxEvent(2, false);
    processSpeedLimiterEvent(2, false);
    processLockEvent(2, false);
    processModeEvent(2, false);

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

    if (shrd->inSettingsMenu)
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

    if ((shrd->button2LpDuration > settings->getS3F().Button_long_press_duration * 1000) && (!shrd->button2LpProcessed))
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

#if ENABLE_LED
        led1.Breathe(SHORT_BREATHE_DURATION).Repeat(5);
#endif
        shrd->button2LpProcessed = true;

        // Enter settings panel
        if (!shrd->inSettingsMenu)
        {
            settings_menu_enter_settings();
        }
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

//////////////////////////

void Buttons::processAuxEvent(uint8_t buttonId, bool isLongPress)
{

    // process AUX order -- button 1
    if (((buttonId == 1) && (!isLongPress) && (settings->getS3F().Button_1_short_press_action == settings->LIST_Button_press_action_Aux_on_off)) ||
        ((buttonId == 1) && (isLongPress) && (settings->getS3F().Button_1_long_press_action == settings->LIST_Button_press_action_Aux_on_off)) ||
        ((buttonId == 2) && (!isLongPress) && (settings->getS3F().Button_2_short_press_action == settings->LIST_Button_press_action_Aux_on_off)) ||
        ((buttonId == 2) && (isLongPress) && (settings->getS3F().Button_2_long_press_action == settings->LIST_Button_press_action_Aux_on_off)))
    {
        if (shrd->auxOrder == 0)
        {
            shrd->auxOrder = 1;
        }
        else
        {
            shrd->auxOrder = 0;
        }
        //    blh->notifyAuxOrder(shrd->auxOrder);
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
    if (((buttonId == 1) && (!isLongPress) && (settings->getS3F().Button_1_short_press_action == settings->LIST_Button_press_action_Startup_speed_limitation_on_off)) ||
        ((buttonId == 1) && (isLongPress) && (settings->getS3F().Button_1_long_press_action == settings->LIST_Button_press_action_Startup_speed_limitation_on_off)) ||
        ((buttonId == 2) && (!isLongPress) && (settings->getS3F().Button_2_short_press_action == settings->LIST_Button_press_action_Startup_speed_limitation_on_off)) ||
        ((buttonId == 2) && (isLongPress) && (settings->getS3F().Button_2_long_press_action == settings->LIST_Button_press_action_Startup_speed_limitation_on_off)))
    {
        if (shrd->speedLimiter == 0)
        {
            shrd->speedLimiter = 1;
        }
        else
        {
            shrd->speedLimiter = 0;
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
    if (((buttonId == 1) && (!isLongPress) && (settings->getS3F().Button_1_short_press_action == settings->LIST_Button_press_action_Anti_theft_manual_lock_on)) ||
        ((buttonId == 1) && (isLongPress) && (settings->getS3F().Button_1_long_press_action == settings->LIST_Button_press_action_Anti_theft_manual_lock_on)) ||
        ((buttonId == 2) && (!isLongPress) && (settings->getS3F().Button_2_short_press_action == settings->LIST_Button_press_action_Anti_theft_manual_lock_on)) ||
        ((buttonId == 2) && (isLongPress) && (settings->getS3F().Button_2_long_press_action == settings->LIST_Button_press_action_Anti_theft_manual_lock_on)))
    {
        blh->setBleLock(true);
        blh->notifyBleLock();

        Serial.println("processLockEvent => ok / ON");

        char print_buffer[500];
        sprintf(print_buffer, "processLockEvent");
        blh->notifyBleLogs(print_buffer);
    }
}

void Buttons::processModeEvent(uint8_t buttonId, bool isLongPress)
{

    // process mode switch 1/2/3
    if (((buttonId == 1) && (!isLongPress) && (settings->getS3F().Button_1_short_press_action == settings->LIST_Button_press_action_Mode_switch_1_2_3)) ||
        ((buttonId == 1) && (isLongPress) && (settings->getS3F().Button_1_long_press_action == settings->LIST_Button_press_action_Mode_switch_1_2_3)) ||
        ((buttonId == 2) && (!isLongPress) && (settings->getS3F().Button_2_short_press_action == settings->LIST_Button_press_action_Mode_switch_1_2_3)) ||
        ((buttonId == 2) && (isLongPress) && (settings->getS3F().Button_2_long_press_action == settings->LIST_Button_press_action_Mode_switch_1_2_3)))
    {
        if (shrd->modeOrder == 1)
            shrd->modeOrder = 2;
        else if (shrd->modeOrder == 2)
            shrd->modeOrder = 3;
        else if (shrd->modeOrder == 3)
            shrd->modeOrder = 1;

        blh->notifyCommandsFeedback();

        Serial.println("processModeEvent => new mode = " + (String)shrd->modeOrder);
    }

    // process mode switch 2/3
    if (((buttonId == 1) && (!isLongPress) && (settings->getS3F().Button_1_short_press_action == settings->LIST_Button_press_action_Mode_switch_2_3)) ||
        ((buttonId == 1) && (isLongPress) && (settings->getS3F().Button_1_long_press_action == settings->LIST_Button_press_action_Mode_switch_2_3)) ||
        ((buttonId == 2) && (!isLongPress) && (settings->getS3F().Button_2_short_press_action == settings->LIST_Button_press_action_Mode_switch_2_3)) ||
        ((buttonId == 2) && (isLongPress) && (settings->getS3F().Button_2_long_press_action == settings->LIST_Button_press_action_Mode_switch_2_3)))
    {
        if (shrd->modeOrder == 1)
            shrd->modeOrder = 2;
        else if (shrd->modeOrder == 2)
            shrd->modeOrder = 3;
        else if (shrd->modeOrder == 3)
            shrd->modeOrder = 2;

        blh->notifyCommandsFeedback();

        Serial.println("processModeEvent => new mode = " + (String)shrd->modeOrder);
    }
}

void Buttons::processTicks()
{
    button1.tick();
    button2.tick();

    /*
    processButton1();
#if DEBUG_DISPLAY_BUTTON1
    displayButton1();
#endif

    processButton2();
#if DEBUG_DISPLAY_BUTTON2
    displayButton2();
#endif
*/

    /*
    extern uint32_t i_loop;
    uint32_t timeBefore = micros();
    */

    //led1.Update();

    /*
    if (i_loop % 10 == 6)
    {
     Serial.println("led update = " + (String)(micros() - timeBefore));
    }
    */
}
