#ifndef INC_IMU_H_
#define INC_IMU_H_

/* IMU.h
 * Author: Arthur Garagulya
 * Description:
 * Код для работы платы IMU
 * нового конструктора регулярной лиги REV.0
 */

#include "config.h"
#include "data_structs.h"
#include "usb_debugger.h"
#include "shift_register.h"
#include "lsm6dsl.h"
#include "lis3mdl.h"
#include "bme280.h"

uint8_t IMU_begin(init_status_t *init, SPI_HandleTypeDef *mspi);
uint8_t IMU_update(IMU_data_t *sensorData);

// 0 if no new data, 1 if new data
uint8_t IMU_lsm6dsl_getData(IMU_data_t *sensorData);
uint8_t IMU_lis3mdl_getData(IMU_data_t *sensorData);
uint8_t IMU_bme280_getData(IMU_data_t *sensorData);

void IMU_blinkBlueLed(void);
void IMU_blinkRedLed(void);
void IMU_blinkGreenLed(void);
void IMU_blinkOrangeLed(void);
void IMU_blinkWhiteLed(void);

void IMU_setBlue(void);
void IMU_setRed(void);
void IMU_setGreen(void);
void IMU_setOrange(void);
void IMU_setWhite(void);

void IMU_disBlue(void);
void IMU_disRed(void);
void IMU_disGreen(void);
void IMU_disOrange(void);
void IMU_disWhite(void);

void IMU_updateLed(void);

void IMU_setBlueLed(void);
void IMU_disBlueLed(void);
void IMU_setRedLed(void);
void IMU_disRedLed(void);
void IMU_setGreenLed(void);
void IMU_disGreenLed(void);
void IMU_setOrangeLed(void);
void IMU_disOrangeLed(void);
void IMU_setWhiteLed(void);
void IMU_disWhiteLed(void);

void IMU_printAvailable(void);

#endif /* INC_IMU_H_ */
