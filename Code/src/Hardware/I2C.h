#ifndef I2C_HEADER_THOMAS_CHEVALIER
#define I2C_HEADER_THOMAS_CHEVALIER

/*
Inspired from https://github.com/g4lvanix/I2C-master-lib
and datasheet
(c) Thomas Chevalier - 2017
*/

#include "../Globals.h"

#if defined(I2C_REQUIRED)

// 100 kHz is a typical frequency for i2c device
// including MB85RC256V fram
#define F_SCL (100000)
#define PRESCALER (1)
#define I2C_READ (0x01)
#define I2C_WRITE (0x00)

void i2c_init(void);

uint8_t i2c_start(uint8_t addr);
uint8_t i2c_write(uint8_t data);
uint8_t i2c_read_ack(void);
uint8_t i2c_read_nack(void);

uint8_t i2c_transmit(uint8_t addr, uint8_t* data, uint16_t length);
uint8_t i2c_receive(uint8_t addr, uint8_t* data, uint16_t length);

void i2c_stop(void);


#endif  // I2C_REQUIRED
#endif // I2C_HEADER_THOMAS_CHEVALIER
