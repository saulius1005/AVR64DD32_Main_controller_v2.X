#include "settings.h"
#include "timersVar.h"

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

void RTC_ON(uint16_t period_ms){
    while (RTC.STATUS > 0);
    RTC.CLKSEL = RTC_CLKSEL_OSC1K_gc;
    RTC.PER = (period_ms * 1.024) + 0.5;  
    RTC.CNT = 0;
    RTC.CTRLA = RTC_RTCEN_bm | RTC_PRESCALER_DIV1_gc;
}

void RTC_OFF(){
    RTC.CTRLA &= ~RTC_RTCEN_bm;
    while (RTC.STATUS > 0);
}

void TCA0_init(){
 TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_NORMAL_gc;
 TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1024_gc;
}

void TCA0_OFF(){
	TCA0.SINGLE.CTRLA &= ~TCA_SINGLE_ENABLE_bm;
}

void TCA0_ON(uint16_t period_ms){ //up to 2s and 796ms if fcpu is 24Mhz
    TCA0.SINGLE.CTRLA = 0;
    uint16_t ticks = ((uint32_t)period_ms * (F_CPU / 1024UL)) / 1000UL;
    TCA0.SINGLE.PER = ticks;
    TCA0.SINGLE.CNT = 0;
    TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;
}

bool TCA0_Checker(){
    if(TCA0.SINGLE.INTFLAGS & TCA_SINGLE_OVF_bm) {       
        TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
        return true;
    }
    return false;
}