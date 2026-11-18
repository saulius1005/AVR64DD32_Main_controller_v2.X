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
    
typedef struct {
	bool lastDirection;
	bool alreadyStarted;
	bool alreadyStoped;
	bool alreadyEnabled;
	bool alreadyDisabled;
    bool inAction;
	uint16_t measuredVoltage;
	int16_t measuredCurrent;
	uint8_t stuckCount;
	uint8_t noChangeCount;
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

