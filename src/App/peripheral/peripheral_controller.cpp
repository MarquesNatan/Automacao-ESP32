/******************************************************************************/
#include <Arduino.h>
#include "include/peripheral_controller.h"
#include "../../common/include/system_common.h"
#include "../../common/include/board_peripheral.h"
/******************************************************************************/
void Peripheral_Init(void *params)
{
    /* Configure digital outputs */
    Peripheral_DigitalConfigure(PIN_DIGITAL_OUTPUT_0, OUTPUT);
    Peripheral_DigitalConfigure(PIN_DIGITAL_OUTPUT_1, OUTPUT);
    Peripheral_DigitalConfigure(PIN_DIGITAL_OUTPUT_2, OUTPUT);
    Peripheral_DigitalConfigure(PIN_DIGITAL_OUTPUT_3, OUTPUT);
    Peripheral_DigitalConfigure(PIN_DIGITAL_OUTPUT_4, OUTPUT);

    /* Pinos para entrada do interruptor */
    pinMode(PIN_DIGITAL_INPUT_0, INPUT_PULLDOWN);

    Serial.begin(115200);
}
/******************************************************************************/
void Peripheral_DigitalConfigure(uint8_t pin, uint8_t type)
{
    pinMode(pin, type);
}
/******************************************************************************/
void Peripheral_DigitalWrite(uint8_t num, uint8_t state)
{
    digitalWrite(num, state);
}
/******************************************************************************/