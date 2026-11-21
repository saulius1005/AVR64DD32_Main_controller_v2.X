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
#define TWI_BAUD ((F_CPU / (2 * F_SCL)) - 5)

typedef enum {
    I2C_WRITE = 0,
    I2C_READ
}I2C_actions_t;   

typedef enum {
    I2C_OK = 0, //not error
    I2C_TIMEOUT,
    I2C_ACK, //not error
    I2C_NACK, //not error
    I2C_BAD_ACTION, //if trying write after initiating of the reading (add+read)
    I2C_BUS_ERROR,
    I2C_ARB_LOST
}I2C_errors_t;
    
typedef struct {
    uint16_t wait_tx_time;
    uint16_t wait_rx_time;
    I2C_errors_t error;
} I2C_module_t;

extern I2C_module_t I2C;

void I2C_recover(); //to recover i2c bus state after crash
void I2C_init();
bool I2C_address(uint8_t address, I2C_actions_t action); //send address write or read and return 0 if error, 1- all ok
bool I2C_write(uint8_t data, bool repstart); //write on byte and if neede repeaded start and return 0 if error, 1- all ok
bool I2C_read(uint8_t *data, bool ack); //read one byte with ack or nack return data trough pointer and error as functions above
bool I2C_stop();//same same 0 error 1 ok


#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */

