#ifndef ISR_H
#define ISR_H

#include <Arduino.h>
#include <GLOBALS.h>


IRAM_ATTR void isr();
void buttonPressed();

#endif