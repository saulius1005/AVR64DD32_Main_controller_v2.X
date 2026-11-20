#include "settings.h"
#include "i2cVar.h"

void I2C_recover() {
	// Soft reset for the TWI interface
	TWI0.MCTRLA &= ~TWI_ENABLE_bm;
	TWI0.MCTRLA |= TWI_ENABLE_bm;
	TWI0.MSTATUS = TWI_BUSSTATE_IDLE_gc;
}

void I2C_init() {
	// Configure hold/setup and disable fast mode plus for reliability
	TWI0.CTRLA = TWI_SDAHOLD_50NS_gc | TWI_SDASETUP_4CYC_gc; // safe timings
	TWI0.MBAUD = (uint8_t)TWI_BAUD;
	TWI0.MCTRLA = TWI_ENABLE_bm;
	TWI0.MSTATUS = TWI_BUSSTATE_IDLE_gc;
}

// Transmit address + R/W
uint8_t TransmitAdd(uint8_t addr, uint8_t read) {
	uint8_t error = Error_None;
	uint8_t retries = I2C_RETRY_COUNT;

	do {
		TWI0.MADDR = (addr << 1) | read;
		uint32_t timeout = TWI_TIMEOUT_COUNTER;

		while (!(TWI0.MSTATUS & (TWI_WIF_bm | TWI_RIF_bm))) {
			if (--timeout == 0) {
				error = Error_Timeout;
				break;
			}
		}

		if (TWI0.MSTATUS & TWI_RXACK_bm) error = Error_NACK;
		if (TWI0.MSTATUS & (TWI_ARBLOST_bm | TWI_BUSERR_bm)) error = Error_Bus;

		if (error == Error_None) break;

		// Recovery sequence
		TWI0.MCTRLB = TWI_MCMD_STOP_gc;
		I2C_recover();
	} while (--retries > 0);

	I2C.error = error;
	return error;
}

// Transmit one data byte
uint8_t TransmitByte(uint8_t data) {
	uint8_t error = Error_None;
	uint8_t retries = I2C_RETRY_COUNT;

	do {
		TWI0.MDATA = data;
		uint32_t timeout = TWI_TIMEOUT_COUNTER;

		while (!(TWI0.MSTATUS & TWI_WIF_bm)) {
			if (--timeout == 0) {
				error = Error_Timeout;
				break;
			}
		}

		if (TWI0.MSTATUS & TWI_RXACK_bm) error = Error_NACK;
		if (TWI0.MSTATUS & (TWI_ARBLOST_bm | TWI_BUSERR_bm)) error = Error_Bus;

		if (error == Error_None) break;

		// Recovery on failure
		TWI0.MCTRLB = TWI_MCMD_STOP_gc;
		I2C_recover();
	} while (--retries > 0);

	I2C.error = error;
	return error;
}

// Write data to a register
void WriteToReg(uint8_t addr, uint8_t reg, uint8_t data) {
	uint8_t error = TransmitAdd(addr, WRITE);
	if (!error) {
		error = TransmitByte(reg);
		if (!error) {
			error = TransmitByte(data);
		}
	}

	TWI0.MCTRLB = TWI_MCMD_STOP_gc;

	if (error == Error_Bus || error == Error_Timeout) {
		I2C_recover();
	}

	I2C.error = error;
}

