/******************************************************************************/
#include "include/command.h"
#include "../error/include/error.h"
#include "../../lib/util/include/util.h"

#include "Arduino.h"

#include "freertos/queue.h"
#include "freertos/task.h"
/******************************************************************************/
QueueHandle_t xQueueCommandReceived;
QueueHandle_t xQueueCommandReady;
extern xQueueHandle xQueueChangeMode;
/******************************************************************************/
void CommandSetAlert( ERROR_ALERT_MODE mode)
{
    SetErrorAlert(mode);
}
/******************************************************************************/
bool ValidateCommand(newcommand_t command, uint8_t receiver[])
{

    uint8_t action = charToInt(command.data, 0, 2);
    uint8_t out    = charToInt(command.data, 2, 2);
    uint8_t params  = charToInt(command.data, 4, 2);

    uint8_t CommandType[] = {COMMAND_TURN_ON, COMMAND_TURN_OFF, COMMAND_TOGGLE, COMMAND_ANALOGIC};
    uint8_t CommandOut[] = {OUTPUT_00, OUTPUT_01, OUTPUT_02, OUTPUT_03, OUTPUT_04};

    #if COMMAND_DEBUG == true
        if(sizeof(CommandType) != COMMAND_TYPES_LEGTH || sizeof(CommandOut) != OUTPUT_LENGTH)
        {
           Serial.printf("Erro na quantidade de comandos/saidas.");
        }
    #endif /* COMMAND_DEBUG */

    if(ValueIsPresent(action, CommandType, 0, COMMAND_TYPES_LEGTH))
    {
        if(ValueIsPresent(out, CommandOut, 0, OUTPUT_LENGTH))
        {
            /* Atualiza o estado */
            #if COMMAND_DEBUG == true
                Serial.printf("Comando valido.\n");
            #endif /* COMMAND_DEBUG */

            receiver[0] = action;
            receiver[1] = out;

            return true;
        }
        #if COMMAND_DEBUG == true
            else 
            {
                Serial.printf("Erro, saída inválida.\n");

                return false;
            }
        #endif /* COMMAND_DEBUG */

    }
    #if COMMAND_DEBUG == true
        else 
        {
            Serial.printf("Erro, acão inválida.\n");
            return false;
        }
    #endif /* COMMAND_DEBUG */

    return false;
}
/******************************************************************************/
void vTaskCommandHandle( void *pvParameters )
{
    newcommand_t commandAux;
    uint8_t commandParams[3];

    xQueueCommandReceived = xQueueCreate(10, sizeof(newcommand_t));
    xQueueCommandReady = xQueueCreate(10, sizeof(commandParams));

    if(xQueueCommandReceived == NULL || xQueueCommandReady == NULL)
    {
        Serial.printf("Erro ao criar fila de comandos\n");

        return;
    }

    if(xTaskCreatePinnedToCore(vTaskCommandRun, "vTaskCommandRun", configMINIMAL_STACK_SIZE + 1024, NULL, 1, NULL, 1) != pdPASS)
    {
        Serial.printf("Erro ao criar task de execução de comandos\n");
        return;
    }

    for(;;)
    {
        /* Pega o comando que foi enviado para a fila de comandos */
        if(xQueueReceive(xQueueCommandReceived, &commandAux, portMAX_DELAY) == pdTRUE)
        {
            /* valida todos os parametros do comando */
            if(ValidateCommand(commandAux, commandParams))
            {
                /* Adiciona na fila de comandos para execução */

                #if COMMAND_DEBUG == true
                    Serial.printf("Comando adicionado na fila de execução.\n");
                #endif /* COMMAND_DEBUG */

                xQueueSendToBack(xQueueCommandReady, commandParams, portMAX_DELAY);
                CommandSetAlert(ERROR_ALERT_INITIALIZING);

            }
            #if COMMAND_DEBUG == true
                else
                {
                    Serial.printf("Erro, comando inválido.\n");

                    CommandSetAlert(ERROR_ALERT_CONNECTION);
                }
            #endif /* COMMAND_DEBUG */
        }

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }

}
/******************************************************************************/
void vTaskCommandRun( void *pvParameters )
{

    uint8_t commandParams[3];

    for(;;)
    {
        /* Pega o comando que foi validado como certo */
        if(xQueueReceive(xQueueCommandReady, &commandParams, portMAX_DELAY) == pdTRUE)
        {
            if(commandParams[0] != COMMAND_ANALOGIC && commandParams[0] != COMMAND_TOGGLE)
            {
                digitalWrite(commandParams[1], commandParams[0]);
                Serial.printf("TURN_ON [%i:%i]\n", commandParams[0], commandParams[1]);
            }
            else if (commandParams[0] == COMMAND_TOGGLE)
            {
                digitalWrite(commandParams[1], !digitalRead(commandParams[1]));
                Serial.printf("TURN_OFF [%i:%i]\n", commandParams[0], commandParams[1]);
            }
            else if(commandParams[0] == COMMAND_ANALOGIC)
            {
                /* Chama a função de controle do Dimmer */
                Serial.printf("COMMAND_ANALOGIC [%i:%i]\n", commandParams[0], commandParams[1]);
            }

        }

    }
}
/******************************************************************************/