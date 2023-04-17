/******************************************************************************/
#ifndef BOARD_PERIPHERAL_H
#define BOARD_PERIPHERAL_H
/******************************************************************************/
#include "../../system/include/system_defs.h"
/******************************************************************************/
    /* Relay control */
    #define PIN_DIGITAL_RELAY_0         GPIO_NUM_19
    #define PIN_DIGITAL_RELAY_1         GPIO_NUM_4
    #define PIN_DIGITAL_RELAY_2         GPIO_NUM_5
    #define PIN_DIGITAL_RELAY_3         GPIO_NUM_16
    #define PIN_DIGITAL_RELAY_4         GPIO_NUM_17

    /* Switch input */
    #define PIN_DIGITAL_SWITCH_0        GPIO_NUM_32
    #define PIN_DIGITAL_SWITCH_1        GPIO_NUM_33
    #define PIN_DIGITAL_SWITCH_MODE      GPIO_NUM_36

    #define PIN_DIGITAL_SQW_RTC         GPIO_NUM_26

    #define PIN_DIGITAL_LED_HEARTBEAT   GPIO_NUM_2
    #define PIN_DIGITAL_LED_ERRO        GPIO_NUM_27

    #define PIN_DIGITAL_DIMMER_IN       GPIO_NUM_13
    #define PIN_DIGITAL_DIMMER_OUT      GPIO_NUM_14
    #define PIN_ANALOGIC_DIMMER_POT     GPIO_NUM_39

    #define PIN_DIGITAL_PIR_IN          GPIO_NUM_26

/******************************************************************************/
#endif /* BOARD_PERIPHERAL_H */
/******************************************************************************/