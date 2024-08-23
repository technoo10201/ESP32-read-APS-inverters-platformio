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
#include <SPIFFS.h>
#include <FS.h>
#include <Async_TCP.h> // We include the customized one
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
#include <soc/soc.h>          // ha for brownout
#include <soc/rtc_cntl_reg.h> // ha for brownout
#include <esp_task_wdt.h>     // ha
#include <soc/rtc_wdt.h>
           
//////////////////////////////////////////
// Project Libs
//////////////////////////////////////////
#include <DEBUG.h>
#include <GLOBALS.h>


//#include <AAA_CONFIG_INVERTERS.H>
#include <AAA_MENUPAGE.H>
#include <AAA_DECODE.h>
#include <AAA_HOMEPAGE.h>
#include <AAA_INVERTERS.h>
#include <AAA_LOG.h>
#include <AA_CONSOLE.h>

#include <PORTAL_HTML.H>
#include <HTML.H>
#include <DETAILSPAGE.H>
#include <OTA.H>
#include <ABOUT.h>
#include <ASYSERVER.h>
#include <CONFIG_BASIS.h>
#include <CONFIG_GEO.h>
#include <CONFIG_IP.h>
#include <DETAILSPAGE.h>
#include <EXTERNAL.h>
#include <FORCE.h>
#include <ISR.h>
#include <MQTT.h>
#include <MQTT_CONFIG.h>
#include <Start_WiFi.h>
#include <PORTAL_WIFI.h>
#include <Reboot_AP.h>
#include <SERIAL.h>
#include <SPIFFS_RW.h>

#include <TIJD_CALC.h>
#include <TIJD_GET.h>
#include <ZIGBEE_COORDINATOR.h>
#include <ZIGBEE_HEALTH.h>
#include <ZIGBEE_HELPERS.h>
#include <ZIGBEE_PAIR.h>
#include <ZIGBEE_POLLING.h>

#include <test.h>

#include <utils.h>

// #include <eventSource.H>

#endif