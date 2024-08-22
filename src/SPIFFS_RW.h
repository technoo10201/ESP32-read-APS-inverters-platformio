#ifndef SPIFFS_RW_H
#define SPIFFS_RW_H

#include <Arduino.h>
#include <SPIFFS.h>
#include "FS.h"
#include <ArduinoJson.h>


void SPIFFS_read();
void writeStruct(String whichfile, int nummer);
bool leesStruct(String whichfile);
void wifiConfigsave();
void basisConfigsave();
void mqttConfigsave();
bool file_open_for_read(String bestand);
void printStruct( String bestand );

#endif