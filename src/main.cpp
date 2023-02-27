/******************************************************************************/
#include <Arduino.h>
#include "system/include/system.h"
#include "system/include/system_defs.h"
#include "App/main/include/main_app.h"

#include "../mqtt/include/mqtt.h"
#include "../commands/include/command.h"
#include "../lib/RTC/include/rtc.h"

#include <PubSubClient.h>
#include "RTClib.h"
/******************************************************************************/
void setup()
{
  System_Init(NULL);
}
/******************************************************************************/
void loop()
{
  main_app(NULL);
}
/******************************************************************************/