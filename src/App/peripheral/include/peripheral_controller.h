/******************************************************************************/
#ifndef PERIPHERAL_CONTROLLER_H
#define PERIPHERAL_CONTROLLER_H
/******************************************************************************/
void Peripheral_Init(void *params);
void Peripheral_DigitalConfigure(uint8_t pin, uint8_t type);
void Peripheral_DigitalWrite(uint8_t num, uint8_t state);
uint8_t Peripheral_DigitalRead(uint8_t pin);
/******************************************************************************/
#endif /* PERIPHERAL_CONTROLLER_H */
/******************************************************************************/