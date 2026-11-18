/*
 * File:   main.c
 * Author: Saulius
 *
 * Created on Antradienis, 2026, lapkritis 17, 22.47
 */


#include "settings.h"
#include "MCU_Config.h"

int main(void) {
    CLOCK_XOSCHF_clock_init();
    GPIO_init();
    while (1) {
    }
}
