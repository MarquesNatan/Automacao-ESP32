/******************************************************************************/
#include <Arduino.h>
#include "system/include/system.h"
#include "system/include/system_defs.h"
#include "../mqtt/include/mqtt.h"

#include <PubSubClient.h>
/******************************************************************************/
extern PubSubClient MQTT;
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
  #if MQTT_ENABLE == true
    while (1)
    {
      if(!MQTT.connected())
      {
        MQTT_tryConnect();
      }

      MQTT.loop();
    }
  #endif /* MQTT_ENABLE */
}
/******************************************************************************/