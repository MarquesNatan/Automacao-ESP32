/******************************************************************************/
#ifndef PIR_SENSOR_H
#define PIR_SENSOR_H
/******************************************************************************/
#define LDR_DEBOUNCE_TIME_US            500000UL
#define PIR_SENSOR_TIMEOUT_US           30000000UL
#define OUTPUT_ENABLED_PIR_SENSOR       PIN_DIGITAL_RELAY_4
/******************************************************************************/
void SetTimerLDRDebounce( void );
void ISR_DebounceLDR( void );

void ISR_PirSensor( void );
void PirSensorAttachInterrutps( void );
void ISR_TimeoutPirSensor( void );
void SetTimerTimeout( void );
void vTaskPirSensorHandle( void * pvParameters );
/******************************************************************************/
#endif /* PIR_SENSOR_H */
/******************************************************************************/