/******************************************************************************/
#include "include/rtc.h"
#include "Wire.h"
#include "RTClib.h"

#include "../../../src/system/include/system_defs.h"
/******************************************************************************/
RTC_DS3231 rtc;
/******************************************************************************/
void RTC_Init(void* params)
{
    if(!rtc.begin())
    {
        #if RTC_DEBUG == true
            Serial.printf("Não foi possível iniciar o RTC");
        #endif /* RTC_DEBUG */
        while(true);
    }

    rtc.adjust(DateTime(__DATE__, __TIME__));
}
/******************************************************************************/
DateTime RTC_GetTime(void *params)
{
    DateTime now = rtc.now();
    return now;
}
/******************************************************************************/
uint16_t RTC_GetYear(void)
{
    DateTime now = rtc.now();
    #if RTC_DEBUG == true
        Serial.printf("Ano: %i\n", now.year());
    #endif /* RTC_DEBUG */
    return now.year();
}
/******************************************************************************/
uint8_t RTC_GetMonth(void)
{
    DateTime now = rtc.now();

    #if RTC_DEBUG == true
        Serial.printf("Mês: %i\n", now.month());
    #endif /* RTC_DEBUG */

    return now.month();
}
/******************************************************************************/
uint8_t RTC_GetDay(void)
{
    DateTime now = rtc.now();

    #if RTC_DEBUG == true
        Serial.printf("Dia: %i\n", now.day());
    #endif /* RTC_DEBUG */

    return now.day();
}
/******************************************************************************/
uint8_t RTC_GetHour(void)
{
    DateTime now = rtc.now();

    #if RTC_DEBUG == true
        Serial.printf("Hora: %i\n", now.hour());
    #endif /* RTC_DEBUG */

    return now.hour();
}
/******************************************************************************/
uint8_t RTC_GetMinute(void)
{
    DateTime now = rtc.now();
    
    #if RTC_DEBUG == true
        Serial.printf("Minuto: %i\n", now.minute());
    #endif /* RTC_DEBUG */

    return now.minute();
}
/******************************************************************************/
uint8_t RTC_GetSecond(void)
{
    DateTime now = rtc.now();
    
    #if RTC_DEBUG == true
        Serial.printf("Segundo: %i\n", now.year());
    #endif /* RTC_DEBUG */

    return now.second();
}
/******************************************************************************/