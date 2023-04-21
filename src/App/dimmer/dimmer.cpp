/******************************************************************************/
#include "include/dimmer.h"

#include "Arduino.h"

#include "../peripheral/include/peripheral_controller.h"
#include "../../common/include/board_peripheral.h"

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

/******************************************************************************/
SemaphoreHandle_t xDimmerSemaphore_ZCD;
SemaphoreHandle_t xDimmerSemaphore_Timer;
SemaphoreHandle_t xDimmerSemaphore_TimerLow;
/******************************************************************************/
xQueueHandle xQueueChangeDimmerValue;
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
static uint16_t movingAverageBuffer[DIMMER_MOVING_AVERAGE_SAMPLE];
/******************************************************************************/
void DimmerConfig( void )
{
    xDimmerSemaphore_ZCD = xSemaphoreCreateBinary();
    xDimmerSemaphore_Timer = xSemaphoreCreateBinary();
    xDimmerSemaphore_TimerLow = xSemaphoreCreateBinary();

    if(xDimmerSemaphore_ZCD == NULL || xDimmerSemaphore_Timer == NULL || xDimmerSemaphore_TimerLow == NULL)
    {
        Serial.printf("\nErro ao criar xSemaphoreDimmer OU xDimmerSemaphore_Timer\n");

        return;
    }

    xQueueChangeDimmerValue = xQueueCreate(1, sizeof(uint16_t));

    if(xQueueChangeDimmerValue == NULL)
    {
        Serial.printf("Erro ao criar fila DimmerValue.\n");
        return;
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

    timerWrite(timerToPinHigh, brightness);
    timerAlarmWrite(timerToPinHigh, DIMMER_MAX_DELAY_US, false);
    timerAlarmEnable(timerToPinHigh);

}
/******************************************************************************/
uint16_t brightnessCalc( void )
{
    uint16_t potRead;
    potRead = analogRead(25);

    uint16_t brightness = (uint16_t) (potRead * 1.9);

    // brightness = (7250 - brightness);

    if(potRead <= 10)
    {
      brightness = 0;
    }

    if (potRead < 4050)
    {
      brightness = 4000;
    }

    Serial.printf("pot: %i brilho: %i\n", potRead, brightness);

    return brightness;
}
/******************************************************************************/
long int CalculaMediaMovel(long int value)
{

    long int sum = 0;

    for(int i = DIMMER_MOVING_AVERAGE_SAMPLE - 1; i > 0; i--)
    {
        movingAverageBuffer[i] = movingAverageBuffer[i - 1];
    }

    movingAverageBuffer[0] = value;

    for(int i = 0; i < DIMMER_MOVING_AVERAGE_SAMPLE; i++)
    {
        sum += movingAverageBuffer[i];
    }

    return sum / DIMMER_MOVING_AVERAGE_SAMPLE;

}
/******************************************************************************/
void vTaskDimmer( void *pvParameters)
{
    uint16_t brightness = 0;
    uint16_t potValue = 0;

    bool useWebAdjust =  false;

    uint16_t LastReceivedValue = 0;
    uint16_t ReceivedValue = 0;

    long int movingAverageValue = 0;
    long int lastMovingAverage = 0;

    DimmerConfig();

    vTaskDelay(1000 / portTICK_PERIOD_MS);

    for(;;)
    {

        potValue = analogRead(PIN_ANALOGIC_DIMMER_POT);

        if(xQueueReceive(xQueueChangeDimmerValue, &ReceivedValue, 0) == pdTRUE)
        {
            ReceivedValue = (uint16_t)map(ReceivedValue, 0, 100, 0, 4095);
            useWebAdjust = true;

            #if DIMMER_DEBUG == true 
                Serial.printf("valor recebido: %i\n", ReceivedValue);
            #endif /* DIMMER_DEBUG */
        }
        else 
        {
            movingAverageValue = CalculaMediaMovel(potValue);

            if(abs(movingAverageValue - lastMovingAverage) > 30)
            {
                if(useWebAdjust == false)
                {
                    if(lastMovingAverage != 0) 
                    {
                        ReceivedValue = movingAverageValue;
                    }

                    lastMovingAverage = movingAverageValue;
                }

                /* Caso esteja usando o valor web, é necessário levar a média movel para zero */
                else if(useWebAdjust == true && movingAverageValue == 0)
                {
                    useWebAdjust = false;
                }
            }

        }

        brightness = ((ReceivedValue - DIMMER_MIN_VALUE) * (DIMMER_MAX_DELAY_US - DIMMER_MIN_DELAY_US)) / ((DIMMER_MAX_VALUE - DIMMER_MIN_VALUE) + DIMMER_MIN_DELAY_US);
        LastReceivedValue = ReceivedValue;


        if(xSemaphoreTake( xDimmerSemaphore_ZCD, portMAX_DELAY ) == pdTRUE)
        {
            /* Desativa a interrupção */
            detachInterrupt(digitalPinToInterrupt(PIN_DIGITAL_DIMMER_IN));

            /* Aguarda o pino ficar em nível baixo */
            while(digitalRead(PIN_DIGITAL_DIMMER_IN));

            SetTimerPinHigh(brightness);

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