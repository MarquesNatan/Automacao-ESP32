/******************************************************************************/
#ifndef BOARD_PERIPHERAL_H
#define BOARD_PERIPHERAL_H
/******************************************************************************/
#include "../../system/include/system_defs.h"
/******************************************************************************/
#if defined SYSTEM_BUILD_TYPE && SYSTEM_BUILD_TYPE == DEBUG
    
    #define PIN_RELAY_0                     2
    #define PIN_RELAY_1                     0
    #define PIN_RELAY_2                     4
    #define PIN_RELAY_3                     16
    #define PIN_RELAY_4                     17

    #define PIN_SWITCH_0                     32 
    #define PIN_SWITCH_1                     33

    #define PIN_ZCD_0                        PIN_SWITCH_0
    #define PIN_TRIAC_ENABLE                 24

#elif defined SYSTEM_BUILD_TYPE && SYSTEM_BUILD_TYPE == RELEASE

    #define PIN_DIGITAL_OUTPUT_0                    0
    #define PIN_DIGITAL_OUTPUT_1                    1
    #define PIN_DIGITAL_OUTPUT_2                    2
    #define PIN_DIGITAL_OUTPUT_3                    3
    #define PIN_DIGITAL_OUTPUT_4                    4

#endif /* SYSTEM_BUILD_TYPE */
/******************************************************************************/
#endif /* BOARD_PERIPHERAL_H */
/******************************************************************************/