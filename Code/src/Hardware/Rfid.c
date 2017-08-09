#include "Rfid.h"

#include "Spi.h"
#include "PinDefinition.h"

#include "../Globals.h"

// Version 2.0 (0x92)
// NXP Semiconductors; Rev. 3.8 - 17 September 2014; 16.1.1 self-test
static const uint8_t MFRC522_firmware_referenceV2_0[] PROGMEM = {
	0x00, 0xEB, 0x66, 0xBA, 0x57, 0xBF, 0x23, 0x95,
	0xD0, 0xE3, 0x0D, 0x3D, 0x27, 0x89, 0x5C, 0xDE,
	0x9D, 0x3B, 0xA7, 0x00, 0x21, 0x5B, 0x89, 0x82,
	0x51, 0x3A, 0xEB, 0x02, 0x0C, 0xA5, 0x00, 0x49,
	0x7C, 0x84, 0x4D, 0xB3, 0xCC, 0xD2, 0x1B, 0x81,
	0x5D, 0x48, 0x76, 0xD5, 0x71, 0x61, 0x21, 0xA9,
	0x86, 0x96, 0x83, 0x38, 0xCF, 0x9D, 0x5B, 0x6D,
	0xDC, 0x15, 0xBA, 0x3E, 0x7D, 0x95, 0x3B, 0x2F
};


// Hardware

static void rfid_select(void)
{
	RFID_CS_PORT &= ~(1<<RFID_CS_PIN_NUM);
}
static void rfid_deselect(void)
{
	RFID_CS_PORT |= (1<<RFID_CS_PIN_NUM);
}

static void rfid_reset_high(void)
{
	RFID_RES_PORT |= (1<<RFID_RES_PIN_NUM);
}
static void rfid_reset_low(void)
{
	RFID_RES_PORT &= ~(1<<RFID_RES_PIN_NUM);
}

void rfid_setup_hardware(void)
{
	RFID_CS_DDR |= (1<<RFID_CS_PIN_NUM);
	RFID_RES_DDR |= (1<<RFID_RES_PIN_NUM);

	rfid_deselect();
	rfid_reset_high();
}

// *****************************************************************
//                    Basic interface functions
// *****************************************************************

void rfid_pcd_write_register(uint8_t reg, uint8_t value)
{
	rfid_select(); // Select slave
	spi_send_8(reg & 0x7E);
	spi_send_8(value);
	rfid_deselect(); // Release slave
}

void rfid_pcd_write_register_multiple(uint8_t reg, uint8_t count, uint8_t* values)
{
	rfid_select(); // Select slave
	spi_send_8(reg & 0x7E);
	spi_send(values, count);
	rfid_deselect(); // Release slave
}

uint8_t rfid_pcd_read_register(	uint8_t reg	/*! The register to read from. One of the PCD_Register enums. */ )
{
	uint8_t value;
	rfid_select(); // Select slave

	spi_send_8(0x80 | (reg & 0x7E));	// MSB == 1 is for reading. LSB is not used in address. Datasheet section 8.1.2.3.
	value = spi_transfer_8(0);			// Read the value back. Send 0 to stop reading.

	rfid_deselect(); // Release slave
	return value;
}

/**
 * Reads a number of uint8_ts from the specified register in the MFRC522 chip.
 * The interface is described in the datasheet section 8.1.2.
 */
//*
void rfid_pcd_read_register_multiple(	uint8_t reg,		///< The register to read from. One of the PCD_Register enums.
										uint8_t count,		///< The number of uint8_ts to read
										uint8_t *values,	///< uint8_t array to store the values in.
										uint8_t rxAlign	///< Only bit positions rxAlign..7 in values[0] are updated.
									)
{
	if (count == 0)
	{
		return;
	}

	uint8_t address = 0x80 | (reg & 0x7E);		// MSB == 1 is for reading. LSB is not used in address. Datasheet section 8.1.2.3.
	uint8_t index = 0;							// Index in values array.
	rfid_select(); // Select slave
	count--;								// One read is performed outside of the loop
	spi_send_8(address);					// Tell MFRC522 which address we want to read
	while (index < count)
	{
		if (index == 0 && rxAlign)  		// Only update bit positions rxAlign..7 in values[0]
		{
			// Create bit mask for bit positions rxAlign..7
			uint8_t mask = 0;
			uint8_t i = rxAlign;
			for (; i <= 7; i++)
			{
				mask |= (1 << i);
			}
			// Read value and tell that we want to read the same address again.
			uint8_t value = spi_transfer_8(address);
			// Apply mask to both current value of values[0] and the new data in value.
			values[0] = (values[index] & ~mask) | (value & mask);
		}
		else   // Normal case
		{
			values[index] = spi_transfer_8(address);	// Read value and tell that we want to read the same address again.
		}
		index++;
	}
	values[index] = spi_transfer_8(0);			// Read the final uint8_t. Send 0 to stop reading.
	rfid_deselect(); // Release slave
}
//*/
void rfid_pcd_set_register_bit_mask(uint8_t reg, uint8_t mask)
{
	uint8_t tmp;
	tmp = rfid_pcd_read_register(reg);
	rfid_pcd_write_register(reg, tmp| mask);			// set bit mask
}

void rfid_pcd_clear_register_bit_mask(uint8_t reg, uint8_t mask)
{
	uint8_t tmp;
	tmp = rfid_pcd_read_register(reg);
	rfid_pcd_write_register(reg, tmp & (~mask));		// clear bit mask
}

