/* 
 * File:   motorsVar.h
 * Author: Saulius
 *
 * Created on Tre?iadienis, 2026, lapkritis 18, 20.24
 */

#ifndef MOTORSVAR_H
#define	MOTORSVAR_H

#include "motors.h"
#include "timers.h"


#ifdef	__cplusplus
extern "C" {
#endif

both_motor_t stepper = {
    .wait_time = 300, //200ms at least according to datasheet
    .enstate = EN_SET,
    .dirstate = DIR_SET,

    .port = &PORTF,
    .enable_pin = PIN1_bm,
    .direction_pin = PIN3_bm,
    
    .callbacks = {
        .init = Stepper_init,
        .enable = Stepper_Enable,
        .disable = Stepper_Enable,
        .dir_forward = Stepper_Set_Dir_F,
        .dir_backward = Stepper_Set_Dir_B,
        .run = Stepper_Run,
        .stop = Stepper_Stop,
        .read_error = Read_Stepper_ALM, //not used but still i keep it
        .read_position = Read_Stepper_PEND, //not used but still i keep it
        .timer_set = RTC_ON, //use ms timer
        .time_checker = RTC_Checker,
        .time_stop = RTC_OFF
    },

    .init = false, //not initialized
    .enabled = false, //not enabled
    .dir_now = true, //dir now and dir last at initialization values should be opposite
    .dir_last = false, 
    .run = false //not running
    
};

both_motor_t linear = {
    .wait_time = 5000, //5ms
    .enstate = EN_SET,
    .dirstate = DIR_SET,
    
    .port = &PORTD,
    .enable_pin = PIN2_bm,
    .direction_pin = PIN5_bm,
    
    .callbacks = {
        .init = Linear_init,
        .enable = Linear_Enable,
        .disable = Linear_Enable,
        .dir_forward = Linear_Set_Dir_F,
        .dir_backward = Linear_Set_Dir_B,              
        .run = Linear_Run,
        .stop = Linear_Stop,
        .read_error = Read_LinearMotor_EF, //not used but still i keep it
        .read_position = NULL, //no position reaching flag function for linear
        .timer_set = TCB1_ON, //use us timer
        .time_checker = TCB1_Checker,
        .time_stop = TCB1_OFF
    },

    .init = false,
    .enabled = false,
    .dir_now = true,
    .dir_last = false,
    .run = false
            
};


#ifdef	__cplusplus
}
#endif

#endif	/* MOTORSVAR_H */

