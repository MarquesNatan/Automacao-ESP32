/******************************************************************************/
#include "include/util.h"
#include "stdlib.h"
/******************************************************************************/
uint8_t charToInt(uint8_t buffer[], uint8_t offset, uint8_t length)
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
bool ValueIsPresent(uint8_t value, uint8_t listValues[], uint8_t startPosition, uint8_t length)
{
    for (int i = 0; i < length; i++)
    {
        if (listValues[i + startPosition] == value)
        {
            return true;
        }
    }

    return false;
}
/******************************************************************************/