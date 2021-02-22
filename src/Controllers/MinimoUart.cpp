#include "MinimoUart.h"
#include "SerialMode.h"
#include "main.h"
#include "debug.h"
#include "tools/utils.h"

#ifndef MINIMO_SIMULATED_DISPLAY
#define MINIMO_SIMULATED_DISPLAY 0
#endif

char simulatedFrameFromLcd[] = {0xaa, 0x4, 0x45, 0x0, 0x0, 0x6e, 0x0, 0x64, 0x0, 0x80, 0x2, 0x2, 0x0, 0x0, 0x61};
uint8_t iMsgLcdToCntrl = 0;

MinimoUart::MinimoUart()
{
}

void MinimoUart::setSettings(Settings *settings_p)
{

  // Init settings
  settings = settings_p;
}

void MinimoUart::setSharedData(SharedData *shrd_p)
{

  // Init shared data
  shrd = shrd_p;
}

void MinimoUart::setBluetoothHandler(BluetoothHandler *blh_p)
{
  blh = blh_p;
}

void MinimoUart::setControllerSerialPort(Stream *port)
{
  hwSerCntrl = port;
}

void MinimoUart::setLcdSerialPort(Stream *port)
{
  hwSerLcd = port;
}

void setSharedData(SharedData *);

//////------------------------------------
//////------------------------------------
////// Various functions

uint8_t MinimoUart::getCheckSum(char *string)
{
  byte rtn = 0;

  for (byte i = 0; i < 14; i++)
  {
    rtn ^= string[i];
  }

  return rtn;
}

void MinimoUart::displayFrame(int mode, char data_buffer[], byte checksum)
{

  char print_buffer[500];

  // for excel
  sprintf(print_buffer, "(%d) %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x / %02x",
          mode,
          data_buffer[0],
          data_buffer[1],
          data_buffer[2],
          data_buffer[3],
          data_buffer[4],
          data_buffer[5],
          data_buffer[6],
          data_buffer[7],
          data_buffer[8],
          data_buffer[9],
          data_buffer[10],
          data_buffer[11],
          data_buffer[12],
          data_buffer[13],
          data_buffer[14],
          checksum);

  Serial.println(print_buffer);
}

void MinimoUart::displayDecodedFrame(int mode, char data_buffer[], byte checksum)
{

  char print_buffer[500];

  // for excel
  sprintf(print_buffer, "(%d) %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x  %02x /  %2d /  %2d",
          mode,
          data_buffer[0],
          data_buffer[1],
          data_buffer[2],
          data_buffer[3],
          (data_buffer[4] - data_buffer[3]) & 0xff,
          (data_buffer[5] - data_buffer[3]) & 0xff,
          (data_buffer[7] - data_buffer[3]) & 0xff,
          (data_buffer[8] - data_buffer[3]) & 0xff,
          (data_buffer[9] - data_buffer[3]) & 0xff,
          (data_buffer[10] - data_buffer[3]) & 0xff,
          (data_buffer[11] - data_buffer[3]) & 0xff,
          (data_buffer[12] - data_buffer[3]) & 0xff,
          (data_buffer[13] - data_buffer[3]) & 0xff,
          data_buffer[14],
          ((data_buffer[10] - data_buffer[3]) & 0xff) >> 1,
          ((data_buffer[10] - data_buffer[3]) & 0xff) >> 2);

  Serial.println(print_buffer);
}

//////------------------------------------
//////------------------------------------
////// Data processing functions

/*
uint8_t MinimoUart::getMode(char var, char data_buffer[])
{
  uint32_t byteDiff = (var - data_buffer[2]);
  uint8_t modeLcd = (byteDiff & 0x03) + 1;

  char print_buffer[500];
  sprintf(print_buffer, "%s %02x / %s %02x",
          "lcd",
          modeLcd,
          "order",
          shrd->modeOrder);

#if DEBUG_DISPLAY_MODE
  Serial.print("LCD mode : ");
  Serial.print(print_buffer);
  Serial.println("");
#endif

  return modeLcd;
}
*/

