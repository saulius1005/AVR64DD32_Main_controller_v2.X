#include "settings.h"
#include "timersVar.h"

void TCD0_init_stepper_PWM(uint32_t freq_hz, uint8_t duty_percent) {

	uint16_t cmpbclr = (F_CPU / (32 * freq_hz * 2)) - 1;
	uint16_t cmpaset = (uint16_t)(cmpbclr * (duty_percent / 100.0)) + 1;
	uint16_t cmpbset = cmpbclr - cmpaset - 1;

	TCD0.CMPBCLR = cmpbclr;
	TCD0.CMPBSET = cmpbset;
	TCD0.CMPASET = cmpaset;
	ccp_write_io((uint8_t *) &TCD0.FAULTCTRL, TCD_CMPCEN_bm); // Enable WOC on PF2
	TCD0.CTRLB = TCD_WGMODE_DS_gc;
	ccp_write_io((uint8_t *) &TCD0.CTRLA, TCD_CLKSEL_CLKPER_gc | TCD_CNTPRES_DIV32_gc);

}

void RTC_ON(uint16_t period_ms){
    while (RTC.STATUS > 0);
    RTC.CLKSEL = RTC_CLKSEL_OSC1K_gc;
    RTC.PER = (period_ms * 1.024) + 0.5;  
    RTC.CNT = 0;
    RTC.CTRLA = RTC_RTCEN_bm | RTC_PRESCALER_DIV1_gc;
}

bool RTC_Checker(){
    if (RTC.INTFLAGS & RTC_CMP_bm){
        RTC.INTFLAGS = RTC_CMP_bm;
        return false;
    }
    return true;
}

void RTC_OFF(){
    RTC.CTRLA &= ~RTC_RTCEN_bm;
    while (RTC.STATUS > 0);
}

void TCA0_init_linear_PWM(uint16_t freq_hz, uint8_t duty_percent){
    
    uint8_t period = (F_CPU / 64UL / freq_hz) - 1;
    uint8_t duty = (uint16_t)period * duty_percent / 100;

    PORTMUX.TCAROUTEA = PORTMUX_TCA0_PORTD_gc; //Use alternate route (to PD3)

    TCA0.SPLIT.CTRLD = TCA_SPLIT_SPLITM_bm;

    TCA0.SPLIT.HPER = period;
    TCA0.SPLIT.HCMP0 = duty;

    TCA0.SPLIT.CTRLA = TCA_SPLIT_CLKSEL_DIV64_gc | TCA_SPLIT_ENABLE_bm;
}

void TCB0_init(){
    TCB0.CTRLA = TCB_CLKSEL_DIV2_gc;   
    TCB0.CTRLB = TCB_CNTMODE_INT_gc;
}

void TCB0_OFF(){
    TCB0.CTRLA &= ~TCB_ENABLE_bm;
}

void TCB0_ON(uint16_t period_us){ //from 1us up to 5.46125ms top value is 5461
    uint16_t ticks = ((uint64_t)period_us  * (F_CPU / 2UL)) / 1000000UL; //1us = 24Mhz/2 = 12Mhz/1M = 12 ticks
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

void TCB2_init(){
    TCB2.CTRLA = TCB_CLKSEL_DIV2_gc;   
    TCB2.CTRLB = TCB_CNTMODE_INT_gc;
}

void TCB2_OFF(){
    TCB2.CTRLA &= ~TCB_ENABLE_bm;
}

void TCB2_ON(uint16_t period_us){ //from 1us up to 5.46125ms top value is 5461
    uint16_t ticks = ((uint64_t)period_us  * (F_CPU / 2UL)) / 1000000UL; //1us = 24Mhz/2 = 12Mhz/1M = 12 ticks
    TCB2.CCMP = ticks;
    TCB2.CNT = 0;
    TCB2.CTRLA |=  TCB_ENABLE_bm;
}

bool TCB2_Checker(){
    if (TCB2.INTFLAGS & TCB_CAPT_bm){
        TCB2.INTFLAGS = TCB_CAPT_bm;
        return false;
    }
    return true;
}