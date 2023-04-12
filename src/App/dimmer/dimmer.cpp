/******************************************************************************/
#include "include/dimmer.h"

#include "Arduino.h"

#include "../peripheral/include/peripheral_controller.h"
#include "../../common/include/board_peripheral.h"

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

/******************************************************************************/
SemaphoreHandle_t xDimmerSemaphore_ZCD;
SemaphoreHandle_t xDimmerSemaphore_Timer;
SemaphoreHandle_t xDimmerSemaphore_TimerLow;
/******************************************************************************/
hw_timer_t *timerToPinHigh = timerBegin(1, 80, true);
hw_timer_t *timerToPinLow = timerBegin(0, 80, true);
/******************************************************************************/
volatile bool isPinHighEnable = false;
/******************************************************************************/
portMUX_TYPE DimmerMux = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE DimmerMuxTimerPinLow = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE DimmerMuxTimerPinHigh = portMUX_INITIALIZER_UNLOCKED;
volatile bool ISRDimmer_Enable = false;
/******************************************************************************/
void DimmerConfig( void )
{
    xDimmerSemaphore_ZCD = xSemaphoreCreateBinary();
    xDimmerSemaphore_Timer = xSemaphoreCreateBinary();
    xDimmerSemaphore_TimerLow = xSemaphoreCreateBinary();

    if(xDimmerSemaphore_ZCD == NULL || xDimmerSemaphore_Timer == NULL || xDimmerSemaphore_TimerLow == NULL)
    {
        Serial.printf("\nErro ao criar xSemaphoreDimmer OU xDimmerSemaphore_Timer\n");

        while(true);
    }
    
    /* Configura a interrupção para o ZCD */
    attachInterruptArg(digitalPinToInterrupt(PIN_DIGITAL_DIMMER_IN), ISR_Dimmer, (void*)PIN_DIGITAL_DIMMER_IN, RISING);
    timerAttachInterrupt(timerToPinHigh, &ISR_TurnPinHigh, true);
    timerAttachInterrupt(timerToPinLow, &ISR_TurnPinLow, true);
}
/******************************************************************************/
void IRAM_ATTR ISR_Dimmer( void *args )
{

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        
    portENTER_CRITICAL_ISR(&DimmerMux);

        xSemaphoreGiveFromISR(xDimmerSemaphore_ZCD, &xHigherPriorityTaskWoken);

        if(xHigherPriorityTaskWoken == pdTRUE)
        {
            portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
        }
    portEXIT_CRITICAL(&DimmerMux);

}
/******************************************************************************/
void IRAM_ATTR ISR_TurnPinLow( void )
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    portENTER_CRITICAL_ISR(&DimmerMux);
        digitalWrite(PIN_DIGITAL_DIMMER_OUT, LOW);

         if(timerAlarmEnabled(timerToPinLow))
        {
            timerAlarmDisable(timerToPinLow);
        }
    

        isPinHighEnable = false;

        xSemaphoreGiveFromISR(xDimmerSemaphore_TimerLow, &xHigherPriorityTaskWoken);

        if(xHigherPriorityTaskWoken == pdTRUE)
        {
           portYIELD_FROM_ISR( xHigherPriorityTaskWoken );        
        }

        
    portEXIT_CRITICAL_ISR(&DimmerMux);

}
/******************************************************************************/
void IRAM_ATTR SetTimerPinLow( void )
{
    isPinHighEnable = false;

    timerWrite(timerToPinLow, 0);
    timerAlarmWrite(timerToPinLow, DIMMER_ENABLE_PULSE_US, false);
    timerAlarmEnable(timerToPinLow);
}
/******************************************************************************/
void IRAM_ATTR ISR_TurnPinHigh( void )
{

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    portENTER_CRITICAL_ISR(&DimmerMux);
        
        if(timerAlarmEnabled(timerToPinHigh))
        {
            timerAlarmDisable(timerToPinHigh);
        }
    
        digitalWrite(PIN_DIGITAL_DIMMER_OUT, HIGH);

        xSemaphoreGiveFromISR(xDimmerSemaphore_Timer, &xHigherPriorityTaskWoken);

        if(xHigherPriorityTaskWoken == pdTRUE)
        {
            portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
        }
    portEXIT_CRITICAL_ISR(&DimmerMux);

}
/******************************************************************************/
void IRAM_ATTR SetTimerPinHigh( int16_t brightness )
{

    isPinHighEnable = true;
    
    timerWrite(timerToPinHigh, 0);
    timerAlarmWrite(timerToPinHigh, 6500, false);
    timerAlarmEnable(timerToPinHigh);

}
/******************************************************************************/
void brightnessCalc(uint16_t potRead)
{

}
/******************************************************************************/
void vTaskDimmer( void *pvParameters)
{
    DimmerConfig();

    vTaskDelay(1000 / portTICK_PERIOD_MS);

    for(;;)
    {

        if(xSemaphoreTake( xDimmerSemaphore_ZCD, portMAX_DELAY) == pdTRUE)
        {
            /* Desativa a interrupção */
            detachInterrupt(digitalPinToInterrupt(PIN_DIGITAL_DIMMER_IN));

            /* Aguarda o pino ficar em nível baixo */
            while(digitalRead(PIN_DIGITAL_DIMMER_IN));

            SetTimerPinHigh(4000);

            if(xSemaphoreTake(xDimmerSemaphore_Timer, portMAX_DELAY) == pdTRUE)
            {
                SetTimerPinLow();

                if(xSemaphoreTake(xDimmerSemaphore_TimerLow, portMAX_DELAY) == pdTRUE)
                {
                    /* Ativa a interrupção novamente */
                    attachInterruptArg(digitalPinToInterrupt(PIN_DIGITAL_DIMMER_IN), ISR_Dimmer, (void*)PIN_DIGITAL_DIMMER_IN, RISING);
                }
            }
            
        }
    }

}
/******************************************************************************/