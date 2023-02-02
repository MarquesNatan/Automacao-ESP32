/******************************************************************************/
#include "include/system.h"
#include "../lib/wifi/include/my_wifi.h"
#include "../lib/mqtt/include/mqtt.h"
/******************************************************************************/
void System_Init(void *params)
{
    /* Configure IO */
    // GPIO_Configure(NULL);
    
    /* Configure WIFI */
    Serial.printf("/* Configure WIFI */\n");
    Wifi_Init(NULL);

    /* Configure MQTT */
    // Mqtt_Init("https://eclipseprojects.org.com", "1883");

    // System_CreateTasks(tasks, sizeof(tasks));

}
/******************************************************************************/