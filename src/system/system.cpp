/******************************************************************************/
#include "include/system.h"
#include "../lib/wifi/include/my_wifi.h"
#include "../lib/mqtt/include/mqtt.h"
#include <PubSubClient.h>

extern PubSubClient MQTT;
/******************************************************************************/
void System_Init(void *params)
{
    /* Configure IO */
    // GPIO_Configure(NULL);
    
    /* Configure WIFI */
    Wifi_Init(NULL);

    /* Configure MQTT */
    Mqtt_Start(NULL);

    // System_CreateTasks(tasks, sizeof(tasks));
}
/******************************************************************************/