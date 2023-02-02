/******************************************************************************/
#include "include/mqtt.h"
#include "../../src/system/include/system_defs.h"

#include <PubSubClient.h>
#include <WiFi.h>
/******************************************************************************/
extern WiFiClient espClient;
PubSubClient MQTT(espClient);
/******************************************************************************/
static void(*mqtt_callback_func)(char *topic, uint8_t *data, unsigned int length);
/******************************************************************************/
void Mqtt_Start(void *params)
{
    MQTT_setCallback(MQTT_DataReceiver);
    MQTT.setServer(MQTT_PUBLIC_BROKER, MQTT_PORT);

    if(mqtt_callback_func != NULL)
    {
        MQTT.setCallback(mqtt_callback_func);
        #if MQTT_DEBUG  == true
            Serial.printf("Função de callback definida.");
        #endif /* MQTT_DEBUG */
    }
    else 
    {
        #if MQTT_DEBUG  == true
            Serial.printf("Erro, função de callback não definida");
        #endif /* MQTT_DEBUG */
        while (true);
    }
}
/******************************************************************************/
void MQTT_tryConnect(void)
{
    uint8_t temp = 11;

    while (!MQTT.connected())
    {
        #if MQTT_DEBUG  == true
            Serial.print("Conectando ao Broker MQTT: ");
            Serial.println(MQTT_PUBLIC_BROKER);
        #endif /* MQTT_DEBUG */

        if (MQTT.connect(BOARD_ID))
        {
            Serial.println("Conectado ao Broker com sucesso!");
            MQTT.subscribe("automacao/digital");
        }
        else
        {
            #if MQTT_DEBUG  == true
                Serial.println("Erro durante a conexão.");
            #endif /* MQTT_DEBUG */
            while(temp != 0)
            {
                delay(1000);
                
                #if MQTT_DEBUG  == true
                    Serial.printf("Nova tentatica de conexao em: ", temp);
                #endif /* MQTT_DEBUG */

                temp = temp - 1;
            }
        }
    }
}
/******************************************************************************/
void MQTT_setCallback(void (*callback)(char *topic, uint8_t *data, unsigned int length))
{
    mqtt_callback_func = callback;
}
/******************************************************************************/
//@@HACK
#define LED     2
void MQTT_DataReceiver(char *topic, uint8_t *data, unsigned int length)
{
    //@@HACK
    char msg;
    Serial.printf("Comando recebido: %c", (char)data[0]);

    for(int i = 0; i < length; i++)
    {
        msg = (char)data[0];
    }

    if(msg == '0')
    {
        digitalWrite(LED, LOW);
    }
    if(msg == '1')
    {
        digitalWrite(LED, HIGH);
    }
}
/******************************************************************************/