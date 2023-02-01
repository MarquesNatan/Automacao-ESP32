/******************************************************************************/
#include "include/system.h"
#include "../lib/wifi/include/wifi.h"
#include "../lib/mqtt/include/mqtt.h"
/******************************************************************************/
void System_Init(void *params)
{
    /* Configure IO */
    // GPIO_Configure(NULL);
    
    /* Configure WIFI */
    Wifi_Init("Metal Pleno 2.4G", "08969712");

    /* Configure MQTT */
    Mqtt_Init("https://eclipseprojects.org.com", "1883");

    // System_CreateTasks(tasks, sizeof(tasks));

}
/******************************************************************************/