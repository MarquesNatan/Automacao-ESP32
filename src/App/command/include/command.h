/******************************************************************************/
#ifndef TASKS_COMMAND_H
#define TASKS_COMMAND_H
/******************************************************************************/
#include "command_defs.h"
/******************************************************************************/
void vTaskRunCommand( void *pvParameters );

bool getCommand(uint8_t *data, unsigned int length);
bool commandIsValid(command_packet_t *command);
bool CommandGetParams(command_packet_t *command);
int8_t paramsIsValid(uint8_t params[]);
bool containsThisValue(uint8_t param, uint8_t values[], uint8_t len);
/******************************************************************************/
#endif /* TASKS_COMMAND_H */
/******************************************************************************/