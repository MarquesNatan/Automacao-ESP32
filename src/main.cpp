/******************************************************************************/
#include <Arduino.h>
#include "system/include/system.h"
#include "system/include/system_defs.h"
#include "App/main/include/main_app.h"
#include "../../lib/mqtt/include/mqtt.h"

#include <PubSubClient.h>
#include "freertos/task.h"

#include "App/main/include/tasks.h"
/******************************************************************************/
void setup()
{
  System_Init(NULL);
  vTasksCreate( NULL );

}
/******************************************************************************/
void loop()
{
  #if MQTT_ENABLE == true
    vTaskMqttHandleConnection(NULL);
  #endif /* MQTT_ENABLE */
}
/******************************************************************************/