#include "nrf24l01.h"

SPI_HandleTypeDef *nrfspi;

uint8_t nRF24_init(SPI_HandleTypeDef *spi) {
	nrfspi = spi;
	return nRF24_readReg(0x00);
}

void nRF24_writeReg(uint8_t reg, uint8_t data) {
}
void nRF24_writeBuff(uint8_t reg, const uint8_t *buf, uint8_t len) {

}
uint8_t nRF24_readReg(uint8_t reg) {
	uint8_t readReg = reg & READ_MASK;
	uint8_t res;
	SR_select(1, 0x02);
	HAL_SPI_Transmit(nrfspi, (uint8_t*) &readReg, 1, 100);
	HAL_SPI_Receive(nrfspi, (uint8_t*) &res, 1, 100);
	SR_deselect(1, 0x02);
	return res;
}

/*
 nRF_radio::nRF_radio() {
 _spi = SPI_interface();
 SPI_DDR |= (1 << NRF_CE);
 }

 void nRF_radio::writeReg(uint8_t reg, uint8_t data ) {
 NSS_high();
 _spi.transfer(reg | NRF_WRITE_MASK);
 _spi.transfer(data);
 NSS_low();
 }

 void nRF_radio::writeReg(uint8_t reg, const uint8_t* buf, uint8_t len) {
 NSS_high();
 _spi.transfer(reg | NRF_WRITE_MASK);
 while (len--) _spi.transfer(*buf++);
 NSS_low();
 }

 uint8_t nRF_radio::readReg(uint8_t reg) {
 NSS_high();
 _spi.transfer(reg & NRF_READ_MASK);
 uint8_t result= _spi.transfer(0xFF);
 NSS_low();
 return result;
 }

 void nRF_radio::flush_tx() {
 NSS_high();
 _spi.transfer(0xE1);
 NSS_low();
 }

 void nRF_radio::send(const void* buf) {
 NCE_low();
 const uint8_t* current = reinterpret_cast<const uint8_t*>(buf);
 flush_tx(); // clear previous transmission
 writeReg(0x07, 0b00110000); // clear previous transmission
 NSS_high();
 _spi.transfer(0xA0);
 for (uint8_t i = 0; i < 32; i++) _spi.transfer(*current++);
 NSS_low();
 NCE_high();
 }

 void nRF_radio::set_address(uint64_t value) {
 writeReg(0x0A, reinterpret_cast<uint8_t*>(&value), 5);
 writeReg(0x10, reinterpret_cast<uint8_t*>(&value), 5);
 }

 uint8_t nRF_radio::init(void) {
 writeReg(0x00, 0b00001110);
 writeReg(0x01, 0b00111111);
 writeReg(0x02, 0b00000000);
 writeReg(0x03, 0b00000011); // 5 byte adres wight
 writeReg(0x04, 0b11111111); // wait 4000us and repeat 15 times, if data will be not given
 writeReg(0x05, 0x5C);       // frequency set
 writeReg(0x06, 0b00000110); // speed and power rate set
 writeReg(0x07, 0b01110000); // Clear status register
 writeReg(0x11, 0b00010100); // PACKET_SIZE pipe 0
 writeReg(0x12, 0b00010100); // PACKET_SIZE pipe 1
 writeReg(0x13, 0b00010100); // PACKET_SIZE pipe 2
 writeReg(0x14, 0b00010100); // PACKET_SIZE pipe 3
 writeReg(0x15, 0b00010100); // PACKET_SIZE pipe 4
 writeReg(0x16, 0b00010100); // PACKET_SIZE pipe 5
 writeReg(0x17, 0b00000000); // Clear status FIFO register
 writeReg(0x1C, 0b00000000);
 writeReg(0x1D, 0b00000011);
 set_address(0x7878787878LL);
 uint8_t cfg = readReg(0x00);
 return (cfg == 0b00001110) ? 1 : 0;
 }
 */
