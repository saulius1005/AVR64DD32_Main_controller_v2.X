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
    DISABLE = 0,
    ENABLE,
    STOP,
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
    uint16_t wait_time;
    bool init_status;
    
} motor_init_t;

typedef struct { //calback for both motors are the same
    void (*init)(void);
    void (*run)(void);
    void (*stop)(void);

    bool (*read_error)(void);
    bool (*read_position)(void); //only for stepper
} motor_callbacks_t;

typedef struct {
    motor_init_t motor_init;
    
    PORT_t *port;
    uint8_t enable_pin;
    uint8_t direction_pin;

    motor_callbacks_t callbacks;

    MotorAction_t action;
    MotorCondition_t condition;
    MotorState_t state;

    MotorDirection_t direction;
    MotorDirection_t lastDirection;

} both_motor_t;

extern both_motor_t stepper;
extern both_motor_t linear;

void Stepper_init();
void Stepper_Run();
void Stepper_Stop();
bool Read_Stepper_ALM();
bool Read_Stepper_PEND();

void Linear_init();
void Linear_Run();
void Linear_Stop();
bool Read_LinearMotor_EF();

void Motor_Actions(both_motor_t *motor); //stepper and linear common control function: enabling, disabling, run, stop, change directions.

#ifdef	__cplusplus
}
#endif

#endif	/* MOTORS_H */

