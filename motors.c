#include "settings.h"
#include "motorsVar.h"


// -------------------------
// Notice
// -------------------------
// for CL86T need swap encoder pair lines 
// -------------------------
// 86HB250-156B MOTOR		CL86T 4.1
// -------------------------
// EA+						EB+			
// EA-						EB-
// EB+						EA+
// EB-						EA-
// -------------------------

void Stepper_init() {
    if(stepper.init)
        return; //skip if initiated already
	TCD0_init_stepper_PWM(25600, 50); // 51.2kHz, 50% duty for one motor shaft spin, so ~20s for gearbox
    PORTF.OUTSET = PIN1_bm; //keep disabled
    stepper.init = true;
}

void Stepper_Run() {
    if(!stepper.enabled) //if disabled do not allow to run
        return;
    if(stepper.run) //if already run skip further code
        return;
    if(TCD0.STATUS & TCD_ENRDY_bm){ //if tcd is ready
        TCD0.CTRLA |= TCD_ENABLE_bm; //enable signal generations
        stepper.run = true; //set flag
    }
}

void Stepper_Stop() {
    if(!stepper.enabled) //if disabled do not allow to stop
        return;
    if(!stepper.run) //if already stopped skip further code
        return;
    if(TCD0.STATUS & TCD_ENRDY_bm){ //if tcd is ready
        TCD0.CTRLA &= ~TCD_ENABLE_bm; //disable signal generations
        stepper.run = false; //set flag
    }
}

bool Read_Stepper_ALM(){ // true if driver has a error (overheat, stepepr stuck and so on)
	return !(PORTF.IN & PIN4_bm);
}

bool Read_Stepper_PEND(){ // true if position reached
	return !(PORTF.IN & PIN5_bm);
}


void Linear_init(){
    if(linear.init)
        return;
	TCA0_init_linear_PWM(20000, 95);
	PORTD.OUTSET = PIN2_bm; //set output disabled for TLE9201SG
    linear.init = true;
};

void Linear_Run() {
    if(!linear.enabled)
        return;
    if(linear.run)
        return;
    TCA0.SPLIT.CTRLB |= TCA_SPLIT_HCMP0EN_bm;
    linear.run = true;
    
}

void Linear_Stop() {
    if(!linear.enabled)
        return;
    if(!linear.run) //skip if stoped
        return;
    TCA0.SPLIT.CTRLB &= ~TCA_SPLIT_HCMP0EN_bm;
    linear.run = false;
}

bool Read_LinearMotor_EF(){ // false if driver has a error (overheat, linear motor stuck and so on)
	return PORTD.IN & PIN4_bm;
}


void Motor_SetEnable(both_motor_t *motor, bool enable){ //for both motors enable and disable function (common)

    if(!motor->init) //if not initialized skip
        return;
    if(motor->enabled == enable) //if it is enable or disable
        return;
    switch(motor->enstate){
        case EN_SET:
            if(enable)
                motor->port->OUTCLR = motor->enable_pin; //enable pin value
            else
                motor->port->OUTSET = motor->enable_pin; //disable

            motor->callbacks.timer_set(motor->wait_time); //set timer 
            motor->enstate = EN_WAIT;
        break;
        case EN_WAIT:
            if(!motor->callbacks.time_checker()){ //after time pass
                motor->callbacks.time_stop(); //stop timer
                motor->enstate = EN_DONE;
            }
        break;
        case EN_DONE:
            motor->enabled = enable; //set value enabled or disabled
            motor->enstate = EN_SET;
        break;
    }
}

void Stepper_Enable(){
    Motor_SetEnable(&stepper, true);
}

void Stepper_Disable(){
    Motor_SetEnable(&stepper, false);
}

void Linear_Enable(){
    Motor_SetEnable(&linear, true);
}

void Linear_Disable(){
    Motor_SetEnable(&linear, false);
}


void Motor_SetDir(both_motor_t *motor, Direction_t dir){
    if(!motor->enabled) //if disabled skip
        return;
    if(motor->run) //not allowing change dir when running
        return;
    if(dir == motor->dir_last) //if direction is the same skip
        return;   
    switch(motor->dirstate){
        case DIR_SET:
            if(dir)
                motor->port->OUTSET = motor->direction_pin;
            else
                motor->port->OUTCLR = motor->direction_pin;

            motor->dirstate = DIR_WAIT;
        break;
        case DIR_WAIT: //some delay
            motor->dirstate = DIR_DONE;
        break;
        case DIR_DONE:
            motor->dir_last = dir;
            motor->dirstate = DIR_SET;
        break;
    }
}

void Stepper_Set_Dir_F(){
    Motor_SetDir(&stepper, FORWARD);
}

void Stepper_Set_Dir_B(){
    Motor_SetDir(&stepper, BACKWARD);
}

void Linear_Set_Dir_F(){
    Motor_SetDir(&linear, FORWARD);
}

void Linear_Set_Dir_B(){
    Motor_SetDir(&linear, BACKWARD);
}