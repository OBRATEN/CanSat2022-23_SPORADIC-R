#include "usb_debugger.h"

void DEBUG_startTest(uint8_t (*foo)(void), const char *name, uint32_t timeout) {
	uint32_t startTime = HAL_GetTick();
	uint8_t testStatus = foo();
	uint32_t endTime = HAL_GetTick();
	DEBUG_printString("Test ");
	DEBUG_printString(name);
	DEBUG_printString(" completed with status: ");
	if (testStatus) DEBUG_printString("OK. Return code: ");
	else DEBUG_printString("ERROR. Return code: ");
	DEBUG_printUint8(testStatus);
	DEBUG_printString(". Time spent: ");
	DEBUG_printUint32(endTime - startTime);
	DEBUG_printString(";\n\r");
}

void DEBUG_printUint8(uint8_t num) {
	char str[3];
	for (uint8_t i = 0; i < 3; ++i)
		str[i] = '\0';
	uint8_t index = 0;
	int32_t z = 0;
	uint8_t q = 0;
	do {
		z = z * 10 + num % 10;
		num /= 10;
		q++;
	} while (num);
	char s;
	while (q) {
		s = '0' + z % 10;
		z /= 10;
		str[index++] = s;
		q--;
	}
	while (CDC_Transmit_FS((uint8_t*) str, 3) == USBD_BUSY) {
	}
}

void DEBUG_printUint16(uint16_t num) {
	char str[5];
	for (uint8_t i = 0; i < 5; ++i)
		str[i] = '\0';
	uint8_t index = 0;
	int32_t z = 0;
	uint8_t q = 0;
	do {
		z = z * 10 + num % 10;
		num /= 10;
		q++;
	} while (num);
	char s;
	while (q) {
		s = '0' + z % 10;
		z /= 10;
		str[index++] = s;
		q--;
	}
	while (CDC_Transmit_FS((uint8_t*) str, 5) == USBD_BUSY) {
	}
}

void DEBUG_printInt16(int16_t data) {
	char str[6];
	sprintf(str, "%hi", data);
	while (CDC_Transmit_FS((uint8_t*) str, strlen(str)) == USBD_BUSY) {
	}
}

void DEBUG_printFloat(float data) {
	char buf[32];
	sprintf(buf, "%f", data);
	while (CDC_Transmit_FS((uint8_t*) buf, strlen(buf)) == USBD_BUSY) {
	}
}

void DEBUG_printUint32(uint32_t data) {
	char buff[16];
	sprintf(buff, "%lu", data);
	while (CDC_Transmit_FS((uint8_t*) buff, strlen(buff)) == USBD_BUSY) {
	}
}

void DEBUG_printString(const char *data) {
	while (CDC_Transmit_FS((uint8_t*) data, strlen(data)) == USBD_BUSY) {
	}
}

void DEBUG_logString(const char *str) {
	DEBUG_printUint32(HAL_GetTick());
	DEBUG_printString("| ");
	DEBUG_printString(str);
	DEBUG_printString(";\n\r");
}

void DEBUG_logStringAndByte(const char *str, uint8_t val) {
	DEBUG_printUint32(HAL_GetTick());
	DEBUG_printString(":");
	DEBUG_printString(str);
	DEBUG_printUint8(val);
	DEBUG_printString("\n\r");
}
