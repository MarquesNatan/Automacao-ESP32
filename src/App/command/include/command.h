/******************************************************************************/
#ifndef NEW_COMMAND_H
#define NEW_COMMAND_H
/******************************************************************************/
#include "../../../system/include/system_defs.h"
#include "../../../common/include/board_peripheral.h"

#include <stdint.h>
/******************************************************************************/
typedef bool (*command_callback_func)(const char *, const char *);
/******************************************************************************/
#define COMMAND_DATA_LENGTH             18
#define COMMAND_CRC_LENGTH              2
/******************************************************************************/
#define COMMAND_TYPES_LEGTH             BOARD_KNOW_COMMANDS

#define COMMAND_TURN_ON                 0
#define COMMAND_TURN_OFF                1
#define COMMAND_TOGGLE                  2
#define COMMAND_ANALOGIC                3
#define COMMAND_READ_PIN                4

#define OUTPUT_LENGTH                   BOARD_DIGITAL_OUTPUT

#define OUTPUT_00                       PIN_DIGITAL_RELAY_0
#define OUTPUT_01                       PIN_DIGITAL_RELAY_1
#define OUTPUT_02                       PIN_DIGITAL_RELAY_2
#define OUTPUT_03                       PIN_DIGITAL_RELAY_3
#define OUTPUT_04                       PIN_DIGITAL_RELAY_4
#define OUTPUT_ANALOGIC                 00
/******************************************************************************/
typedef struct 
{
    uint8_t data[COMMAND_DATA_LENGTH];
    uint8_t crc[COMMAND_CRC_LENGTH];
    uint8_t length;
}newcommand_t;
/******************************************************************************/
void vTaskCommandHandle( void *pvParameters );
void vTaskCommandRun( void *pvParameters );
void SetCommandCallback(command_callback_func ptr);
bool ValidateCommand(newcommand_t command, uint8_t receiver[]);
/******************************************************************************/
#endif /* NEW_COMMAND_H */
/******************************************************************************/
