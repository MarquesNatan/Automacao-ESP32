/******************************************************************************/
#include <Arduino.h>
#include "include/peripheral_controller.h"
#include "../../common/include/system_common.h"
#include "../../common/include/board_peripheral.h"
/******************************************************************************/
void Peripheral_Init(void *params)
{
    /* Configure digital outputs */
    Peripheral_DigitalConfigure(PIN_RELAY_0, OUTPUT);
    Peripheral_DigitalConfigure(PIN_RELAY_1, OUTPUT);
    Peripheral_DigitalConfigure(PIN_RELAY_2, OUTPUT);
    Peripheral_DigitalConfigure(PIN_RELAY_3, OUTPUT);
    Peripheral_DigitalConfigure(PIN_RELAY_4, OUTPUT);

    digitalWrite(PIN_RELAY_0, LOW);
    digitalWrite(PIN_RELAY_1, LOW);
    digitalWrite(PIN_RELAY_2, LOW);
    digitalWrite(PIN_RELAY_3, LOW);
    digitalWrite(PIN_RELAY_4, LOW);
    

    /* Pinos para entrada do interruptor */
    pinMode(PIN_SWITCH_0, INPUT_PULLDOWN);
    digitalWrite(PIN_SWITCH_0, LOW);

    /* Pinos de entrada analógica */
    pinMode(36, INPUT);
    digitalWrite(36, LOW);

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