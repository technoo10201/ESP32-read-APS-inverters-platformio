#ifndef GLOBALS_H
#define GLOBALS_H

#define VERSION  "ESP32-ECU_v0_8"

// ZIGBEE_globals
#define CC2530_MAX_SERIAL_BUFFER_SIZE 512
#define YC600_MAX_NUMBER_OF_INVERTERS 9

// Logs
#define Log_MaxEvents 18 // variables To record and display last events on the home page

// UART2 (Zigbee module conected)
#define RXD2 16 // GPIO_NUM_16
#define TXD2 17 // GPIO_NUM_17

#define LED_AAN    HIGH  // sinc
#define LED_UIT    LOW
#define knop       0     // GPIO_NUM_0
#define led_onb    2     // onboard led was 2
#define ZB_RESET   5     // GPIO_NUM_5
//#define ZB_TX    17    // GPIO_NUM_17

#endif