uint8_t MinimoUart::modifyMode(char var, char data_buffer[])
{
  uint8_t newModeLcd;

  uint32_t byteDiff = (var - data_buffer[2]);
  uint8_t modeLcd = (byteDiff & 0x03) + 1;

  // override Smartphone mode with LCD mode
  if (MINIMO_SIMULATED_DISPLAY == 0)
  {
    if (shrd->modeLcdOld != modeLcd)
    {

#if DEBUG_DISPLAY_MODE
      Serial.println("LCD mode : modeOrder = " + (String)shrd->modeLcdOld + " / modeLcd = " + (String)modeLcd + " ==> new mode set to modeLcd");
#endif

      shrd->modeOrder = modeLcd;
      shrd->modeLcdOld = modeLcd;

      // notify bluetooth
      blh->notifyCommandsFeedback();
    }
    else
    {
#if DEBUG_DISPLAY_MODE
      Serial.println("LCD mode : modeOrder = " + (String)shrd->modeLcdOld + " / modeLcd = " + (String)modeLcd + " ==> no mod");
#endif
    }
  }

  /*
  #if DEBUG_DISPLAY_NITRO
  Serial.print("button1LpDuration = ");
  Serial.print(button1LpDuration);
  Serial.print(" / settings->getS3F().Button_1_long_press_action = ");
  Serial.print(settings->getS3F().Button_1_long_press_action);
  Serial.print(" / settings->LIST_Button_press_action_Nitro_boost = ");
  Serial.println(settings->LIST_Button_press_action_Nitro_boost);
#endif
*/

  // Nitro boost
  if (((settings->getS3F().Button_1_short_press_action == settings->LIST_Button_press_action_Nitro_boost_on_off) && (shrd->button1ClickStatus == ACTION_ON)) ||
      ((settings->getS3F().Button_1_long_press_action == settings->LIST_Button_press_action_Nitro_boost_cont) && (shrd->button1LpDuration > 0)) ||
      ((settings->getS3F().Button_2_short_press_action == settings->LIST_Button_press_action_Nitro_boost_on_off) && (shrd->button2ClickStatus == ACTION_ON)) ||
      ((settings->getS3F().Button_2_long_press_action == settings->LIST_Button_press_action_Nitro_boost_cont) && (shrd->button2LpDuration > 0)))
  {
    if (shrd->modeOrderBeforeNitro < 0)
    {
      shrd->modeOrderBeforeNitro = shrd->modeOrder;
    }
    shrd->modeOrder = 3;
    //blh->notifyModeOrder(shrd->modeOrder);
    blh->notifyCommandsFeedback();

#if DEBUG_DISPLAY_NITRO
    Serial.print(" !!!!!!!!!!!!! LIST_Button_press_action_Nitro_boost in PROGRESS !!!!!!!!!!!!! ");
    Serial.println(shrd->modeOrder);
#endif
  }
  else
  {
    if (shrd->modeOrderBeforeNitro > 0)
    {
      shrd->modeOrder = shrd->modeOrderBeforeNitro;

      // blh->notifyModeOrder(shrd->modeOrder);
      blh->notifyCommandsFeedback();

#if DEBUG_DISPLAY_NITRO
      Serial.print(" !!!!!!!!!!!!! LIST_Button_press_action_Nitro_boost STOPPED !!!!!!!!!!!!! ");
      Serial.println(shrd->modeOrder);
#endif

      shrd->modeOrderBeforeNitro = -1;
    }
  }

  if (shrd->modeOrder == 1)
    newModeLcd = modeLcd0[(uint8_t)(data_buffer[2])];
  else if (shrd->modeOrder == 2)
    newModeLcd = modeLcd1[(uint8_t)(data_buffer[2])];
  else if (shrd->modeOrder == 3)
    newModeLcd = modeLcd2[(uint8_t)(data_buffer[2])];
  else
    newModeLcd = modeLcd2[(uint8_t)(data_buffer[2])];

  return newModeLcd;
}

uint8_t MinimoUart::modifyPower(char var, char data_buffer[])
{
  uint8_t newPower = 100;

  float voltage = shrd->voltageFilterMean / 1000.0;
  float bat_min = settings->getS3F().Battery_min_voltage / 10.0;
  float bat_max = settings->getS3F().Battery_max_voltage / 10.0;
  //float bat_med_save = settings->getS3F().Battery_saving_medium_voltage;

  float bat_med_save_voltage = ((bat_max - bat_min) * settings->getS3F().Battery_saving_medium_voltage / 100.0) + bat_min;

  /*
  Serial.print("voltage : ");
  Serial.print(voltage);
  Serial.print("V / bat_min : ");
  Serial.print(bat_min);
  Serial.print("V / bat_max : ");
  Serial.print(bat_max);
  Serial.print("V / bat_med_save_pourcent : ");
  Serial.print(bat_med_save);
  Serial.print("% / bat_med_save_voltage : ");
  Serial.print(bat_med_save_voltage);
  Serial.print("V");
  */

#if DEBUG_DISPLAY_MINIMO_MOD_POWER
  Serial.print("blh->bleLockStatus = ");
  Serial.print(blh->bleLockStatus);
  Serial.print(" / shrd->speedLimiter = ");
  Serial.print(shrd->speedLimiter);
  Serial.print(" / shrd->Speed_limiter_max_speed = ");
  Serial.print(settings->getS1F().Speed_limiter_max_speed);
  Serial.println("%");
#endif

  // lock escooter by reducing power to 5%
  if (shrd->isLocked == true)
  {
    // REMINDER : never put bellow 5
    newPower = 5;
  }
  else if (shrd->speedLimiter == 1)
  {
    // Apply limit
    newPower = settings->getS1F().Speed_limiter_max_speed;
    if (newPower < 5)
    {
      newPower = 5;
    }

#if DEBUG_DISPLAY_MINIMO_MOD_POWER
    Serial.print(" / new_power = ");
    Serial.print(newPower);
    Serial.println("%");
#endif
  }
  else
  {

    // override with battery status
    int min_power = 20;
    if (voltage < bat_min)
    {
      if (min_power < var)
      {
        newPower = min_power;
      }
    }
    else if (voltage < bat_med_save_voltage)
    {
      float factor = ((min_power - 100) / (bat_min - bat_med_save_voltage));

      float origin = 100 - (factor * bat_med_save_voltage);

      /*
      Serial.print(" / factor = ");
      Serial.print(factor);
      Serial.print(" / origin = ");
      Serial.print(origin);
*/

      if (newPower < var)
      {
        newPower = (voltage * factor) + origin;
      }
    }
    else
    {
      newPower = var;
    }
  }

  return newPower;
}

