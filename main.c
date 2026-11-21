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
    TCA0_init();
    TCB0_init(); //initiate ms timer module
    TCB1_init(); //initiate us timer module (I2c timeout)
    
    screen_init();   
    screen_clear();
    TCB0_ON(500);  
    screen_write_formatted_text("---------------------", 1, ALIGN_CENTER); //full screen width line
    while (1) {

         if(!TCB0_Checker()) //led toggling every 500ms
            PORTC.OUTTGL = PIN0_bm;   
    }
}
