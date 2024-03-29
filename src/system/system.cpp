/******************************************************************************/
#include "include/system.h"
#include "include/system_defs.h"

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#include <PubSubClient.h>

#include "../App/peripheral/include/peripheral_controller.h"

#include "../lib/wifi/include/my_wifi.h"
#include "../lib/mqtt/include/mqtt.h"
/******************************************************************************/
extern PubSubClient MQTT;
extern QueueHandle_t commQueue;
/******************************************************************************/
void System_Init(void *params)
{
    /* Configure IO */
    Peripheral_Init(NULL);
    
    /* Configure WIFI */
    #if WIFI_ENABLE == true
        Wifi_Init(NULL);
    #endif /* WIFI_ENABLE */

    /* Configure MQTT */
    #if MQTT_ENABLE == true
        Mqtt_Start(NULL);
    #endif /* MQTT_ENABLE */
}
/******************************************************************************/
void System_InitializeFirstTime( void * pvParameters )
{
    bool configured = false;
    for(;;)
    {


        if(configured == true)
        {
            break;
        }
    }
    
    
    /* Deleta a tarefa depois de conectado uma única vez */
    vTaskDelete(NULL);
}