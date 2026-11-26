/* 
 * File:   motorsVar.h
 * Author: Saulius
 *
 * Created on Tre?iadienis, 2026, lapkritis 18, 20.24
 */

#ifndef MOTORSVAR_H
#define	MOTORSVAR_H

#include "motors.h"


#ifdef	__cplusplus
extern "C" {
#endif

both_motor_t stepper = {
    .wait_time = 300,
    .enstate = EN_SET,
    .dirstate = DIR_SET,

    //.port = &PORTF,
    //.enable_pin = PIN1_bm,
    //.direction_pin = PIN3_bm,
    
    .callbacks = {
        .init = Stepper_init,
        .enable = Stepper_Enable,
        .disable = Stepper_Disable,
        .set_dir = Stepper_Set_Dir,
        .run = Stepper_Run,
        .stop = Stepper_Stop,
        .read_error = Read_Stepper_ALM, //not used but still i keep it
        .read_position = Read_Stepper_PEND //not used but still i keep it
    },

    .init = false,
    .enabled = false,
    .dir_now = true,
    .dir_last = false,
    .run = false
    
};

both_motor_t linear = {
    .wait_time = 1,
    .enstate = EN_SET,
    .dirstate = DIR_SET,
    
    //.port = &PORTD,
    //.enable_pin = PIN2_bm,
    //.direction_pin = PIN5_bm,
    
    .callbacks = {
        .init = Linear_init,
        .enable = Linear_Enable,
        .disable = Linear_Disable,
        .set_dir = Linear_Set_Dir,              
        .run = Linear_Run,
        .stop = Linear_Stop,
        .read_error = Read_LinearMotor_EF, //not used but still i keep it
        .read_position = NULL //no analog function for linear
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

