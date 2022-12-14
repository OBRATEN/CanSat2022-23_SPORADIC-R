#ifndef LIS3MDL_H
#define LIS3MDL_H

/* lis3mdl.h
 * Author: Arthur Garagulya
 * Description:
 * Библиотека взаимодействия с магнитометром
 * LIS3MDL по интерфейсу SPI
 * для микроконтроллеров STM32F4xx
 */

#include <shift_register.h>
#include <stm32f4xx_hal.h>
#include "usb_debugger.h"
#include "config.h"

enum LIS3MDL_registersMap_e {
	LIS3MDL_WHO_AM_I = 0x0F,
	CTRL_REG1 = 0x20,
	CTRL_REG2 = 0x21,
	CTRL_REG3 = 0x22,
	CTRL_REG4 = 0x23,
	CTRL_REG5 = 0x24,
	STATUS_REG = 0x27,
	OUT_X_L = 0x28,
	OUT_X_H = 0x29,
	OUT_Y_L = 0x2A,
	OUT_Y_H = 0x2B,
	OUT_Z_L = 0x2C,
	OUT_Z_H = 0x2D,
	TEMP_OUT_L = 0x2E,
	TEMP_OUT_H = 0x2F,
};

uint8_t LIS3MDL_init(SPI_HandleTypeDef *spi);

uint8_t LIS3MDL_availableXYZ(void);
uint8_t LIS3MDL_availableX(void);
uint8_t LIS3MDL_availableY(void);
uint8_t LIS3MDL_availableZ(void);

void LIS3MDL_readXYZ(float *x, float *y, float *z);
void LIS3MDL_readX(float *x);
void LIS3MDL_readY(float *y);
void LIS3MDL_readZ(float *z);
void LIS3MDL_readTemp(int16_t *temp);

void LIS3MDL_writeReg(uint8_t addr, uint8_t data);
uint8_t LIS3MDL_readReg(uint8_t addr);
void LIS3MDL_readRegs(uint8_t addr, uint8_t *buff, uint8_t size);

#endif // LIS3MDL_H
