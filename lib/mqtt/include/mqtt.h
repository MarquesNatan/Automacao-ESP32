/******************************************************************************/
#ifndef MQTT_H
#define MQTT_H
/******************************************************************************/
#include "Arduino.h"
/******************************************************************************/
void Mqtt_Init(const char *broker_address, const char *broker_port);
void Mqtt_OnConnected(void *params);

void Mqtt_MesagePublish(char *message);
char * Mqtt_MesageReceive(void *params);

bool Mqtt_TopicSubscribe(char *topic);
bool Mqtt_TopicUnscribe(char *topic);
/******************************************************************************/
#endif /* MQTT_H */
/******************************************************************************/