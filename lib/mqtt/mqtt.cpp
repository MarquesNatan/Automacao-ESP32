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
            Serial.printf("\nFunção de callback definida.\n");
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
            Serial.printf("******************************************************");
            Serial.printf("\n* Conectando ao Broker MQTT: %s\n*", MQTT_PUBLIC_BROKER);
            Serial.printf("******************************************************");
        #endif /* MQTT_DEBUG */

        if (MQTT.connect(BOARD_ID))
        {   
            #if MQTT_DEBUG  == true
                Serial.println("\r\nConectado ao Broker com sucesso!\r\n");
            #endif /* MQTT_DEBUG */

            /*
             * 7db8cbb3-47f8-48a7-8c5a-d0aa81fad54b/tccautomacao/digital/
             */
            #if MQTT_DEBUG  == true
                Serial.println("\n");
                Serial.printf(PASTE3_SIMPLE(BOARD_ID, BOARD_BASE_TOPIC, BOARD_TOPIC_DIGITAL));
                Serial.println("\n");
                Serial.printf(PASTE3_SIMPLE(BOARD_ID, BOARD_BASE_TOPIC, BOARD_TOPIC_ANALOGIC));
                Serial.println("\n");
                Serial.printf(PASTE3_SIMPLE(BOARD_ID, BOARD_BASE_TOPIC, BOARD_TOPIC_DIGITAL));
                Serial.println("\n");
                Serial.printf(PASTE3_SIMPLE(BOARD_ID, BOARD_BASE_TOPIC, BOAD_TOPIC_TASK));
                Serial.println("\n");
            #endif /* MQTT_DEBUG */

            MQTT.subscribe(PASTE3_SIMPLE(BOARD_ID, BOARD_BASE_TOPIC, BOARD_TOPIC_DIGITAL));
            MQTT.subscribe(PASTE3_SIMPLE(BOARD_ID, BOARD_BASE_TOPIC, BOARD_TOPIC_ANALOGIC));
            MQTT.subscribe(PASTE3_SIMPLE(BOARD_ID, BOARD_BASE_TOPIC, BOARD_TOPIC_SENSOR));
            MQTT.subscribe(PASTE3_SIMPLE(BOARD_ID, BOARD_BASE_TOPIC, BOAD_TOPIC_TASK));

        }
        else
        {
            while(temp != 0)
            {   
                #if MQTT_DEBUG  == true
                    Serial.printf("Nova tentativa de conexao em: %i\n", temp);
                #endif /* MQTT_DEBUG */

                temp = temp - 1;
                delay(1000);
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

    if(data != NULL)
    {
        command = Command_Parse(data);
    }

    #if COMMAND_DEBUG == true
        Serial.println();
        Serial.printf("*******************************\n");
        Serial.printf("* COMANDO:  %i                *\n", (char)command.data[0]);
        Serial.printf("* PINO:     %i                *\n", (char)command.data[1]);
        Serial.printf("* VALOR:    %i                *\n", (char)command.data[2]);
        Serial.printf("*******************************\n\n");
    #endif /* MQTT_DEBUG */
}
/******************************************************************************/
bool MQTT_Publish(const char *message, const char *topic)
{
    bool result = false;
    result  = MQTT.publish(topic, message);

    if(!result)
    {
        #if MQTT_DEBUG == true
            Serial.printf("Erro, não foi possível publicar a sua mensagem");
        #endif /* MQTT_DEBUG */
    }

    return result;
}
/******************************************************************************/