/******************************************************************************/
#include <Arduino.h>
#include <stdio.h>
#include "freertos/queue.h"

#include "include/command.h"
#include "../../src/common/include/protocol_common.h"
#include "../../src/system/include/system_defs.h"
/******************************************************************************/
QueueHandle_t commQueue;
/******************************************************************************/
void Command_CreateQueue(uint8_t length, QueueHandle_t *queue)
{
    *queue = xQueueCreate(length, sizeof(command_t));

    Serial.printf("Fila criada com sucesso\n");
}
/******************************************************************************/
command_t Command_Parse(uint8_t *comm)
{
    command_t command;
    char temp[2];
    char commandAux[3];
    int val = 0;

    static int count = 0;
    uint8_t i = 0;
    uint8_t j = 0;

    if(comm[0] == COMMAND_SOF)
    {
        while (comm[i] != COMMAND_EOF)
        {
            if(comm[i] != (uint8_t)COMMAND_SOF && comm[i] != (uint8_t)COMMAND_PARAMS_TOKEN)
            {
                temp[0] = comm[i];
                temp[1] = comm[i + 1];

                val = (int)(strtol(temp, NULL, 16));
                commandAux[j] = val;

                #if COMMAND_DEBUG == true
                    Serial.printf("%i", val);
                #endif /* COMMAND_DEBUG */

                i = i + 1;
                j = j + 1;
            }

            i = i + 1;
        }

        j = 0;
        while(j < PARAMS_IN_COMMAND)
        {
            command.data[j] = commandAux[j];
            j = j + 1;
        }

        xQueueSendToBack(commQueue, &command, portMAX_DELAY);   
    }
    else 
    {
        #if COMMAND_DEBUG == true
            Serial.printf("Erro, start frame");
        #endif /* MQTT_DEBUG */
    }

    return command;
}
/******************************************************************************/