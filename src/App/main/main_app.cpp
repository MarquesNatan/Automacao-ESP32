/******************************************************************************/
#include "include/main_app.h"
#include <Arduino.h>
#include "../../lib/mqtt/include/mqtt.h"
#include "../../system/include/system_defs.h"
#include "../../lib/commands/include/command.h"
#include "../../lib/RTC/include/rtc.h"

#include "freertos/task.h"
#include "freertos/queue.h"

#include "../relay_manager/include/relay_manager.h"
#include "../../common/include/board_peripheral.h"

#include <PubSubClient.h>
#include <RTClib.h>
/******************************************************************************/
extern PubSubClient MQTT;
extern QueueHandle_t commQueue;
extern volatile unsigned long ISRCount;
extern volatile bool ISRIsTrigged;
/******************************************************************************/
void main_app(void *params)
{
    xTaskCreate(commandHandleTask, "command handle", 4096, NULL, 1, NULL);
    xTaskCreate(switchManagerTask, "switch handle", 4096, NULL, 1, NULL);
    // xTaskCreate(rtcHandleTask, "rtc handle", 4096, NULL, 1, NULL);

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
void commandHandleTask(void *params)
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
void switchManagerTask(void *params)
{
    volatile uint8_t pinValue = 0;
    uint8_t newValue = 0;
    while(true)
    {
        if(ISRIsTrigged)
        {
            ISRIsTrigged = false;
            pinValue = digitalRead(PIN_DIGITAL_OUTPUT_1);
            (pinValue == 0) ? newValue = 1 : newValue = 0;
            digitalWrite(PIN_DIGITAL_OUTPUT_1, newValue);
            Serial.printf("Valor pino: %i | Valor Interrupção: %i\n", newValue, ISRCount);
        }

    }
}
/******************************************************************************/
void rtcHandleTask(void *params)
{
    DateTime now;

    while(1)
    {
        now = RTC_GetTime(NULL);

        Serial.printf("%i/%i/%i - %i:%i:%i\n", now.day(), now.month(), now.year(), now.hour(), now.minute(), now.second());
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}
/******************************************************************************/