StatusCode rfid_pcd_calculate_crc(	uint8_t *data,		///< In: Pointer to the data to transfer to the FIFO for CRC calculation.
									uint8_t length,		///< In: The number of uint8_ts to transfer.
									uint8_t *result)	///< Out: Pointer to result buffer. Result is written to result[0..1], low uint8_t first.
{
	rfid_pcd_write_register(CommandReg, PCD_Idle);		// Stop any active command.
	rfid_pcd_write_register(DivIrqReg, 0x04);				// Clear the CRCIRq interrupt request bit
	rfid_pcd_set_register_bit_mask(FIFOLevelReg, 0x80);		// FlushBuffer = 1, FIFO initialization
	rfid_pcd_write_register_multiple(FIFODataReg, length, data);	// Write data to the FIFO
	rfid_pcd_write_register(CommandReg, PCD_CalcCRC);		// Start the calculation

	// Wait for the CRC calculation to complete. Each iteration of the while-loop takes 17.73�s.
	uint16_t i = 5000;
	uint8_t n;
	while (1)
	{
		n = rfid_pcd_read_register(DivIrqReg);	// DivIrqReg[7..0] bits are: Set2 reserved reserved MfinActIRq reserved CRCIRq reserved reserved
		if (n & 0x04)  						// CRCIRq bit set - calculation done
		{
			break;
		}
		if (--i == 0)  						// The emergency break. We will eventually terminate on this one after 89ms. Communication with the MFRC522 might be down.
		{
			return STATUS_TIMEOUT;
		}
	}
	rfid_pcd_write_register(CommandReg, PCD_Idle);		// Stop calculating CRC for new content in the FIFO.

	// Transfer the result from the registers to the result buffer
	result[0] = rfid_pcd_read_register(CRCResultRegL);
	result[1] = rfid_pcd_read_register(CRCResultRegH);
	return STATUS_OK;
}

// *****************************************************************
//             Functions for manipulating the MFRC522
// *****************************************************************

void rfid_init(void)
{
	rfid_deselect(); // No select the slave yet

	// Check if the MFRC522 is in power down mode
	if((RFID_RES_PIN & (1<<RFID_RES_PIN_NUM)) == 0) // yes
	{
		rfid_reset_high();
		_delay_ms(50);
	}
	else
	{
		rfid_pcd_reset();
	}

	// When communicating with a PICC we need a timeout if something goes wrong.
	// f_timer = 13.56 MHz / (2*TPreScaler+1) where TPreScaler = [TPrescaler_Hi:TPrescaler_Lo].
	// TPrescaler_Hi are the four low bits in TModeReg. TPrescaler_Lo is TPrescalerReg.

	rfid_pcd_write_register(TModeReg, 0x80);			// TAuto=1; timer starts automatically at the end of the transmission in all communication modes at all speeds
	rfid_pcd_write_register(TPrescalerReg, 0xA9);		// TPreScaler = TModeReg[3..0]:TPrescalerReg, ie 0x0A9 = 169 => f_timer=40kHz, ie a timer period of 25�s.
	rfid_pcd_write_register(TReloadRegH, 0x03);		// Reload timer with 0x3E8 = 1000, ie 25ms before timeout.
	rfid_pcd_write_register(TReloadRegL, 0xE8);

	rfid_pcd_write_register(TxASKReg, 0x40);		// Default 0x00. Force a 100 % ASK modulation independent of the ModGsPReg register setting
	rfid_pcd_write_register(ModeReg, 0x3D);		// Default 0x3F. Set the preset value for the CRC coprocessor for the CalcCRC command to 0x6363 (ISO 14443-3 part 6.2.4)
	rfid_pcd_antenna_on();						// Enable the antenna driver pins TX1 and TX2 (they were disabled by the reset)
}

void rfid_power_down(void)
{
	rfid_reset_low();
}

uint8_t rfid_is_power_down(void)
{
	return (RFID_RES_PORT & (1<<RFID_RES_PIN_NUM)) ? FALSE : TRUE;
}

void rfid_pcd_reset(void)
{
	rfid_pcd_write_register(CommandReg, PCD_SoftReset);	// Issue the SoftReset command.
	// The datasheet does not mention how long the SoftRest command takes to complete.
	// But the MFRC522 might have been in soft power-down mode (triggered by bit 4 of CommandReg)
	// Section 8.8.2 in the datasheet says the oscillator start-up time is the start up time of the crystal + 37,74�s. Let us be generous: 50ms.
	_delay_ms(50);
	// Wait for the PowerDown bit in CommandReg to be cleared
	while (rfid_pcd_read_register(CommandReg) & (1<<4))
	{
		// PCD still restarting - unlikely after waiting 50ms, but better safe than sorry.
	}
}

void rfid_pcd_antenna_on(void)
{
	uint8_t value = rfid_pcd_read_register(TxControlReg);
	if ((value & 0x03) != 0x03)
	{
		rfid_pcd_write_register(TxControlReg, value | 0x03);
	}
}

void rfid_pcd_antenna_off(void)
{
	rfid_pcd_clear_register_bit_mask(TxControlReg, 0x03);
}

uint8_t rfid_pcd_get_antenna_gain(void)
{
	return rfid_pcd_read_register(RFCfgReg) & (0x07<<4);
}

void rfid_pcd_set_antenna_gain(uint8_t mask)
{
	if (rfid_pcd_get_antenna_gain() != mask)  						// only bother if there is a change
	{
		rfid_pcd_clear_register_bit_mask(RFCfgReg, (0x07<<4));		// clear needed to allow 000 pattern
		rfid_pcd_set_register_bit_mask(RFCfgReg, mask & (0x07<<4));	// only set RxGain[2:0] bits
	}
}