uint8_t MinimoUart::modifyPas(char var, char data_buffer[])
{

  if (MINIMO_SIMULATED_DISPLAY == 1)
  {
    shrd->pasEnabled = settings->getS6F().Pas_enabled;
    if (shrd->pasEnabled)
      var = var | 0x02;
    else
      var = var & 0xfd;
  }
  else
  {
    shrd->pasEnabled = (var >> 1) & 0x01;
  }

#if DEBUG_DISPLAY_MINIMO_MOD_PAS
  Serial.printf("var = %02x / shrd->pasEnable = %d\n", var, shrd->pasEnabled);
#endif
  return var;
}

bool MinimoUart::isContrlInError(char var, char data_buffer[])
{

  uint8_t byte4 = (data_buffer[4] - data_buffer[3]);
  uint8_t byte5 = (data_buffer[5] - data_buffer[3]);

  return !((byte4 == 0xf8) && (byte5 == 0xf8));
}

uint8_t MinimoUart::getBrakeFromCntrlFrame(char var, char data_buffer[])
{

  uint8_t brake = (var - data_buffer[3]) & 0x20;
  uint8_t brakePressedStatusFromControllerNew = brake >> 5;

  //uint8_t brakeStatusFromLcdNew = brakeStatus;
  if ((brakePressedStatusFromControllerNew == 1) && (shrd->brakePressedStatusOld == 0))
  {
    shrd->brakePressedStatus = brakePressedStatusFromControllerNew;
    timeLastBrake = millis();

#if DEBUG_DISPLAY_DIGITAL_BRAKE
    Serial.print("Brake pressed at : ");
    Serial.println(timeLastBrake);
#endif

    // notify bluetooth
    blh->notifyCommandsFeedback();
  }
  else if ((brakePressedStatusFromControllerNew == 0) && (shrd->brakePressedStatusOld == 1))
  {
    shrd->brakePressedStatus = brakePressedStatusFromControllerNew;

    // reset brake sent to controller
    if (settings->getS1F().Electric_brake_progressive_mode == 0)
    {
      if (settings->getS2F().Electric_brake_type == settings->LIST_Electric_brake_type_cntrl)
      {
        if (shrd->brakeSentOrderFromBLE < 0)
        {
          shrd->brakeSentOrder = shrd->brakeDisplay;
        }
        else
        {
          //  do nothing ...
          shrd->brakeSentOrder = shrd->brakeSentOrderFromBLE;
        }
      }
    }
    else
    {
      shrd->brakeSentOrder = settings->getS1F().Electric_brake_min_value;
#if DEBUG_BRAKE_SENT_ORDER
      Serial.println("getBrakeFromLCD - 1 - brakeSentOrder : " + (String)shrd->brakeSentOrder);
#endif
    }

#if DEBUG_DISPLAY_DIGITAL_BRAKE
    Serial.print("Brake released at : ");
    Serial.println(millis());
#endif

    // notify bluetooth
    blh->notifyCommandsFeedback();
  }

  shrd->brakePressedStatusOld = brakePressedStatusFromControllerNew;
  shrd->brakePressedStatus = brakePressedStatusFromControllerNew;

  /*
  char print_buffer[500];
  sprintf(print_buffer, "%s %02x / %s %02x / %s %02x",
          "var",
          var,
          "data_buffer[3]",
          data_buffer[3],
          "brake",
          brake);

  Serial.print("Brake : ");
  Serial.print(print_buffer);
  Serial.println("");
*/

  return brake;
}

