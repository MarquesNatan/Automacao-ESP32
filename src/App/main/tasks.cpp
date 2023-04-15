/******************************************************************************/
#include "include/tasks.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "../command/include/command.h"
#include "../switch/include/switch.h"
#include "../heartbeat/include/heartbeat.h"
#include "../dimmer/include/dimmer.h"
/******************************************************************************/
void vTasksCreate( void *pvParameters )
{
    xTaskCreatePinnedToCore(vTaskCommandHandle, "vTaskCommandReceiver", configMINIMAL_STACK_SIZE + 2048, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(vTaskSwitchHandle, "SwitchHandle", configMINIMAL_STACK_SIZE + 1024, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(vTaskDimmer, "TaskDimmer", configMINIMAL_STACK_SIZE + 2048, NULL, 2, NULL, 1);
    xTaskCreatePinnedToCore(vTaskLedHeartbeat, "LedHeartbeat", configMINIMAL_STACK_SIZE, NULL, 1, NULL, 1);
}
/******************************************************************************/