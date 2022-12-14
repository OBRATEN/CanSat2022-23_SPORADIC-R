#include "CU.h"

SPI_HandleTypeDef *CUspi_;

void CU_begin(init_status_t *init, SPI_HandleTypeDef *spi) {
	CUspi_ = spi;
	init->CU = 1;
}

void CU_disLed(void) {
	SR_deselect(1, CU_LED);
}

void CU_setLed(void) {
	SR_select(1, CU_LED);
}

void CU_blinkLed(void) {
	SR_deselect(1, CU_LED);
	HAL_Delay(100);
	SR_select(1, CU_LED);
	HAL_Delay(100);
}