uint8_t MinimoUart::modifyBrakeFromDisplay(char var, char data_buffer[])
{

  uint32_t currentTime = millis();

#if DEBUG_BRAKE_SENT_ORDER
  Serial.println("modifyBrakeFromLCD - 1 - brakeSentOrder : " + (String)shrd->brakeSentOrder);
#endif

  // init from LCD brake mode
  if (shrd->brakeSentOrder == -1)
    shrd->brakeSentOrder = var;

  shrd->brakeDisplay = var;

#if DEBUG_BRAKE_SENT_ORDER
  Serial.println("modifyBrakeFromLCD - 2 - brakeSentOrder : " + (String)shrd->brakeSentOrder);
#endif

  if (settings->getS2F().Electric_brake_type == settings->LIST_Electric_brake_type_cntrl)
  {

    // progressive mode
    if ((settings->getS1F().Electric_brake_progressive_mode == 1))
    {
      if ((shrd->brakePressedStatus == 1) && (shrd->brakeFordidenHighVoltage == 0))
      {
        if (shrd->brakeSentOrder < settings->getS1F().Electric_brake_max_value)
        {
          if (currentTime - timeLastBrake > settings->getS1F().Electric_brake_time_between_mode_shift * 5)
          {
            shrd->brakeSentOrder = settings->getS1F().Electric_brake_min_value + 5;
          }
          else if (currentTime - timeLastBrake > settings->getS1F().Electric_brake_time_between_mode_shift * 4)
          {
            shrd->brakeSentOrder = settings->getS1F().Electric_brake_min_value + 4;
          }
          else if (currentTime - timeLastBrake > settings->getS1F().Electric_brake_time_between_mode_shift * 3)
          {
            shrd->brakeSentOrder = settings->getS1F().Electric_brake_min_value + 3;
          }
          else if (currentTime - timeLastBrake > settings->getS1F().Electric_brake_time_between_mode_shift * 2)
          {
            shrd->brakeSentOrder = settings->getS1F().Electric_brake_min_value + 2;
          }
          else if (currentTime - timeLastBrake > settings->getS1F().Electric_brake_time_between_mode_shift * 1)
          {
            shrd->brakeSentOrder = settings->getS1F().Electric_brake_min_value + 1;
          }
        }

        // notify bluetooth
        blh->notifyCommandsFeedback();
      }
      else if (shrd->brakeFordidenHighVoltage == 1)
      // progressive brake enabled but brake released
      {
        shrd->brakeSentOrder = 0;
      }
      else // progressive brake enabled but brake released
      {
        shrd->brakeSentOrder = settings->getS1F().Electric_brake_min_value;
      }
    }
    else
    // progressive brake disabled
    {
      if (shrd->brakeFordidenHighVoltage == 1)
      {
        shrd->brakeSentOrder = 0;
      }
      else
      {
        // take value from display ... (not the best, we should get the last BLE value)
        if (shrd->brakeSentOrderFromBLE >= 0)
          shrd->brakeSentOrder = shrd->brakeSentOrderFromBLE;
        else
          shrd->brakeSentOrder = var;
      }

      // notify brake LCD value
      if (shrd->brakeSentOrder != shrd->brakeSentOrderOld)
      {
        // notify bluetooth
        blh->notifyCommandsFeedback();
      }

#if DEBUG_BRAKE_SENT_ORDER
      Serial.println("modifyBrakeFromLCD - 3 - brakeSentOrder : " + (String)shrd->brakeSentOrder);
#endif

      shrd->brakeSentOrderOld = shrd->brakeSentOrder;

#if DEBUG_BRAKE_SENT_ORDER
      Serial.println("modifyBrakeFromLCD - 4 - brakeSentOrder : " + (String)shrd->brakeSentOrder);
#endif
    }

#if DEBUG_DISPLAY_DIGITAL_BRAKE
    char print_buffer[500];
    sprintf(print_buffer, "%s %02x %s %02x %s %02x %s %d %s %d %s %d",
            "Brake Status : ",
            shrd->brakePressedStatus,
            " / brakeSentOrder  : ",
            shrd->brakeSentOrder,
            " / Current LCD brake  : ",
            var,
            " / timeLastBrake  : ",
            timeLastBrake,
            " / currentTime  : ",
            currentTime,
            " / timeDiff  : ",
            currentTime - timeLastBrake);

    Serial.println(print_buffer);
#endif
  }

#if DEBUG_BRAKE_SENT_ORDER
  Serial.println("modifyBrakeFromLCD - 5 - brakeSentOrder : " + (String)shrd->brakeSentOrder);
#endif

  return shrd->brakeSentOrder;
}

