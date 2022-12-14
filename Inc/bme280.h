#ifndef BME280_H
#define BME280_H

/* bme280.h
 * Author: Arthur Garagulya
 * Description:
 * Библиотека взаимодействия с барометром
 * BME280 по интерфейсу SPI
 * для микроконтроллеров STM32F4xx
 */

#include <shift_register.h>
#include <stm32f4xx_hal.h>
#include <math.h>
#include "config.h"

enum BME280_registerMap_e {
	CHIP_ID = 0xD0,
	CTRL_HUM = 0xF2,
	BME_STATUS_REG = 0xF3,
	CTRL_MEAS = 0xF4,
	CONFIG_REG = 0xF5,
	PRESS_REG = 0xF7,
	TEMP_REG = 0xFA,
	HUM_REG = 0xFD,
	TEMP_DIG_REG = 0x88,
	PRESS_DIG_REG = 0x8E,
	HUM_DIG_REG1 = 0xA1,
	HUM_DIG_REG2 = 0xE2,
	RESET_REG = 0xE0
};

uint8_t BME280_init(SPI_HandleTypeDef *spi);

uint8_t BME280_inCalibrationMode(void);

void BME280_update(void);

void BME280_readTemp(float *temp);
void BME280_readPress(float *press);
void BME280_readHum(float *hum);
float BME280_calcAlt(float press);

void BME280_readCalibrations(void);
void BME280_clearCalibrations(void);

int32_t BME280_assembleRaw(uint8_t *data, uint8_t xlsb);

void BME280_writeUint8(uint8_t reg, uint8_t data);
uint8_t BME280_readUint8(uint8_t reg);
uint16_t BME280_readUint16(uint8_t reg);
uint16_t BME280_readUint16_LE(uint8_t reg);
int16_t BME280_readInt16(uint8_t reg);
int16_t BME280_readInt16_LE(uint8_t reg);
uint32_t BME280_readUint32(uint8_t reg);

#endif // BME280_H
