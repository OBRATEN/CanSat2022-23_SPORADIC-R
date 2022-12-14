#include "lis3mdl.h"

/* lis3mdl.сс
 * Author: Arthur Garagulya
 * Description:
 * Библиотека взаимодействия с магнитометром
 * LIS3MDL по интерфейсу SPI
 * для микроконтроллеров STM32F4xx
 */

SPI_HandleTypeDef *lis3spi;

uint8_t LIS3MDL_init(SPI_HandleTypeDef *spi) {
#if defined(USE_USB_DEBUG) && defined(USE_DIAGNOSTICS_MODE)
	DEBUG_logString("Starting LIS3MDL initialization protocol...");
#endif
	lis3spi = spi;
	uint8_t chipVal = LIS3MDL_readReg(0x0F);
	if (chipVal != 0x3D) {
#if defined(USE_USB_DEBUG) && defined(USE_DIAGNOSTICS_MODE)
		DEBUG_logStringAndByte("No LIS3MDL found at SPI2 bus, got CHIPID value: ",
				chipVal);
#endif
		return 0;
	}
#if defined(USE_USB_DEBUG) && defined(USE_DIAGNOSTICS_MODE)
	DEBUG_logString("LIS3MDL found at SPI2 bus");
#endif
	LIS3MDL_writeReg(CTRL_REG1, 0xFD); // en temperature, UH-perfomance, 80Hz, no FAST_ODR, self test
	LIS3MDL_writeReg(CTRL_REG2, 0x60); // 16 gauss
	LIS3MDL_writeReg(CTRL_REG3, 0x00); // DO NOT TOUCH WITH YOUR FUCKING HANDS
	LIS3MDL_writeReg(CTRL_REG4, 0x0C); // IDK :)
#if defined(USE_USB_DEBUG) && defined(USE_DIAGNOSTICS_MODE)
	DEBUG_logString("LIS3MDL sensor has been initialized");
#endif
	return 1;
}

void LIS3MDL_writeReg(uint8_t addr, uint8_t data) {
	SR_select(0, LIS3MDL_VAL);
	HAL_SPI_Transmit(lis3spi, (uint8_t*) &addr, 1, 100);
	HAL_SPI_Transmit(lis3spi, (uint8_t*) &data, 1, 100);
	SR_deselect(0, LIS3MDL_VAL);
}

uint8_t LIS3MDL_readReg(uint8_t addr) {
	uint8_t readAddr = (addr | 0x80);
	uint8_t answer = 0;
	SR_select(0, LIS3MDL_VAL);
	HAL_SPI_Transmit(lis3spi, (uint8_t*) &readAddr, 1, 100);
	HAL_SPI_Receive(lis3spi, (uint8_t*) &answer, 1, 100);
	SR_deselect(0, LIS3MDL_VAL);
	return answer;
}

void LIS3MDL_readRegs(uint8_t addr, uint8_t *buff, uint8_t size) {
	uint8_t readAddr = (addr | 0x80);
	SR_select(0, LIS3MDL_VAL);
	HAL_SPI_Transmit(lis3spi, (uint8_t*) &readAddr, 1, 100);
	HAL_SPI_Receive(lis3spi, buff, size, 100);
	SR_deselect(0, LIS3MDL_VAL);
}

uint8_t LIS3MDL_availableXYZ(void) {
	return (LIS3MDL_readReg(STATUS_REG) & 0x08);
}

uint8_t LIS3MDL_availableX(void) {
	return (LIS3MDL_readReg(STATUS_REG) & 0x01);
}

uint8_t LIS3MDL_availableY(void) {
	return (LIS3MDL_readReg(STATUS_REG) & 0x02);
}
uint8_t LIS3MDL_availableZ(void) {
	return (LIS3MDL_readReg(STATUS_REG) & 0x03);
}

void LIS3MDL_readXYZ(float *x, float *y, float *z) {
	int16_t raw[3];
	raw[0] = (LIS3MDL_readReg(OUT_X_H) << 8)
			| (LIS3MDL_readReg(OUT_X_L) & 0x00FF);
	raw[1] = (LIS3MDL_readReg(OUT_Y_H) << 8)
			| (LIS3MDL_readReg(OUT_Y_L) & 0x00FF);
	raw[2] = (LIS3MDL_readReg(OUT_Z_H) << 8)
			| (LIS3MDL_readReg(OUT_Z_L) & 0x00FF);
	//readRegs(OUT_X_L, (uint8_t*)raw, 6);
	*x = (float) raw[0] / 1711.0;
	*y = (float) raw[1] / 1711.0;
	*z = (float) raw[2] / 1711.0;
}

void LIS3MDL_readX(float *x) {
	int16_t raw = (LIS3MDL_readReg(OUT_X_H) << 8)
			| (LIS3MDL_readReg(OUT_X_L) & 0x00FF);
	//readRegs(OUT_X_L, (uint8_t*)raw, 2);
	*x = (float) raw / 1711.0;
}

void LIS3MDL_readY(float *y) {
	int16_t raw = (LIS3MDL_readReg(OUT_Y_H) << 8)
			| (LIS3MDL_readReg(OUT_Y_L) & 0x00FF);
	//readRegs(OUT_Y_L, (uint8_t*)raw, 2);
	*y = (float) raw / 1711.0;
}

void LIS3MDL_readZ(float *z) {
	int16_t raw = (LIS3MDL_readReg(OUT_Z_H) << 8)
			| (LIS3MDL_readReg(OUT_Z_L) & 0x00FF);
	//readRegs(OUT_Z_L, (uint8_t*)raw, 2);
	*z = (float) raw / 1711.0;
}

void LIS3MDL_readTemp(int16_t *temp) {
	*temp = (int16_t) ((LIS3MDL_readReg(TEMP_OUT_H) << 8)
			| (LIS3MDL_readReg(TEMP_OUT_L) & 0x00FF));
	//readRegs(TEMP_OUT_L, (uint8_t*)raw, 2);
}
