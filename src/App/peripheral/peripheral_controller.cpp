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
    
    pinMode(PIN_DIGITAL_SWITCH_0, INPUT);
    pinMode(PIN_DIGITAL_SWITCH_1, INPUT);

    // pinMode(PIN_ANALOGIC_POT_0, INPUT);
    // pinMode(PIN_TRIAC_ENABLE, OUTPUT);
    // pinMode(PIN_ZCD_0, INPUT);
    // digitalWrite(PIN_TRIAC_ENABLE, LOW);

    pinMode(PIN_DIGITAL_LED_HEARTBEAT, OUTPUT);

    pinMode(PIN_DIGITAL_SQW_RTC, INPUT_PULLUP);

    Serial.begin(115200);
}
/******************************************************************************/