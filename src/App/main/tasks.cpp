/******************************************************************************/
#include "include/tasks.h"
#include "Arduino.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "../command/include/command.h"
#include "../switch/include/switch.h"
#include "../heartbeat/include/heartbeat.h"
#include "../dimmer/include/dimmer.h"
#include "../pirsensor/include/pirsensor.h"

#include "../../lib/mqtt/include/mqtt.h"
/******************************************************************************/
void vTasksCreate( void *pvParameters )
{
    if(xTaskCreatePinnedToCore(vTaskMqttHandleConnection, "vTaskMqtt", configMINIMAL_STACK_SIZE + 4096, NULL, 1, NULL, 1) != pdPASS)
    {
        Serial.printf("vTaskMqttHandleConnection not created.\n");
        return;
    }

    if(xTaskCreatePinnedToCore(vTaskCommandHandle, "vTaskCommandReceiver", configMINIMAL_STACK_SIZE + 3072, NULL, 2, NULL, 0) != pdPASS)
    {
        Serial.printf("vTaskCommandHandle not created.\n");
    }

    if(xTaskCreatePinnedToCore(vTaskCommandRun, "vTaskCommandRun", configMINIMAL_STACK_SIZE + 2048, NULL, 2, NULL, 1) != pdPASS)
    {
        Serial.printf("vTaskCommandRun not created.\n");
        return;
    }

    if(xTaskCreatePinnedToCore(vTaskDimmer, "vTaskDimmer", configMINIMAL_STACK_SIZE + 4096, NULL, 2, NULL, 1) != pdPASS)
    {
        Serial.printf("vTaskDimmer not created.\n");
        return;
    }

    if(xTaskCreatePinnedToCore(vTaskSwitchHandle, "vTaskSwitchHandle", configMINIMAL_STACK_SIZE + 4096, NULL, 0, NULL, 0) != pdPASS)
    {
        Serial.printf("vTaskSwitchHandle not created.\n");
        return;
    }

    if(xTaskCreatePinnedToCore(vTaskLedHeartbeat, "LedHeartbeat", configMINIMAL_STACK_SIZE + 2048, NULL, 0, NULL, 0) != pdPASS)
    {
        Serial.printf("vTaskLedHeartbeat not created.\n");
        return;
    }

    if(xTaskCreatePinnedToCore(vTaskPirSensorHandle, "vTaskPirSensor", configMINIMAL_STACK_SIZE + 2048, NULL, 1, NULL, 0) != pdPASS)
    {
        Serial.printf("vTaskPirSensorHandle not created.\n");
        return;
    }
}
/******************************************************************************/