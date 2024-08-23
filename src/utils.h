#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>
#include <EEPROM.h>
#include <GLOBALS.h>
#include <WiFi.h>
#include <ISR.h>
#include <TIJD_GET.h>
#include <ZIGBEE_PAIR.h>
#include <ZIGBEE_HEALTH.h>
#include <ZIGBEE_COORDINATOR.h>
#include <ZIGBEE_HELPERS.h>
#include <ZIGBEE_POLLING.h>
#include <test.h>

void ledblink(int i, int wacht);
void empty_serial();
void empty_serial2();
void write_eeprom();
void read_eeprom();
void test_actionFlag();
void poll_all(); 
String getChipId(bool sec);
void showDir();
void eventSend(byte what);



#endif