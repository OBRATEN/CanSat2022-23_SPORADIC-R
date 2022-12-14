#ifndef CONFIG_H
#define CONFIG_H

// Включает вывод параметров в USB
// #define USE_USB_DEBUG

/* Включает вывод чистых данных или полную диагностику
 * ВЫБРАТЬ ОДНО
 */
// #define USE_RF_EMULATOR
// #define USE_DIAGNOSTICS_MODE

/* Режим 5 - используется во время написания начального кода
 * Режим 4 - используется для добавления нового кода
 * Режим 3 - используется для теста интерфейсов и переферии
 * Режим 2 - используется для полного тестирования спутника
 * Режим 1 - используется для предполётной диагностики
 * Режим 0 - используется во время полёта
 */

enum shiftRegVals_e {
	GREEN_LED = 0x0008,
	RED_LED = 0x0002,
	BLUE_LED = 0x0001,
	ORANGE_LED = 0x0004,
	WHITE_LED = 0x0010,
	CU_LED = 0xFF,
	LIS3MDL_VAL = 0x0800,
	LSM6DSL_VAL = 0x1000,
	BME280_VAL = 0x0400
};

#define TESTING_MODE 5

#if TESTING_MODE == 5
#define USE_USB_DEBUG
#define USE_DIAGNOSTICS_MODE
#endif

#define FIRMWARE_VERSION "R0V1S01"

#endif // CONFIG_H
