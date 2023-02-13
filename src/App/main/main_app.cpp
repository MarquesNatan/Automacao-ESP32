/******************************************************************************/
#include "include/main_app.h"
#include "../../lib/mqtt/include/mqtt.h"
#include "../../system/include/system_defs.h"
#include "../../lib/commands/include/command.h"

#include "../relay_manager/include/relay_manager.h"

#include "freertos/task.h"
#include "freertos/queue.h"

#include <PubSubClient.h>
/******************************************************************************/
extern PubSubClient MQTT;
/******************************************************************************/
extern QueueHandle_t commQueue;
bool isValiIndexd(uint8_t index, uint8_t type);
void realy_manager(uint8_t index, uint8_t action);
void MainTask(void *params);
/******************************************************************************/
void main_app(void *params)
{
    /* Create Tasks */
    /*
     * char command;
     * char message[] = "Mensagem de teste:  ";
     * int count = 0;
     * int control = 0;
     */

    xTaskCreate(MainTask, "Main Task", 4096, NULL, 1, NULL);

    while (true)
    {

#if MQTT_ENABLE == true
        while (true)
        {
            if (!MQTT.connected())
            {
                MQTT_tryConnect();
            }

            MQTT.loop();
        }

#endif /* MQTT_ENABLE */
    }
}
/******************************************************************************/
void MainTask(void *params)
{
    /* Manage outputs */
    uint8_t type = 255;
    uint8_t outputNum = 255;
    uint8_t newState = 255;

    command_t command;
    uint8_t isEmpty = 0;

    while (true)
    {

        isEmpty = xQueueReceive(commQueue, &command, 0);
        if (isEmpty != errQUEUE_EMPTY || isEmpty != errQUEUE_FULL)
        {
            type = command.data[0];
            outputNum = command.data[1];
            newState = command.data[2];

            if(type == 0)
            {
                RelayManager(outputNum, newState);
            }
            else 
            {
                // AnalogicManager(index, value);
            }


        }

        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}
/******************************************************************************/