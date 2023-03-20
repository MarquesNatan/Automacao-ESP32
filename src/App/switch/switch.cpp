/******************************************************************************/
#include "include/switch.h"

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

#include "Arduino.h"

#include "../../common/include/board_peripheral.h"
/******************************************************************************/
uint8_t switchPressed = 0;
long int isrStartTime = 0;
long int isrLastDebounceTime = 0;
SemaphoreHandle_t xSwitchSemaphore;
/******************************************************************************/
void SwitchISR_Configure( void (*isrHandle)(void *args) )
{
    attachInterruptArg(digitalPinToInterrupt(PIN_DIGITAL_SWITCH_0), ISR_Switch, (void*)PIN_DIGITAL_SWITCH_0, CHANGE);
    attachInterruptArg(digitalPinToInterrupt(PIN_DIGITAL_SWITCH_1), ISR_Switch, (void*)PIN_DIGITAL_SWITCH_1, CHANGE);
}
/******************************************************************************/
void IRAM_ATTR ISR_Switch( void *args )
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    switchPressed = (int)args;
    isrStartTime = millis();

    if(isrStartTime - isrLastDebounceTime > 100)
    {
        isrLastDebounceTime = isrStartTime;
        xSemaphoreGiveFromISR(xSwitchSemaphore, &xHigherPriorityTaskWoken);

        if(xHigherPriorityTaskWoken == pdTRUE)
        {
            portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
        }
    }

}
/******************************************************************************/
void vTaskSwitchHandle( void *pvParameters )
{
    xSwitchSemaphore = xSemaphoreCreateBinary();
    if(xSwitchSemaphore != NULL)
    {
        SwitchISR_Configure(ISR_Switch);
        Serial.printf("Semaforo criado com sucesso.\n");
    }
    else 
    {
        Serial.printf("Erro durante a criação do semaforo.\n");
    }

    // command_packet_t switchCommand;
    uint8_t pinLastState = 0;
    uint8_t pinToRead = 0;

    for(;;)
    {
        if(xSemaphoreTake( xSwitchSemaphore, portMAX_DELAY) == pdTRUE)
        {
            /* @Hack: Alterar para adicionar na fila de coandos. */
            switch (switchPressed)
            {
                case PIN_DIGITAL_SWITCH_0:
                    digitalWrite(PIN_DIGITAL_RELAY_0, !digitalRead(PIN_DIGITAL_RELAY_0));
                    break;
                case PIN_DIGITAL_SWITCH_1:
                    digitalWrite(PIN_DIGITAL_RELAY_1, !digitalRead(PIN_DIGITAL_RELAY_1));
                    break;
                default:
                    break;
            }
        }
    }
}
/******************************************************************************/