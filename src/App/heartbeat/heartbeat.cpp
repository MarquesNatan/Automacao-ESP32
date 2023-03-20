/******************************************************************************/
#include "include/heartbeat.h"
#include "../../common/include/board_peripheral.h"

#include "Arduino.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
/******************************************************************************/
void vTaskLedHeartbeat( void *pvParameters )
{
    bool sequenceControl = false;
    for(;;)
    {
        digitalWrite(PIN_DIGITAL_LED_HEARTBEAT, LOW);
        vTaskDelay(pdMS_TO_TICKS(300));
        digitalWrite(PIN_DIGITAL_LED_HEARTBEAT, HIGH);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
/******************************************************************************/