uint8_t MinimoUart::modifyBrakeFromAnalog(char var, char data_buffer[])
{

  //*********************************
  // shrd->brakeSentOrder = var;
  // BUG TO FIX ???
#if DEBUG_BRAKE_SENT_ORDER
  Serial.println("modifyBrakeFromLCD - 1 - modifyBrakeFromAnalog : " + (String)shrd->brakeSentOrder);
#endif

  shrd->brakeSentOrder = settings->getS1F().Electric_brake_min_value;
#if DEBUG_BRAKE_SENT_ORDER
  Serial.println("modifyBrakeFromLCD - 2 - modifyBrakeFromAnalog : " + (String)shrd->brakeSentOrder);
#endif

  if (settings->getS1F().Electric_brake_progressive_mode == 1)
  {

    uint32_t step = 0;
    uint32_t diff = 0;
    uint32_t diffStep = 0;

    if (settings->getS1F().Electric_brake_max_value - settings->getS1F().Electric_brake_min_value > 0)
    {
      step = (shrd->brakeMaxPressureRaw - shrd->brakeMinPressureRaw) / (settings->getS1F().Electric_brake_max_value - settings->getS1F().Electric_brake_min_value);

      if (shrd->brakeFilterMeanErr > shrd->brakeMinPressureRaw)
      {

        diff = shrd->brakeFilterMeanErr - shrd->brakeMinPressureRaw;
        diffStep = diff / step;
#if DEBUG_BRAKE_SENT_ORDER
        Serial.println("modifyBrakeFromLCD - 3 - modifyBrakeFromAnalog : " + (String)shrd->brakeSentOrder);
#endif

        shrd->brakeSentOrder = diffStep + settings->getS1F().Electric_brake_min_value;
#if DEBUG_BRAKE_SENT_ORDER
        Serial.println("modifyBrakeFromLCD - 4 - modifyBrakeFromAnalog : " + (String)shrd->brakeSentOrder);
#endif
      }
    }

#if DEBUG_DISPLAY_ANALOG_BRAKE

    char print_buffer[500];
    sprintf(print_buffer, "brakeFilter : %d / brakePercent : %d / brakeSentOrder : %d  / brakeSentOrderOld : %d / shrd->brakeStatus : %d / step : %d ",
            shrd->brakeFilterMeanErr,
            shrd->brakePercent,
            shrd->brakeSentOrder,
            shrd->brakeSentOrderOld,
            shrd->brakePressedStatus,
            step);
    blh->notifyBleLogs(print_buffer);

    Serial.println(print_buffer);

#endif
  }

  return shrd->brakeSentOrder;
}

uint8_t MinimoUart::modifyEco(char var, char data_buffer[])
{

  if (MINIMO_SIMULATED_DISPLAY == 0)
    shrd->ecoLcd = var;
  else
    var = shrd->ecoOrder;

  // override Smartphone mode with LCD mode
  if (shrd->ecoLcd != shrd->ecoLcdOld)
  {
    shrd->ecoOrder = shrd->ecoLcd;
    shrd->ecoLcdOld = shrd->ecoLcd;

    // notify bluetooth
    blh->notifyCommandsFeedback();
  }

#if DEBUG_DISPLAY_ECO
  char print_buffer[500];
  sprintf(print_buffer, "%s %02x",
          "Eco Status : ",
          var);

  Serial.println(print_buffer);
#endif

  return var;
}

uint8_t MinimoUart::modifyAccel(char var, char data_buffer[])
{

  if (MINIMO_SIMULATED_DISPLAY == 0)
    shrd->accelOrder = var;
  else
    var = shrd->accelOrder;

  // override Smartphone mode with LCD mode
  if (shrd->accelLcd != shrd->accelLcdOld)
  {
    shrd->accelOrder = shrd->accelLcd;
    shrd->accelLcdOld = shrd->accelLcd;

    // notify bluetooth
    blh->notifyCommandsFeedback();

    /*
    Serial.print("Accel ==> notify new accelOrder : ");
    Serial.println(accelOrder);
*/
  }

#if DEBUG_DISPLAY_ACCEL
  char print_buffer[500];
  sprintf(print_buffer, "%s %02x",
          "Accel Status : ",
          var);

  Serial.println(print_buffer);
#endif

  return var;
}

double MinimoUart::getSpeed()
{
  uint8_t high1 = (data_buffer_cntrl_ori[7] - data_buffer_cntrl_ori[3]) & 0xff;
  uint8_t offset_regul = (data_buffer_cntrl_ori[5] - data_buffer_cntrl_ori[3]) & 0xff;
  uint8_t high2 = (high1 - offset_regul) & 0xff;
  uint8_t low = (data_buffer_cntrl_ori[8] - data_buffer_cntrl_ori[3]);

  double speed = (((int)high2 * 256) + (low));
  speed = RpmToKmh(settings, speed);

  computeDistance(speed);

  // eject error values
  if (speed > 150)
    return shrd->speedOld;

  return speed;
}

uint16_t MinimoUart::generateSpeedRawValue(double speed)
{
  uint16_t rawValue;

  /*
  double wheelFactor = (settings->getS1F().Wheel_size / 10.0);
  double polesFactor = settings->getS1F().Motor_pole_number * 10.5;
  rawValue = (uint16_t)(speed / wheelFactor * polesFactor);
*/

  rawValue = KmhToRpm(settings, speed);

  return rawValue;
}

