/******************************************************************************/
#ifndef COMMAND_H
#define COMMAND_H
/******************************************************************************/
#include <stdint.h>
#include "freertos/queue.h"
#include "command_def.h"
/******************************************************************************/
#define PARAMS_IN_COMMAND                   3
/******************************************************************************/
typedef struct 
{
    uint8_t data[PARAMS_IN_COMMAND];
}command_t;
/******************************************************************************/
command_t Command_Parse(uint8_t *comm);
void Command_CreateQueue(uint8_t length, QueueHandle_t *queue);
void Command_PushToQueue(const char *command);

bool CommandGetParams(command_packet_t *command);
bool getCommand(uint8_t *data, unsigned int  length);
bool commandIsValid(command_packet_t *command);
/******************************************************************************/
#endif /* COMMAND_H */
/******************************************************************************/