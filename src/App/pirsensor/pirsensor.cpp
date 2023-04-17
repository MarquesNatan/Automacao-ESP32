/******************************************************************************/
#include "include/pirsensor.h"
#include "Preferences.h"

#include "../src/system/include/system_defs.h"
#include "../../common/include/board_peripheral.h"

#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
/******************************************************************************/
xSemaphoreHandle xSemaphorePIRSensor;
xQueueHandle xQueuePirSensor; 
portMUX_TYPE PIRSensorMux = portMUX_INITIALIZER_UNLOCKED;
/******************************************************************************/
static bool pirIsEnabled = false;
/******************************************************************************/
void PirSensor_EnableInterrupts( void )
{
    attachInterrupt(digitalPinToInterrupt(PIN_DIGITAL_PIR_IN), ISR_PIRSensor, RISING);
}
/******************************************************************************/
void IRAM_ATTR ISR_PIRSensor( void )
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        
    portENTER_CRITICAL_ISR(&PIRSensorMux);
    
        xSemaphoreGiveFromISR(xSemaphorePIRSensor, &xHigherPriorityTaskWoken);

        if(xHigherPriorityTaskWoken == pdTRUE)
        {
            if(xHigherPriorityTaskWoken == pdTRUE)
            {
                portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
            }
        }
    portEXIT_CRITICAL(&PIRSensorMux);

}
/******************************************************************************/
bool xQueuePirSensorCreate( void )
{
    if(xQueuePirSensor == NULL)
    {
        xQueuePirSensor = xQueueCreate(1, sizeof(uint8_t));
        if(xQueuePirSensor == NULL)
        {
            return false;
        }
    }

    return true;
}
/******************************************************************************/
void vTaskPIRSensorHandle( void *pvParameters )
{
    if(xQueuePirSensor == NULL)
    {
        xQueuePirSensor = xQueueCreate(1, sizeof(uint8_t));

        if(xQueuePirSensor == NULL )
        {
            Serial.printf("Erro, ao criar fila do Sensor PIR.\n");
        }
    }

    for(;;)
    {

        vTaskDelay(pdMS_TO_TICKS(1000));
    }

}
/******************************************************************************/

