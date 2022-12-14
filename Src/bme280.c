#include "bme280.h"

/* bme280.сс
 * Author: Arthur Garagulya
 * Description:
 * Библиотека взаимодействия с барометром
 * BME280 по интерфейсу SPI
 * для микроконтроллеров STM32F4xx
 */

uint16_t _dig_T1;
int16_t _dig_T2;
int16_t _dig_T3;
uint16_t _dig_P1;
int16_t _dig_P2;
int16_t _dig_P3;
int16_t _dig_P4;
int16_t _dig_P5;
int16_t _dig_P6;
int16_t _dig_P7;
int16_t _dig_P8;
int16_t _dig_P9;
uint8_t _dig_H1;
int16_t _dig_H2;
uint8_t _dig_H3;
int16_t _dig_H4;
int16_t _dig_H5;
int8_t _dig_H6;
int32_t _t_fine;

int32_t _adc_p;
int32_t _adc_t;
int32_t _adc_h;

SPI_HandleTypeDef *bmespi2;

uint8_t BME280_init(SPI_HandleTypeDef *spi) {
#if defined(USE_USB_DEBUG) && defined(USE_DIAGNOSTICS_MODE)
	DEBUG_logString("Starting BME280 initialization protocol...");
#endif
	bmespi2 = spi;
	uint8_t chipVal = BME280_readUint8(CHIP_ID);
	if (chipVal != 0x60) {
#if defined(USE_USB_DEBUG) && defined(USE_DIAGNOSTICS_MODE)
		DEBUG_logStringAndByte("No BME280 found at SPI2 bus, got CHIPID value: ",
				chipVal);
#endif
		return 0;
	}
#if defined(USE_USB_DEBUG) && defined(USE_DIAGNOSTICS_MODE)
	DEBUG_logString("LIS3MDL found at SPI2 bus");
#endif
	BME280_writeUint8(RESET_REG, 0xB6);
	HAL_Delay(10);
	BME280_clearCalibrations();
	BME280_readCalibrations();
	BME280_writeUint8(CTRL_MEAS, 0x00);
	BME280_writeUint8(CTRL_HUM, 0xA0);
	BME280_writeUint8(CONFIG_REG, 0x00);
	BME280_writeUint8(CTRL_MEAS, 0xB7);
#if defined(USE_USB_DEBUG) && defined(USE_DIAGNOSTICS_MODE)
	DEBUG_logString("LIS3MDL sensor has been initialized");
#endif
	return 1;
}

void BME280_update(void) {
	uint8_t data[8];
	uint8_t readAddr = PRESS_REG | 0x80;
	SR_select(0, BME280_VAL);
	HAL_SPI_Transmit(bmespi2, (uint8_t*) &readAddr, 1, 100);
	HAL_SPI_Receive(bmespi2, (uint8_t*) data, 8, 100);
	SR_deselect(0, BME280_VAL);
	_adc_p = BME280_assembleRaw(&data[0], 1);
	_adc_t = BME280_assembleRaw(&data[3], 1);
	_adc_h = BME280_assembleRaw(&data[6], 0);
}

float BME280_calcAlt(float press) {
	return 44330 * (1.0 - pow((press / 100) / 1013.25, 0.1903));
}

void BME280_readTemp(float *temp) {
	double v_x1_u32;
	double v_x2_u32;
	v_x1_u32 = (((double) _adc_t) / 16384.0 - ((double) _dig_T1) / 1024.0)
			* ((double) _dig_T2);
	v_x2_u32 = ((((double) _adc_t) / 131072.0 - ((double) _dig_T1) / 8192.0)
			* (((double) _adc_t) / 131072.0 - ((double) _dig_T1) / 8192.0))
			* ((double) _dig_T3);
	_t_fine = (int32_t)(v_x1_u32 + v_x2_u32);
	*temp = (v_x1_u32 + v_x2_u32) / 5120.0;
}

void BME280_readPress(float *press) {
	double v_x1_u32;
	double v_x2_u32;
	double pressure;
	v_x1_u32 = ((double) _t_fine / 2.0) - 64000.0;
	v_x2_u32 = v_x1_u32 * v_x1_u32 * ((double) _dig_P6) / 32768.0;
	v_x2_u32 = v_x2_u32 + v_x1_u32 * ((double) _dig_P5) * 2.0;
	v_x2_u32 = (v_x2_u32 / 4.0) + (((double) _dig_P4) * 65536.0);
	v_x1_u32 = (((double) _dig_P3) * v_x1_u32 * v_x1_u32 / 524288.0
			+ ((double) _dig_P2) * v_x1_u32) / 524288.0;
	v_x1_u32 = (1.0 + v_x1_u32 / 32768.0) * ((double) _dig_P1);
	pressure = 1048576.0 - (double) _adc_p;
	// Avoid exception caused by division by zero.
	if (v_x1_u32 != 0) pressure = (pressure - (v_x2_u32 / 4096.0)) * 6250.0
			/ v_x1_u32;
	else return;
	v_x1_u32 = ((double) _dig_P9) * pressure * pressure / 2147483648.0;
	v_x2_u32 = pressure * ((double) _dig_P8) / 32768.0;
	pressure = pressure + (v_x1_u32 + v_x2_u32 + ((double) _dig_P7)) / 16.0;
	*press = pressure;
}

