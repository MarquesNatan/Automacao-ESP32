/******************************************************************************/
#include "include/scenes.h"
#include "include/scenes_defs_h"

#include "Arduino.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "../../lib/file/include/file.h"
#include "LittleFS.h"
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
void RegisterNewScene(command_packet_t scenceCommand, scene_time_t sceneTriggerTime, scene_day day)
{
    if(day >= 0 && day <= 6)
    {
        
    }
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