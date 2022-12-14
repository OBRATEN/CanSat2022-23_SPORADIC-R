#include "shift_register.h"

uint16_t IMU_state_ = 0x1FE0;
uint16_t CU_state_ = 0x00;
SPI_HandleTypeDef *srspi_;

void SR_enable_nOE(uint8_t id) {
	if (id == 0) HAL_GPIO_WritePin(IMU_nOE_GPIO_Port, IMU_nOE_Pin, GPIO_PIN_RESET);
	else HAL_GPIO_WritePin(IMU_nOE_GPIO_Port, IMU_nOE_Pin, GPIO_PIN_RESET);
}

void SR_disable_nOE(uint8_t id) {
	if (id == 0) HAL_GPIO_WritePin(IMU_nOE_GPIO_Port, IMU_nOE_Pin, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(IMU_nOE_GPIO_Port, IMU_nOE_Pin, GPIO_PIN_SET);
}

void SR_enable_latch(uint8_t id) {
	if (id == 0)
		HAL_GPIO_WritePin(IMU_Latch_GPIO_Port, IMU_Latch_Pin, GPIO_PIN_SET);
	if (id == 1) HAL_GPIO_WritePin(CU_Latch_GPIO_Port, CU_Latch_Pin,
			GPIO_PIN_SET);
}

void SR_disable_latch(uint8_t id) {
	if (id == 0)
		HAL_GPIO_WritePin(IMU_Latch_GPIO_Port, IMU_Latch_Pin, GPIO_PIN_RESET);
	if (id == 1) HAL_GPIO_WritePin(CU_Latch_GPIO_Port, CU_Latch_Pin,
			GPIO_PIN_RESET);
}

void SR_sync(uint8_t id) {
	SR_disable_nOE(id);
	if (id == 0) HAL_SPI_Transmit(srspi_, (uint8_t*) &IMU_state_, 2, 100);
	if (id == 1) HAL_SPI_Transmit(srspi_, (uint8_t*) &CU_state_, 2, 100);
	SR_enable_latch(id);
	SR_disable_latch(id);
	SR_enable_nOE(id);
}

void SR_enablePins(uint8_t id, uint16_t pins) {
	if (id == 0) IMU_state_ |= pins;
	if (id == 1) CU_state_ |= pins;
}

void SR_disablePins(uint8_t id, uint16_t pins) {
	if (id == 0) IMU_state_ &= ~pins;
	if (id == 1) CU_state_ &= ~pins;
}

void SR_select(uint8_t id, uint16_t pin) {
#if defined(USE_USB_DEBUG) && defined(USE_DIAGNOSTICS_MODE)
	if (id != 0 && id != 1) DEBUG_logString("WARNING: called SR with unknown id!");
#endif
	if (id == 0) {
		IMU_state_ |= 0x1FE0;
		IMU_state_ &= ~pin;
	}
	if (id == 1) {
		CU_state_ = 0x00;
		CU_state_ &= ~pin;
	}
	SR_sync(id);
}

void SR_deselect(uint8_t id, uint16_t pin) {
#if defined(USE_USB_DEBUG) && defined(USE_DIAGNOSTICS_MODE)
	if (id != 0 && id != 1) DEBUG_logString("WARNING: called SR with unknown id!");
#endif
	if (id == 0) {
		IMU_state_ |= 0x1FE0;
		IMU_state_ |= pin;
	}
	if (id == 1) {
		CU_state_ = 0x00;
		CU_state_ |= pin;
	}
	SR_sync(id);
}

void SR_deselectAll(uint8_t id) {
#if defined(USE_USB_DEBUG) && defined(USE_DIAGNOSTICS_MODE)
	if (id != 0 && id != 1) DEBUG_logString("WARNING: called SR with unknown id!");
#endif
	if (id == 0) IMU_state_ |= 0x1FE0;
	if (id == 1) CU_state_ |= 0x1FE0;
	SR_sync(id);
}

void SR_init(SPI_HandleTypeDef *spi) {
	srspi_ = spi;
	SR_enable_nOE(0);
	SR_enable_nOE(1);
	SR_sync(0);
	SR_sync(1);
#if defined(USE_USB_DEBUG) && defined(USE_DIAGNOSTICS_MODE)
	DEBUG_logString("Shift register library attached");
#endif
}