/**
 * Performs a self-test of the MFRC522
 * See 16.1.1 in http://www.nxp.com/documents/data_sheet/MFRC522.pdf
 * 
 * @return Whether or not the test passed. Or false if no firmware reference is available.
 */
uint8_t rfid_pcd_perform_self_test(void)
{
	// This follows directly the steps outlined in 16.1.1
	// 1. Perform a soft reset.
	rfid_pcd_reset();
	
	// 2. Clear the internal buffer by writing 25 bytes of 00h
	uint8_t ZEROES[25] = {0x00};
	rfid_pcd_set_register_bit_mask(FIFOLevelReg, 0x80);	// flush the FIFO buffer
	rfid_pcd_write_register_multiple(FIFODataReg, 25, ZEROES);	// write 25 bytes of 00h to FIFO
	rfid_pcd_write_register(CommandReg, PCD_Mem);		// transfer to internal buffer
	
	// 3. Enable self-test
	rfid_pcd_write_register(AutoTestReg, 0x09);
	
	// 4. Write 00h to FIFO buffer
	rfid_pcd_write_register(FIFODataReg, 0x00);
	
	// 5. Start self-test by issuing the CalcCRC command
	rfid_pcd_write_register(CommandReg, PCD_CalcCRC);
	
	// 6. Wait for self-test to complete
	uint8_t n;
	for (uint8_t i = 0; i < 0xFF; i++) {
		// The datasheet does not specify exact completion condition except
		// that FIFO buffer should contain 64 bytes.
		// While selftest is initiated by CalcCRC command
		// it behaves differently from normal CRC computation,
		// so one can't reliably use DivIrqReg to check for completion.
		// It is reported that some devices does not trigger CRCIRq flag
		// during selftest.
		n = rfid_pcd_read_register(FIFOLevelReg);
		if (n >= 64) {
			break;
		}
	}
	rfid_pcd_write_register(CommandReg, PCD_Idle);		// Stop calculating CRC for new content in the FIFO.
	
	// 7. Read out resulting 64 bytes from the FIFO buffer.
	uint8_t result[64];
	rfid_pcd_read_register_multiple(FIFODataReg, 64, result, 0);
	
	// Auto self-test done
	// Reset AutoTestReg register to be 0 again. Required for normal operation.
	rfid_pcd_write_register(AutoTestReg, 0x00);
	
	const uint8_t *reference = MFRC522_firmware_referenceV2_0;
	
	// Verify that the results match up to our expectations
	for (uint8_t i = 0; i < 64; i++) {
		if (result[i] != pgm_read_byte(&(reference[i]))) {
			return RETURN_ERROR;
		}
	}
	
	// Test passed; all is good.
	return RETURN_SUCCESS;
}
/**
 * Executes the Transceive command.
 * CRC validation can only be done if backData and backLen are specified.
 *
 * @return STATUS_OK on success, STATUS_??? otherwise.
 */
StatusCode rfid_pcd_transceive_data(	uint8_t *sendData,		///< Pointer to the data to transfer to the FIFO.
										uint8_t sendLen,		///< Number of uint8_ts to transfer to the FIFO.
										uint8_t *backData,		///< NULL or pointer to buffer if data should be read back after executing the command.
										uint8_t *backLen,		///< In: Max number of uint8_ts to write to *backData. Out: The number of uint8_ts returned.
										uint8_t *validBits,	///< In/Out: The number of valid bits in the last uint8_t. 0 for 8 valid bits. Default NULL.
										uint8_t rxAlign,		///< In: Defines the bit position in backData[0] for the first bit received. Default 0.
										uint8_t /*bool*/ checkCRC		///< In: True => The last two uint8_ts of the response is assumed to be a CRC_A that must be validated.
								   )
{
	uint8_t waitIRq = 0x30;		// RxIRq and IdleIRq
	return rfid_pcd_communicate_with_PICC(PCD_Transceive, waitIRq, sendData, sendLen, backData, backLen, validBits, rxAlign, checkCRC);
} // End rfid_pcd_transceive_data()

/**
 * Transfers data to the MFRC522 FIFO, executes a command, waits for completion and transfers data back from the FIFO.
 * CRC validation can only be done if backData and backLen are specified.
 *
 * @return STATUS_OK on success, STATUS_??? otherwise.
 */
