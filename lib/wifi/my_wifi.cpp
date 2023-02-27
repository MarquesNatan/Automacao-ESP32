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
        Serial.printf("*********************************************");
        Serial.printf("\n* Conetando-se a rede: %s      *", WIFI_SSID);
        Serial.printf("\n*********************************************\n");
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
        Serial.printf("\n*********************************************");
        Serial.printf("\nConectado com sucesso, IP obtido: ");
        Serial.print(WiFi.localIP());
        Serial.printf("\n*********************************************");
    #endif /* WIFI_DEBUG */
}
/******************************************************************************/