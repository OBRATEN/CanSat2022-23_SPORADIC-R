#ifndef LSM6DSL_H
#define LSM6DSL_H

/* lsm6dsl.h
 * Author: Arthur Garagulya
 * Description:
 * Библиотека взаимодействия с гироакселерометром
 * LSM6DSL по интерфейсу SPI
 * для микроконтроллеров STM32F4xx
 */

#include <shift_register.h>
#include <stm32f4xx_hal.h>
#include "config.h"

enum LSM6DSL_registerMap_e {
	WHO_AM_I = 0x0F,
	CTRL2_XL = 0x10,
	CTRL2_G = 0x11,
	CTRL7_G = 0x16,
	CRTL8_XL = 0x17,
	OUTX_L_XL = 0x28,
	OUTX_H_XL = 0x29,
	OUTY_L_XL = 0x2A,
	OUTY_H_XL = 0x2B,
	OUTZ_L_XL = 0x2C,
	OUTZ_H_XL = 0x2D,
	OUTX_L_G = 0x22,
	OUTX_H_G = 0x23,
	OUTY_L_G = 0x24,
	OUTY_H_G = 0x25,
	OUTZ_L_G = 0x26,
	OUTZ_H_G = 0x27,
	OUT_TEMP_L = 0x20,
	OUT_TEMP_H = 0x21
};

uint8_t LSM6DSL_init(SPI_HandleTypeDef *spi);

uint8_t LSM6DSL_axelAvailable(void);
uint8_t LSM6DSL_gyroAvailable(void);

void LSM6DSL_readAxel(float *x, float *y, float *z);
void LSM6DSL_readGyro(float *x, float *y, float *z);
void LSM6DSL_readTemp(int16_t *temp);

void LSM6DSL_writeReg(uint8_t addr, uint8_t data);
uint8_t LSM6DSL_readReg(uint8_t addr);
void LSM6DSL_readRegs(uint8_t addr, uint8_t *buff, uint8_t size);

#endif // LSM6DSL_H
