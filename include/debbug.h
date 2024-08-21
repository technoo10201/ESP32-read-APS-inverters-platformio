/*
 * if we define TEST we have a special environment to test the decoding of a string
 * for the different invertertypes. We only test inv0. If we set this up as a DS3
 * a ds3 string will be analyzed.
 * - no automatic polling
 * - no automatic healthcheck
 * - no start of zigbee at boot\
 * - in the console an option to test inv 0 '10;TESTINV'
 * - more debug information in the console
 */

//#define TEST

//#define DEBUG
//#ifdef  DEBUG
//#define DebugPrint(x)     Serial.print (x)
//#define DebugPrintDec(x)  Serial.print (x, DEC)
//#define DebugPrintln(x)   Serial.println (x)
//#else
//#define DebugPrint(x)
//#define DebugPrintDec(x)
//#define DebugPrintln(x)
//#endif