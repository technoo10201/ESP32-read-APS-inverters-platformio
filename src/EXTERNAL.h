#ifndef EXTERNAL_H
#define EXTERNAL_H

#include <Arduino.h>
#include <ESPAsyncWebServer.h>

bool checkRemote(String url);
void handleNotFound(AsyncWebServerRequest *request);

#endif