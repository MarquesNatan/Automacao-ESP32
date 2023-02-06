/******************************************************************************/
#include "include/command.h"
/******************************************************************************/
command_t Command_Parse(uint8_t *comm)
{
    command_t command;
    // comm[] = type=output&index=10&value=low
    uint8_t i = 0;
    uint8_t j = 0;

    while (comm[i] != (uint8_t)'\0')
    {
        if(comm[i] == (uint8_t)'=')
        {
            command.command[j] = comm[i + 1];
            j = j + 1;
        }
        i = i + 1;
    }

    return command;
}
/******************************************************************************/