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
extern QueueHandle_t commandQueue;
extern command_packet_t newCommand;
extern volatile unsigned long ISRCount;
extern volatile bool ISRIsTrigged;
extern volatile uint8_t argsValue;
/******************************************************************************/
void main_app(void *params)
{
    xTaskCreate(switchManagerTask, "switch handle", 4096, NULL, 1, NULL);
    xTaskCreate(commandExecuteTask, "command execute", 4096, NULL, 1, NULL);
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
void commandExecuteTask(void *params)
{

    uint8_t isEmpty = 0;
    command_packet_t localCommand;

    while(1)
    {
        isEmpty = xQueueReceive(commandQueue, &localCommand, 0);
        if (isEmpty != errQUEUE_EMPTY || isEmpty != errQUEUE_FULL)
        {
            if(commandIsValid(&localCommand))
            {
                #if COMMAND_DEBUG == true
                    Serial.printf("Comando sem erros, pode ser processado.\n");
                #endif /* COMMAND_DEBUG */   
            }
        }

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
/******************************************************************************/
void switchManagerTask(void *params)
{
    volatile uint8_t pinValue = 0;
    uint8_t newValue = 0;
    uint8_t pin;
    while(true)
    {
        if(ISRIsTrigged)
        {
            ISRIsTrigged = false;
            (argsValue == PIN_SWITCH_0) ? pin = PIN_RELAY_0 : pin = PIN_RELAY_1;

            pinValue = digitalRead(pin);
            (pinValue == 0) ? newValue = 1 : newValue = 0;
            digitalWrite(pin, newValue);

            Serial.printf("ISR count: %i\n", ISRCount);
        }

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
/******************************************************************************/
void rtcHandleTask(void *params)
{
    DateTime now;

    while(1)
    {
        now = RTC_GetTime(NULL);

        Serial.printf("%i/%i/%i - %i:%i:%i\n", now.day(), now.month(), now.year(), 
                                               now.hour(), now.minute(), now.second());
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}
/******************************************************************************/