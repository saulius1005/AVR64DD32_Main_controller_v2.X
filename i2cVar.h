/* 
 * File:   i2cVar.h
 * Author: Saulius
 *
 * Created on Penktadienis, 2026, lapkritis 20, 19.27
 */

#ifndef I2CVAR_H
#define	I2CVAR_H

#ifdef	__cplusplus
extern "C" {
#endif

I2C_module_t I2C = {
    .wait_tx_time = 5000, //wait at max 5ms
    .wait_rx_time = 5000
};


#ifdef	__cplusplus
}
#endif

#endif	/* I2CVAR_H */

