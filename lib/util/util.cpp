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
void decimalToHexadecimal(int decimal, char *hexadecimal) {
    
    if (decimal == 0) {
        hexadecimal[0] = '0';
        hexadecimal[1] = '0';
        hexadecimal[2] = '\0';
        return;
    }

    char hexMap[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    int i = 0;

    while (decimal > 0) {
        int remainder = decimal % 16;
        hexadecimal[i] = hexMap[remainder];
        decimal = decimal / 16;
        i++;
    }

    if (i == 1) {
        hexadecimal[1] = hexadecimal[0];
        hexadecimal[0] = '0';
        hexadecimal[2] = '\0';
    } else {
        int j;
        int k = i - 1;
        char temp;

        for (j = 0; j < i / 2; j++) {
            temp = hexadecimal[j];
            hexadecimal[j] = hexadecimal[k];
            hexadecimal[k] = temp;
            k--;
        }

        hexadecimal[i] = '\0';
    }
}

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