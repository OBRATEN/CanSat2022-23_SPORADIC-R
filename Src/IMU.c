#include "IMU.h"

/* IMU.cc
 * Author: Arthur Garagulya
 * Description:
 * Код для работы платы IMU
 * нового конструктора регулярной лиги REV.0
 */

SPI_HandleTypeDef *IMUspi;

uint8_t IMU_begin(init_status_t *init, SPI_HandleTypeDef *spi) {
	IMUspi = spi;
#if defined(USE_USB_DEBUG) && defined(USE_DIAGNOSTICS_MODE)
	DEBUG_logString("Starting IMU initialization protocol...");
#endif
	init->lsm6dsl = LSM6DSL_init(IMUspi);
	init->lis3mdl = LIS3MDL_init(IMUspi);
	init->bme280 = BME280_init(IMUspi);
	init->IMU = (init->lsm6dsl) & (init->lis3mdl) & (init->bme280);
#if defined(USE_USB_DEBUG) && defined(USE_DIAGNOSTICS_MODE)
	if (init->IMU) DEBUG_logString("IMU initialization end with status: OK");
	else DEBUG_logString("IMU initialization end with status: ERROR");
#endif
	return init->IMU;
}

uint8_t IMU_update(IMU_data_t *sensorData) {
	sensorData->lastUpdate = 0;
	sensorData->lastUpdate |= IMU_lsm6dsl_getData(sensorData);
	sensorData->lastUpdate |= IMU_lis3mdl_getData(sensorData);
	sensorData->lastUpdate |= IMU_bme280_getData(sensorData);
	sensorData->id++;
	return sensorData->lastUpdate;
}

uint8_t IMU_lsm6dsl_getData(IMU_data_t *sensorData) {
	uint8_t updateFlag = 0;
	if (LSM6DSL_axelAvailable()) {
		LSM6DSL_readAxel(&(sensorData->aX), &(sensorData->aY),
				&(sensorData->aZ));
		updateFlag |= 1;
	}
	if (LSM6DSL_gyroAvailable()) {
		LSM6DSL_readGyro(&(sensorData->gX), &(sensorData->gY),
				&(sensorData->gZ));
		updateFlag |= 1;
	}
	return updateFlag;
}

uint8_t IMU_lis3mdl_getData(IMU_data_t *sensorData) {
	uint8_t updateFlag = 0;
	if (LIS3MDL_availableXYZ()) {
		LIS3MDL_readXYZ(&(sensorData->mX), &(sensorData->mY),
				&(sensorData->mZ));
		updateFlag |= 1;
	}
	return updateFlag;
}

uint8_t IMU_bme280_getData(IMU_data_t *sensorData) {
	BME280_update();
	uint8_t updateFlag = 0;
	float tempTemp = sensorData->temp;
	float tempPress = sensorData->press;
	float tempHum = sensorData->hum;
	BME280_readTemp(&(sensorData->temp));
	BME280_readPress(&(sensorData->press));
	BME280_readHum(&(sensorData->hum));
	sensorData->alt = BME280_calcAlt(sensorData->press);
	updateFlag |= (tempTemp != sensorData->temp);
	updateFlag |= (tempPress != sensorData->press);
	updateFlag |= (tempHum != sensorData->hum);
	return updateFlag;
}

void IMU_printAvailable(void) {
#ifdef USE_USB_DEBUG
	DEBUG_printString("IMU sensors available info;\n\r  LSM6DSL axel: ");
	DEBUG_printUint8(LSM6DSL_axelAvailable());
	DEBUG_printString(" | LSM6DSL gyro: ");
	DEBUG_printUint8(LSM6DSL_gyroAvailable());
	DEBUG_printString(";\n  LIS3MDL mag: ");
	DEBUG_printUint8(LIS3MDL_availableXYZ());
	DEBUG_printString(";\n\r");
#endif // USE_USB_DEBUG
}

void IMU_blinkBlueLed(void) {
	SR_deselect(0, BLUE_LED);
	HAL_Delay(100);
	SR_select(0, BLUE_LED);
	HAL_Delay(100);
}

void IMU_blinkRedLed(void) {
	SR_deselect(0, RED_LED);
	HAL_Delay(100);
	SR_select(0, RED_LED);
	HAL_Delay(100);
}

void IMU_blinkGreenLed(void) {
	SR_deselect(0, GREEN_LED);
	HAL_Delay(100);
	SR_select(0, GREEN_LED);
	HAL_Delay(100);
}

void IMU_blinkOrangeLed(void) {
	SR_deselect(0, ORANGE_LED);
	HAL_Delay(100);
	SR_select(0, ORANGE_LED);
	HAL_Delay(100);
}

void IMU_blinkWhiteLed(void) {
	SR_deselect(0, WHITE_LED);
	HAL_Delay(100);
	SR_select(0, WHITE_LED);
	HAL_Delay(100);
}

void IMU_setBlueLed(void) {
	SR_deselect(0, BLUE_LED);
}

void IMU_disBlueLed(void) {
	SR_select(0, BLUE_LED);
}

void IMU_setRedLed(void) {
	SR_deselect(0, RED_LED);
}

void IMU_disRedLed(void) {
	SR_select(0, RED_LED);
}

void IMU_setGreenLed(void) {
	SR_deselect(0, GREEN_LED);
}

void IMU_disGreenLed(void) {
	SR_select(0, GREEN_LED);
}

void IMU_setOrangeLed(void) {
	SR_deselect(0, ORANGE_LED);
}

void IMU_disOrangeLed(void) {
	SR_select(0, ORANGE_LED);
}

void IMU_setWhiteLed(void) {
	SR_deselect(0, WHITE_LED);
}

void IMU_disWhiteLed(void) {
	SR_select(0, WHITE_LED);
}
