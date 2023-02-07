/******************************************************************************/
#include <Arduino.h>
#include <stdio.h>
#include "include/command.h"
#include "../../src/common/include/protocol_common.h"
#include "../../src/system/include/system_defs.h"
/******************************************************************************/
command_t Command_Parse(uint8_t *comm)
{
    // <FF:FF:FF>
    command_t command;
    char temp[2];
    int val = 0;

    uint8_t i = 0;
    uint8_t j = 0;

    if(comm[0] == COMMAND_SOF)
    {
        while (comm[i] != COMMAND_EOF)
        {
            if(comm[i] != (uint8_t)COMMAND_SOF && comm[i] != (uint8_t)COMMAND_PARAMS_TOKEN)
            {
                temp[0] = comm[i];
                temp[1] = comm[i + 1];

                val = (int)(strtol(temp, NULL, 16));
                Serial.printf("%i", val);
                i = i + 1;
            }
            else 
            {
                Serial.print(" ");
            }
            i = i + 1;
        }
    }
    return command;
}
/******************************************************************************/