/******************************************************************************/
#include <PubSubClient.h>

#include "include/system.h"
#include "include/system_defs.h"

#include "../App/peripheral/include/peripheral_controller.h"
#include "../lib/wifi/include/my_wifi.h"
#include "../lib/mqtt/include/mqtt.h"

extern PubSubClient MQTT;
/******************************************************************************/
void System_Init(void *params)
{
    /* Configure IO */
    Peripheral_Init(NULL);
    
    /* Configure WIFI */
    Wifi_Init(NULL);

    /* Configure MQTT */
    #if MQTT_ENABLE == true
        Mqtt_Start(NULL);
    #endif /* MQTT_ENABLE */

    // System_CreateTasks(tasks, sizeof(tasks));
}
/******************************************************************************/