/******************************************************************************/
#include "include/command.h"
#include "../error/include/error.h"
#include "../../lib/util/include/util.h"
#include "../../lib/mqtt/include/mqtt.h"
#include "../pirsensor/include/pirsensor.h"
#include "../dimmer/include/dimmer.h"

#include "Arduino.h"

#include "freertos/queue.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
/******************************************************************************/
xQueueHandle xQueueCommandReceived;
xQueueHandle xQueueCommandReady;
/******************************************************************************/
extern xQueueHandle xQueueChangeMode;
/******************************************************************************/
extern xQueueHandle xQueueChangeDimmerValue;
extern SemaphoreHandle_t xGetGlobalDimmerValueSemaphore;
extern uint16_t globalDimmerValue;
/******************************************************************************/
static command_callback_func commandCallback;
/******************************************************************************/
volatile uint8_t receivedAnalogicValue = 0;
/******************************************************************************/
void SetCommandCallback(command_callback_func ptr)
{
    if(ptr != NULL)
    {
        commandCallback = ptr;
    }
}
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

    uint8_t CommandType[] = {COMMAND_TURN_ON, COMMAND_TURN_OFF, COMMAND_TOGGLE, COMMAND_ANALOGIC, COMMAND_READ_PIN};
    uint8_t CommandOut[] = {OUTPUT_00, OUTPUT_01, OUTPUT_02, OUTPUT_03, OUTPUT_04, OUTPUT_ANALOGIC};

    if(ValueIsPresent(action, CommandType, 0, sizeof(CommandType)))
    {
        if(ValueIsPresent(out, CommandOut, 0, sizeof(CommandOut)))
        {
            /* Atualiza o estado */
            #if COMMAND_DEBUG == true
                Serial.printf("Comando valido. [%i][%i][%i]\n", action, out, params);
            #endif /* COMMAND_DEBUG */

            receiver[0] = action;
            receiver[1] = out;
            receiver[2] = params;

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

    SetCommandCallback(MQTT_Publish);

    xQueueCommandReceived = xQueueCreate(10, sizeof(newcommand_t));
    xQueueCommandReady = xQueueCreate(10, sizeof(commandParams));

    if(xQueueCommandReceived == NULL || xQueueCommandReady == NULL)
    {
        Serial.printf("Erro ao criar fila de comandos\n");

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
                CommandSetAlert(ERROR_ALERT_NONE);

            }
            #if COMMAND_DEBUG == true
                else
                {
                    Serial.printf("Erro, comando inválido.\n");

                    CommandSetAlert(ERROR_ALERT_CONNECTION);
                }
            #endif /* COMMAND_DEBUG */
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }

}
/******************************************************************************/
void vTaskCommandRun( void *pvParameters )
{

    uint8_t commandParams[3];
    uint8_t output = 0;
    char valueConversion[2];

    char msgRead[] = "05006600";
    const char topic[] = "tccautomacao/comando/";
   

    for(;;)
    {
        /* Pega o comando que foi validado como certo */
        if(xQueueReceive(xQueueCommandReady, &commandParams, 0) == pdTRUE)
        {
            if(commandParams[0] != COMMAND_ANALOGIC && commandParams[0] != COMMAND_TOGGLE && commandParams[0] != COMMAND_READ_PIN)
            {
                digitalWrite(commandParams[1], commandParams[0]);

                #if COMMAND_DEBUG == true
                    Serial.printf("TURN_ON [%i:%i]\n", commandParams[0], commandParams[1]);
                #endif /* COMMAND_DEBUG */

            }
            else if (commandParams[0] == COMMAND_TOGGLE)
            {
                digitalWrite(commandParams[1], !digitalRead(commandParams[1]));
                #if COMMAND_DEBUG == true
                    Serial.printf("TURN_OFF [%i:%i]\n", commandParams[0], commandParams[1]);
                #endif /* COMMAND_DEBUG */
            }
            else if(commandParams[0] == COMMAND_ANALOGIC)
            {
                /* Chama a função de controle do Dimmer */
                #if COMMAND_DEBUG == true
                    Serial.printf("COMMAND_ANALOGIC [%i:%i]\n", commandParams[0], commandParams[2]);
                #endif /* COMMAND_DEBUG */
                
                if(xQueueChangeDimmerValue != NULL)
                {
                    receivedAnalogicValue = commandParams[2];
                    if(xQueueSendToBack(xQueueChangeDimmerValue, (void *)&receivedAnalogicValue, 0) != pdTRUE)
                    {
                        #if COMMAND_DEBUG == true
                            Serial.printf("Erro ao inserir comando analógico.\n");
                        #endif /* COMMAND_DEBUG */   
                    }
                }
            }
            else if(commandParams[0] == COMMAND_READ_PIN)
            {
                #if COMMAND_DEBUG == true
                    Serial.printf("COMMAND_READ_PIN [%i:%i]\n", commandParams[0], commandParams[1]);
                #endif /* COMMAND_DEBUG */

                if(commandParams[1] != OUTPUT_ANALOGIC)
                {
                    output = digitalRead(commandParams[1]);
                }
                else 
                {
                    xSemaphoreTake(xGetGlobalDimmerValueSemaphore, portMAX_DELAY);
                        output = (uint8_t)map(globalDimmerValue, 0, 4095, 0, 100);
                    xSemaphoreGive(xGetGlobalDimmerValueSemaphore);
                }

                msgRead[2] = ((commandParams[1] & 0xF0) >> 4) + '0';
                msgRead[3] = (commandParams[1] & 0x0F) + '0';

                decimalToHexadecimal(output, valueConversion);

                msgRead[4] = valueConversion[0];
                msgRead[5] = valueConversion[1];

                Serial.printf("%i %s | %s\n", output, valueConversion, msgRead);
            
                /* Envia o estado atual da saída /  valor de ajuste analógico */
                MQTT_Publish(msgRead, MQTT_TOPIC_READ);
                
            }


            // 05001=00
        }

        vTaskDelay(pdMS_TO_TICKS(25));
    }
}
/******************************************************************************/