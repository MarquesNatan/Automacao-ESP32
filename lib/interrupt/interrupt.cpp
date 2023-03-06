/******************************************************************************/
#include <Arduino.h>
#include <stdint.h>

#include "include/interrupt.h"
#include "../../src/common/include/board_peripheral.h"
#include "../../commands/include/command.h"
#include "../../src/system/include/system_defs.h"
/******************************************************************************/
volatile unsigned long button_time = 0;
volatile unsigned long last_debounce_time = 0;
volatile unsigned long ISRCount = 0;
volatile bool ISRIsTrigged = false;
volatile int argsValue = 0;
/******************************************************************************/
void Interrupt_Config(void *params)
{
    // attachInterruptArg(digitalPinToInterrupt(PIN_DIGITAL_INPUT_0), ISR_Handle, (void*)PIN_DIGITAL_INPUT_0, CHANGE);
    // attachInterrupt(digitalPinToInterrupt(PIN_SWITCH_0), ISR_Handle, CHANGE);
    attachInterruptArg(digitalPinToInterrupt(PIN_SWITCH_0), ISR_HandleAux, (void*)PIN_SWITCH_0, CHANGE);
    attachInterruptArg(digitalPinToInterrupt(PIN_SWITCH_1), ISR_HandleAux, (void*)PIN_SWITCH_1, CHANGE);
}
/******************************************************************************/
void IRAM_ATTR ISR_HandleAux(void *args)
{
    argsValue = (int)args;
    button_time = millis();

    if(button_time - last_debounce_time > 250)
    {
        ISRCount++;
        ISRIsTrigged = true;
        last_debounce_time = button_time;
    }
}
/******************************************************************************/
void IRAM_ATTR ISR_Handle(void)
{
    button_time = millis();
    if(button_time - last_debounce_time > 250)
    {
        ISRCount++;
        ISRIsTrigged = true;
        last_debounce_time = button_time;
    }
}
/******************************************************************************/