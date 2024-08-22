#ifndef MAIN_H
#define MAIN_H

//////////////////////////////////////////
// Arduino Libs
//////////////////////////////////////////
#include <Arduino.h>
#include <EEPROM.h>
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <AsyncEventSource.h>
#include <AsyncJson.h>
#include <AsyncWebSocket.h>
#include <AsyncWebSynchronization.h>
#include <SPIFFSEditor.h>
#include <StringArray.h>
#include <WebAuthentication.h>
#include <WebHandlerImpl.h>
#include <WebResponseImpl.h>
#include <WiFi.h>
#include <DNSServer.h> 
#include <Update.h>
#include <Hash.h>
#include "SPIFFS.h"
#include "FS.h"
#include "Async_TCP.h" // We include the customized one
#include <PubSubClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h>
#include <time.h>
#include <sunMoon.h>
//#include <AsyncTCP.h>

//////////////////////////////////////////
// Espressif Libs for ESP32
//////////////////////////////////////////
#include "soc/soc.h"          // ha for brownout
#include "soc/rtc_cntl_reg.h" // ha for brownout
#include <esp_task_wdt.h>     // ha
#include "soc/rtc_wdt.h"
           
//////////////////////////////////////////
// Project Libs
//////////////////////////////////////////
#include <debug.h>
#include <globals.h>


//#include "AAA_CONFIG_INVERTERS.H"
#include "AAA_MENUPAGE.H"
#include "AAA_DECODE.cpp"
#include "AAA_HOMEPAGE.cpp"
#include "AAA_INVERTERS.cpp"
#include "AAA_LOG.cpp"
#include "AA_CONSOLE.cpp"

#include "PORTAL_HTML.H"
#include "HTML.H"
#include "DETAILSPAGE.H"
#include "OTA.H"
#include "ABOUT.cpp"
#include "ASYSERVER.cpp"
#include "CONFIG_BASIS.cpp"
#include "CONFIG_GEO.cpp"
#include "CONFIG_IP.cpp"
#include "DETAILSPAGE.cpp"
#include "EXTERNAL.cpp"
#include "FORCE.cpp"
#include "INFOPAGE.cpp"
#include "ISR.cpp"
#include "legende.cpp"
#include "LOGPAGE.cpp"
#include "MQTT.cpp"
#include "MQTT_CONFIG.cpp"
#include "Start_WiFi.cpp"
#include "PORTAL_WIFI.cpp"
#include "Reboot__AP.cpp"
#include "SERIAL.cpp"
#include "SPIFFS_RW.cpp"

#include "TIJD_CALC.cpp"
#include "TIJD_GET.cpp"
#include "ZIGBEE_COORDINATOR.cpp"
#include "ZIGBEE_HEALTH.cpp"
#include "ZIGBEE_HELPERS.cpp"
#include "ZIGBEE_PAIR.cpp"
#include "ZIGBEE_POLLING.cpp"

#include "test.cpp"

// #include "eventSource.H"

#endif