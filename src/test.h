#ifndef TEST_H
#define TEST_H

#include <Arduino.h>
#include <ZIGBEE_HELPERS.h>

void testMessage(bool console);
#ifdef TEST
void testDecode();
#endif

#endif