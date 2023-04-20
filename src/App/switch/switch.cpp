/******************************************************************************/
#include "include/switch.h"
#include "driver/gpio.h"

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

#include "Arduino.h"

#include "../peripheral/include/peripheral_controller.h"
#include "../../common/include/board_peripheral.h"
/******************************************************************************/
volatile bool ISR_Trigged = false;
volatile int ISRPinSource = 0;
uint8_t ISR_Edge = CHANGE;

SemaphoreHandle_t xSwitchSemaphore;
portMUX_TYPE SwitchMux = portMUX_INITIALIZER_UNLOCKED;
/******************************************************************************/
#define GPIO_INPUT_IO_0 GPIO_NUM_32
/******************************************************************************/
void SwitchISR_Configure( void )
{
    uint8_t modeSelect = INTERRUPTOR;
    modeSelect = digitalRead(PIN_DIGITAL_SWITCH_MODE);

    if(modeSelect == PULSADOR)
    {
        ISR_Edge = RISING;
        Serial.printf("Modo: PULSADOR\n");
    }
    else 
    {
        Serial.printf("Modo: INTERRUPTOR\n");
    }

    attachInterruptArg(digitalPinToInterrupt(PIN_DIGITAL_SWITCH_0), ISR_AUX_FUNCTION, (void*)PIN_DIGITAL_SWITCH_0, ISR_Edge);
    attachInterruptArg(digitalPinToInterrupt(PIN_DIGITAL_SWITCH_1), ISR_AUX_FUNCTION, (void*)PIN_DIGITAL_SWITCH_1, ISR_Edge);
}
/******************************************************************************/
void IRAM_ATTR ISR_AUX_FUNCTION(void * args)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        
    portENTER_CRITICAL_ISR(&SwitchMux);
        ISR_Trigged = true;
        ISRPinSource = (int) args;
    
        xSemaphoreGiveFromISR(xSwitchSemaphore, &xHigherPriorityTaskWoken);

        if(xHigherPriorityTaskWoken == pdTRUE)
        {
            if(xHigherPriorityTaskWoken == pdTRUE)
            {
                portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
            }
        }
    portEXIT_CRITICAL(&SwitchMux);

}
/******************************************************************************/
void vTaskSwitchHandle( void *pvParameters )
{
    SwitchISR_Configure();

    xSwitchSemaphore = xSemaphoreCreateBinary();

    if(xSwitchSemaphore == NULL)
    {
        Serial.printf("Erro ao criar semaforo.\n");
    }

    uint8_t pinState = 0;
    gpio_num_t ISR_SRC = GPIO_NUM_NC;
    static uint8_t contador = 0;

    for(;;)
    {
        if(xSemaphoreTake( xSwitchSemaphore, portMAX_DELAY) == pdTRUE)
        {
            if (ISR_Trigged == true)
            {

                ISR_SRC = (gpio_num_t) ISRPinSource;

                pinState = digitalRead(ISR_SRC);
                
                if(pinState == HIGH)
                {
                    /* Disable interrupt service routine */
                    // gpio_isr_handler_remove(ISR_SRC);
                    detachInterrupt(ISR_SRC);

                    while(digitalRead(ISR_SRC))
                    {
                        vTaskDelay(50 / portTICK_PERIOD_MS);
                    }

                    contador++;
                    switch (ISR_SRC)
                    {
                        case PIN_DIGITAL_SWITCH_0:
                            digitalWrite(PIN_DIGITAL_RELAY_1, !digitalRead(PIN_DIGITAL_RELAY_1));
                            break;
                        case PIN_DIGITAL_SWITCH_1:
                            digitalWrite(PIN_DIGITAL_RELAY_2, !digitalRead(PIN_DIGITAL_RELAY_2));
                            break;
                        default:
                            Serial.printf("Erro, fonte de interrupção:%i\n", ISR_SRC);
                        break;
                    }

                    attachInterruptArg(digitalPinToInterrupt(ISR_SRC), ISR_AUX_FUNCTION, (void*)ISR_SRC, ISR_Edge);

                }

            }
        }
    }
}
/******************************************************************************/