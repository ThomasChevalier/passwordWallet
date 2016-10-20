#include "Spi.h"

//Initialize SPI hardware

void spi_setup_hardware (void)
{
    DDRB |= (1<<PB1) | (1<<PB2); // Init spi pin as output
    DDRB &= ~(1<<PB3); // Init spi pin as output
}

// Shift full array through target device
void spi_transfer(uint8_t * dataout, uint8_t * datain, uint8_t len)
{
    uint8_t i;
    for (i = 0; i < len; i++)
    {
        SPDR = dataout[i];
        while (!(SPSR & _BV(SPIF)));
        datain[i] = SPDR;
    }
}

// Shift full array to target device without receiving any byte
void spi_send (uint8_t * dataout, uint8_t len)
{
    uint8_t i;
    for (i = 0; i < len; i++)
    {
        SPDR = dataout[i];
        while (!(SPSR & _BV(SPIF)));
    }
}

// Write to the SPI bus (MOSI pin) and also receive (MISO pin)
uint8_t spi_transfer_8(uint8_t data)
{
    SPDR = data;
    /*
     * The following NOP introduces a small delay that can prevent the wait
     * loop form iterating when running at the maximum speed. This gives
     * about 10% more speed, even if it seems counter-intuitive. At lower
     * speeds it is unnoticed.
     */
    asm volatile("nop");
    while (!(SPSR & _BV(SPIF))) ; // wait
    return SPDR;
}

void spi_read(uint8_t* buffer, uint8_t size)
{
    uint8_t i = 0;
    for(; i < size; ++i)
    {
        SPDR = 0x00;
        while (!(SPSR & _BV(SPIF)));
        buffer[i] = SPDR;
    }
}

uint8_t spi_read_8()
{
    SPDR = 0x00;
    while (!(SPSR & _BV(SPIF)));
    return SPDR;
}

// Write 8 bit on bus
void spi_send_8(uint8_t data)
{
    SPDR = data;
    while (!(SPSR & _BV(SPIF)));
}

/*
inline uint16_t spi_transfer_16(uint16_t data)
{
union { uint16_t val; struct { uint8_t lsb; uint8_t msb; }; } in, out;
in.val = data;
if (!(SPCR & _BV(DORD))) {
	SPDR = in.msb;
	asm volatile("nop"); // See spi_transfer_8(uint8_t) function
	while (!(SPSR & _BV(SPIF))) ;
	out.msb = SPDR;
	SPDR = in.lsb;
	asm volatile("nop");
	while (!(SPSR & _BV(SPIF))) ;
	out.lsb = SPDR;
} else {
	SPDR = in.lsb;
	asm volatile("nop");
	while (!(SPSR & _BV(SPIF))) ;
	out.lsb = SPDR;
	SPDR = in.msb;
	asm volatile("nop");
	while (!(SPSR & _BV(SPIF))) ;
	out.msb = SPDR;
}
return out.val;
}*/

/*
inline void spi_setup_rfid()
{
	SPCR = _BV(SPE) | _BV(MSTR) | 3;
	SPSR = 0;
}*/
