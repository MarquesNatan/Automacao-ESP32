/******************************************************************************/
#ifndef SWITCH_H
#define SWITCH_H
/******************************************************************************/
#define GET_SWITCH_MODE(a)       Peripheral_DigitalRead(a);
/******************************************************************************/
typedef enum 
{
    INTERRUPTOR = 0x00,
    PULSADOR = 0x01
}SWITCH_MODE;
/******************************************************************************/
void vTaskSwitchHandle( void *pvParameters );
void ISR_AUX_FUNCTION(void * args);
void SwitchISR_Configure( void );
void ISR_Switch( void *args );
/******************************************************************************/
#endif /* SWITCH_H */
/******************************************************************************/
