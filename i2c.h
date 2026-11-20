/* 
 * File:   i2c.h
 * Author: Saulius
 *
 * Created on Penktadienis, 2026, lapkritis 20, 19.27
 */

#ifndef I2C_H
#define	I2C_H

#ifdef	__cplusplus
extern "C" {
#endif

#define F_SCL 400000UL 
#define TWI_TIMEOUT_COUNTER 5000 ///< Timeout counter value for operations
#define I2C_RETRY_COUNT 3
#define TWI_BAUD ((F_CPU / (2 * F_SCL)) - 5)
    
#define WRITE 0 ///< Write operation flag
#define READ 1 ///< Read operation flag
#define STOP 1 ///< Stop condition flag

#define Error_None    0
#define Error_Timeout 1
#define Error_NACK    2
#define Error_Bus     3

typedef struct {
    volatile uint8_t error; ///< Error code for I2C operations
} I2C_Status;

extern I2C_Status I2C; ///< Global I2C status instance


#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */

