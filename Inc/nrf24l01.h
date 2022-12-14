#ifndef NRF24_H
#define NRF24_H

/* bme280.h
 * Author: Arthur Garagulya
 * Description:
 * Библиотека взаимодействия с радиомодулем
 * nRF24l01 по интерфейсу SPI
 * для микроконтроллеров STM32F4xx
 */

#include <shift_register.h>
#include <stm32f4xx_hal.h>
#include "config.h"

enum nRF24_maskMap_e {
	READ_MASK = 0x1F, WRITE_MASK = 0x20
};

uint8_t nRF24_init(SPI_HandleTypeDef *spi);
void nRF24_writeReg(uint8_t reg, uint8_t data);
void nRF24_writeBuff(uint8_t reg, const uint8_t *buf, uint8_t len);
uint8_t nRF24_readReg(uint8_t reg);

void nRF24_flush_tx();
void nRF24_set_address(uint64_t c);
void nRF24_send(const void *buf);

#endif // NRF24_H
