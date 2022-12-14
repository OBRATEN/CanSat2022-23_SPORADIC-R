#ifndef DATA_STRUCTS_H
#define DATA_STRUCTS_H

#include <stm32f4xx_hal.h>

typedef struct {
	uint32_t lastUpdate;
	uint32_t id;
	float temp;
	float alt;
	float press;
	float hum;
	float aX;
	float aY;
	float aZ;
	float gX;
	float gY;
	float gZ;
	float mX;
	float mY;
	float mZ;
} IMU_data_t;

typedef struct {
	uint8_t bme280;
	uint8_t lsm6dsl;
	uint8_t lis3mdl;
	uint8_t nrf24l01;
	uint8_t IMU;
	uint8_t MCU;
	uint8_t CU;
	uint8_t MU;
} init_status_t;

#endif // DATA_STRUCTS_H
