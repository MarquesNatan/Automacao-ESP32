/******************************************************************************/
#include "include/scenes.h"
#include "include/scenes_defs_h"
#include "../../lib/file/include/file.h"
#include "../command/include/command.h"
#include "../command/include/command_defs.h"
#include "../../system/include/system_defs.h"

#include "Arduino.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "FS.h"
#include "LittleFS.h"

#include <stdlib.h>
#include <stdint.h>
/******************************************************************************/
uint8_t ScenesSearch( uint8_t day )
{

    FileSystemListDir(LittleFS, "/", 0);
    return 0;
}
/******************************************************************************/
bool IRAM_ATTR RegisterNewScene(uint8_t sceneParams[], unsigned int length)
{
    uint8_t params[] = {CharToByte(sceneParams, 8, 2), CharToByte(sceneParams, 10, 2)};
    scene_t newScene;
    command_packet_t sceneCommand;

    uint8_t day = CharToByte(sceneParams, 0, 2);
    uint8_t hour = CharToByte(sceneParams, 2, 2);
    uint8_t min = CharToByte(sceneParams, 4, 2);

    uint8_t lastWrittenCommand[15];
    uint8_t lastHour =  0;

    char buffer[15];

    GetCommandScene(&sceneCommand, sceneParams);

    if(commandIsValid(&sceneCommand))
    {
        #if SCENES_DEBUG == true
            Serial.printf("Comando sem erros, pode ser processado.\n");
        #endif /* SCENES_DEBUG */ 

        if(day >= SEG && day <= DOM)
        {
            if(hour >= 0 && hour <= 24 && min >= 0 && min <= 59)
            {                
                if(FileSystemStart())
                {
                    FileSystemDeleteFile(LittleFS, "/scenes/seg.txt");
                    FileSystemWriteFile(LittleFS, "/scenes/seg.txt", "000012144C0002\n");
                    appendFile(LittleFS, "/scenes/seg.txt", "000012144C0003\n");
                    appendFile(LittleFS, "/scenes/seg.txt", "000012144C0004\n");
                    appendFile(LittleFS, "/scenes/seg.txt", "000012144C0005\n");
                
                    FileSystemFindLastWrite(LittleFS, "/scenes/seg.txt", lastWrittenCommand, 15);
                    lastHour = CharToByte(lastWrittenCommand, 2, 2);

                    #if SCENES_DEBUG == true 
                        Serial.printf("Comando escrito, ultima hora: %i hora atual: %i\n", lastHour, hour);
                    #endif /* SCENES_DEBUG */
                    
                    if(lastHour >= 0 && lastHour <= 24 )
                    {
                        if(lastHour  < hour)
                        {
                            #if SCENES_DEBUG == true 
                                Serial.printf("Horário sem necessidade de alteração.\n");
                            #endif /* SCENES_DEBUG */

                            lastWrittenCommand[14] = '\n';
                            for(int i = 0; i < 14; i++)
                            {
                                buffer[i] = (char)sceneParams[i];
                            }
                            
                            appendFile(LittleFS, "/scenes/seg.txt", buffer);
                        }
                        else 
                        {
                            #if SCENES_DEBUG == true 
                                Serial.printf("Necessário ordenar os horários.\n");
                            #endif /* SCENES_DEBUG */
                        }

                        FileSystemReadFile(LittleFS, "/scenes/seg.txt");
                    }
                    else
                    {
                        #if SCENES_DEBUG == true 
                            Serial.printf("Erro, horário de cena inválido.\n");
                        #endif /* SCENES_DEBUG */
                    }
                }
                else 
                {
                    #if SCENES_DEBUG == true
                        Serial.printf("Erro ao montar o sistema de arquivos.\n");
                    #endif /* SCENES_DEBUG */    
                }
            }

        }
        else if(day == ALL_DAYS)
        {

        }

    }
    else 
    {
        #if SCENES_DEBUG == true
            Serial.printf("Comando invalido.\n");
        #endif /* SCENES_DEBUG */ 
    }

    return true;
}
/******************************************************************************/
void GetCommandScene(command_packet_t *command, uint8_t params[])
{

    command->data[0] = (uint8_t)COMMAND_SOF;
    command->data[8] = (uint8_t)COMMAND_EOF;

    command->data[2] = (uint8_t)COMMAND_END_PARAM;
    command->data[5] = (uint8_t)COMMAND_END_PARAM;

    command->data[1] = CharToByte(params, 8, 2);
    command->data[3] = params[10];
    command->data[4] = params[11];
    command->data[6] = params[12];
    command->data[7] = params[13];

    command->length = 9;
}
/******************************************************************************/
uint8_t CharToByte(uint8_t buffer[], uint8_t offset, uint8_t length)
{
    char temp[length];
    uint8_t value;

    for(int i = 0; i < length; i++)
    {
        temp[i] = (char)buffer[offset + i];
    }

    value = (uint8_t) strtol(temp, NULL, 16);

    return value;
}
/******************************************************************************/
void vTaskScenesHandle( void *pvParameters )
{
    ScenesSearch(1);

    for(;;)
    {

        vTaskDelay(portMAX_DELAY);
    }
}
/******************************************************************************/