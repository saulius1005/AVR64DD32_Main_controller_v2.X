/* 
 * File:   ADCVar.h
 * Author: Saulius
 *
 * Created on Penktadienis, 2026, rugs?jis 18, 16.20
 */

#ifndef ADCVAR_H
#define	ADCVAR_H

#ifdef	__cplusplus
extern "C" {
#endif

analog_sensors_t AnalogSensors = {
    .JX = {
        ADC_MUXPOS_AIN30_gc,//adc channel PC1
        VREF_REFSEL_VDD_gc,//reference
        0, //result
        0, //output for weather station
    },
    .JY = {
        ADC_MUXPOS_AIN29_gc,//adc channel PC2
        VREF_REFSEL_VDD_gc,//reference
        0, //result
        0, //output for weather station
    },
    .SU = {
        ADC_MUXPOS_AIN6_gc, //AMC1311 full range is 2.0V for stepper 2.0V = 71.0V (710k) //PD6
        VREF_REFSEL_2V048_gc,//reference
        0, //result
        0, //output for weather station
    },
    .SI = {
        ADC_MUXPOS_AIN16_gc, // up to 9,21A (if MCU Vcc== 4.096V)| 0A= 0.4096V| 4.096- 0.4096 = 3,6864V/0,4 = 9.21A | max current at normal working conditions for stepper motor is ~6A //PF0
        VREF_REFSEL_4V096_gc,//reference
        0, //result
        0, //output for weather station
    },
    .LU = {
        ADC_MUXPOS_AIN31_gc, //AMC1311 full range is 2.0V for linear motor 2.0V = 30.0V (300k) //PC3
        VREF_REFSEL_2V048_gc,//reference
        0, //result
        0, //output for weather station
    },    
    .LI = {
        ADC_MUXPOS_AIN1_gc, // up to 9,21A (if MCU Vcc== 4.096V)| 0A= 0.4096V| 4.096- 0.4096 = 3,6864V/0,4 = 9.21A | max current at normal working conditions for linear motor is ~4A //PD1
        VREF_REFSEL_4V096_gc,//reference
        0, //result
        0, //output for weather station
    },     
    .MCU = {
        ADC_MUXPOS_VDDDIV10_gc,
        VREF_REFSEL_1V024_gc,
        5000, //as default MCU voltage is 5.00V
        0,
    }
};

analog_t *ADC0_sensors[7] = {
    &AnalogSensors.JX,
    &AnalogSensors.JY,
    &AnalogSensors.SU,
    &AnalogSensors.SI,
    &AnalogSensors.LU,
    &AnalogSensors.LI,
    &AnalogSensors.MCU
};


#ifdef	__cplusplus
}
#endif

#endif	/* ADCVAR_H */

