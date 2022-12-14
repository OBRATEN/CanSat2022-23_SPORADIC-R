#ifndef USB_DEBUGGER_H
#define USB_DEBUGGER_H

#include <stm32f4xx_hal.h>
#include "usb_device.h"

void DEBUG_startTest(uint8_t (*foo)(void), const char *name, uint32_t timeout);

void DEBUG_printString(const char *str);

void DEBUG_printBool(uint8_t seq);

void DEBUG_printUint8(uint8_t data);

void DEBUG_printUint16(uint16_t data);

void DEBUG_printInt16(int16_t data);

void DEBUG_printFloat(float data);

void DEBUG_printUint32(uint32_t data);

void DEBUG_logString(const char *str);
void DEBUG_logStringAndByte(const char *str, uint8_t val);

#endif // USB_DEBUGGER_H
