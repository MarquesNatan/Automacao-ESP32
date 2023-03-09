/******************************************************************************/
#include <Arduino.h>
#include <stdint.h>

#include "include/interrupt.h"
#include "../../src/common/include/board_peripheral.h"
#include "../../dimmer/include/dimmer.h"
#include "freertos/semphr.h"
/******************************************************************************/
volatile unsigned long button_time = 0;
volatile unsigned long last_debounce_time = 0;
volatile unsigned long ISRCount = 0;
volatile bool ISRIsTrigged = false;
volatile int argsValue = 0;
/******************************************************************************/
void Interrupt_Config(void *params)
{
    /* Interrupção do switch */
    // attachInterruptArg(digitalPinToInterrupt(PIN_SWITCH_0), ISR_HandleAux, (void*)PIN_SWITCH_0, CHANGE);
    attachInterruptArg(digitalPinToInterrupt(PIN_SWITCH_1), ISR_HandleAux, (void*)PIN_SWITCH_1, CHANGE);

    /* Interrupção do ZCD */
    attachInterrupt(digitalPinToInterrupt(PIN_ZCD_0), DimmerZCDTrigger, FALLING);

    /* Interrupção do Timer */
}
/******************************************************************************/
void IRAM_ATTR ISR_HandleAux(void *args)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    argsValue = (int)args;
    button_time = millis();

    if(button_time - last_debounce_time > 250)
    {
        ISRCount++;
        ISRIsTrigged = true;
        last_debounce_time = button_time;

        // xSemaphoreGiveFromISR(ZOHtrigged, &xHigherPriorityTaskWoken);
    }
}
/******************************************************************************/