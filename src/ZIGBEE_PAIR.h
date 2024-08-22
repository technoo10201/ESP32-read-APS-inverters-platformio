#ifndef ZIGBEE_PAIR_H
#define ZIGBEE_PAIR_H

#include <Arduino.h>
#include <AsyncWebSocket.h>

#include <ZIGBEE_HEALTH.h>
#include <ZIGBEE_COORDINATOR.h>
#include <ZIGBEE_HELPERS.h>

#include <AAA_LOG.h>
#include <SPIFFS_RW.h>


void pairOnActionflag();
void handlePair(AsyncWebServerRequest *request);
bool pairing(int which);
bool decodePairMessage(int which);

#endif