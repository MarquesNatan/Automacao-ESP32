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
        Serial.printf("* Conetando-se a rede: %s      *\n", WIFI_SSID);
    #endif /* WIFI_DEBUG */

    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while(WiFi.status() != WL_CONNECTED)
    {
        vTaskDelay(pdTICKS_TO_MS(100));
        #if WIFI_DEBUG == true
            Serial.print(".");
        #endif /* WIFI_DEBUG */
    }

    #if WIFI_DEBUG == true
        Serial.printf("Conectado com sucesso, IP obtido: \n", WiFi.localIP());
    #endif /* WIFI_DEBUG */
}
/******************************************************************************/