StatusCode rfid_pcd_communicate_with_PICC(	uint8_t command,		///< The command to execute. One of the PCD_Command enums.
		uint8_t waitIRq,		///< The bits in the ComIrqReg register that signals successful completion of the command.
		uint8_t *sendData,		///< Pointer to the data to transfer to the FIFO.
		uint8_t sendLen,		///< Number of uint8_ts to transfer to the FIFO.
		uint8_t *backData,		///< NULL or pointer to buffer if data should be read back after executing the command.
		uint8_t *backLen,		///< In: Max number of uint8_ts to write to *backData. Out: The number of uint8_ts returned.
		uint8_t *validBits,	///< In/Out: The number of valid bits in the last uint8_t. 0 for 8 valid bits.
		uint8_t rxAlign,		///< In: Defines the bit position in backData[0] for the first bit received. Default 0.
		uint8_t /*bool*/ checkCRC		///< In: True => The last two uint8_ts of the response is assumed to be a CRC_A that must be validated.
										 )
{
	uint8_t n, _validBits = 0;
	unsigned int i;

	// Prepare values for BitFramingReg
	uint8_t txLastBits = validBits ? *validBits : 0;
	uint8_t bitFraming = (rxAlign << 4) + txLastBits;		// RxAlign = BitFramingReg[6..4]. TxLastBits = BitFramingReg[2..0]

	rfid_pcd_write_register(CommandReg, PCD_Idle);			// Stop any active command.
	rfid_pcd_write_register(ComIrqReg, 0x7F);					// Clear all seven interrupt request bits
	rfid_pcd_set_register_bit_mask(FIFOLevelReg, 0x80);			// FlushBuffer = 1, FIFO initialization
	rfid_pcd_write_register_multiple(FIFODataReg, sendLen, sendData);	// Write sendData to the FIFO
	rfid_pcd_write_register(BitFramingReg, bitFraming);		// Bit adjustments
	rfid_pcd_write_register(CommandReg, command);				// Execute the command
	if (command == PCD_Transceive)
	{
		rfid_pcd_set_register_bit_mask(BitFramingReg, 0x80);	// StartSend=1, transmission of data starts
	}

	// Wait for the command to complete.
	// In rfid_pcd_init we set the TAuto flag in TModeReg. This means the timer automatically starts when the PCD stops transmitting.
	// Each iteration of the do-while-loop takes 17.86�s.
	i = 2000;
	while (1)
	{
		n = rfid_pcd_read_register(ComIrqReg);	// ComIrqReg[7..0] bits are: Set1 TxIRq RxIRq IdleIRq HiAlertIRq LoAlertIRq ErrIRq TimerIRq
		if (n & waitIRq)  					// One of the interrupts that signal success has been set.
		{
			break;
		}
		if (n & 0x01)  						// Timer interrupt - nothing received in 25ms
		{
			return STATUS_TIMEOUT;
		}
		if (--i == 0)  						// The emergency break. If all other conditions fail we will eventually terminate on this one after 35.7ms. Communication with the MFRC522 might be down.
		{
			return STATUS_TIMEOUT;
		}
	}

	// Stop now if any errors except collisions were detected.
	uint8_t errorRegValue = rfid_pcd_read_register(ErrorReg); // ErrorReg[7..0] bits are: WrErr TempErr reserved BufferOvfl CollErr CRCErr ParityErr ProtocolErr
	if (errorRegValue & 0x13)  	 // BufferOvfl ParityErr ProtocolErr
	{
		return STATUS_ERROR;
	}

	// If the caller wants data back, get it from the MFRC522.
	if (backData && backLen)
	{
		n = rfid_pcd_read_register(FIFOLevelReg);			// Number of uint8_ts in the FIFO
		if (n > *backLen)
		{
			return STATUS_NO_ROOM;
		}
		*backLen = n;											// Number of uint8_ts returned
		rfid_pcd_read_register_multiple(FIFODataReg, n, backData, rxAlign);	// Get received data from FIFO
		_validBits = rfid_pcd_read_register(ControlReg) & 0x07;		// RxLastBits[2:0] indicates the number of valid bits in the last received uint8_t. If this value is 000b, the whole uint8_t is valid.
		if (validBits)
		{
			*validBits = _validBits;
		}
	}

	// Tell about collisions
	if (errorRegValue & 0x08)  		// CollErr
	{
		return STATUS_COLLISION;
	}

	// Perform CRC_A validation if requested.
	if (backData && backLen && checkCRC)
	{
		// In this case a MIFARE Classic NAK is not OK.
		if (*backLen == 1 && _validBits == 4)
		{
			return STATUS_MIFARE_NACK;
		}
		// We need at least the CRC_A value and all 8 bits of the last uint8_t must be received.
		if (*backLen < 2 || _validBits != 0)
		{
			return STATUS_CRC_WRONG;
		}
		// Verify CRC_A - do our own calculation and store the control in controlBuffer.
		uint8_t controlBuffer[2];
		StatusCode status = rfid_pcd_calculate_crc(&backData[0], *backLen - 2, &controlBuffer[0]);
		if (status != STATUS_OK)
		{
			return status;
		}
		if ((backData[*backLen - 2] != controlBuffer[0]) || (backData[*backLen - 1] != controlBuffer[1]))
		{
			return STATUS_CRC_WRONG;
		}
	}

	return STATUS_OK;
}



/**
 * Transmits a REQuest command, Type A. Invites PICCs in state IDLE to go to READY and prepare for anticollision or selection. 7 bit frame.
 * Beware: When two PICCs are in the field at the same time I often get STATUS_TIMEOUT - probably due do bad antenna design.
 *
 * @return STATUS_OK on success, STATUS_??? otherwise.
 */
StatusCode rfid_PICC_requestA(	uint8_t *bufferATQA,	///< The buffer to store the ATQA (Answer to request) in
								uint8_t *bufferSize	///< Buffer size, at least two uint8_ts. Also number of uint8_ts returned if STATUS_OK.
							 )
{
	uint8_t command = PICC_CMD_REQA;
	uint8_t validBits;
	StatusCode status;

	if (bufferATQA == NULL || *bufferSize < 2)      // The ATQA response is 2 uint8_ts long.
	{
		return STATUS_NO_ROOM;
	}
	rfid_pcd_clear_register_bit_mask(CollReg, 0x80);        // ValuesAfterColl=1 => Bits received after collision are cleared.
	validBits = 7;                                  // For REQA and WUPA we need the short frame format - transmit only 7 bits of the last (and only) uint8_t. TxLastBits = BitFramingReg[2..0]
	status = rfid_pcd_transceive_data(&command, 1, bufferATQA, bufferSize, &validBits, 0, false);
	if (status != STATUS_OK)
	{
		return status;
	}
	if (*bufferSize != 2 || validBits != 0)         // ATQA must be exactly 16 bits.
	{
		return STATUS_ERROR;
	}
	return STATUS_OK;
}

