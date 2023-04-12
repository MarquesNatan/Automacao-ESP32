/******************************************************************************/
#include "include/heartbeat.h"
#include "../../common/include/board_peripheral.h"

#include "Arduino.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
/******************************************************************************/
uint16_t delayTimeMS = 300;
uint16_t delayTimeTask = 1000;
/******************************************************************************/
void vTaskLedHeartbeat( void *pvParameters )
{
    bool sequenceControl = false;
    for(;;)
    {
        digitalWrite(PIN_DIGITAL_LED_HEARTBEAT, LOW);
        vTaskDelay(pdMS_TO_TICKS(delayTimeMS));
        digitalWrite(PIN_DIGITAL_LED_HEARTBEAT, HIGH);
        
        vTaskDelay(pdMS_TO_TICKS(delayTimeTask));
    }
}
/******************************************************************************/