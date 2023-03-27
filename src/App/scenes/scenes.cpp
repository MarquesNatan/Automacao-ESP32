/******************************************************************************/
#include "Arduino.h"

#include "include/scenes.h"
#include "include/scenes_defs_h"
#include "../command/include/command.h"
#include "../command/include/command_defs.h"
#include "../../system/include/system_defs.h"

#include "../../lib/file/include/file.h"
#include "../../lib/RTC/include/rtc.h"
#include "../../lib/util/include/util.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "FS.h"
#include "LittleFS.h"

#include <stdlib.h>
#include <stdint.h>
/******************************************************************************/
QueueHandle_t xCenasQueue;
/******************************************************************************/
uint8_t ultimoDia = 0xFF;
/******************************************************************************/
extern xSemaphoreHandle xRTCSemaphore;
extern RTC_DS3231 rtc;
/******************************************************************************/
bool CopiarComando(uint8_t src[], uint8_t dest[], uint8_t offset, uint8_t length)
{
    int i = 0;
    int temp = 0;

    for(i = 0; i < length; i++)
    {
        dest[i] = src[offset + i];
        temp = temp + 1;
    }

    if(temp == (length - 1))
    {
        return true;
    }

    return true;
}
/******************************************************************************/
uint8_t BuscarCenas( uint8_t dia )
{
    uint8_t buffer[15 * 10];
    uint8_t bufferAux[15];
    uint8_t aux = 0;

    uint8_t offset = dia * 15;
    uint8_t bytes = FileSystemGetInterval(LittleFS, "/cenas/cenas.txt", offset, 10, 15, buffer);

    uint8_t temp = 0;

    #if SCENES_DEBUG == false
        Serial.printf("Dados das cenas: \n");
        for(int i = 0; i < bytes; i++)
        {
            Serial.printf("%c", buffer[i]);
        }

    #endif /* SCENES_DEBUG */

    if(bytes != (10 - offset) * 15)
    {
        Serial.printf("Erro, quantidade de dados lidos não é valida.\n");

        return 0;
    }

    for(int i = 0; i < 10; i++)
    {
        CopiarComando(buffer, bufferAux, temp, 15);

        aux = CharToByte(bufferAux, 0, 2);

        if(aux != dia)
        {
            return 1;
        }


        RegisterNewScene(bufferAux, 0);

        temp = temp + 15;
    }
    
    return 0;
}
/******************************************************************************/
void IniciarCenas( void )
{

    File file;
    if(FileSystemStart())
    {
        #if SCENES_DEBUG == true
            Serial.printf("\nSistema de arquivos iniciado com sucesso.\n");
        #endif /* SCENES_DEBUG */

        if(FileSystemFileExists(LittleFS, "/cenas/cenas.txt"))
        {
            #if SCENES_DEBUG == true
                Serial.printf("Arquivo de Registro de cenas encontrado.\n");
            #endif /* SCENES_DEBUG */

            FileSystemWriteFile(LittleFS, "/cenas/cenas.txt", "001001004C0000\n");
            FileSystemAppendFile(LittleFS, "/cenas/cenas.txt","001002014C0000\n");
            FileSystemAppendFile(LittleFS, "/cenas/cenas.txt","011020024C0403\n");
            FileSystemAppendFile(LittleFS, "/cenas/cenas.txt","010102034C0D04\n");
            FileSystemAppendFile(LittleFS, "/cenas/cenas.txt","011020044C0F05\n");
            FileSystemAppendFile(LittleFS, "/cenas/cenas.txt","FFFFFFFF4D0000\n");
            FileSystemAppendFile(LittleFS, "/cenas/cenas.txt","FFFFFFFF4D0000\n");
            FileSystemAppendFile(LittleFS, "/cenas/cenas.txt","FFFFFFFF4D0000\n");
            FileSystemAppendFile(LittleFS, "/cenas/cenas.txt","FFFFFFFF4D0000\n");
            FileSystemAppendFile(LittleFS, "/cenas/cenas.txt","FFFFFFFF4D0000\n");

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

    for(int i = 0; i < 9; i++)
    {
        Serial.printf("%c", novaCena.comandoCena.data[i]);
    }

    Serial.println();

     if(novaCena.diaCena >= SEG && novaCena.diaCena <= DOM)
    {
        if(commandIsValid(&novaCena.comandoCena))
        {
            if(xQueueSendToBack(xCenasQueue, &novaCena, 0) == pdTRUE)
            {
                #if SCENES_DEBUG == true
                    Serial.printf("A cena adicionada a biblioteca.\n");
                #endif /* SCENES_DEBUG */
            }
            
        }
        else 
        {
            Serial.printf("A cena inválida.\n");
        }
    }
    else
    {
        Serial.printf("Erro na data da cena.\n");
    }

    return true;
}
/******************************************************************************/
void IniciaEstruturaNovaCena(cena_t *novaCena, uint8_t bytesRecebidos[])
{
    novaCena->diaCena = (SCENE_DAY_ENUM)CharToByte(bytesRecebidos, 0, 2);
    novaCena->horarioCena.hour = CharToByte(bytesRecebidos, 2, 2);
    novaCena->horarioCena.minute = CharToByte(bytesRecebidos, 4, 2);
    novaCena->horarioCena.secound = CharToByte(bytesRecebidos, 6, 2);

    /* Busca o comando da cena */
    BuscarParametrosNovaCena(novaCena, bytesRecebidos);

    novaCena->comandoCena.length = 9;
}
/******************************************************************************/
void BuscarParametrosNovaCena(cena_t *novaCena, uint8_t bytesRecebidos[])
{
    // 001020004C0000 
    novaCena->comandoCena.data[0] = (uint8_t)COMMAND_SOF;
    novaCena->comandoCena.data[8] = (uint8_t)COMMAND_EOF;

    /* offset dos dados da cena */
    const uint8_t offset = 8;

    novaCena->comandoCena.data[1] = CharToByte(bytesRecebidos, 8, 2);

    novaCena->comandoCena.data[2] = (uint8_t)COMMAND_END_PARAM;

    novaCena->comandoCena.data[3] = bytesRecebidos[10];
    novaCena->comandoCena.data[4] = bytesRecebidos[11];

    novaCena->comandoCena.data[5] = (uint8_t)COMMAND_END_PARAM;

    novaCena->comandoCena.data[6] = bytesRecebidos[12];
    novaCena->comandoCena.data[7] = bytesRecebidos[13];
}
/******************************************************************************/
void vTaskScenesHandle( void *pvParameters )
{
    xCenasQueue = xQueueCreate(10, sizeof(cena_t));
    cena_t cenaAtual;
    uint8_t diaAtual;
    DateTime now;
    
    now = rtc.now();

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
    }

    IniciarCenas();

    for(;;)
    {

        diaAtual = now.dayOfTheWeek();
        if(ultimoDia != diaAtual)
        {
            BuscarCenas(diaAtual);

            ultimoDia = diaAtual;
        }

        /* Se existir algum comando na fila de execução */
        if(xQueueReceive(xCenasQueue, &cenaAtual, 0) == pdTRUE)
        {
            rtc.setAlarm1(DateTime(2023, 3, 26, 16, cenaAtual.horarioCena.minute, 
                        1), DS3231_A1_Minute);

            /* rtc.setAlarm1(DateTime(2023, 3, 26, cenaAtual.horarioCena.hour, cenaAtual.horarioCena.minute, 
            cenaAtual.horarioCena.secound), DS3231_A1_Date);*/
            Serial.printf("Hora:%i Minuto:%i Segundo: %i\n", cenaAtual.horarioCena.hour, cenaAtual.horarioCena.minute, cenaAtual.horarioCena.secound);

            /* Espera o alarme liberar o semaforo */
            if(xSemaphoreTake(xRTCSemaphore, portMAX_DELAY) == pdTRUE)
            {
                digitalWrite(0, !digitalRead(0));

                rtc.disableAlarm(1);
                rtc.clearAlarm(1);
            }
        }
        
        vTaskDelay(250);
    }
}
/******************************************************************************/