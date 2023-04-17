/******************************************************************************/
#ifndef UTIL_H
#define UTIL_H
/******************************************************************************/
#include "stdint.h"
/******************************************************************************/
uint8_t charToInt(uint8_t buffer[], uint8_t offset, uint8_t length);
bool ValueIsPresent(uint8_t value, uint8_t listValues[], uint8_t startPosition, uint8_t length);
bool ValidateDateTime(uint8_t hour, uint8_t minute);
/******************************************************************************/
#endif /* UTIL_H */
/******************************************************************************/