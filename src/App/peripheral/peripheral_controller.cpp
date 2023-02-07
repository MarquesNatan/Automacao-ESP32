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

    /* Configure Analogic outputs */
}
/******************************************************************************/
inline static void Peripheral_DigitalConfigure(uint8_t pin, uint8_t type)
{
    pinMode(pin, type);
}
/******************************************************************************/