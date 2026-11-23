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
	TCD0_init_stepper_PWM(25600, 50); // 51.2kHz, 50% duty for one motor shaft spin, so ~20s for gearbox
    PORTF.OUTSET = PIN1_bm; //keep disabled
}

void Linear_init(){
	TCA0_init_linear_PWM(20000, 95);
	PORTD.OUTSET = PIN2_bm; //set output disabled for TLE9201SG
};

bool Read_Stepper_ALM(){ // true if driver has a error (overheat, stepepr stuck and so on)
	return !(PORTF.IN & PIN4_bm);
}

bool Read_LinearMotor_EF(){ // false if driver has a error (overheat, linear motor stuck and so on)
	return PORTD.IN & PIN4_bm;
}

bool Read_Stepper_PEND(){ // true if position reached
	return !(PORTF.IN & PIN5_bm);
}

void Stepper_Run() {
    while (!(TCD0.STATUS & TCD_ENRDY_bm));
    TCD0.CTRLA |= TCD_ENABLE_bm;
}

void Stepper_Stop() {
    while (!(TCD0.STATUS & TCD_ENRDY_bm));
    TCD0.CTRLA &= ~TCD_ENABLE_bm;
}

void Linear_Run() {
    TCA0.SPLIT.CTRLB |= TCA_SPLIT_HCMP0EN_bm;
}

void Linear_Stop() {
    TCA0.SPLIT.CTRLB &= ~TCA_SPLIT_HCMP0EN_bm;
}


void Motor_Actions(both_motor_t *motor) {

    switch(motor->action) {
        case ENABLE:
        case DISABLE:
            if (motor->condition != (motor->action == ENABLE ? DISABLED : ENABLED))//if condition is not the same as action if enabled != enabling if disabled != disabling
                break;//if so condition and action is the same skip further code
            switch(motor->state) {
                case SET:
                    if (motor->action == ENABLE)
                        motor->port->OUTCLR  = motor->enable_pin;
                    else
                        motor->port->OUTSET  = motor->enable_pin;

                    RTC_ON(motor->motor_init.wait_time);//set timer for stepper 300 (according to driver datasheet 200 is minimum), for linear 1ms (longest time is wake up, but here no such thing, only using SPI interface)
                    motor->state = WAIT;
                break;

                case WAIT:
                    if (!RTC_Checker()) {
                        RTC_OFF();
                        motor->state = DONE;
                    }
                break;

                case DONE:
                    motor->condition = (motor->action == ENABLE) ? ENABLED : DISABLED; //set new condition
                    motor->state = SET; //reset for further actions
                    motor->motor_init.init_status = true; //motor complete initialized and RTC timer now is free
                break;
            }
        break;

        case RUN:
        case STOP:
            if (motor->condition != (motor->action == RUN ? STOPED : RUNING)) //identical to enabling/ disabling
                break;

            switch(motor->state) {
                case SET:
                    if (motor->action == RUN)
                        motor->callbacks.run();
                    else
                        motor->callbacks.stop();

                    motor->state = WAIT;
                break;

                case WAIT:
                    motor->state = DONE;
                break;

                case DONE:
                    motor->condition = (motor->action == RUN) ? RUNING : STOPED;
                    motor->state = SET;
                break;
            }
        break;

        case SET_DIRECTION:
            if (motor->direction != motor->lastDirection) { //only if new direction is not the same as old direction
                switch(motor->state) {
                    case SET:
                        if(motor->direction == FORWARD) //if new dir is forward
                            motor->port->OUTSET = motor->direction_pin; 
                        else //backward
                            motor->port->OUTCLR = motor->direction_pin;
                        motor->lastDirection = motor->direction; //update old direction value
                        motor->state = WAIT;
                    break;

                    case WAIT:
                        motor->state = DONE;
                    break;

                    case DONE:
                        motor->state = SET;
                    break;
                }
            }
        break;
    }
}

