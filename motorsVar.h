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
    .motor_init = {
        .state = SET,
        .action = ENABLE,
        .enable_condition = DISABLED,
        .run_condition = STOPED,
        .direction = FORWARD,
        .lastDirection = BACKWARD,
        .init_status = false,
        .wait_time = 300
    },
    .port = &PORTF,
    .enable_pin = PIN1_bm,
    .direction_pin = PIN3_bm,
    .callbacks = {
        .init = Stepper_init,
        .run = Stepper_Run,
        .stop = Stepper_Stop,
        .read_error = Read_Stepper_ALM, //not used but still i keep it
        .read_position = Read_Stepper_PEND //not used but still i keep it
    }
};

both_motor_t linear = {
    .motor_init = {
        .state = SET,
        .action = ENABLE,
        .enable_condition = DISABLED,
        .run_condition = STOPED,
        .direction = FORWARD,
        .lastDirection = BACKWARD,
        .init_status = false,
        .wait_time = 1
    },
    .port = &PORTD,
    .enable_pin = PIN2_bm,
    .direction_pin = PIN5_bm,
    .callbacks = {
        .init = Linear_init,
        .run = Linear_Run,
        .stop = Linear_Stop,
        .read_error = Read_LinearMotor_EF, //not used but still i keep it
        .read_position = NULL //no analog function for linear
    }
};


#ifdef	__cplusplus
}
#endif

#endif	/* MOTORSVAR_H */

