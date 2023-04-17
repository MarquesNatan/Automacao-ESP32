/******************************************************************************/
#include "include/mqtt.h"
#include "../../src/system/include/system_defs.h"
#include "../../src/common/include/system_common.h"
#include "../../src/common/include/board.h"

#include "../../src/App/command/include/command.h"

#include <PubSubClient.h>
#include <WiFi.h>
/******************************************************************************/
extern QueueHandle_t xQueueCommandReceived;
/******************************************************************************/
extern WiFiClient espClient;
PubSubClient MQTT(espClient);
static void(*mqtt_callback_func)(char *topic, uint8_t *data, unsigned int length);
/******************************************************************************/
void vTaskMqttHandleConnection(void *pvParameters)
{
    while (true)
    {
        #if MQTT_ENABLE == true
        while (true)
        {
            if (!MQTT.connected())
            {
                MQTT_tryConnect();
            }

            MQTT.loop();
        }
        #endif /* MQTT_ENABLE */
    }
}
/******************************************************************************/
void Mqtt_Start(void *params)
{
    MQTT_setCallback(MQTT_DataReceiver);
    
    MQTT.setServer(MQTT_PUBLIC_BROKER, MQTT_PORT);

    if(mqtt_callback_func != NULL)
    {
        MQTT.setCallback(MQTT_DataReceiver);
        #if MQTT_DEBUG  == true
            Serial.printf("\nFunção de callback definida.\n");
        #endif /* MQTT_DEBUG */
    }
    #if MQTT_DEBUG  == true
        else 
        {
            Serial.printf("Erro, função de callback não definida");
            while (true);
        }
    #endif /* MQTT_DEBUG */
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
                Serial.println("\nConectado ao Broker com sucesso!\n");
            #endif /* MQTT_DEBUG */

            #if MQTT_DEBUG  == true
                Serial.println("\n");
                Serial.printf(PASTE3_SIMPLE(BOARD_ID, BOARD_BASE_TOPIC, BOARD_TOPIC_DIGITAL));
                Serial.println("\n");
                Serial.printf(PASTE3_SIMPLE(BOARD_ID, BOARD_BASE_TOPIC, BOARD_TIPOC_SCENES));
                Serial.println("\n");
            #endif  /* MQTT_DEBUG */

            // MQTT.subscribe(PASTE3_SIMPLE(BOARD_ID, BOARD_BASE_TOPIC, BOARD_TOPIC_DIGITAL));
            MQTT.subscribe("tccautomacao/digital/");
            MQTT.subscribe("tccautomacao/sensor/");
            MQTT.subscribe(PASTE3_SIMPLE(BOARD_ID, BOARD_BASE_TOPIC, BOARD_TIPOC_SCENES));

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
    newcommand_t command; 
    uint8_t i = 0;

    /* Tópico recebido é para execução de comandos */
    if(strcasestr(topic, "digital") || strcasestr(topic, "analogico"))
    {
            for(int i = 0; i < length; i++)
            {
                command.data[i] = data[i];
                #if MQTT_DEBUG  == true
                    Serial.printf("%c", command.data[i]);
                #endif /* MQTT_DEBUG */
            }

            #if MQTT_DEBUG  == true
                Serial.printf("\n");
            #endif /* MQTT_DEBUG */

        xQueueSendToBack(xQueueCommandReceived, &command, portMAX_DELAY);
    }
    else 
    {
        /* Trata os comandos para o sensor */

    }
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