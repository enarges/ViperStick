/*
  Viperstick

  Pi 2040 based flight stick for the Jflyer81 F-16 sidestick

  Supports:
  1 5 way Hat (Trim)
  3 4 way hats (CMS,TMS, NWS)
  2 Analog axes
  6 Buttons (pinkie, paddle, pickle, 2 stage trigger, nws)

  Routed through a chained 74HC165 ISR.


  Using:
  https://www.gitlab.com/realrobots/PicoGamepad
*/

#include "viper/viper.h"

stick::Viper m_stick;

/******************************************************************************
 * Setup Routine
 * Setup of Serial IO, Input & Output Pins, etc.
 *
 *****************************************************************************/
void setup()
{
  Serial.begin(115200);

  m_stick.setup();

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  static unsigned long previousTime = 0 ;
  unsigned long currentTime = millis();
  if (currentTime - previousTime >= 1000)
  {
    // readInputsWithDigitalRead();
    previousTime = currentTime;

    // Flash the LED just for fun
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }

  m_stick.readStick();
}