/*
void stepper_actions(){ //azimuth
    switch(motor.stepper.action){
        case ENABLING:
        case DISABLING:
            if (motor.stepper.condition != (motor.stepper.action == ENABLING ? DISABLED : ENABLED))//if condition is not the same as action if enabled != enabling if disabled != disabling
                break; //if so condition and action is the same skip further code

            switch(motor.stepper.state) {
                case SET:
                    if (motor.stepper.action == ENABLING)
                        PORTF.OUTCLR = PIN1_bm;
                    else
                        PORTF.OUTSET = PIN1_bm;
                    RTC_ON(300); //set timer to 300ms according to stepper driver datasheet
                    motor.stepper.state = WAIT;
                break;

                case WAIT:
                    if (!RTC_Checker()) { // after wait 
                        RTC_OFF();
                        motor.stepper.state = DONE;
                    }
                break;

                case DONE:
                    motor.stepper.state = SET; //reseting state for further actions
                    motor.stepper.condition = (motor.stepper.action == ENABLING) ? ENABLED : DISABLED; //set motor condition
                break;
            }
        break;
        
        case RUN:
        case STOPING:
            if (motor.stepper.condition != (motor.stepper.action == RUN ? STOPED : RUNING)) // same sasme as before
                break;

            switch(motor.stepper.state) {
                case SET:
                    if (TCD0.STATUS & TCD_ENRDY_bm) {
                        if (motor.stepper.action == RUN)
                            TCD0.CTRLA |= TCD_ENABLE_bm;
                        else
                            TCD0.CTRLA &= ~TCD_ENABLE_bm;

                        motor.stepper.state = WAIT;
                    }
                break;
                
                case WAIT:
                    motor.stepper.state = DONE;
                break;

                case DONE:
                    motor.stepper.condition = (motor.stepper.action == RUN) ? RUNING : STOPED;                    
                    motor.stepper.state = SET;
                break;
            }
        break;   

        case SET_DIRECTION:
            if(motor.stepper.direction != motor.stepper.lastDirection){ //only if new direction is not the same as before
                switch(motor.stepper.state){
                    case SET:
                        if(motor.stepper.direction == FORWARD){
                            PORTF.OUTSET = PIN3_bm;
                            motor.stepper.lastDirection = FORWARD;
                        }
                        else{
                            PORTF.OUTCLR = PIN3_bm;
                            motor.stepper.lastDirection = BACKWARD;
                        }
                        motor.stepper.state = WAIT;
                    break;
                    
                    case WAIT: // for 3us not worth to use timer (after all state steps time delay will be enough)
                        motor.stepper.state = DONE;
                    break; 
                    
                    case DONE:
                        motor.stepper.state = SET;
                    break;   
                }
            }           
        break;
    } 
}

void linear_actions(){ //elevation
    switch(motor.linear.action){
        case ENABLING:
        case DISABLING:
            if (motor.linear.condition != (motor.linear.action == ENABLING ? DISABLED : ENABLED))//if condition is not the same as action if enabled != enabling if disabled != disabling
                break; //if so condition and action is the same skip further code

            switch(motor.linear.state) {
                case SET:
                    if (motor.linear.action == ENABLING)
                        PORTD.OUTCLR = PIN2_bm;
                    else
                        PORTD.OUTSET = PIN2_bm;
                    RTC_ON(1); //for easier further optimization
                    motor.linear.state = WAIT;
                break;

                case WAIT: //not waste timer for 5us
                    if (!RTC_Checker()) {
                        motor.linear.state = DONE;
                        RTC_OFF();
                    }
                break;

                case DONE:
                    motor.linear.condition = (motor.linear.action == ENABLING) ? ENABLED : DISABLED; //set motor condition                    
                    motor.linear.state = SET; //reseting state for further actions
                break;
            }
        break;
        
        case RUN:
        case STOPING:
            if (motor.linear.condition != (motor.linear.action == RUN ? STOPED : RUNING)) // same sasme as before
                break;

            switch(motor.linear.state) {
                case SET:
                    if (motor.linear.action == RUN)
                        TCA0.SPLIT.CTRLB |= TCA_SPLIT_HCMP0EN_bm;
                    else
                        TCA0.SPLIT.CTRLB &= ~TCA_SPLIT_HCMP0EN_bm;

                    motor.linear.state = WAIT;
                break;
                
                case WAIT: //not used but for compilator to be happy
                    motor.linear.state = DONE;
                break;

                case DONE:
                    motor.linear.state = SET;
                    motor.linear.condition = (motor.linear.action == RUN) ? RUNING : STOPED;
                break;
            }
        break;   

        case SET_DIRECTION:
            if(motor.linear.direction != motor.linear.lastDirection){ //only if new direction is not the same as before
                switch(motor.linear.state){
                    case SET:
                        if(motor.linear.direction == FORWARD){
                            PORTD.OUTSET = PIN5_bm;
                            motor.linear.lastDirection = FORWARD; //up in this case
                        }
                        else{
                            PORTD.OUTCLR = PIN5_bm;
                            motor.linear.lastDirection = BACKWARD; //down in this case
                        }
                        motor.linear.state = WAIT;
                    break;
                    
                    case WAIT:
                            motor.linear.state = DONE;
                    break; 
                    
                    case DONE:
                        motor.linear.state = SET;
                    break;   
                }
            }           
        break;
    } 
}
 * */