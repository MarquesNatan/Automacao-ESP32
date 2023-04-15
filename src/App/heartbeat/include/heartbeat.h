/******************************************************************************/
#ifndef HEARTBEAT_H
#define HEARTBEAT_H
/******************************************************************************/
#include <stdint.h>
/******************************************************************************/
typedef void(*ptr_heartbeat)(void);
/******************************************************************************/
typedef enum 
{
    LED_NORMAL_MODE  = 0x00,
    LED_ERROR_COMMAND = 0x01
    
}LED_STATUS_MODE;
/******************************************************************************/
#define LED_HEARTBEAT_FUNC_QNT          2
#define LED_HEARTBEAT_COMMAND_ERROR_PULSES              4
#define LED_HEARTBEAT_COMMAND_INTIALIZING_PULSES        2
/******************************************************************************/
void vTaskLedHeartbeat( void *pvParameters );
void heartbeatCleanMode( void );
ptr_heartbeat setHeartbeatMode(LED_STATUS_MODE mode);
void heartbeatInitializing( void );
void heartbeatConnectionError( void );
void heartbeatNormalMode( void );
/******************************************************************************/
#endif /* HEARTBEAT_H */
/******************************************************************************/