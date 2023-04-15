/******************************************************************************/
#include "include/heartbeat.h"
#include "../../common/include/board_peripheral.h"

#include "Arduino.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
/******************************************************************************/
xQueueHandle xQueueChangeMode;
/******************************************************************************/
ptr_heartbeat errorModeList[] = {heartbeatNormalMode, heartbeatConnectionError, heartbeatInitializing};
/******************************************************************************/
void vTaskLedHeartbeat( void *pvParameters )
{
    ptr_heartbeat currMode = NULL;
    ptr_heartbeat newMode = NULL;

    xQueueChangeMode = xQueueCreate(1, sizeof(ptr_heartbeat));
    if(xQueueChangeMode == NULL)
    {
        Serial.printf("Erro ao criar fila de callback visual.\n");
    }

    for(;;)
    {
        if(xQueueReceive(xQueueChangeMode, &currMode, 0) == pdTRUE)
        {
            heartbeatCleanMode();

            if(newMode != NULL)
            {
                currMode = newMode;
            }
        }

        if(currMode != NULL)
        {
            (*currMode)();
        }

    }
}
/******************************************************************************/
void heartbeatConnectionError( void )
{
    bool initialState = true;

    for(int i = 0; i < LED_HEARTBEAT_COMMAND_ERROR_PULSES * 2; i++)
    {
        if(i % 2)
        {
            digitalWrite(PIN_DIGITAL_LED_ERRO, !initialState);
            vTaskDelay(pdMS_TO_TICKS(250));
        }
        else 
        {
            digitalWrite(PIN_DIGITAL_LED_ERRO, !initialState);
            vTaskDelay(pdMS_TO_TICKS(100));
        }

        initialState = !initialState; 
    }
            
    vTaskDelay(pdMS_TO_TICKS(1000));
}
/******************************************************************************/
void heartbeatNormalMode( void )
{

    digitalWrite(PIN_DIGITAL_LED_HEARTBEAT, LOW);
    vTaskDelay(pdMS_TO_TICKS(250));
    digitalWrite(PIN_DIGITAL_LED_HEARTBEAT, HIGH);

    vTaskDelay(pdMS_TO_TICKS(1000));
}
/******************************************************************************/
void heartbeatInitializing( void )
{
    bool initialState = false;
    for(int i = 0; i < LED_HEARTBEAT_COMMAND_INTIALIZING_PULSES * 2; i++)
    {
        (i % 2) ? vTaskDelay(pdMS_TO_TICKS(250)) : vTaskDelay(pdMS_TO_TICKS(100));

        digitalWrite(PIN_DIGITAL_LED_ERRO, !initialState);
        digitalWrite(PIN_DIGITAL_LED_HEARTBEAT, !initialState);
        initialState = !initialState; 
    }
            
    vTaskDelay(pdMS_TO_TICKS(1000));

}
/******************************************************************************/
void heartbeatCleanMode( void )
{
    digitalWrite(PIN_DIGITAL_LED_HEARTBEAT, LOW);
    digitalWrite(PIN_DIGITAL_LED_ERRO, LOW);
}
/******************************************************************************/