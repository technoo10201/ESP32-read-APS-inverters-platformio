#ifndef START_WIFI_H
#define START_WIFI_H

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncWebSocket.h>

void start_wifi();
void checkFixed();
void loginBoth(AsyncWebServerRequest *request, String who);

#endif