void BME280_readHum(float *hum) {
	int32_t v_x1_u32r;
	v_x1_u32r = (_t_fine - ((int32_t) 76800));
	v_x1_u32r = (((((_adc_h << 14) - (((int32_t) _dig_H4) << 20)
			- (((int32_t) _dig_H5) * v_x1_u32r)) + ((int32_t) 16384)) >> 15)
			* (((((((v_x1_u32r * ((int32_t) _dig_H6)) >> 10)
					* (((v_x1_u32r * ((int32_t) _dig_H3)) >> 11) + ((int32_t) 32768)))
					>> 10) + ((int32_t) 2097152)) * ((int32_t) _dig_H2) + 8192) >> 14));
	v_x1_u32r = (v_x1_u32r
			- (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) * ((int32_t) _dig_H1))
					>> 4));
	v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
	v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);
	*hum = (float)((uint32_t)(v_x1_u32r >> 12)) / 2048;
}

int32_t BME280_assembleRaw(uint8_t *data, uint8_t xlsb) {
	uint32_t value = data[0];
	value <<= 8;
	value |= data[1];
	if (xlsb != 0) {
		value <<= 4;
		value |= (data[2] >> 4);
	}
	return (int32_t) value;
}

void BME280_readCalibrations(void) {
	// Адреса регистров тут константны, так что не вижу смысла в макросах для них
	// P.S. Их можно даже простейшим образом посчитать
	_dig_T1 = BME280_readUint16_LE(0x88);
	_dig_T2 = BME280_readInt16_LE(0x8A);
	_dig_T3 = BME280_readInt16_LE(0x8C);

	_dig_P1 = BME280_readUint16_LE(0x8E);
	_dig_P2 = BME280_readInt16_LE(0x90);
	_dig_P3 = BME280_readInt16_LE(0x92);
	_dig_P4 = BME280_readInt16_LE(0x94);
	_dig_P5 = BME280_readInt16_LE(0x96);
	_dig_P6 = BME280_readInt16_LE(0x98);
	_dig_P7 = BME280_readInt16_LE(0x9A);
	_dig_P8 = BME280_readInt16_LE(0x9C);
	_dig_P9 = BME280_readInt16_LE(0x9E);

	_dig_H1 = BME280_readUint8(0xA1);
	_dig_H2 = BME280_readInt16_LE(0xE1);
	_dig_H3 = BME280_readUint8(0xE3);
	_dig_H4 = ((int8_t) BME280_readUint8(0xE4) << 4)
			| (BME280_readUint8(0xE4 + 1) & 0xF);
	_dig_H5 = ((int8_t) BME280_readUint8(0xE5 + 1) << 4)
			| (BME280_readUint8(0xE5) >> 4);
	_dig_H6 = (int8_t) BME280_readUint8(0xE7);
}

void BME280_clearCalibrations(void) {
	_dig_T1 = 0;
	_dig_T2 = 0;
	_dig_T3 = 0;
	_dig_P1 = 0;
	_dig_P2 = 0;
	_dig_P3 = 0;
	_dig_P4 = 0;
	_dig_P5 = 0;
	_dig_P6 = 0;
	_dig_P7 = 0;
	_dig_P8 = 0;
	_dig_P9 = 0;
	_dig_H1 = 0;
	_dig_H2 = 0;
	_dig_H3 = 0;
	_dig_H4 = 0;
	_dig_H5 = 0;
	_dig_H6 = 0;
}

void BME280_writeUint8(uint8_t reg, uint8_t data) {
	uint8_t writeReg = reg & ~0x80;
	SR_select(0, BME280_VAL);
	HAL_SPI_Transmit(bmespi2, (uint8_t*) &writeReg, 1, 100);
	HAL_SPI_Transmit(bmespi2, (uint8_t*) &data, 1, 100);
	SR_deselect(0, BME280_VAL);
}

uint8_t BME280_readUint8(uint8_t reg) {
	uint8_t readReg = reg | 0x80;
	uint8_t res;
	SR_select(0, BME280_VAL);
	HAL_SPI_Transmit(bmespi2, (uint8_t*) &readReg, 1, 100);
	HAL_SPI_Receive(bmespi2, (uint8_t*) &res, 1, 100);
	SR_deselect(0, BME280_VAL);
	return res;
}

uint16_t BME280_readUint16(uint8_t reg) {
	uint8_t readReg = reg | 0x80;
	uint8_t buff[2];
	SR_select(0, BME280_VAL);
	HAL_SPI_Transmit(bmespi2, (uint8_t*) &readReg, 1, 100);
	HAL_SPI_Receive(bmespi2, (uint8_t*) buff, 2, 100);
	SR_deselect(0, BME280_VAL);
	return ((uint16_t) buff[0] << 8 | (uint16_t) buff[1]);
}

uint16_t BME280_readUint16_LE(uint8_t reg) {
	uint16_t temp = BME280_readUint16(reg);
	return (temp >> 8) | (temp << 8);
}

int16_t BME280_readInt16(uint8_t reg) {
	return (int16_t) BME280_readUint16(reg);
}

int16_t BME280_readInt16_LE(uint8_t reg) {
	return (int16_t) BME280_readUint16_LE(reg);
}

uint32_t BME280_readUint32(uint8_t reg) {
	uint8_t readReg = reg | 0x80;
	uint8_t buff[3];
	SR_select(0, BME280_VAL);
	HAL_SPI_Transmit(bmespi2, (uint8_t*) &readReg, 1, 100);
	HAL_SPI_Receive(bmespi2, (uint8_t*) buff, 3, 100);
	SR_deselect(0, BME280_VAL);
	return ((uint32_t) buff[0] << 16 | (uint32_t) buff[1] << 8
			| (uint32_t) buff[2]);
}
