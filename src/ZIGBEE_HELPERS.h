#ifndef ZIGBEE_HELPERS_H
#define ZIGBEE_HELPERS_H

#include <Arduino.h>
#include <GLOBALS.h>
#include <utils.h>
#include <AAA_DECODE.h>


char * readZB(char inMess[]);
String checkSumString(char Command[]);
void sendZB(char printString[]);
char *sLen(char Command[]);
int StrToHex(char str[]);
String ECU_REVERSE();
void inverterReboot(int which);
void resetValues(bool energy, bool mustSend);
bool waitSerial2Available();
char *split(char *str, const char *delim);

#endif