uint8_t MinimoUart::modifySpeed(char var, char data_buffer[], uint8_t byte)
{

  // LCD Speed adjustement
  if ((settings->getS1F().LCD_Speed_adjustement != 0) || (shrd->speedLimiter == 1))
  {
    double speedToProcess = shrd->speedOld * ((settings->getS1F().LCD_Speed_adjustement + 100) / 100.0);

    if ((shrd->speedLimiter == 1) && (speedToProcess > settings->getS1F().Speed_limiter_max_speed))
    {
      speedToProcess = 25;
    }

    uint16_t rawSpeed = generateSpeedRawValue(speedToProcess);

    uint8_t low = rawSpeed & 0xff;
    uint8_t high = (rawSpeed >> 8) & 0xff;

    uint8_t regulatorOffset = (data_buffer_cntrl_ori[5] - data_buffer_cntrl_ori[3]) & 0xff;

    /*
    char print_buffer[500];
    sprintf(print_buffer, "speed : %.2f / reg %02x / mc %02x %02x / oc %02x %02x / md %02x %02x / od %02x %02x %02x %02x %02x %02x ",
            speedToProcess,

            regulatorOffset,

            (((high + regulatorOffset) & 0xff) + data_buffer_cntrl_ori[3]) & 0xff,
            (low + data_buffer_cntrl_ori[3]) & 0xff,

            data_buffer_cntrl_ori[7],
            data_buffer_cntrl_ori[8],

            high,
            low,

            (data_buffer_cntrl_ori[3] - data_buffer_cntrl_ori[3]) & 0xff,
            (data_buffer_cntrl_ori[4] - data_buffer_cntrl_ori[3]) & 0xff,
            (data_buffer_cntrl_ori[5] - data_buffer_cntrl_ori[3]) & 0xff,
            (data_buffer_cntrl_ori[6]) & 0xff,
            (data_buffer_cntrl_ori[7] - data_buffer_cntrl_ori[3]) & 0xff,
            (data_buffer_cntrl_ori[8] - data_buffer_cntrl_ori[3]) & 0xff);
    Serial.print(print_buffer);
    */

    if (byte == 0)
      return (((high + regulatorOffset) & 0xff) + data_buffer_cntrl_ori[3]) & 0xff;
    else
      return (low + data_buffer_cntrl_ori[3]) & 0xff;
  }
  return var;
}

//////------------------------------------
//////------------------------------------
////// Serial link functions

bool MinimoUart::getSerialStatusOkFromContrl()
{
  if (timeLastValidFrameFromCntrl + 500 > millis())
  {
    return true;
  }
  return false;
}

bool MinimoUart::getSerialStatusOkFromLcd()
{
  if (timeLastValidFrameFromLcd + 500 > millis())
  {
    return true;
  }
  return false;
}

bool MinimoUart::getContrlStatusOk()
{
  return cntrlInError;
}

