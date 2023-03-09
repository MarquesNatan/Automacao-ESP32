/******************************************************************************/
#include <Arduino.h>

#include "include/dimmer.h"
#include "../../src/system/include/system_defs.h"
#include "freertos/semphr.h"
/******************************************************************************/
xSemaphoreHandle ZOHtrigged;
portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
/******************************************************************************/
volatile  unsigned long zohTime = 0;
volatile unsigned long zcdLastDebounceTime = 0;
volatile unsigned long zcdCount = 0;
volatile bool zcdIsTrigged = false;
/******************************************************************************/
void DimmerZCDTrigger_Config(void)
{
}
/******************************************************************************/
void IRAM_ATTR DimmerZCDTrigger(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    
    zohTime = millis();

    if(zohTime - zcdLastDebounceTime > 250)
    {
        zcdCount++;
        zcdIsTrigged = true;
        zcdLastDebounceTime = zohTime;

        xSemaphoreGiveFromISR(ZOHtrigged, &xHigherPriorityTaskWoken);
    }

    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}
/******************************************************************************/