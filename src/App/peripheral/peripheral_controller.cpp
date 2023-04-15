/******************************************************************************/
#include <Arduino.h>
#include "include/peripheral_controller.h"
#include "../../common/include/system_common.h"
#include "../../common/include/board_peripheral.h"
/******************************************************************************/
void Peripheral_Init(void *params)
{
    pinMode(PIN_DIGITAL_RELAY_0, OUTPUT);
    pinMode(PIN_DIGITAL_RELAY_1, OUTPUT);
    pinMode(PIN_DIGITAL_RELAY_2, OUTPUT);
    pinMode(PIN_DIGITAL_RELAY_3, OUTPUT);
    pinMode(PIN_DIGITAL_RELAY_4, OUTPUT);

    digitalWrite(PIN_DIGITAL_RELAY_0, LOW);
    digitalWrite(PIN_DIGITAL_RELAY_1, LOW);
    digitalWrite(PIN_DIGITAL_RELAY_2, LOW);
    digitalWrite(PIN_DIGITAL_RELAY_3, LOW);
    digitalWrite(PIN_DIGITAL_RELAY_4, LOW);
    
    pinMode(PIN_DIGITAL_SWITCH_0, INPUT);
    pinMode(PIN_DIGITAL_SWITCH_1, INPUT);
    pinMode(PIN_DIGITAL_SWITCH_MODE, INPUT);

    pinMode(PIN_DIGITAL_LED_HEARTBEAT, OUTPUT);
    pinMode(PIN_DIGITAL_LED_ERRO, OUTPUT);

    pinMode(PIN_DIGITAL_SQW_RTC, INPUT_PULLUP);

    pinMode(PIN_DIGITAL_DIMMER_IN, INPUT);

    pinMode(PIN_DIGITAL_DIMMER_OUT, OUTPUT);
    digitalWrite(PIN_DIGITAL_DIMMER_OUT, LOW);

    pinMode(PIN_ANALOGIC_DIMMER_POT, INPUT);
    

    Serial.begin(115200);
}
/******************************************************************************/
uint8_t Peripheral_DigitalRead(uint8_t pin)
{
    return digitalRead(pin);
}
/******************************************************************************/