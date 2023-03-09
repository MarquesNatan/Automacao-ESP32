/******************************************************************************/
#include "include/main_app.h"
#include <Arduino.h>

#include "../../lib/mqtt/include/mqtt.h"
#include "../../system/include/system_defs.h"
#include "../../lib/commands/include/command.h"
#include "../../lib/timer/include/timer.h"
#include "../../lib/RTC/include/rtc.h"

#include "freertos/task.h"
#include "freertos/queue.h"

#include "../relay_manager/include/relay_manager.h"
#include "../../common/include/board_peripheral.h"

#include <PubSubClient.h>
#include <RTClib.h>
/******************************************************************************/
extern PubSubClient MQTT;

extern QueueHandle_t commandQueue;
extern command_packet_t newCommand;
extern volatile unsigned long ISRCount;
extern volatile bool ISRIsTrigged;
extern volatile uint8_t argsValue;

extern xSemaphoreHandle ZOHtrigged;
extern xSemaphoreHandle Semph_Timer0_ISR;
extern hw_timer_t *Timer0_Cfg;
/******************************************************************************/
void main_app(void *params)
{
    ZOHtrigged = xSemaphoreCreateBinary();

    /* Inicia o Semaforo do Timer */
    Semph_Timer0_ISR = xSemaphoreCreateBinary();

    Timer0_Cfg = timerBegin(0, 80, true);
    timerAttachInterrupt(Timer0_Cfg, &Timer0_ISRHandle, true);

    // xTaskCreate(switchManagerTask, "switch handle", 4096, NULL, 1, NULL);
    // xTaskCreate(commandExecuteTask, "command execute", 4096, NULL, 1, NULL);
    xTaskCreate(DimmerInputReadTask, "dimmer input", 4096, NULL, 1, NULL);

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
void DimmerInputReadTask(void *params)
{
    /* Atualiza o valor lido no ADC do ESP32 a cada 120 ticks do processador */
    int adcRead = 0;
    uint64_t delayEnablePulse = 0;
    int aux = 0;

    while(true)
    {
        if(xSemaphoreTake(ZOHtrigged, portMAX_DELAY) == pdTRUE)
        {

            adcRead = analogRead(36);

            if(adcRead <= 250)
            {
                digitalWrite(2, LOW);
            }
            else if(adcRead > 250 && adcRead <= 3500)
            {   
                delayEnablePulse = (int)(adcRead * 2.46);
                delayEnablePulse = 8333 - delayEnablePulse;

                /* Inicia a estrutura do Timer */  
                timerAlarmWrite(Timer0_Cfg, delayEnablePulse, true);
                timerAlarmEnable(Timer0_Cfg);

                xSemaphoreTake(Semph_Timer0_ISR, portMAX_DELAY);
                
                timerAlarmDisable(Timer0_Cfg);

                digitalWrite(2, HIGH);
                ets_delay_us(50);
                digitalWrite(2, LOW);;
            }
            else 
            {
                digitalWrite(2, HIGH);
            }
        }
        
        vTaskDelay(300 / portTICK_PERIOD_MS);
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