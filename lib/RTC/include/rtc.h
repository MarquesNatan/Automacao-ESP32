/******************************************************************************/
#ifndef RTC_H
#define RTC_H 
/******************************************************************************/
#include <stdint.h>
#include "RTClib.h"
/******************************************************************************/
void RTC_Init(void* params);
DateTime RTC_GetTime(void *params);

uint16_t RTC_GetYear(void);
uint8_t RTC_GetMonth(void);
uint8_t RTC_GetDay(void);

uint8_t RTC_GetHour(void);
uint8_t RTC_GetMinute(void);
uint8_t RTC_GetSecond(void);
void ISR_RTCAlarm(void);
/******************************************************************************/
#endif /* RTC_H */
/******************************************************************************/
