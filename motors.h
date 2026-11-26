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
    
typedef enum { //not used for now
	MOTOR_LINEAR = 0,
	MOTOR_STEPPER
} MotorType_t;  

typedef enum {
    FORWARD = 0,
    BACKWARD
} Direction_t;

typedef enum {
    EN_SET = 0,
    EN_WAIT,
    EN_DONE            
}MotorEnablingState_t;

typedef enum {
    DIR_SET = 0,
    DIR_WAIT,
    DIR_DONE            
}ChangeDirState_t;

typedef struct { //calback for both motors are the same
    void (*init)(void);
    void (*enable)(void);
    void (*disable)(void);
    void (*dir_forward)(void);  
    void (*dir_backward)(void);
    void (*run)(void);
    void (*stop)(void);
    bool (*read_error)(void);
    bool (*read_position)(void); //only for stepper
    void (*timer_set)(uint16_t);
    bool (*time_checker) (void);
    void (*time_stop) (void);
} motor_callbacks_t;

typedef struct {
    uint16_t wait_time;    
    MotorEnablingState_t enstate;
    ChangeDirState_t dirstate;
    
    PORT_t *port;
    uint8_t enable_pin;
    uint8_t direction_pin;

    motor_callbacks_t callbacks;

    bool init;
    bool enabled;  
    bool run;   
    Direction_t dir_last;
    
} both_motor_t;

extern both_motor_t stepper;
extern both_motor_t linear;

void Stepper_init();
void Stepper_Enable();
void Stepper_Set_Dir_F();
void Stepper_Set_Dir_B();
void Stepper_Run();
void Stepper_Stop();
bool Read_Stepper_ALM();
bool Read_Stepper_PEND();

void Linear_init();
void Linear_Enable();
void Linear_Set_Dir_F();
void Linear_Set_Dir_B();
void Linear_Run();
void Linear_Stop();
bool Read_LinearMotor_EF();


#ifdef	__cplusplus
}
#endif

#endif	/* MOTORS_H */