/**
 * Transmits SELECT/ANTICOLLISION commands to select a single PICC.
 * Before calling this function the PICCs must be placed in the READY(*) state by calling PICC_RequestA() or PICC_WakeupA().
 * On success:
 * 		- The chosen PICC is in state ACTIVE(*) and all other PICCs have returned to state IDLE/HALT. (Figure 7 of the ISO/IEC 14443-3 draft.)
 * 		- The UID size and value of the chosen PICC is returned in *uid along with the SAK.
 *
 * A PICC UID consists of 4, 7 or 10 uint8_ts.
 * Only 4 uint8_ts can be specified in a SELECT command, so for the longer UIDs two or three iterations are used:
 * 		UID size	Number of UID uint8_ts		Cascade levels		Example of PICC
 * 		========	===================		==============		===============
 * 		single				 4						1				MIFARE Classic
 * 		double				 7						2				MIFARE Ultralight
 * 		triple				10						3				Not currently in use?
 *
 * @return STATUS_OK on success, STATUS_??? otherwise.
 */
StatusCode rfid_PICC_select(	Uid *uid			///< Pointer to Uid struct. Normally output, but can also be used to supply a known UID.
						   )
{
	uint8_t /*bool*/ uidComplete;
	uint8_t /*bool*/ selectDone;
  //  uint8_t /*bool*/ useCascadeTag;
	uint8_t cascadeLevel = 1;
	StatusCode result;
	uint8_t count;
	uint8_t index;
	uint8_t uidIndex;					// The first index in uid->uiduint8_t[] that is used in the current Cascade Level.
	int8_t currentLevelKnownBits;		// The number of known UID bits in the current Cascade Level.
	uint8_t buffer[9];					// The SELECT/ANTICOLLISION commands uses a 7 uint8_t standard frame + 2 uint8_ts CRC_A
	uint8_t bufferUsed;				// The number of uint8_ts used in the buffer, ie the number of uint8_ts to transfer to the FIFO.
	uint8_t rxAlign;					// Used in BitFramingReg. Defines the bit position for the first bit received.
	uint8_t txLastBits;				// Used in BitFramingReg. The number of valid bits in the last transmitted uint8_t.
	uint8_t *responseBuffer;
	uint8_t responseLength;

	// Description of buffer structure:
	//		uint8_t 0: SEL 				Indicates the Cascade Level: PICC_CMD_SEL_CL1, PICC_CMD_SEL_CL2 or PICC_CMD_SEL_CL3
	//		uint8_t 1: NVB					Number of Valid Bits (in complete command, not just the UID): High nibble: complete uint8_ts, Low nibble: Extra bits.
	//		uint8_t 2: UID-data or CT		See explanation below. CT means Cascade Tag.
	//		uint8_t 3: UID-data
	//		uint8_t 4: UID-data
	//		uint8_t 5: UID-data
	//		uint8_t 6: BCC					Block Check Character - XOR of uint8_ts 2-5
	//		uint8_t 7: CRC_A
	//		uint8_t 8: CRC_A
	// The BCC and CRC_A are only transmitted if we know all the UID bits of the current Cascade Level.
	//
	// Description of uint8_ts 2-5: (Section 6.5.4 of the ISO/IEC 14443-3 draft: UID contents and cascade levels)
	//		UID size	Cascade level	uint8_t2	uint8_t3	uint8_t4	uint8_t5
	//		========	=============	=====	=====	=====	=====
	//		 4 uint8_ts		1			uid0	uid1	uid2	uid3
	//		 7 uint8_ts		1			CT		uid0	uid1	uid2
	//						2			uid3	uid4	uid5	uid6
	//		10 uint8_ts		1			CT		uid0	uid1	uid2
	//						2			CT		uid3	uid4	uid5
	//						3			uid6	uid7	uid8	uid9

	// Prepare MFRC522
	rfid_pcd_clear_register_bit_mask(CollReg, 0x80);		// ValuesAfterColl=1 => Bits received after collision are cleared.

	// Repeat Cascade Level loop until we have a complete UID.
	uidComplete = false;
	while (!uidComplete)
	{
		// Set the Cascade Level in the SEL uint8_t, find out if we need to use the Cascade Tag in uint8_t 2.
		switch (cascadeLevel)
		{
		case 1:
			buffer[0] = PICC_CMD_SEL_CL1;
			uidIndex = 0;
			break;

		case 2:
			buffer[0] = PICC_CMD_SEL_CL2;
			uidIndex = 3;
			break;

		case 3:
			buffer[0] = PICC_CMD_SEL_CL3;
			uidIndex = 6;
			break;

		default:
			return STATUS_INTERNAL_ERROR;
			break;
		}

		currentLevelKnownBits = 0;
		// Copy the known bits from uid->uiduint8_t[] to buffer[]
		index = 2; // destination index in buffer[]
		uint8_t uint8_tsToCopy = 0;

		// Repeat anti collision loop until we can transmit all UID bits + BCC and receive a SAK - max 32 iterations.
		selectDone = false;
		while (!selectDone)
		{
			// Find out how many bits and uint8_ts to send and receive.
			if (currentLevelKnownBits >= 32)   // All UID bits in this Cascade Level are known. This is a SELECT.
			{
				buffer[1] = 0x70; // NVB - Number of Valid Bits: Seven whole uint8_ts
				// Calculate BCC - Block Check Character
				buffer[6] = buffer[2] ^ buffer[3] ^ buffer[4] ^ buffer[5];
				// Calculate CRC_A
				result = rfid_pcd_calculate_crc(buffer, 7, &buffer[7]);
				if (result != STATUS_OK)
				{
					return result;
				}
				txLastBits		= 0; // 0 => All 8 bits are valid.
				bufferUsed		= 9;
				// Store response in the last 3 uint8_ts of buffer (BCC and CRC_A - not needed after tx)
				responseBuffer	= &buffer[6];
				responseLength	= 3;
			}
			else   // This is an ANTICOLLISION.
			{
				//Serial.print(F("ANTICOLLISION: currentLevelKnownBits=")); Serial.println(currentLevelKnownBits, DEC);
				txLastBits		= currentLevelKnownBits % 8;
				count			= currentLevelKnownBits / 8;	// Number of whole uint8_ts in the UID part.
				index			= 2 + count;					// Number of whole uint8_ts: SEL + NVB + UIDs
				buffer[1]		= (index << 4) + txLastBits;	// NVB - Number of Valid Bits
				bufferUsed		= index + (txLastBits ? 1 : 0);
				// Store response in the unused part of buffer
				responseBuffer	= &buffer[index];
				responseLength	= sizeof(buffer) - index;
			}

			// Set bit adjustments
			rxAlign = txLastBits;											// Having a separate variable is overkill. But it makes the next line easier to read.
			rfid_pcd_write_register(BitFramingReg, (rxAlign << 4) + txLastBits);	// RxAlign = BitFramingReg[6..4]. TxLastBits = BitFramingReg[2..0]

			// Transmit the buffer and receive the response.
			result = rfid_pcd_transceive_data(buffer, bufferUsed, responseBuffer, &responseLength, &txLastBits, rxAlign, false);
			if (result == STATUS_COLLISION)   // More than one PICC in the field => collision.
			{
				uint8_t valueOfCollReg = rfid_pcd_read_register(CollReg); // CollReg[7..0] bits are: ValuesAfterColl reserved CollPosNotValid CollPos[4:0]
				if (valueOfCollReg & 0x20)   // CollPosNotValid
				{
					return STATUS_COLLISION; // Without a valid collision position we cannot continue
				}
				uint8_t collisionPos = valueOfCollReg & 0x1F; // Values 0-31, 0 means bit 32.
				if (collisionPos == 0)
				{
					collisionPos = 32;
				}
				if (collisionPos <= currentLevelKnownBits)   // No progress - should not happen
				{
					return STATUS_INTERNAL_ERROR;
				}
				// Choose the PICC with the bit set.
				currentLevelKnownBits = collisionPos;
				count			= (currentLevelKnownBits - 1) % 8; // The bit to modify
				index			= 1 + (currentLevelKnownBits / 8) + (count ? 1 : 0); // First uint8_t is index 0.
				buffer[index]	|= (1 << count);
			}
			else if (result != STATUS_OK)
			{
				return result;
			}
			else   // STATUS_OK
			{
				if (currentLevelKnownBits >= 32)   // This was a SELECT.
				{
					selectDone = true; // No more anticollision
					// We continue below outside the while.
				}
				else   // This was an ANTICOLLISION.
				{
					// We now have all 32 bits of the UID in this Cascade Level
					currentLevelKnownBits = 32;
					// Run loop again to do the SELECT.
				}
			}
		} // End of while (!selectDone)

		// We do not check the CBB - it was constructed by us above.

		// Copy the found UID uint8_ts from buffer[] to uid->uiduint8_t[]
		index			= (buffer[2] == PICC_CMD_CT) ? 3 : 2; // source index in buffer[]
		uint8_tsToCopy		= (buffer[2] == PICC_CMD_CT) ? 3 : 4;
		for (count = 0; count < uint8_tsToCopy; count++)
		{
			uid->uidByte[uidIndex + count] = buffer[index++];
		}

		// Check response SAK (Select Acknowledge)
		if (responseLength != 3 || txLastBits != 0)   // SAK must be exactly 24 bits (1 uint8_t + CRC_A).
		{
			return STATUS_ERROR;
		}
		// Verify CRC_A - do our own calculation and store the control in buffer[2..3] - those uint8_ts are not needed anymore.
		result = rfid_pcd_calculate_crc(responseBuffer, 1, &buffer[2]);
		if (result != STATUS_OK)
		{
			return result;
		}
		if ((buffer[2] != responseBuffer[1]) || (buffer[3] != responseBuffer[2]))
		{
			return STATUS_CRC_WRONG;
		}
		if (responseBuffer[0] & 0x04)   // Cascade bit set - UID not complete yes
		{
			cascadeLevel++;
		}
		else
		{
			uidComplete = true;
			uid->sak = responseBuffer[0];
		}
	} // End of while (!uidComplete)

	// Set correct uid->size
	uid->size = 3 * cascadeLevel + 1;

	return STATUS_OK;
}


