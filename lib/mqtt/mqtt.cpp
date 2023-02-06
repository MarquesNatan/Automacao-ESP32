/******************************************************************************/
#include "include/mqtt.h"
#include "../../src/system/include/system_defs.h"
#include "../../src/common/include/system_common.h"
#include "../../src/common/include/board.h"

#include "../commands/include/command.h"

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
            #if MQTT_DEBUG  == true
                Serial.println("Conectado ao Broker com sucesso!");
            #endif /* MQTT_DEBUG */

            /*
             * 7db8cbb3-47f8-48a7-8c5a-d0aa81fad54b/tccautomacao/digital/
             */
            #if MQTT_DEBUG  == true
                Serial.printf(PASTE3_SIMPLE(BOARD_ID, BOARD_BASE_TOPIC, BOARD_TOPIC_DIGITAL));
                Serial.println();
                Serial.printf(PASTE3_SIMPLE(BOARD_ID, BOARD_BASE_TOPIC, BOARD_TOPIC_ANALOGIC));
                Serial.println();
                Serial.printf(PASTE3_SIMPLE(BOARD_ID, BOARD_BASE_TOPIC, BOARD_TOPIC_DIGITAL));
                Serial.println();
            #endif /* MQTT_DEBUG */

            MQTT.subscribe(PASTE3_SIMPLE(BOARD_ID, BOARD_BASE_TOPIC, BOARD_TOPIC_DIGITAL));
            MQTT.subscribe(PASTE3_SIMPLE(BOARD_ID, BOARD_BASE_TOPIC, BOARD_TOPIC_ANALOGIC));
            MQTT.subscribe(PASTE3_SIMPLE(BOARD_ID, BOARD_BASE_TOPIC, BOARD_TOPIC_DIGITAL));

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
void MQTT_DataReceiver(char *topic, uint8_t *data, unsigned int length)
{

    command_t command; 
    command = Command_Parse(data);

    #if MQTT_DEBUG == true
        Serial.printf("\n");
        Serial.printf("Command Type:  %c\n", (char)command.command[0]);
        Serial.printf("Command index: %c\n", (char)command.command[1]);
        Serial.printf("Command value: %c\n", (char)command.command[2]);
    #endif /* MQTT_DEBUG */

}
/******************************************************************************/