/******************************************************************************/
#ifndef MQTT_H
#define MQTT_H
/******************************************************************************/
#include "Arduino.h"
/******************************************************************************/
void vTaskMqttHandleConnection(void *pvParameters);
void Mqtt_Start(void *params);
void MQTT_setCallback(void (*callback)(char *topic, uint8_t *data, unsigned int length));
void MQTT_DataReceiver(char *topic, uint8_t *data, unsigned int length);
bool MQTT_Publish(const char *message, const char *topic);
void MQTT_tryConnect(void);
/******************************************************************************/
#endif /* MQTT_H */
/******************************************************************************/