/**
 * Instructs a PICC in state ACTIVE(*) to go to state HALT.
 *
 * @return STATUS_OK on success, STATUS_??? otherwise.
 */
StatusCode rfid_PICC_haltA(void)
{
	StatusCode result;
	uint8_t buffer[4];

	// Build command buffer
	buffer[0] = PICC_CMD_HLTA;
	buffer[1] = 0;
	// Calculate CRC_A
	result = rfid_pcd_calculate_crc(buffer, 2, &buffer[2]);
	if (result != STATUS_OK)
	{
		return result;
	}

	// Send the command.
	// The standard says:
	//		If the PICC responds with any modulation during a period of 1 ms after the end of the frame containing the
	//		HLTA command, this response shall be interpreted as 'not acknowledge'.
	// We interpret that this way: Only STATUS_TIMEOUT is a success.
	result = rfid_pcd_transceive_data(buffer, sizeof(buffer), NULL, 0, 0, 0, false);
	if (result == STATUS_TIMEOUT)
	{
		return STATUS_OK;
	}
	if (result == STATUS_OK)   // That is ironically NOT ok in this case ;-)
	{
		return STATUS_ERROR;
	}
	return result;
}


/////////////////////////////////////////////////////////////////////////////////////
// Functions for communicating with MIFARE PICCs
/////////////////////////////////////////////////////////////////////////////////////

