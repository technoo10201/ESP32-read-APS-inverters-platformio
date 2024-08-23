#ifndef ASYSERVER_H
#define ASYSERVER_H

#include <Arduino.h>
#include <AAA_MENUPAGE.H>
#include <DETAILSPAGE.H>
#include <OTA.H>
#include <ESPAsyncWebServer.h>
#include <HTML.H>
#include <EXTERNAL.h>
#include <AAA_HOMEPAGE.h>
#include <AAA_INVERTERS.h>
#include <AA_CONSOLE.h>
#include <Start_WiFi.h>
#include <CONFIG_BASIS.h>
#include <CONFIG_IP.h>
#include <MQTT_CONFIG.h>
#include <CONFIG_GEO.h>
#include <ABOUT.h>
#include <sunMoon.h>

void start_server();
void confirm();

#endif