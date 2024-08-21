#include <Arduino.h>

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

#include "OTA.H"
#include <Update.h>
#include <Hash.h>

#define VERSION  "ESP32-ECU_v0_8"

#include <TimeLib.h>
#include <time.h>
#include <sunMoon.h>

#include "soc/soc.h" // ha for brownout
#include "soc/rtc_cntl_reg.h" // ha for brownout
#include <esp_task_wdt.h> // ha
#include "soc/rtc_wdt.h"
           
#include "SPIFFS.h"
#include "FS.h"
#include <EEPROM.h>
#include <ArduinoJson.h>
//#include "AsyncJson.h"
#include <Arduino.h>

//#include <AsyncTCP.h>
#include "Async_TCP.h" //we include the customized one

#include <PubSubClient.h>

#include <NTPClient.h>
#include <WiFiUdp.h>

#include "HTML.H"
//#include "AAA_CONFIG_INVERTERS.H"
#include "AAA_MENUPAGE.H"
#include "PORTAL_HTML.H"
#include "DETAILSPAGE.H"

#include "AAA_DECODE.cpp"
#include "AAA_HOMEPAGE.cpp"
#include "AAA_INVERTERS.cpp"
#include "AAA_LOG.cpp"
#include "AA_CONSOLE.cpp"
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
#include "PORTAL_WIFI.cpp"
#include "Reboot__AP.cpp"
#include "SERIAL.cpp"
#include "SPIFFS_RW.cpp"
#include "Start_WiFi.cpp"
#include "test.cpp"
#include "TIJD_CALC.cpp"
#include "TIJD_GET.cpp"
#include "ZIGBEE_COORDINATOR.cpp"
#include "ZIGBEE_HEALTH.cpp"
#include "ZIGBEE_HELPERS.cpp"
#include "ZIGBEE_PAIR.cpp"
#include "ZIGBEE_POLLING.cpp"



// #include "eventSource.H"