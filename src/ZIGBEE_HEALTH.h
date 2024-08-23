#ifndef ZIGBEE_HEALTH_H
#define ZIGBEE_HEALTH_H

#include <Arduino.h>
#include <mqtt_client.h>
#include <mqtt_supported_features.h>
#include <TIJD_GET.h>
#include <ZIGBEE_HELPERS.h>

// *************************************************************************
//                   system healtcheck 
//**************************************************************************

void healthCheck();
int checkCoordinator();
void ZigbeePing();
void ZBhardReset();
void consoleOut(String toLog);

#endif