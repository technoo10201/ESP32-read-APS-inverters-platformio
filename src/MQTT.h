#ifndef MQTT_H
#define MQTT_H

#include <Arduino.h>
#include <mqtt_client.h>
#include <PubSubClient.h>
#include <AAA_LOG.h>
#include <utils.h>

bool mqttConnect();
void MQTT_Receive_Callback(char *topic, byte *payload, unsigned int length);

#endif