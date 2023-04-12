/******************************************************************************/
#ifndef DIMMER_H
#define DIMMER_H
/******************************************************************************/
#include <stdint.h>
/******************************************************************************/
#define DIMMER_ENABLE_PULSE_US          500
/******************************************************************************/
void DimmerConfig( void );
void ISR_Dimmer( void *args );

void ISR_TurnPinLow( void );
void SetTimerPinLow( void );

void ISR_TurnPinHigh( void );
void SetTimerPinHigh( int16_t brightness );

void vTaskDimmer( void *pvParameters);
/******************************************************************************/
#endif /* DIMMER_H */
/******************************************************************************/