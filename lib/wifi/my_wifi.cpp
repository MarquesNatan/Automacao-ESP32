/******************************************************************************/
#include "include/my_wifi.h"
#include "../../src/system/include/system_defs.h"

#include <WiFi.h>
/******************************************************************************/
char SSID[] = WIFI_SSID;
char PASS[] = WIFI_PASS;
/******************************************************************************/
WiFiClient espClient;
/******************************************************************************/
void Wifi_Init(void *params)
{
    if(WiFi.status() == WL_CONNECTED)
    {
        return;
    }

    #if WIFI_DEBUG == true
        Serial.printf("Conetando-se a rede: Meu teste ");
        Serial.printf(WIFI_SSID);
        Serial.printf("\nEspere a conexão completa");
    #endif /* WIFI_DEBUG */

    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while(WiFi.status() != WL_CONNECTED)
    {
        delay(100);
        #if WIFI_DEBUG == true
            Serial.print(".");
        #endif /* WIFI_DEBUG */
    }

    #if WIFI_DEBUG == true
        Serial.print("\n\nConectado com sucesso ");
        Serial.print(SSID);  
        Serial.print("  IP obtido: ");
        Serial.println(WiFi.localIP()); 
    #endif /* WIFI_DEBUG */
}
/******************************************************************************/