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

I2C_Status I2C = {
    .error = 0 ///< Initialize the error state to 0 (no error)
};


#ifdef	__cplusplus
}
#endif

#endif	/* I2CVAR_H */

