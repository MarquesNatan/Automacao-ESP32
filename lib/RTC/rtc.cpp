/******************************************************************************/
#include "Arduino.h"

#include "include/rtc.h"
#include "Wire.h"
#include "RTClib.h"

#include "../../src/system/include/system.h"
#include "../../src/common/include/board_peripheral.h"
#include "freertos/semphr.h"

/******************************************************************************/
RTC_DS3231 rtc;
xSemaphoreHandle xRTCSemaphore;
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

    #if RTC_USE_ALARM == true
        /* Disable all alarms */
        rtc.disableAlarm(1);
        rtc.disableAlarm(2);
        rtc.clearAlarm(1);
        rtc.clearAlarm(2);

        /* Alarm pin in interrup mode */
        rtc.writeSqwPinMode(DS3231_OFF);

        /* Interrupt Service Routine */
        attachInterrupt(digitalPinToInterrupt(PIN_DIGITAL_SQW_RTC), ISR_RTCAlarm, FALLING);
    #endif /* RTC_USE_ALARM */

    rtc.adjust(DateTime(2023, 3, 26, 16, 0, 0));
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
uint8_t RTC_GetWeekDay(void)
{
    DateTime now = rtc.now();

    #if RTC_DEBUG == true
        Serial.printf("Segundo: %i\n", now.dayOfTheWeek());
    #endif /* RTC_DEBUG */

    return now.dayOfTheWeek();
}
/******************************************************************************/
void RTC_SetAlarm(DateTime datetime, uint8_t mode)
{

}
/******************************************************************************/
void ISR_RTCAlarm(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    /* Quando existir um alarme, libera o semaforo para a task que está esperando */
    xSemaphoreGiveFromISR(xRTCSemaphore, &xHigherPriorityTaskWoken);
    if(xHigherPriorityTaskWoken == pdTRUE)
    {
        portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
    }

}
/******************************************************************************/