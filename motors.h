/* 
 * File:   motors.h
 * Author: Saulius
 *
 * Created on Tre?iadienis, 2026, lapkritis 18, 20.23
 */

#ifndef MOTORS_H
#define	MOTORS_H

#ifdef	__cplusplus
extern "C" {
#endif

#define Stepper_Voltage_Compensation_koef 1.05 //59,62V Actual measured voltage and calculated 56,7V    
#define LinearMotor_Voltage_Compensation_koef 0.987755 //24,2 Actual measured voltage and calculated 24,5    
    
    
typedef enum {
	MOTOR_LINEAR = 0,
	MOTOR_STEPPER
} MotorType_t;  

typedef enum {
    SET = 0,
    WAIT,
    DONE            
}MotorState_t;

typedef enum {
    FORWARD = 0,
    BACKWARD
}MotorDirection_t;

typedef enum {
    DISABLING = 0,
    ENABLING,
    STOPING,
    RUN,
    SET_DIRECTION
}MotorAction_t;

typedef enum {
    DISABLED = 0,
    ENABLED,
    STOPED,
    RUNING,
}MotorCondition_t;
    
typedef struct {
    MotorState_t state;
    MotorAction_t action;
    MotorCondition_t condition;
    MotorDirection_t direction;
    MotorDirection_t lastDirection;
    
} motor_init_t;

typedef struct {
	motor_init_t stepper;
    motor_init_t linear;
} motor_t;

extern motor_t motor;

#ifdef	__cplusplus
}
#endif

#endif	/* MOTORS_H */

