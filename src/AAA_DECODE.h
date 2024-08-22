#ifndef AAA_DECODE_H
#define AAA_DECODE_H

#include <Arduino.h>
#include <GLOBALS.h>
#include <ZIGBEE_HELPERS.h>

int decodePollAnswer(int which);
float extractValue(uint8_t startPosition, uint8_t valueLength, float valueSlope, float valueOffset, char toDecode[CC2530_MAX_SERIAL_BUFFER_SIZE]);
void mqttPoll(int which);

#endif