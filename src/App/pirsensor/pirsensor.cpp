/******************************************************************************/
#include "include/pirsensor.h"
#include "../../common/include/board_peripheral.h"
#include "Arduino.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
/******************************************************************************/
xSemaphoreHandle xSemaphorePirSensor;
xSemaphoreHandle xSemaphoreLDRsensor;
/******************************************************************************/
portMUX_TYPE TimerLDRMux = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE PirSensorMux = portMUX_INITIALIZER_UNLOCKED;
/******************************************************************************/
hw_timer_t *TimerLDRDebouce = timerBegin(2, 80, true);
hw_timer_t *TimerTimeoutPirSensor = timerBegin(3, 80, true);
/******************************************************************************/
bool timeout = false;
/******************************************************************************/
void PirSensorAttachInterrutps( void )
{
    attachInterrupt(digitalPinToInterrupt(PIN_DIGITAL_PIR_IN), &ISR_PirSensor, RISING);
    timerAttachInterrupt(TimerLDRDebouce, &ISR_DebounceLDR  , true);
    timerAttachInterrupt(TimerTimeoutPirSensor, &ISR_TimeoutPirSensor  , true);
}
/******************************************************************************/
void SetTimerLDRDebounce( void )
{
    timerWrite(TimerLDRDebouce, 0);
    timerAlarmWrite(TimerLDRDebouce, LDR_DEBOUNCE_TIME_US, false);
    timerAlarmEnable(TimerLDRDebouce);
}
/******************************************************************************/
void IRAM_ATTR ISR_DebounceLDR( void )
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        
    portENTER_CRITICAL_ISR(&TimerLDRMux);

        timerAlarmDisable(TimerLDRDebouce);

        xSemaphoreGiveFromISR(xSemaphoreLDRsensor, &xHigherPriorityTaskWoken);

        if(xHigherPriorityTaskWoken == pdTRUE)
        {
            portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
        }
    portEXIT_CRITICAL(&TimerLDRMux);

}
/******************************************************************************/
void IRAM_ATTR ISR_PirSensor( void )
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        
    portENTER_CRITICAL_ISR(&PirSensorMux);

        timeout = false;
        xSemaphoreGiveFromISR(xSemaphorePirSensor, &xHigherPriorityTaskWoken);

        if(xHigherPriorityTaskWoken == pdTRUE)
        {
            portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
        }
    portEXIT_CRITICAL(&PirSensorMux);

}
/******************************************************************************/
void IRAM_ATTR ISR_TimeoutPirSensor( void )
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        
    portENTER_CRITICAL_ISR(&PirSensorMux);

        timeout = true;

        if(xHigherPriorityTaskWoken == pdTRUE)
        {
            portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
        }
    portEXIT_CRITICAL(&PirSensorMux);

}
/******************************************************************************/
void SetTimerTimeout( void )
{

    timerAlarmDisable(TimerTimeoutPirSensor);

    timeout = false;

    timerWrite(TimerTimeoutPirSensor, 0);
    timerAlarmWrite(TimerTimeoutPirSensor, PIR_SENSOR_TIMEOUT_US, false);
    timerAlarmEnable(TimerTimeoutPirSensor);
}
/******************************************************************************/
void vTaskPirSensorHandle( void * pvParameters)
{
    bool ldrIsEnabled = false;
    bool LDRCurrState = false;

    uint8_t outputValue = 0;

    if(xSemaphorePirSensor == NULL)
    {
        xSemaphorePirSensor = xSemaphoreCreateBinary();
        if(xSemaphorePirSensor == NULL)
        {
            #if PIR_SENSOR_DEBUG == true
                Serial.printf("Erro ao criar semaforo do sensor de presença.\n");
            #endif /* PIR_SENSOR_DEBUG */
            return;
        }
    }

    if(xSemaphoreLDRsensor == NULL)
    {
        xSemaphoreLDRsensor = xSemaphoreCreateBinary();
        if(xSemaphoreLDRsensor == NULL)
        {
            #if PIR_SENSOR_DEBUG == true
                Serial.printf("Erro ao criar semaforo do sensor de luminosidade.\n");
            #endif /* PIR_SENSOR_DEBUG */

            return;
        }
    }

    PirSensorAttachInterrutps();


    for(;;)
    {
        ldrIsEnabled = (bool)digitalRead(PIN_DIGITAL_LDR_IN);

        if(ldrIsEnabled)
        {
            SetTimerLDRDebounce();

            if(xSemaphoreTake(xSemaphoreLDRsensor, portMAX_DELAY) == pdTRUE)
            {
                LDRCurrState = (bool)digitalRead(PIN_DIGITAL_LDR_IN);

                
                if(LDRCurrState == ldrIsEnabled)
                {
                    /* Verificação do sensor de presença */
                    if(xSemaphoreTake(xSemaphorePirSensor, 0) == pdTRUE)
                    {
                        Serial.printf("Semaforo PIR capturado com sucesso.\n");
                        detachInterrupt(digitalPinToInterrupt(PIN_DIGITAL_PIR_IN));

                        while(digitalRead(PIN_DIGITAL_PIR_IN))
                        {
                            vTaskDelay(pdMS_TO_TICKS(300));
                        }

                        outputValue = digitalRead(OUTPUT_ENABLED_PIR_SENSOR);
                        if(!outputValue)
                        {
                            #if PIR_SENSOR_DEBUG == true
                                Serial.printf("Ligando led, sensor de presença ativado.\n");
                            #endif /* PIR_SENSOR_DEBUG */

                            digitalWrite(OUTPUT_ENABLED_PIR_SENSOR, HIGH);
                        }

                        /* A cada pulso do sensor de presença o timeout é reiniciado  */
                        SetTimerTimeout();

                        /* Reanexa a interrupção */
                        attachInterrupt(digitalPinToInterrupt(PIN_DIGITAL_PIR_IN), &ISR_PirSensor, RISING);

                    }
                    else if(timeout)
                    {
                        outputValue = digitalRead(OUTPUT_ENABLED_PIR_SENSOR);
                        if(outputValue)
                        {
                            #if PIR_SENSOR_DEBUG == true
                                Serial.printf("DELIGANDO OUTPUT: TIMEOUT \n");
                            #endif /* PIR_SENSOR_DEBUG */

                            digitalWrite(OUTPUT_ENABLED_PIR_SENSOR, LOW);

                            /* Inicia o timeout */
                            timeout = false;
                        }
                        
                    }
                    
                }
                else if(!LDRCurrState)
                {

                    outputValue = digitalRead(OUTPUT_ENABLED_PIR_SENSOR);
                    if(outputValue)
                    {
                         #if PIR_SENSOR_DEBUG == true
                            Serial.printf("DELIGANDO OUTPUT: Mundança no nível do LDR.\n");
                        #endif /* PIR_SENSOR_DEBUG */

                        digitalWrite(OUTPUT_ENABLED_PIR_SENSOR, LOW);

                        /* Inicia o timeout */
                    }

                }
            }

        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
/******************************************************************************/
