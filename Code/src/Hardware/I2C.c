#include "I2C.h"

#if defined(I2C_REQUIRED)

#include <avr/io.h>
#include <util/twi.h>

void i2c_init(void)
{
	// Active internal pullups
	// on PD0 (SCL) and PD1 (SDA)
	PORTD |= 0x03;

	TWSR &= ~(1<<TWPS0);
	TWSR &= ~(1<<TWPS1);
	// See section 20.5.2 of Atmega32u4 datasheet (page 231)
	TWBR = (uint8_t)((F_CPU / F_SCL) - 16)/2;
	// enable twi module, and acks
	TWCR = (1<<TWEN) | (1<<TWEA);
	
}

uint8_t i2c_start(uint8_t addr)
{
	// Clear TWCR register
	TWCR = 0;

	// See section 20.6 of datasheet (page 235)
	// Send START condition
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);

	// Wait for TWINT Flag set. This
	// indicates that the START
	// condition has been transmitted
	while (!(TWCR & (1<<TWINT)));

	// Check value of TWI Status
	// Register. Mask prescaler bits. If
	// status different from START go to
	// ERROR
	if ((TWSR & 0xF8) != TW_START)
	{
		return RETURN_ERROR;
	}

	// Load slave addr into TWDR
	// Register. Clear TWINT bit in
	// TWCR to start transmission of
	// addresse
	TWDR = addr;
	TWCR = (1<<TWINT) | (1<<TWEN);

	// Wait for TWINT Flag set. This
	// indicates that the SLA+W has
	// been transmitted, and
	// ACK/NACK has been received.
	while (!(TWCR &(1<<TWINT)));

	// Check value of TWI Status
	// Register. Mask prescaler bits. If
	// status different from
	// MT_SLA_ACK or MR_SLA_ACK go to ERROR
	if ( ((TWSR & 0xF8) != TW_MT_SLA_ACK) && ((TWSR & 0xF8) != TW_MR_SLA_ACK) )
	{
		return RETURN_ERROR;
	}


	return RETURN_SUCCESS;
}

uint8_t i2c_write(uint8_t data)
{
	// Load DATA into TWDR Register.
	// Clear TWINT bit in TWCR to start
	// transmission of data
	TWDR = data;
	TWCR = (1<<TWINT) | (1<<TWEN);

	// Wait for TWINT Flag set. This
	// indicates that the DATA has been
	// transmitted, and ACK/NACK has
	// been received.
	while (!(TWCR & (1<<TWINT)));

	// Check value of TWI Status
	// Register. Mask prescaler bits. If
	// status different from
	// MT_DATA_ACK go to ERROR
	if ((TWSR & 0xF8) != TW_MT_DATA_ACK)
	{
		return RETURN_ERROR;
	}
	return RETURN_SUCCESS;
}


uint8_t i2c_read_ack(void)
{
	
	// start TWI module and acknowledge data after reception
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA); 
	// wait for end of transmission
	while( !(TWCR & (1<<TWINT)) );
	// return received data from TWDR
	return TWDR;
}

uint8_t i2c_read_nack(void)
{
	
	// start receiving without acknowledging reception
	TWCR = (1<<TWINT) | (1<<TWEN);
	// wait for end of transmission
	while( !(TWCR & (1<<TWINT)) );
	// return received data from TWDR
	return TWDR;
}

uint8_t i2c_transmit(uint8_t addr, uint8_t* data, uint16_t length)
{
	if (i2c_start((addr<<1) | I2C_WRITE) != RETURN_SUCCESS) return RETURN_ERROR;
	
	for (uint16_t i = 0; i < length; i++)
	{
		if (i2c_write(data[i]) != RETURN_SUCCESS) return RETURN_ERROR;
	}
	
	i2c_stop();
	
	return RETURN_SUCCESS;
}

uint8_t i2c_receive(uint8_t addr, uint8_t* data, uint16_t length)
{
	if (i2c_start((addr<<1) | I2C_READ) != RETURN_SUCCESS) return RETURN_ERROR;
	
	for (uint16_t i = 0; i < (length-1); i++)
	{
		data[i] = i2c_read_ack();
	}
	data[(length-1)] = i2c_read_nack();
	
	i2c_stop();
	
	return RETURN_SUCCESS;
}

void i2c_stop(void)
{
	// Transmit STOP condition
	TWCR = (1<<TWINT) | (1<<TWEN)| (1<<TWSTO);
	
	// wait for stop condition to be exectued on bus
	// TWINT is not set after a stop condition!
	while(TWCR & (1<<TWSTO));
}




#endif // I2C_REQUIRED