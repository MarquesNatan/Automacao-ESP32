/******************************************************************************/
#include "include/main_app.h"
#include "../../lib/mqtt/include/mqtt.h"
#include "../../system/include/system_defs.h"
#include "../../lib/commands/include/command.h"

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
                /* Publicar as temperaturas no Endpoint do sensor */
                /*
                  message[19] = count + '0';
                  MQTT.publish("7db8cbb3-47f8-48a7-8c5a-d0aa81fad54b/tccautomacao/sensor/", message);
                  Serial.printf("%s \n", message);
                  count = count + 1;
                  delay(5000);
                 */
            }

        #endif /* MQTT_ENABLE */
    }
}
/******************************************************************************/
void MainTask(void *params)
{
    /* Manage outputs */
    uint8_t type = 255;
    uint8_t action = 255;
    uint8_t index = 255;

    command_t command;
    uint8_t isEmpty = 0;

    while(true)
    {

        isEmpty = xQueueReceive(commQueue, &command, 0);
        if(isEmpty != errQUEUE_EMPTY || isEmpty != errQUEUE_FULL)
        {
          type = command.data[0];
          index = command.data[1];
          action = command.data[2];

          Serial.printf("Entrando na main task.\n");
            if(isValiIndexd(index, type))
            {
                if(type == 0)
                {
                    realy_manager(index, action);
                    Serial.printf("index: %i  action: %i", index, action);
                }
            }
        }

        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}
/******************************************************************************/
void realy_manager(uint8_t index, uint8_t action)
{
    digitalWrite(index, action); 
}
/******************************************************************************/
bool isValiIndexd(uint8_t index, uint8_t type)
{
    bool result = false;

    if(type == 0)
    {
        (index >= 0 && index <= 5)? result = true : result = false;
    }
    else if(type == 1)
    {
        (index >= 0 && index <= 3)? result = true : result = false;
    }

    return result;
}
/******************************************************************************/
