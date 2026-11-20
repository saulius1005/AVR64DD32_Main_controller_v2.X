/* 
 * File:   timers.h
 * Author: Saulius
 *
 * Created on Ketvirtadienis, 2026, lapkritis 19, 20.44
 */

#ifndef TIMERS_H
#define	TIMERS_H

#ifdef	__cplusplus
extern "C" {
#endif
    
typedef struct{
    volatile uint16_t top_controller_respond_time;
    volatile uint16_t weather_station_respond_time;
} timer_t;    

extern timer_t timer;

void TCA0_init();

void TCB0_init();
void TCB0_ON(uint16_t period_ms);
void TCB0_OFF();
bool TCB0_Checker();

void TCB1_init(); //
void TCB1_ON(uint16_t period_us);
void TCB1_OFF();
bool TCB1_Checker();

void TCD0_init_stepper_PWM(uint32_t freq_hz, uint8_t duty_percent);

void RTC_ON(uint16_t period_ms);
void RTC_OFF();


#ifdef	__cplusplus
}
#endif

#endif	/* TIMERS_H */

