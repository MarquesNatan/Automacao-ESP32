/******************************************************************************/
#include "include/command.h"
#include "include/command_defs.h"
#include "../../system/include/system_defs.h"

#include "Arduino.h"

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"

#include <stdlib.h>
/******************************************************************************/
QueueHandle_t commandQueue;
/******************************************************************************/
uint8_t ptrParams[3];
/******************************************************************************/
command_packet_t newCommand = {
    .length = 0,
};
/******************************************************************************/
void vTaskRunCommand( void *pvParameters )
{
    uint8_t isEmpty = 0;
    command_packet_t localCommand;

    commandQueue = xQueueCreate(10, sizeof(newCommand));
    if(commandQueue == NULL)
    {
        Serial.printf("Erro durante a criação da fila de comandos.\n");
    }

    while(1)
    {
        if (xQueueReceive(commandQueue, &localCommand, portMAX_DELAY))
        {
            if(commandIsValid(&localCommand))
            {
                #if COMMAND_DEBUG == true
                    Serial.printf("Comando sem erros, pode ser processado.\n");
                #endif /* COMMAND_DEBUG */   
            }
        }

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
/******************************************************************************/
bool getCommand(uint8_t *data, unsigned int length)
{
    int i = 0;
    if (length <= sizeof(newCommand.data))
    {
        for (int i = 0; i < length; i++)
        {
            newCommand.data[i] = *(data + i);
        }

        newCommand.length = length;
    }
    #if COMMAND_DEBUG == true
        else
        {
            newCommand.status = COMMANDL_RECEIVE_FAIL;
            Serial.printf("\nTamanho de comando errado.\n");
            return false;
        }
    #endif /* COMMAND_DEBUG */

    newCommand.status = COMMAND_VALIDATION_WAIT;
    return true;
}
/******************************************************************************/
bool CommandGetParams(command_packet_t *command)
{
    uint8_t len = sizeof(command->data) / sizeof(command->data[0]);

    uint8_t i = 0;
    uint8_t paramsCount = 0;
    char temp[2];

    if (command->data[0] == COMMAND_SOF || command->data[len] != COMMAND_EOF)
    {
        while (i < command->length)
        {
            if (command->data[i] == COMMAND_END_PARAM || command->data[i] == COMMAND_EOF)
            {
                temp[0] = command->data[i - 2];
                temp[1] = command->data[i - 1];

                if(paramsCount != 0)
                {
                    ptrParams[paramsCount] = (uint8_t)strtol(temp, NULL, 16);
                }
                else 
                {
                    ptrParams[paramsCount] = (uint8_t)command->data[i - 1];
                }
                
                paramsCount = paramsCount + 1;
            }

            i = i + 1;
        }

        if (paramsCount != COMMAND_MAX_PARAMS)
        {
            command->status = COMMAND_PARAMS_FAIL;
            #if COMMAND_DEBUG == true
                Serial.printf("Erro, parametros esperados[%i] recebidos[%i]\n", COMMAND_MAX_PARAMS, paramsCount);
            #endif /* COMMAND_DEBUG */

            return false;
        }
        else
        {
            command->status = COMMAND_VALIDATION_WAIT;
        }
    }
    else
    {
        #if COMMAND_DEBUG == true
            Serial.printf("Erro, SOF or EOF\n");
        #endif /* COMMAND_DEBUG */
        return false;
    }
    return true;
}
/******************************************************************************/
bool containsThisValue(uint8_t param, uint8_t values[], uint8_t len)
{
    int i = 0;
    while (i < len)
    {   
        if(param == values[i]){ return true; }
        i = i + 1;
    }
    return false;
}
/******************************************************************************/
int8_t paramsIsValid(uint8_t params[])
{
    const char actions[4] = {COMM_TURN_ON_OUTPUT, COMM_TURN_OFF_OUTPUT, COMM_DIMMER_ADJUST, COMM_TOGGLE_OUTPUT};
    const uint8_t outputs[5] = {0, 2, 4, 13, 15};

    uint8_t qntError = 0;
    uint8_t i = 0;

    #if COMMAND_DEBUG == false
        Serial.printf("Parametros: [%c] [%i]\n", params[0], params[1]);
    #endif /* COMMAND_DEBUG */

    if (!containsThisValue(params[0], (uint8_t *)actions, 4))
    {
        qntError = qntError + 1;
        #if COMMAND_DEBUG == true
            Serial.printf("Erro na action.\n");
        #endif /* COMMAND_DEBUG */
    }

    if (!containsThisValue(params[1], (uint8_t *)outputs, 5))
    {
        qntError = qntError + 1;
        #if COMMAND_DEBUG == true
            Serial.printf("Erro no output.\n");
        #endif /* COMMAND_DEBUG */
    }

    return qntError;
}
/******************************************************************************/
bool commandIsValid(command_packet_t *command)
{
    int8_t errorParam = 0;
    uint8_t i = 0;
    if (CommandGetParams(command))
    {
        errorParam = paramsIsValid(ptrParams);
        if (!errorParam)
        {
            return true;
        }
        #if COMMAND_DEBUG == true
            else
            {
                Serial.printf("Comando com erro, params Errados: [%i]\n", errorParam);
                return false;
            }
        #endif /* COMMAND_DEBUG */
    }
    #if COMMAND_DEBUG == true
        else
        {
            Serial.printf("Erro, não foi possível capturar os parametros.");
            return false;
        }
    #endif /* COMMAND_DEBUG */

    return false;
}
/******************************************************************************/