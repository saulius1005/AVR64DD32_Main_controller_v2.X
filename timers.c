#include "settings.h"

void TCD0_init_stepper_PWM(uint32_t freq_hz, uint8_t duty_percent) {

	// Calculate compare registers
	uint16_t cmpbclr = (F_CPU / (32 * freq_hz * 2)) - 1;
	uint16_t cmpaset = (uint16_t)(cmpbclr * (duty_percent / 100.0)) + 1;
	uint16_t cmpbset = cmpbclr - cmpaset - 1;

	// Set TCD compare registers
	TCD0.CMPBCLR = cmpbclr;
	TCD0.CMPBSET = cmpbset;
	TCD0.CMPASET = cmpaset;

	ccp_write_io((uint8_t *) &TCD0.FAULTCTRL, TCD_CMPCEN_bm); ///< Enable WOC on PF2
	TCD0.CTRLB = TCD_WGMODE_DS_gc; ///< Set waveform mode to double slope
	ccp_write_io((uint8_t *) &TCD0.CTRLA, TCD_CLKSEL_CLKPER_gc | TCD_CNTPRES_DIV32_gc);

}