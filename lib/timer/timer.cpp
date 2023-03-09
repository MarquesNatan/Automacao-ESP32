/******************************************************************************/
#include <Arduino.h>
#include "include/timer.h"
#include "../../../src/system/include/system_defs.h"
#include "../../../src/common/include/board_peripheral.h"

#include "freertos/semphr.h"
/******************************************************************************/
hw_timer_t *Timer0_Cfg = NULL;
xSemaphoreHandle Semph_Timer0_ISR;
/******************************************************************************/
void Timer0_Config(hw_timer_t *TimerConfig, uint8_t timerIndex, uint16_t timerPrescaler)
{

}
/******************************************************************************/
void Timer0_ISRHandle(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    int aux = 0;

    xSemaphoreGiveFromISR(Semph_Timer0_ISR, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}
/******************************************************************************/