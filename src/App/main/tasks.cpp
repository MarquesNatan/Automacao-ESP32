/******************************************************************************/
#include "include/tasks.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "../command/include/command.h"
#include "../switch/include/switch.h"
#include "../scenes/include/scenes.h"
#include "../heartbeat/include/heartbeat.h"

/******************************************************************************/
void vTasksCreate( void *pvParameters )
{
    xTaskCreatePinnedToCore(vTaskSwitchHandle, "SwitchHandle", configMINIMAL_STACK_SIZE + 1024, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(vTaskRunCommand, "RunCommand", configMINIMAL_STACK_SIZE + 1024, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(vTaskLedHeartbeat, "LedHeartbeat", configMINIMAL_STACK_SIZE, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(vTaskScenesHandle, "TaskScenesHandle", configMINIMAL_STACK_SIZE + 10 * 1024, NULL, 1, NULL, 1);
}
/******************************************************************************/