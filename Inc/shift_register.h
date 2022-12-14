#ifndef SHIFTREG_H
#define SHIFTREG_H

/* shiftRegister.h
 * Author: Arthur Garagulya
 * Description:
 * Библиотека взаимодействия с сдвиговым регистром по интерфейсу SPI
 * для микроконтроллеров STM32F4xx
 */

#include <stm32f4xx_hal.h>
#include "usb_debugger.h"
#include "config.h"
#include "pinout.h"

void SR_init(SPI_HandleTypeDef *spi);
void SR_sync(uint8_t id);
void SR_enable_nOE(uint8_t id);
void SR_disable_nOE(uint8_t id);
void SR_enable_latch(uint8_t id);
void SR_disable_latch(uint8_t id);
void SR_enablePins(uint8_t id, uint16_t pins);
void SR_disablePins(uint8_t id, uint16_t pins);
uint16_t SR_getPins(uint8_t id);
void SR_select(uint8_t id, uint16_t pin);
void SR_deselect(uint8_t id, uint16_t pin);
void SR_deselectAll(uint8_t id);

#endif // SHIFTREG_H