void MinimoUart::readHardSerial(int mode, int *i, Stream *hwSerCntrl, Stream *hwSerLcd, int serialMode, char *data_buffer_ori, char *data_buffer_mod)
{

  byte var;
  Stream *ss_in, *ss_out;
  if (mode == MODE_LCD_TO_CNTRL)
  {
    ss_in = hwSerLcd;
    ss_out = hwSerCntrl;
  }
  else
  {
    ss_in = hwSerCntrl;
    ss_out = hwSerLcd;
  }

  if (((MINIMO_SIMULATED_DISPLAY == 0) && (ss_in->available() > 0)) ||                                      // normal mode
      ((MINIMO_SIMULATED_DISPLAY == 1) && (ss_in->available() > 0) && (serialMode == MODE_CNTRL_TO_LCD)) || // simulated display but read serial on controller
      ((MINIMO_SIMULATED_DISPLAY == 1) && (serialMode == MODE_LCD_TO_CNTRL)))                               // simulated display and bypass serial data availability
  {

    // use real serial data or simulated data
    if (MINIMO_SIMULATED_DISPLAY == 1 && (serialMode == MODE_LCD_TO_CNTRL))
    {
      var = simulatedFrameFromLcd[*i];
    }
    else
    {
      var = ss_in->read();
    }

    data_buffer_ori[*i] = var;

    /*
    if (serialMode == MODE_LCD_TO_CNTRL)
    {
      Serial.printf("var = %02x / *i = %d\n", var, *i);
    }
*/

    // LCD -> CNTRL
    if (serialMode == MODE_LCD_TO_CNTRL)
    {
      if ((var == MODE_LCD_TO_CNTRL_START_BYTE) && (begin_LcdToCntrl == 1))
      {
        begin_LcdToCntrl = 0;

        char log[] = PSTR(" ===> detect begin 0xAA / MODE_LCD_TO_CNTRL");
        Serial.println(log);
        blh->notifyBleLogs(log);

        *i = 0;
      }
    }
    else
    // CNTRL -> LCD
    {
      if ((var == MODE_CNTRL_TO_LCD_START_BYTE) && (begin_CntrlToLcd == 1))
      {

        begin_CntrlToLcd = 0;

        char log[] = PSTR(" ===> detect begin 0x36 / MODE_CNTRL_TO_LCD");
        Serial.println(log);
        blh->notifyBleLogs(log);

        *i = 0;
      }
    }

    //---------------------
    // MODIFY LCD_TO_CNTRL
#if ALLOW_LCD_TO_CNTRL_MODIFICATIONS
    if ((!begin_LcdToCntrl) && (serialMode == MODE_LCD_TO_CNTRL))
    {
      if (*i == 5)
      {

        var = modifyMode(var, data_buffer_ori);
        isModified_LcdToCntrl = 1;
      }

      if (*i == 6)
      {

        var = modifyPas(var, data_buffer_ori);
        isModified_LcdToCntrl = 1;
      }

      if (*i == 7)
      {
        var = modifyPower(var, data_buffer_ori);
        isModified_LcdToCntrl = 1;
      }

      if (*i == 10)
      {

        if (settings->getS2F().Electric_brake_type == settings->LIST_Electric_brake_type_cntrl)
        {
          var = modifyBrakeFromDisplay(var, data_buffer_ori);
        }
        else if ((settings->getS2F().Electric_brake_type == settings->LIST_Electric_brake_type_smart_analog) ||
                 (settings->getS2F().Electric_brake_type == settings->LIST_Electric_brake_type_smart_digital))
        {
          var = modifyBrakeFromAnalog(var, data_buffer_ori);
        }

        isModified_LcdToCntrl = 1;
      }

      if (*i == 11)
      {
        var = modifyEco(var, data_buffer_ori);
        isModified_LcdToCntrl = 1;
      }

      if (*i == 12)
      {
        var = modifyAccel(var, data_buffer_ori);
        isModified_LcdToCntrl = 1;
      }
    }
#endif

    //---------------------
    // MODIFY CNTRL_TO_LCD

    if ((!begin_CntrlToLcd) && (serialMode == MODE_CNTRL_TO_LCD))
    {
      if (*i == 4)
      {
        if (settings->getS2F().Electric_brake_type == settings->LIST_Electric_brake_type_cntrl)
        {
          getBrakeFromCntrlFrame(var, data_buffer_ori);
        }
      }

      if (*i == 5)
      {
        cntrlInError = isContrlInError(var, data_buffer_ori);
      }

      if (*i == 6)
      {
        //getError(var);
      }

      // modify speed
      if (*i == 7)
      {
        var = modifySpeed(var, data_buffer_mod, 0);

        isModified_CntrlToLcd = 1;
      }

      // modify speed
      if (*i == 8)
      {
        shrd->speedCurrent = getSpeed();

#if ALLOW_CNTRL_TO_LCD_MODIFICATIONS
        var = modifySpeed(var, data_buffer_mod, 1);
#endif

        shrd->speedOld = shrd->speedCurrent;

        if (shrd->speedCurrent > shrd->speedMax)
          shrd->speedMax = shrd->speedCurrent;

        isModified_CntrlToLcd = 1;
      }

      if (*i == 10)
      {
#if ALLOW_CNTRL_TO_LCD_MODIFICATIONS
//        var = data_buffer_cntrl_ori[3];
//        isModified_CntrlToLcd = true;
#endif
      }
    }

    // GENERATE CHECKSUM
    if (*i == 14)
    {

      if ((isModified_LcdToCntrl == 1) && (serialMode == MODE_LCD_TO_CNTRL))
      {
        var = getCheckSum(data_buffer_mod);

#if DEBUG_SERIAL_CHECKSUM_LCD_TO_CNTRL
        char print_buffer[500];
        sprintf(print_buffer, "%02x",
                var);

        Serial.print(" ===> modified checksum LCD_TO_CNTRL : ");
        Serial.println(print_buffer);
#endif

        isModified_LcdToCntrl = 0;
      }
      else if ((isModified_CntrlToLcd == 1) && (serialMode == MODE_CNTRL_TO_LCD))
      {
        var = getCheckSum(data_buffer_mod);

#if DEBUG_SERIAL_CHECKSUM_CNTRL_TO_LCD
        char print_buffer[500];
        sprintf(print_buffer, "%02x",
                var);

        Serial.print(" ===> modified checksum CNTRL_TO_LCD : ");
        Serial.println(print_buffer);
#endif

        isModified_CntrlToLcd = 0;
      }
    }

    data_buffer_mod[*i] = var;

    uint32_t time = millis();

    ss_out->write(var);

    //Serial.println("readHardSerial -- step4 / serialMode = " + (String)serialMode + " / i = " + (String)*i + " / millis = " + (String)(millis() - time));

    // display
    if (*i == 14)
    {

      // compute new checksum with original data
      uint8_t checksum = 0;

      // if simulated, compute check on moded datas and for frame checksum to bypass test
      if ((MINIMO_SIMULATED_DISPLAY == 1) && (serialMode == MODE_LCD_TO_CNTRL))
      {
        checksum = getCheckSum(data_buffer_mod);
        data_buffer_ori[14] = checksum;
      }
      else
      {
        checksum = getCheckSum(data_buffer_ori);
      }

      // display datas
      if (serialMode == MODE_CNTRL_TO_LCD)
      {
#if DEBUG_BLE_DISPLAY_FRAME
        notifyBleLogFrame(serialMode, data_buffer_mod, checksum);
#endif
#if DEBUG_DISPLAY_FRAME_CNTRL_TO_LCD
        //        displayFrame(serialMode, data_buffer_mod, checksum);
        displayFrame(serialMode, data_buffer_ori, checksum);
#endif
#if DEBUG_DISPLAY_DECODED_FRAME_CNTRL_TO_LCD
        //displayDecodedFrame(serialMode, data_buffer_ori, checksum);
        displayDecodedFrame(serialMode, data_buffer_mod, checksum);
//        Serial.println("");
#endif
#if DEBUG_DISPLAY_SPEED
        displaySpeed();
#endif

        /*
        char print_buffer[500];
        sprintf(print_buffer, " %02x %02x %02x %02x %02x ",

                (data_buffer_cntrl_ori[9] - data_buffer_cntrl_ori[3]) & 0xff,
                (data_buffer_cntrl_ori[10] - data_buffer_cntrl_ori[3]) & 0xff,
                (data_buffer_cntrl_ori[11] - data_buffer_cntrl_ori[3]) & 0xff,
                (data_buffer_cntrl_ori[12] - data_buffer_cntrl_ori[3]) & 0xff,
                (data_buffer_cntrl_ori[13] - data_buffer_cntrl_ori[3]) & 0xff);
        Serial.println(print_buffer);
        */
      }
      else
      {
#if DEBUG_BLE_DISPLAY_FRAME
        notifyBleLogFrame(serialMode, data_buffer_mod, checksum);
#endif
#if DEBUG_DISPLAY_FRAME_LCD_TO_CNTRL
        displayFrame(serialMode, data_buffer_ori, checksum);
#endif
#if DEBUG_DISPLAY_MODIFIED_FRAME_LCD_TO_CNTRL
        Serial.print("ori : ");
        displayFrame(serialMode, data_buffer_ori, checksum);
        Serial.print("mod : ");
        displayFrame(serialMode, data_buffer_mod, checksum);
#endif
      }

      // check comptued checksum vs original checksum to detect frames error and reset the begin detectio
      if (checksum != data_buffer_ori[14])
      {

        if (serialMode == MODE_CNTRL_TO_LCD)
        {
          char log[] = "====> CHECKSUM error MODE_CNTRL_TO_LCD ==> reset ";
          Serial.println(log);
          blh->notifyBleLogs(log);

          begin_CntrlToLcd = 1;
        }
        else
        {
          char log[] = "====> CHECKSUM error MODE_LCD_TO_CNTRL ==> reset ";
          Serial.println(log);
          blh->notifyBleLogs(log);

          begin_LcdToCntrl = 1;
        }
      }
      else
      {

        if (serialMode == MODE_CNTRL_TO_LCD)
        {
          timeLastValidFrameFromCntrl = millis();
        }
        else
        {
          timeLastValidFrameFromLcd = millis();
        }
      }

      *i = 0;
    }
    else
    {
      (*i)++;
    }
  }
}

void MinimoUart::processMinimotorsSerial(uint32_t i_loop, boolean simulatedDisplay)
{

  if ((MINIMO_SIMULATED_DISPLAY == 0) || ((i_loop % 200 >= 0) && (i_loop % 200 <= 14)))
  {
    if (MINIMO_SIMULATED_DISPLAY == 1)
    {
      if (i_loop % 200 == 0)
      {
        iMsgLcdToCntrl++;
        if (iMsgLcdToCntrl > 255)
        {
          iMsgLcdToCntrl = 0;
        }
        simulatedFrameFromLcd[2] = iMsgLcdToCntrl;
      }
    }

    readHardSerial(MODE_LCD_TO_CNTRL, &i_LcdRcv, hwSerCntrl, hwSerLcd, MODE_LCD_TO_CNTRL, data_buffer_lcd_ori, data_buffer_lcd_mod);
  }

  readHardSerial(MODE_CNTRL_TO_LCD, &i_CntrlRcv, hwSerCntrl, hwSerLcd, MODE_CNTRL_TO_LCD, data_buffer_cntrl_ori, data_buffer_cntrl_mod);
}