/**
 * Executes the MFRC522 MFAuthent command.
 * This command manages MIFARE authentication to enable a secure communication to any MIFARE Mini, MIFARE 1K and MIFARE 4K card.
 * The authentication is described in the MFRC522 datasheet section 10.3.1.9 and http://www.nxp.com/documents/data_sheet/MF1S503x.pdf section 10.1.
 * For use with MIFARE Classic PICCs.
 * The PICC must be selected - ie in state ACTIVE(*) - before calling this function.
 * Remember to call PCD_StopCrypto1() after communicating with the authenticated PICC - otherwise no new communications can start.
 *
 * All keys are set to FFFFFFFFFFFFh at chip delivery.
 *
 * @return STATUS_OK on success, STATUS_??? otherwise. Probably STATUS_TIMEOUT if you supply the wrong key.
 */
StatusCode rfid_pcd_authenticate(uint8_t command,		///< PICC_CMD_MF_AUTH_KEY_A or PICC_CMD_MF_AUTH_KEY_B
								 uint8_t blockAddr, 	///< The block number. See numbering in the comments in the .h file.
								 MIFARE_Key *key,	///< Pointer to the Crypteo1 key to use (6 uint8_ts)
								 Uid *uid			///< Pointer to Uid struct. The first 4 uint8_ts of the UID is used.
								)
{
	uint8_t waitIRq = 0x10;		// IdleIRq

	// Build command buffer
	uint8_t sendData[12];
	sendData[0] = command;
	sendData[1] = blockAddr;
	uint8_t i;
	for (i = 0; i < MF_KEY_SIZE; i++)  	// 6 key uint8_ts
	{
		sendData[2+i] = key->keyByte[i];
	}
	for (i = 0; i < 4; i++)  				// The first 4 uint8_ts of the UID
	{
		sendData[8+i] = uid->uidByte[i];
	}

	// Start the authentication.
	return rfid_pcd_communicate_with_PICC(PCD_MFAuthent, waitIRq, &sendData[0], sizeof(sendData), 0, 0, 0, 0, false);
}

/**
 * Used to exit the PCD from its authenticated state.
 * Remember to call this function after communicating with an authenticated PICC - otherwise no new communications can start.
 */
void rfid_pcd_stopCrypto1(void)
{
	// Clear MFCrypto1On bit
	rfid_pcd_clear_register_bit_mask(Status2Reg, 0x08); // Status2Reg[7..0] bits are: TempSensClear I2CForceHS reserved reserved MFCrypto1On ModemState[2:0]
} // End PCD_StopCrypto1()

/**
 * Reads 16 uint8_ts (+ 2 uint8_ts CRC_A) from the active PICC.
 *
 * For MIFARE Classic the sector containing the block must be authenticated before calling this function.
 *
 * For MIFARE Ultralight only addresses 00h to 0Fh are decoded.
 * The MF0ICU1 returns a NAK for higher addresses.
 * The MF0ICU1 responds to the READ command by sending 16 uint8_ts starting from the page address defined by the command argument.
 * For example; if blockAddr is 03h then pages 03h, 04h, 05h, 06h are returned.
 * A roll-back is implemented: If blockAddr is 0Eh, then the contents of pages 0Eh, 0Fh, 00h and 01h are returned.
 *
 * The buffer must be at least 18 uint8_ts because a CRC_A is also returned.
 * Checks the CRC_A before returning STATUS_OK.
 *
 * @return STATUS_OK on success, STATUS_??? otherwise.
 */
StatusCode rfid_MIFARE_read(	uint8_t blockAddr, 	///< MIFARE Classic: The block (0-0xff) number. MIFARE Ultralight: The first page to return data from.
								uint8_t *buffer,		///< The buffer to store the data in
								uint8_t *bufferSize	///< Buffer size, at least 18 uint8_ts. Also number of uint8_ts returned if STATUS_OK.
						   )
{
	StatusCode result;

	// Sanity check
	if (buffer == NULL || *bufferSize < 18)
	{
		return STATUS_NO_ROOM;
	}

	// Build command buffer
	buffer[0] = PICC_CMD_MF_READ;
	buffer[1] = blockAddr;
	// Calculate CRC_A
	result = rfid_pcd_calculate_crc(buffer, 2, &buffer[2]);
	if (result != STATUS_OK)
	{
		return result;
	}

	// Transmit the buffer and receive the response, validate CRC_A.
	return rfid_pcd_transceive_data(buffer, 4, buffer, bufferSize, NULL, 0, true);
}

