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

    xTaskCreate(vTaskSwitchHandle, "SwitchHandle", configMINIMAL_STACK_SIZE + 1024, NULL, 3, NULL);
    xTaskCreate(vTaskLedHeartbeat, "LedHeartbeat", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(vTaskRunCommand, "RunCommand", configMINIMAL_STACK_SIZE + 1024, NULL, 1, NULL);
    xTaskCreate(vTaskScenesHandle, "TaskScenesHandle", configMINIMAL_STACK_SIZE + 10 * 1024, NULL, 1, NULL);
}
/******************************************************************************/