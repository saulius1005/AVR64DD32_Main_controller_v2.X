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
    
    TCB0_init(); //initiate us timer module (I2c timeout)
    TCB1_init();
    
    screen_init();   
    screen_clear();
    
    TCB1_ON(5000);

    bool testflag = false;
    show_window();    

    while (1) {
         
         if(!TCB1_Checker()){ //run stop every 5ms?
            PORTC.OUTTGL = PIN0_bm;   
            if(testflag){
                testflag = false;
            }
            else{
                testflag = true;                 
            }
         }
    }
}