/**
 * Writes 16 uint8_ts to the active PICC.
 *
 * For MIFARE Classic the sector containing the block must be authenticated before calling this function.
 *
 * For MIFARE Ultralight the operation is called "COMPATIBILITY WRITE".
 * Even though 16 uint8_ts are transferred to the Ultralight PICC, only the least significant 4 uint8_ts (uint8_ts 0 to 3)
 * are written to the specified address. It is recommended to set the remaining uint8_ts 04h to 0Fh to all logic 0.
 * *
 * @return STATUS_OK on success, STATUS_??? otherwise.
 */
StatusCode rfid_MIFARE_write(	uint8_t blockAddr, ///< MIFARE Classic: The block (0-0xff) number. MIFARE Ultralight: The page (2-15) to write to.
								uint8_t *buffer,	///< The 16 uint8_ts to write to the PICC
								uint8_t bufferSize	///< Buffer size, must be at least 16 uint8_ts. Exactly 16 uint8_ts are written.
							)
{
	StatusCode result;

	// Sanity check
	if (buffer == NULL || bufferSize < 16)
	{
		return STATUS_INVALID;
	}

	// Mifare Classic protocol requires two communications to perform a write.
	// Step 1: Tell the PICC we want to write to block blockAddr.
	uint8_t cmdBuffer[2];
	cmdBuffer[0] = PICC_CMD_MF_WRITE;
	cmdBuffer[1] = blockAddr;
	result = rfid_pcd_MIFARE_transceive(cmdBuffer, 2, false); // Adds CRC_A and checks that the response is MF_ACK.
	if (result != STATUS_OK)
	{
		return result;
	}

	// Step 2: Transfer the data
	result = rfid_pcd_MIFARE_transceive(buffer, bufferSize, false); // Adds CRC_A and checks that the response is MF_ACK.
	if (result != STATUS_OK)
	{
		return result;
	}

	return STATUS_OK;
}

/////////////////////////////////////////////////////////////////////////////////////
// Support functions
/////////////////////////////////////////////////////////////////////////////////////

/**
 * Wrapper for MIFARE protocol communication.
 * Adds CRC_A, executes the Transceive command and checks that the response is MF_ACK or a timeout.
 *
 * @return STATUS_OK on success, STATUS_??? otherwise.
 */
StatusCode rfid_pcd_MIFARE_transceive(	uint8_t *sendData,		///< Pointer to the data to transfer to the FIFO. Do NOT include the CRC_A.
										uint8_t sendLen,		///< Number of uint8_ts in sendData.
										uint8_t /*bool*/ acceptTimeout	///< True => A timeout is also success
									 )
{
	StatusCode result;
	uint8_t cmdBuffer[18]; // We need room for 16 uint8_ts data and 2 uint8_ts CRC_A.

	// Sanity check
	if (sendData == NULL || sendLen > 16)
	{
		return STATUS_INVALID;
	}

	// Copy sendData[] to cmdBuffer[] and add CRC_A
	uint8_t i;
	for(i = 0; i < sendLen; ++i)
	{
		cmdBuffer[i] = sendData[i];
	}
	result = rfid_pcd_calculate_crc(cmdBuffer, sendLen, &cmdBuffer[sendLen]);
	if (result != STATUS_OK)
	{
		return result;
	}
	sendLen += 2;

	// Transceive the data, store the reply in cmdBuffer[]
	uint8_t waitIRq = 0x30;		// RxIRq and IdleIRq
	uint8_t cmdBufferSize = sizeof(cmdBuffer);
	uint8_t validBits = 0;
	result = rfid_pcd_communicate_with_PICC(PCD_Transceive, waitIRq, cmdBuffer, sendLen, cmdBuffer, &cmdBufferSize, &validBits, 0, false);
	if (acceptTimeout && result == STATUS_TIMEOUT)
	{
		return STATUS_OK;
	}
	if (result != STATUS_OK)
	{
		return result;
	}
	// The PICC must reply with a 4 bit ACK
	if (cmdBufferSize != 1 || validBits != 4)
	{
		return STATUS_ERROR;
	}
	if (cmdBuffer[0] != MF_ACK)
	{
		return STATUS_MIFARE_NACK;
	}
	return STATUS_OK;
}

/////////////////////////////////////////////////////////////////////////////////////
// Convenience functions - does not add extra functionality
/////////////////////////////////////////////////////////////////////////////////////

/**
 * Returns true if a PICC responds to PICC_CMD_REQA.
 * Only "new" cards in state IDLE are invited. Sleeping cards in state HALT are ignored.
 *
 * @return bool
 */
uint8_t /*bool*/ rfid_PICC_IsNewCardPresent(void)
{
	uint8_t bufferATQA[2];
	uint8_t bufferSize = sizeof(bufferATQA);
	StatusCode result = rfid_PICC_requestA(bufferATQA, &bufferSize);
	return (result == STATUS_OK || result == STATUS_COLLISION);
} // End PICC_IsNewCardPresent()

/**
 * Simple wrapper around PICC_Select.
 * Returns true if a UID could be read.
 * Remember to call PICC_IsNewCardPresent(), PICC_RequestA() or PICC_WakeupA() first.
 * The read UID is available in the class variable uid.
 *
 * @return bool
 */
uint8_t /*bool*/ rfid_PICC_ReadCardSerial(void)
{
	StatusCode result = rfid_PICC_select(&rfid_uid);
	return (result == STATUS_OK);
} // End
