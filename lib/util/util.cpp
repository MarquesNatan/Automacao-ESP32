/******************************************************************************/
#include "stdlib.h"
/******************************************************************************/
uint8_t CharToByte(uint8_t buffer[], uint8_t offset, uint8_t length)
{
    char temp[length];
    uint8_t value;

    for(int i = 0; i < length; i++)
    {
        temp[i] = (char)buffer[offset + i];
    }

    value = (uint8_t) strtol(temp, NULL, 16);

    return value;
}
/******************************************************************************/