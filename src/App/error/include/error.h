/******************************************************************************/
#ifndef ERROR_H
#define ERROR_H
/******************************************************************************/
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
/******************************************************************************/
#if ERROR_CALLBACK_MODE == LEDHEARTBEAT
    #include "../../heartbeat/include/heartbeat.h"
    extern ptr_heartbeat errorModeList[];
    extern xQueueHandle xQueueChangeMode;
#endif
/******************************************************************************/
typedef enum 
{
    ERROR_ALERT_NONE    = 0x00,
    ERROR_ALERT_CONNECTION  = 0x01,
    ERROR_ALERT_INITIALIZING = 0x02,
    ERROR_ALERT_RUN_COMMAND = 0x02, 
    ERROR_ALERT_INVALID_COMMAND = 0x03
}ERROR_ALERT_MODE;
/******************************************************************************/
ptr_heartbeat SetErrorAlert(ERROR_ALERT_MODE mode);
/******************************************************************************/
#endif /* ERROR_H */
/******************************************************************************/