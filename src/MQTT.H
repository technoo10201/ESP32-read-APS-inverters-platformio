#ifndef MQTT_H
#define MQTT_H

#include <Arduino.h>
#include <mqtt_client.h>

bool mqttConnect();
void MQTT_Receive_Callback(char *topic, byte *payload, unsigned int length);

#endif