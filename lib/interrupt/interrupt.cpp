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
/******************************************************************************/
void Interrupt_Config(void *params)
{
    // attachInterruptArg(digitalPinToInterrupt(PIN_DIGITAL_INPUT_0), ISR_Handle, (void*)PIN_DIGITAL_INPUT_0, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_DIGITAL_INPUT_0), ISR_Handle, CHANGE);
    // attachInterruptArg(digitalPinToInterrupt(GPIO_NUM_39), ISR_Handle, (void*)GPIO_NUM_39, CHANGE);
}
/******************************************************************************/
void IRAM_ATTR ISR_Handle(void)
{
    button_time = millis();
    uint8_t state = digitalRead(PIN_DIGITAL_OUTPUT_1);

    if(button_time - last_debounce_time > 250)
    {
        ISRCount++;
        ISRIsTrigged = true;
        last_debounce_time = button_time;
    }
}
/******************************************************************************/