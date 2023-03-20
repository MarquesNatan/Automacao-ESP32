/******************************************************************************/
#ifndef BOARD_PERIPHERAL_H
#define BOARD_PERIPHERAL_H
/******************************************************************************/
#include "../../system/include/system_defs.h"
/******************************************************************************/
    /* Relay control */
    #define PIN_DIGITAL_RELAY_0         GPIO_NUM_0 
    #define PIN_DIGITAL_RELAY_1         GPIO_NUM_4
    #define PIN_DIGITAL_RELAY_2         GPIO_NUM_5
    #define PIN_DIGITAL_RELAY_3         GPIO_NUM_16
    #define PIN_DIGITAL_RELAY_4         GPIO_NUM_17

    /* Switch input */
    #define PIN_DIGITAL_SWITCH_0        GPIO_NUM_32 
    #define PIN_DIGITAL_SWITCH_1        GPIO_NUM_33

    /* Dimmer input control*/
    #define PIN_ANALOGIC_POT_0          GPIO_NUM_17

    /* Dimmer */
    #define PIN_ZCD_0                        4
    #define PIN_TRIAC_ENABLE                 23

    #define PIN_DIGITAL_LED_HEARTBEAT   GPIO_NUM_2
/******************************************************************************/
#endif /* BOARD_PERIPHERAL_H */
/******************************************************************************/