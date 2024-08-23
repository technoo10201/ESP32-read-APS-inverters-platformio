#ifndef PORTAL_WIFI_H
#define PORTAL_WIFI_H

#include <Arduino.h>
#include "esp_task_wdt.h"
#include <ESPAsyncWebServer.h>
#include <PORTAL_HTML.H>
#include <SPIFFS_RW.h>
#include <GLOBALS.h>
#include <DNSServer.h>
#include <utils.h>


void start_portal();
void handlePortalRoot();
void handleForm();
void wifiConnect();
void handlePortalNotFound(AsyncWebServerRequest *request);
int connectWifi();
void handlePortalClose();
void eraseFiles();
void resetStatic();
void scanWifi();
String makeList(int aantal);

#endif