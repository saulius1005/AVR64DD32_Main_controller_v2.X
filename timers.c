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

void TCA0_init(){ //only for tcb counters to work
 TCA0.SINGLE.CTRLA = 0;    
 TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_NORMAL_gc;
 TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1024_gc | TCA_SINGLE_ENABLE_bm;
}


void TCB0_init(){
    TCB0.CTRLA = TCB_CLKSEL_TCA0_gc;       // tca0 as source    
    TCB0.CTRLB = TCB_CNTMODE_INT_gc;       // Periodic Interrupt mode
}

void TCB0_OFF(){
    TCB0.CTRLA &= ~TCB_ENABLE_bm;
}

void TCB0_ON(uint16_t period_ms){ //from 1ms up to 2.796s top value is 2796
    uint16_t ticks = ((uint32_t)period_ms  * (F_CPU / 1024)) / 1000UL; //1ms = 24Mhz/1024 = 23.4375kHz / 1k = 23 ticks (not nice but still ok actually it is 23.4375)
    TCB0.CCMP = ticks;
    TCB0.CNT = 0;
    TCB0.CTRLA |=  TCB_ENABLE_bm;
}

bool TCB0_Checker(){
    if (TCB0.INTFLAGS & TCB_CAPT_bm){
        TCB0.INTFLAGS = TCB_CAPT_bm;
        return false;
    }
    return true;
}


void TCB1_init(){
    TCB1.CTRLA = TCB_CLKSEL_DIV2_gc;   
    TCB1.CTRLB = TCB_CNTMODE_INT_gc;
}

void TCB1_OFF(){
    TCB1.CTRLA &= ~TCB_ENABLE_bm;
}

void TCB1_ON(uint16_t period_us){ //from 1us up to 5.46125ms top value is 5461
    uint16_t ticks = ((uint64_t)period_us  * (F_CPU / 2UL)) / 1000000UL; //1us = 24Mhz/2 = 12Mhz/1M = 12 ticks
    TCB1.CCMP = ticks;
    TCB1.CNT = 0;
    TCB1.CTRLA |=  TCB_ENABLE_bm;
}

bool TCB1_Checker(){
    if (TCB1.INTFLAGS & TCB_CAPT_bm){
        TCB1.INTFLAGS = TCB_CAPT_bm;
        return false;
    }
    return true;
}