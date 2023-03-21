/******************************************************************************/
#include "include/scenes.h"
#include "include/scenes_defs_h"
#include "../../lib/file/include/file.h"
#include "../command/include/command.h"

#include "Arduino.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "LittleFS.h"

#include <stdlib.h>
#include <stdint.h>
/******************************************************************************/
uint8_t ScenesSearch( uint8_t day )
{
    FileSystemStart();

    FileSystemCreateDir(LittleFS, "/scenes");

    // FileSystemWriteFile(LittleFS, "/scenes/seg.txt", "first write");
    FileSystemDeleteFile(LittleFS, "/scenes/seg.txt");

    FileSystemWriteFile( LittleFS, "/scenes/seg.txt", " " );
    FileSystemWriteFile( LittleFS, "/scenes/ter.txt", " " );
    FileSystemWriteFile( LittleFS, "/scenes/que.txt", " " );
    FileSystemWriteFile( LittleFS, "/scenes/qui.txt", " " );
    FileSystemWriteFile( LittleFS, "/scenes/sex.txt", " " );
    FileSystemWriteFile( LittleFS, "/scenes/sab.txt", " " );
    FileSystemWriteFile( LittleFS, "/scenes/dom.txt", " " );

    FileSystemListDir(LittleFS, "/", 0);

    return 0;
}
/******************************************************************************/
bool IRAM_ATTR RegisterNewScene(uint8_t sceneParams[], unsigned int length)
{
    uint8_t params[] = {CharToByte(sceneParams, 8, 2), CharToByte(sceneParams, 10, 2)};

    if(!paramsIsValid(params))
    {
        Serial.printf("Todos os parametros são validos.\n");
    }
    else 
    {
        Serial.printf("Parametros invalidos.\n");
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
void vTaskScenesHandle( void *pvParameters )
{
    ScenesSearch(1);

    for(;;)
    {

        vTaskDelay(portMAX_DELAY);
    }
}
/******************************************************************************/