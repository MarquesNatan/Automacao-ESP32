/******************************************************************************/
#ifndef BOARD_PERIPHERAL_H
#define BOARD_PERIPHERAL_H
/******************************************************************************/
#include "../../system/include/system_defs.h"
/******************************************************************************/
#if defined SYSTEM_BUILD_TYPE && SYSTEM_BUILD_TYPE == DEBUG
    
    #define PIN_DIGITAL_OUTPUT_0                    15
    #define PIN_DIGITAL_OUTPUT_1                    2
    #define PIN_DIGITAL_OUTPUT_2                    0
    #define PIN_DIGITAL_OUTPUT_3                    4
    #define PIN_DIGITAL_OUTPUT_4                    13

    #define PIN_DIGITAL_INPUT_0                     23
    #define PIN_DIGITAL_INPUT_1                     39

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