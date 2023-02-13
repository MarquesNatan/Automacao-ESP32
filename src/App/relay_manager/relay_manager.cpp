/******************************************************************************/
#include <Arduino.h>
#include "include/relay_manager.h"
#include "../../App/peripheral/include/peripheral_controller.h"
/******************************************************************************/
void RelayManager(uint8_t numRelay, uint8_t newState)
{
    if (Relay_IsValidIndex(numRelay))
    {
        Relay_StateUpdate(numRelay, newState);
        #if DEBUG == true
            Serial.printf("index: %i  action: %i", index, numRelay);
        #endif /* DEBUG */
    }
    #if DEBUG == true
        else 
        {
            Serial.printf("Relay out of scope. Relay number: %i", num);
        }
    #endif /* DEBUG */
}
/******************************************************************************/
inline bool Relay_IsValidIndex(uint8_t numRelay)
{
    bool isValid = false;

    if(numRelay >= 0 && numRelay <= 20)
    {
        isValid = true;
    }

    return isValid;
}
/******************************************************************************/
inline void Relay_StateUpdate(uint8_t num, uint8_t newState)
{
    Peripheral_DigitalWrite(num, newState);
}
/******************************************************************************/
