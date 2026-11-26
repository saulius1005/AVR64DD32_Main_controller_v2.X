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
    void (*set_dir)(void);    
    void (*run)(void);
    void (*stop)(void);
    bool (*read_error)(void);
    bool (*read_position)(void); //only for stepper
} motor_callbacks_t;

typedef struct {
    uint16_t wait_time;    
    MotorEnablingState_t enstate;
    ChangeDirState_t dirstate;
    
    //PORT_t *port;
    //uint8_t enable_pin;
    //uint8_t direction_pin;

    motor_callbacks_t callbacks;

    bool init;
    bool enabled;  
    bool run;   
    bool dir_now;
    bool dir_last;
    
} both_motor_t;

extern both_motor_t stepper;
extern both_motor_t linear;

void Stepper_init();
void Stepper_Enable();
void Stepper_Disable();
void Stepper_Set_Dir();
void Stepper_Run();
void Stepper_Stop();
bool Read_Stepper_ALM();
bool Read_Stepper_PEND();

void Linear_init();
void Linear_Enable();
void Linear_Disable();
void Linear_Set_Dir();
void Linear_Run();
void Linear_Stop();
bool Read_LinearMotor_EF();


#ifdef	__cplusplus
}
#endif

#endif	/* MOTORS_H */

