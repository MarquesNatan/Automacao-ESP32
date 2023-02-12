/******************************************************************************/
#include <Arduino.h>
#include "system/include/system.h"
#include "system/include/system_defs.h"
#include "App/main/include/main_app.h"

#include "../mqtt/include/mqtt.h"
#include "../commands/include/command.h"

#include <PubSubClient.h>
/******************************************************************************/
void setup()
{
  Serial.begin(115200);
  pinMode(2, OUTPUT);
  /* System Configuration */
  System_Init(NULL);

  /* System task start */
  /* Tasks Create */
}
/******************************************************************************/
void loop()
{
  main_app(NULL);
}
/******************************************************************************/