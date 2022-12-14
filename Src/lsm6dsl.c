#include "lsm6dsl.h"

/* lsm6dsl.cc
 * Author: Arthur Garagulya
 * Description:
 * Библиотека взаимодействия с гироакселерометром
 * LSM6DSL по интерфейсу SPI
 * для микроконтроллеров STM32F4xx
 */

SPI_HandleTypeDef *lsm6spi;

uint8_t LSM6DSL_init(SPI_HandleTypeDef *spi) {
#if defined(USE_USB_DEBUG) && defined(USE_DIAGNOSTICS_MODE)
		DEBUG_logString("Starting LSM6DSL initialization protocol...");
#endif
	lsm6spi = spi;
	uint8_t chipVal = LSM6DSL_readReg(WHO_AM_I);
	if (chipVal != 0x6A) {
#if defined(USE_USB_DEBUG) && defined(USE_DIAGNOSTICS_MODE)
	DEBUG_logStringAndByte("No LSM6DSL found at SPI2 bus, got CHIPID value: ", chipVal);
#endif
		return 0;
	}
#if defined(USE_USB_DEBUG) && defined(USE_DIAGNOSTICS_MODE)
	DEBUG_logString("LSM6DSL found at SPI2 bus");
#endif
	LSM6DSL_writeReg(CTRL2_XL, 0x46);
	LSM6DSL_writeReg(CTRL2_G, 0x4E);
	LSM6DSL_writeReg(CTRL7_G, 0x00);
	LSM6DSL_writeReg(CRTL8_XL, 0x09);
#if defined(USE_USB_DEBUG) && defined(USE_DIAGNOSTICS_MODE)
	DEBUG_logString("LSM6DSL sensor has been initialized");
#endif
	return 1;
}

void LSM6DSL_writeReg(uint8_t addr, uint8_t data) {
	SR_select(0, LSM6DSL_VAL);
	HAL_SPI_Transmit(lsm6spi, (uint8_t*)&addr, 1, 100);
	HAL_SPI_Transmit(lsm6spi, (uint8_t*)&data, 1, 100);
	SR_deselect(0, LSM6DSL_VAL);
}

uint8_t LSM6DSL_readReg(uint8_t addr) {
	uint8_t readAddr = (addr | 0x80);
	uint8_t answer = 0;
	SR_select(0, LSM6DSL_VAL);
	HAL_SPI_Transmit(lsm6spi, (uint8_t*)&readAddr, 1, 100);
	HAL_SPI_Receive(lsm6spi, (uint8_t*)&answer, 1, 100);
	SR_deselect(0, LSM6DSL_VAL);
	return answer;
}

void LSM6DSL_readRegs(uint8_t addr, uint8_t *buff, uint8_t size) {
	uint8_t readAddr = (addr | 0x80);
	SR_select(0, LSM6DSL_VAL);
	HAL_SPI_Transmit(lsm6spi, (uint8_t*)&readAddr, 1, 100);
	HAL_SPI_Receive(lsm6spi, buff, size, 100);
	SR_deselect(0, LSM6DSL_VAL);
}

uint8_t LSM6DSL_gyroAvailable(void) {
	return (LSM6DSL_readReg(0x1E) & 0x02);
}

uint8_t LSM6DSL_axelAvailable(void) {
	return (LSM6DSL_readReg(0x1E) & 0x01);
}

void LSM6DSL_readAxel(float *x, float *y, float *z) {
	int16_t raw[3];
	LSM6DSL_readRegs(OUTX_L_XL, (uint8_t*)raw, 6);
	//*x = raw[0] * 0.488; mg
	//*y = raw[1] * 0.488;
	//*z = raw[2] * 0.488;
	*x = raw[0] * 16.0 / 32768.0; // g
	*y = raw[1] * 16.0 / 32768.0;
	*z = raw[2] * 16.0 / 32768.0;
}

void LSM6DSL_readGyro(float *x, float *y, float *z) {
  int16_t raw[3];
  LSM6DSL_readRegs(OUTX_L_G, (uint8_t*)raw, 6);
  *x = raw[0] * 2000.0 / 32768.0;
  *y = raw[1] * 2000.0 / 32768.0;
	*z = raw[2] * 2000.0 / 32768.0;
}

void LSM6DSL_readTemp(int16_t *temp) {
	*temp = (int16_t)((LSM6DSL_readReg(OUT_TEMP_H) << 8) | (LSM6DSL_readReg(OUT_TEMP_L) & 0x00FF));
}
