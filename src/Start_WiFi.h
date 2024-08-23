#ifndef START_WIFI_H
#define START_WIFI_H

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncWebSocket.h>
#include <utils.h>
#include <ASYSERVER.h>
#include <PORTAL_WIFI.h>

void start_wifi();
void checkFixed();
void loginBoth(AsyncWebServerRequest *request, String who);

#endif