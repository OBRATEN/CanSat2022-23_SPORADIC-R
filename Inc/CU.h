#ifndef CU_H
#define CU_H

#include <stm32f4xx_hal.h>
#include "config.h"
#include "data_structs.h"
#include "shift_register.h"

void CU_begin(init_status_t *init, SPI_HandleTypeDef *spi);
void CU_disLed(void);
void CU_setLed(void);
void CU_blinkLed(void);

#endif // CU_H
