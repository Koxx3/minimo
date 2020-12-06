#include "MinimoUart.h"
#include "SerialMode.h"
#include "main.h"
#include "debug.h"

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

void MinimoUart::setPID(PID *pidSpeed_p)
{
  pidSpeed = pidSpeed_p;
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

void MinimoUart::displayMode(char data_buffer[])
{

  uint32_t byteDiff = (data_buffer[5] - data_buffer[2]);
  uint8_t modeLcd = byteDiff & 0x03;
  uint8_t modeLcd2 = (byteDiff >> 2) & 0x1;
  uint8_t modeLcd3 = (byteDiff >> 3) & 0x1;
  uint8_t modeLcd4 = (byteDiff >> 4) & 0x1;
  uint8_t modeLcd5 = (byteDiff >> 5) & 0x1;
  uint8_t modeLcd6 = (byteDiff >> 6) & 0x1;
  uint8_t modeLcd7 = (byteDiff >> 7) & 0x1;

  char print_buffer[500];
  sprintf(print_buffer, "%02x %02x / %02x / %02x / %02x / %02x / %02x / %02x / %02x", data_buffer[2], data_buffer[5], modeLcd, modeLcd2, modeLcd3, modeLcd4, modeLcd5, modeLcd6, modeLcd7);

#if DEBUG_DISPLAY_MODE
  Serial.print("LCD mode : ");
  Serial.print(print_buffer);
  Serial.println("");
#endif
}

uint8_t MinimoUart::modifyModeOld(char var, char data_buffer[])
{
  uint32_t byteDiff = (var - data_buffer[2]);
  uint8_t modeLcd = byteDiff & 0x03;
  uint8_t modeLcdMask = byteDiff & 0xfc;
  uint8_t newmodeLcd2 = shrd->modeOrder | modeLcdMask;
  uint32_t newmodeLcd3 = (newmodeLcd2 + data_buffer[2]) & 0xff;

  char print_buffer[500];
  /*
  sprintf(print_buffer, "%02x %02x / %s %02x / %s %02x / %s %02x / %s %02x  / %s %02x  / %s %02x ",
          data_buffer[2],
          var,
          "byteDiff",
          byteDiff,
          "lcd",
          modeLcd,
          "mask",
          modeLcdMask,
          "order",
          orderMode,
          "newmodeLcd2",
          newmodeLcd2,
          "newmodeLcd3",
          newmodeLcd3);
          */

  sprintf(print_buffer, "%s %02x / %s %02x",
          "lcd",
          modeLcd,
          "order",
          shrd->modeOrder);

  Serial.print("LCD mode : ");
  Serial.print(print_buffer);
  Serial.println("");

  return newmodeLcd3;
}

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

uint8_t MinimoUart::modifyMode(char var, char data_buffer[])
{
  uint8_t newModeLcd;

  uint32_t byteDiff = (var - data_buffer[2]);
  uint8_t modeLcd = (byteDiff & 0x03) + 1;

  // override Smartphone mode with LCD mode
  if (shrd->modeLcdOld != modeLcd)
  {
    shrd->modeOrder = modeLcd;
    shrd->modeLcdOld = modeLcd;

    // notify bluetooth
    blh->notifyModeOrder(shrd->modeOrder);
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
    blh->notifyModeOrder(shrd->modeOrder);

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

      blh->notifyModeOrder(shrd->modeOrder);

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

  // lock escooter by reducing power to 5%
  if (blh->bleLockStatus == true)
  {
    // REMINDER : never put bellow 5
    newPower = 5;
  }
  else if (shrd->speedLimiter == 1)
  {
    //    newPower = 37;

    // PID regulation for 25 km/h
    newPower = shrd->pidOutput / 2.55;
    if (newPower < 5)
    {
      newPower = 5;
    }

#if DEBUG_DISPLAY_SPEED_PID
    Serial.print(" / output = ");
    Serial.print(shrd->pidOutput);
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

uint8_t MinimoUart::getBrakeFromLCD(char var, char data_buffer[])
{

  uint8_t brake = (var - data_buffer[3]) & 0x20;
  uint8_t brakeStatusFromLcdNew = brake >> 5;

  shrd->brakeLcd = var;

  //uint8_t brakeStatusFromLcdNew = brakeStatus;
  if ((brakeStatusFromLcdNew == 1) && (shrd->brakeStatusOld == 0))
  {
    shrd->brakeStatus = brakeStatusFromLcdNew;
    timeLastBrake = millis();

#if DEBUG_DISPLAY_DIGITAL_BRAKE
    Serial.print("Brake pressed at : ");
    Serial.println(timeLastBrake);
#endif

    // notify bluetooth
    blh->notifyBreakeSentOrder(shrd->brakeSentOrder, shrd->brakeStatus, shrd->brakeFordidenHighVoltage);
  }
  else if ((brakeStatusFromLcdNew == 0) && (shrd->brakeStatusOld == 1))
  {
    shrd->brakeStatus = brakeStatusFromLcdNew;

    // reset to min
    shrd->brakeSentOrder = settings->getS1F().Electric_brake_min_value;

#if DEBUG_DISPLAY_DIGITAL_BRAKE
    Serial.print("Brake released at : ");
    Serial.println(millis());
#endif

    // notify bluetooth
    blh->notifyBreakeSentOrder(shrd->brakeSentOrder, shrd->brakeStatus, shrd->brakeFordidenHighVoltage);
  }

  shrd->brakeStatusOld = brakeStatusFromLcdNew;
  shrd->brakeStatus = brakeStatusFromLcdNew;

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

uint8_t MinimoUart::modifyBrakeFromLCD(char var, char data_buffer[])
{

  uint32_t currentTime = millis();

  // init from LCD brake mode
  if (shrd->brakeSentOrder == -1)
    shrd->brakeSentOrder = var;

  if (settings->getS2F().Electric_brake_type == settings->LIST_Electric_brake_type_digital)
  {

    // progressive mode
    if (settings->getS1F().Electric_brake_progressive_mode == 1)
    {
      if (shrd->brakeStatus == 1)
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
        blh->notifyBreakeSentOrder(shrd->brakeSentOrder, shrd->brakeStatus, shrd->brakeFordidenHighVoltage);
      }
      else
      // progressive brake enabled but brake released
      {
        shrd->brakeSentOrder = settings->getS1F().Electric_brake_min_value;
      }
    }
    else
    // progressive brake disabled
    {

      // notify brake LCD value
      if (shrd->brakeSentOrder != shrd->brakeSentOrderOld)
      {
        // notify bluetooth
        blh->notifyBreakeSentOrder(shrd->brakeSentOrder, shrd->brakeStatus, shrd->brakeFordidenHighVoltage);
      }

      shrd->brakeSentOrderOld = shrd->brakeSentOrder;
    }

#if DEBUG_DISPLAY_DIGITAL_BRAKE
    char print_buffer[500];
    sprintf(print_buffer, "%s %02x %s %02x %s %02x %s %d %s %d %s %d",
            "Brake Status : ",
            shrd->brakeStatus,
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

  return shrd->brakeSentOrder;
}

uint8_t MinimoUart::modifyEco(char var, char data_buffer[])
{

  shrd->ecoLcd = var;
  var = shrd->ecoOrder;

  // override Smartphone mode with LCD mode
  if (shrd->ecoLcd != shrd->ecoLcdOld)
  {
    shrd->ecoOrder = shrd->ecoLcd;
    shrd->ecoLcdOld = shrd->ecoLcd;

    // notify bluetooth
    blh->notifyEcoOrder(shrd->ecoOrder);
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

  shrd->accelLcd = var;
  var = shrd->accelOrder;

  // override Smartphone mode with LCD mode
  if (shrd->accelLcd != shrd->accelLcdOld)
  {
    shrd->accelOrder = shrd->accelLcd;
    shrd->accelLcdOld = shrd->accelLcd;

    // notify bluetooth
    blh->notifyAccelOrder(shrd->accelOrder);

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
  speed = speed * (settings->getS1F().Wheel_size / 10.0) / settings->getS1F().Motor_pole_number / 10.5;

  computeDistance(speed);

  // eject error values
  if (speed > 150)
    return shrd->speedOld;

  return speed;
}

uint16_t MinimoUart::generateSpeedRawValue(double speed)
{
  uint16_t rawValue;
  double wheelFactor = (settings->getS1F().Wheel_size / 10.0);
  double polesFactor = settings->getS1F().Motor_pole_number * 10.5;
  rawValue = (uint16_t)(speed / wheelFactor * polesFactor);

  rawValue = rawValue;

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
      speedToProcess = settings->getS1F().Speed_limiter_max_speed;
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

int MinimoUart::readHardSerial(int mode, int i, Stream *hwSerCntrl, Stream *hwSerLcd, int serialMode, char *data_buffer_ori, char *data_buffer_mod)
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

  if (ss_in->available() > 0)
  {

    var = ss_in->read();
    data_buffer_ori[i] = var;

    // LCD -> CNTRL
    if (serialMode == MODE_LCD_TO_CNTRL)
    {
      if ((var == MODE_LCD_TO_CNTRL_START_BYTE) && (begin_LcdToCntrl == 1))
      {
        begin_LcdToCntrl = 0;

        char log[] = PSTR(" ===> detect begin 0xAA / MODE_LCD_TO_CNTRL");
        Serial.println(log);
        blh->notifyBleLogs(log);

        i = 0;
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

        i = 0;
      }
    }

    //---------------------
    // MODIFY LCD_TO_CNTRL
#if ALLOW_LCD_TO_CNTRL_MODIFICATIONS
    if ((!begin_LcdToCntrl) && (serialMode == MODE_LCD_TO_CNTRL))
    {
      if (i == 5)
      {

        var = modifyMode(var, data_buffer_ori);
        isModified_LcdToCntrl = 1;
      }

      if (i == 7)
      {
        var = modifyPower(var, data_buffer_ori);
        isModified_LcdToCntrl = 1;
      }

      if (i == 10)
      {
        //        var = modifyBrakeFromLCD(var, data_buffer_ori);
        var = modifyBrakeFromAnalog(var, data_buffer_ori);
        isModified_LcdToCntrl = 1;
      }

      if (i == 11)
      {
        var = modifyEco(var, data_buffer_ori);
        isModified_LcdToCntrl = 1;
      }

      if (i == 12)
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
      if (i == 4)
      {
        //getBrakeFromLCD(var, data_buffer_ori);
      }

      // modify speed
      if (i == 7)
      {
        var = modifySpeed(var, data_buffer_mod, 0);

        isModified_CntrlToLcd = 1;
      }

      // modify speed
      if (i == 8)
      {
        shrd->speedCurrent = getSpeed();

#if ALLOW_CNTRL_TO_LCD_MODIFICATIONS
        shrd->pidInput = shrd->speedCurrent;
        pidSpeed->Compute();

#if DEBUG_DISPLAY_SPEED_PID
        Serial.print("Input = ");
        Serial.print(shrd->pidInput);
#endif

        var = modifySpeed(var, data_buffer_mod, 1);
#endif

        shrd->speedOld = shrd->speedCurrent;

        if (shrd->speedCurrent > shrd->speedMax)
          shrd->speedMax = shrd->speedCurrent;

        isModified_CntrlToLcd = 1;
      }

      if (i == 10)
      {
#if ALLOW_CNTRL_TO_LCD_MODIFICATIONS
//        var = data_buffer_cntrl_ori[3];
//        isModified_CntrlToLcd = true;
#endif
      }
    }

    // GENERATE CHECKSUM
    if (i == 14)
    {

      if ((isModified_LcdToCntrl == 1) && (serialMode == MODE_LCD_TO_CNTRL))
      {
        var = getCheckSum(data_buffer_mod);

#if DEBUG_SERIAL_CHECKSUM_LCD_TO_CNTRL
        char print_buffer[500];
        sprintf(print_buffer, "%02x %02x",
                oldChecksum,
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
        sprintf(print_buffer, "%02x %02x",
                oldChecksum,
                var);

        Serial.print(" ===> modified checksum CNTRL_TO_LCD : ");
        Serial.println(print_buffer);
#endif

        isModified_CntrlToLcd = 0;
      }
    }

    data_buffer_mod[i] = var;

    ss_out->write(var);

    // display
    if (i == 14)
    {

      // Check original checksum
      uint8_t checksum = getCheckSum(data_buffer_ori);

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
#if DEBUG_DISPLAY_MODE
        displayMode(data_buffer_mod);
#endif
      }

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

      i = 0;
    }
    else
    {
      i++;
    }
  }

  return i;
}

void MinimoUart::processMinimotorsSerial()
{
  i_LcdRcv = readHardSerial(MODE_LCD_TO_CNTRL, i_LcdRcv, hwSerCntrl, hwSerLcd, MODE_LCD_TO_CNTRL, data_buffer_lcd_ori, data_buffer_lcd_mod);

  i_CntrlRcv = readHardSerial(MODE_CNTRL_TO_LCD, i_CntrlRcv, hwSerCntrl, hwSerLcd, MODE_CNTRL_TO_LCD, data_buffer_cntrl_ori, data_buffer_cntrl_mod);
}