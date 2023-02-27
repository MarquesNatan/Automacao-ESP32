/******************************************************************************/
#include <PubSubClient.h>

#include "freertos/queue.h"

#include "include/system.h"
#include "include/system_defs.h"

#include "../App/peripheral/include/peripheral_controller.h"
#include "../lib/commands/include/command.h"
#include "../lib/wifi/include/my_wifi.h"
#include "../lib/mqtt/include/mqtt.h"
#include "../lib/RTC/include/rtc.h"
#include "../lib/interrupt/include/interrupt.h"
/******************************************************************************/
extern PubSubClient MQTT;
extern QueueHandle_t commQueue;
/******************************************************************************/
void System_Init(void *params)
{
    /* Configure IO */
    Peripheral_Init(NULL);

    /* Start Command queue */
    Command_CreateQueue(10, &commQueue);

    /* Configure WIFI */
    Wifi_Init(NULL);

    /* Configure MQTT */
    #if MQTT_ENABLE == true
        Mqtt_Start(NULL);
    #endif /* MQTT_ENABLE */

    /* Start RTC */
    // RTC_Init(NULL);

    /* Configure handler to Interrtups */
    Interrupt_Config(NULL);
}
/******************************************************************************/