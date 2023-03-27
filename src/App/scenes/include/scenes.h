/******************************************************************************/
#ifndef SCENES_H
#define SCENES_H
/******************************************************************************/
#include "scenes_defs_h"
/******************************************************************************/
void vTaskScenesHandle( void *pvParameters );
uint8_t BuscarCenas( uint8_t dia );
bool CopiarComando(uint8_t src[], uint8_t dest[], uint8_t offset, uint8_t length);
void ImprimeCena_Debug(cena_t cena);
/******************************************************************************/
#endif /* SCENES_H */
/******************************************************************************/