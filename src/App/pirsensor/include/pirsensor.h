/******************************************************************************/
#ifndef PIR_SENSOR_H
#define PIR_SENSOR_H
/******************************************************************************/
#include <stdint.h>
#include "../../../system/include/system_defs.h"
#include "RTClib.h"
/******************************************************************************/
void PirSensor_EnableInterrupts( void );
void ISR_PIRSensor( void );
bool xQueuePirSensorCreate( void );
void vTaskPIRSensorHandle( void *pvParameters );
/******************************************************************************/
#endif /* PIR_SENSOR_H */
/******************************************************************************/