/******************************************************************************/
#include "Arduino.h"

#include "include/scenes.h"
#include "include/scenes_defs_h"
#include "../command/include/command.h"
#include "../command/include/command_defs.h"
#include "../../system/include/system_defs.h"

#include "../../lib/file/include/file.h"
#include "../../lib/RTC/include/rtc.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "FS.h"
#include "LittleFS.h"

#include <stdlib.h>
#include <stdint.h>
/******************************************************************************/
QueueHandle_t xCenasQueue;
extern xSemaphoreHandle xRTCSemaphore;
extern RTC_DS3231 rtc;
/******************************************************************************/
uint8_t BuscarCenas( uint8_t dia )
{
    uint8_t buffer[12 * 10];
    uint8_t bytes = FileSystemGetInterval(LittleFS, "/cenas/cenas.txt", 1, 5, 13, buffer);

    #if SCENES_DEBUG == true
        Serial.printf("Dia de cadastro das cenas: %i\n", dia);
        Serial.printf("Quanditdade de bytes lidos: %i\n", bytes);
        Serial.printf("Quanditdade de cenas registradas: %i\n", (int)(bytes/12));
        for(int i = 0; i < bytes; i++)
        {
            Serial.printf("%c", buffer[i]);
        }

        Serial.printf("\n");
    #endif /* SCENES_DEBUG */


    
    return 0;
}
/******************************************************************************/
void IniciarCenas( void )
{

    File file;
    if(FileSystemStart())
    {
        #if SCENES_DEBUG == true
            Serial.printf("Sistema de arquivos iniciado com sucesso.\n");
        #endif /* SCENES_DEBUG */

        if(FileSystemFileExists(LittleFS, "/cenas/cenas.txt"))
        {
            #if SCENES_DEBUG == true
                Serial.printf("Arquivo de Registro de cenas encontrado.\n");
            #endif /* SCENES_DEBUG */

            FileSystemWriteFile(LittleFS, "/cenas/cenas.txt", "0010204D0000\n");

            FileSystemAppendFile(LittleFS, "/cenas/cenas.txt","0010204D0001\n");
            FileSystemAppendFile(LittleFS, "/cenas/cenas.txt","0010204D0002\n");
            FileSystemAppendFile(LittleFS, "/cenas/cenas.txt","0010204D0003\n");
            FileSystemAppendFile(LittleFS, "/cenas/cenas.txt","0010204D0004\n");
            FileSystemAppendFile(LittleFS, "/cenas/cenas.txt","0010204D0005\n");
            FileSystemAppendFile(LittleFS, "/cenas/cenas.txt","0010204D0006\n");
            FileSystemAppendFile(LittleFS, "/cenas/cenas.txt","0010204D0007\n");
            FileSystemAppendFile(LittleFS, "/cenas/cenas.txt","0010204D0008\n");
            FileSystemAppendFile(LittleFS, "/cenas/cenas.txt","0010204D0009\n");
        }
        else 
        {
            /* Cria o diretório e o Arquivo */
            if(FileSystemCreateDir(LittleFS, "/cenas"))
            {
                if(FileSystemCreateFile(LittleFS, "/cenas/cenas.txt"))
                {
                    #if SCENES_DEBUG == true
                        Serial.printf("Arquivo criado com sucesso.\n");
                    #endif /* SCENES_DEBUG */
                }
                else 
                {
                    #if SCENES_DEBUG == true
                        Serial.printf("Falha durante a criação do arquivo.\n");
                    #endif /* SCENES_DEBUG */
                }
            }
            else 
            {
                #if SCENES_DEBUG == true
                    Serial.printf("Erro ao criar diretório.\n");
                #endif /* SCENES_DEBUG */
            }
        }
    }
}
/******************************************************************************/
bool IRAM_ATTR RegisterNewScene(uint8_t sceneParams[], unsigned int length)
{
    cena_t novaCena;
    
    IniciaEstruturaNovaCena(&novaCena, sceneParams);

    if(novaCena.diaCena >= SEG && novaCena.diaCena <= DOM)
    {
        if(commandIsValid(&novaCena.comandoCena))
        {
            if(sceneParams[0] == 1)
            {
                /* Insere o comando no arquivo */
                FileSystemAppendFile(LittleFS, "/cenas/cenas.txt", (const char *)sceneParams);
            }
            else
            {
                FileSystemReadFile(LittleFS, "/cenas/cenas.txt");
            }
        }
        else 
        {
            Serial.printf("O comando inválido.\n");
        }
    }


    return true;
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
void IniciaEstruturaNovaCena(cena_t *novaCena, uint8_t bytesRecebidos[])
{
    novaCena->diaCena = (SCENE_DAY_ENUM)CharToByte(bytesRecebidos, 0, 2);
    novaCena->horarioCena.hour = CharToByte(bytesRecebidos, 2, 2);
    novaCena->horarioCena.minute = CharToByte(bytesRecebidos, 4, 3);

    /* Busca o comando da cena */
    BuscarParametrosNovaCena(novaCena, bytesRecebidos);

    novaCena->comandoCena.length = 9;
}
/******************************************************************************/
void BuscarParametrosNovaCena(cena_t *novaCena, uint8_t bytesRecebidos[])
{
    novaCena->comandoCena.data[0] = (uint8_t)COMMAND_SOF;
    novaCena->comandoCena.data[8] = (uint8_t)COMMAND_EOF;

    /* offset dos dados da cena */
    const uint8_t offset = 6;

    novaCena->comandoCena.data[1] = CharToByte(bytesRecebidos, 6, 2);
    Serial.printf("%c", novaCena->comandoCena.data[1]);

    novaCena->comandoCena.data[2] = (uint8_t)COMMAND_END_PARAM;

    novaCena->comandoCena.data[3] = bytesRecebidos[8];
    novaCena->comandoCena.data[4] = bytesRecebidos[9];

    novaCena->comandoCena.data[5] = (uint8_t)COMMAND_END_PARAM;

    novaCena->comandoCena.data[6] = bytesRecebidos[10];
    novaCena->comandoCena.data[7] = bytesRecebidos[11];

}
/******************************************************************************/
void vTaskScenesHandle( void *pvParameters )
{
    xCenasQueue = xQueueCreate(10, sizeof(cena_t));
    DateTime dataHoraAtual;

    DateTime now = rtc.now();
    char buff[] = "Alarm triggered at hh:mm:ss DDD, DD MMM YYYY\n";

    if(!xCenasQueue)
    {
        #if SCENES_DEBUG == true
            Serial.printf("Erro ao criar fila de comandos de cena.\n");
        #endif /* SCENES_DEBUG */
    }
    else
    {
        xRTCSemaphore = xSemaphoreCreateBinary();
        if(!xRTCSemaphore)
        {
            #if SCENES_DEBUG == true
                Serial.printf("Erro ao criar xRTCSemaphore.\n");
            #endif /* SCENES_DEBUG */
        }

        rtc.setAlarm1(rtc.now() + TimeSpan(0, 0, 0, 10), DS3231_A1_Second);
        Serial.printf(now.toString(buff));
    }

    IniciarCenas();

    for(;;)
    {
        // BuscarCenas(1);
        if(xSemaphoreTake( xRTCSemaphore, portMAX_DELAY) == pdTRUE)
        {
            digitalWrite(0, !digitalRead(0));

            rtc.disableAlarm(1);
            rtc.clearAlarm(1);
            rtc.setAlarm1(rtc.now() + TimeSpan(0, 0, 0, 1), DS3231_A1_Second);

            now = rtc.now();
            Serial.printf(now.toString(buff));
        }


        vTaskDelay(1000);
    }
}
/******************************************************************************/