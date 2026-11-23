/*
 * File:   main.c
 * Author: Saulius
 *
 * Created on Antradienis, 2026, lapkritis 17, 22.47
 */


#include "settings.h"
#include "MCU_Config.h"

int main(void) {
    CLOCK_INHF_clock_init();
    GPIO_init();
    I2C_init();  
    
    stepper.callbacks.init(); //MCU config for stepper
    linear.callbacks.init(); //MCU config for linear
    
    TCB0_init(); //initiate us timer module (I2c timeout)
    TCB1_init();
    
    RTC_ON(500);
    
    screen_init();   
    screen_clear();

    bool on = false;
    show_window();
    
        stepper.action = ENABLE; //once Condition will be the same Enabled 
        linear.action = ENABLE; 
        
        stepper.direction = FORWARD; //set direction once
        linear.direction = FORWARD;         
    
    while (1) {
        
   
        
        Motor_Actions(&stepper);
        if (stepper.motor_init.init_status) //wait until stepper driver enabling free up RTC timer. same here skip if enabled
            Motor_Actions(&linear); 
        
        
         if(!RTC_Checker()){ //run stop every 500ms?
            PORTC.OUTTGL = PIN0_bm;   
            if(on){
                on = false;
                stepper.action = RUN;
                linear.action = RUN;
            }
            else{
                on = true;
                stepper.action = STOP;
                linear.action = STOP;
            }
         }
    }
}
