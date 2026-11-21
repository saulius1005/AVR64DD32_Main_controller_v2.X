#include "settings.h"
#include "i2cVar.h"

void I2C_recover() { //at least one old survivor
	TWI0.MCTRLA &= ~TWI_ENABLE_bm;
	TWI0.MCTRLA |= TWI_ENABLE_bm;
	TWI0.MSTATUS = TWI_BUSSTATE_IDLE_gc;
}

void I2C_init() { //it changes a bit
	TWI0.CTRLA = TWI_SDAHOLD_OFF_gc; // sda hold settings for master keep of or make it 50ns?
	TWI0.MBAUD = (uint8_t)TWI_BAUD; //baudrate is set	
	TWI0.MSTATUS = TWI_BUSSTATE_IDLE_gc; //bus state set idle
    TWI0.MCTRLA = TWI_ENABLE_bm; //on i2c
}

bool I2C_address(uint8_t address, I2C_actions_t action){ //after that need to check nack error, ack actions and stop
    
    bool ok_flag = false; 
    
    TWI0.MADDR = (address << 1) | action; //write address and w or r

    TCB1_ON(I2C.wait_tx_time); //set timer to 5ms
    while (TCB1_Checker()) {
        if (TWI0.MSTATUS & (TWI_BUSERR_bm | TWI_ARBLOST_bm)) {
            I2C.error = (TWI0.MSTATUS & TWI_BUSERR_bm) ? I2C_BUS_ERROR : I2C_ARB_LOST;
            TCB1_OFF();
            return 0;
        }
        if (TWI0.MSTATUS & TWI_WIF_bm){
            ok_flag = true;
            break;
        }
    }
    TCB1_OFF();

    if (!ok_flag){ // if timeout
        I2C.error = I2C_TIMEOUT; //set error
        return 0; //return 0
    }

    if (TWI0.MSTATUS & TWI_RXACK_bm){ //if slave is busy or just is mad
        I2C.error = I2C_NACK; //not error just nack status
        return 0;
    }
    
    if (action == I2C_READ) { //if reading
        ok_flag = false; //reset flag
        TCB1_ON(I2C.wait_rx_time); //star timer
        while (TCB1_Checker()) {
            if (TWI0.MSTATUS & (TWI_BUSERR_bm | TWI_ARBLOST_bm)) {
                I2C.error = (TWI0.MSTATUS & TWI_BUSERR_bm) ? I2C_BUS_ERROR : I2C_ARB_LOST;
                TCB1_OFF();
                return 0;
            }
            if (TWI0.MSTATUS & TWI_RIF_bm){ 
                ok_flag = true;
                break;
            }
        }
        TCB1_OFF();
        if (!ok_flag){ // if timeout
            I2C.error = I2C_TIMEOUT; //set error
            return 0; //return 0
        }
        I2C.error = I2C_OK; //if add+read return all ok
        return 1;
    }
    I2C.error = I2C_ACK; //if add+write ok return ACK and 1
    return 1;
}

bool I2C_write(uint8_t data, bool repstart){ //check if possible to write, then check bus state, then send data, then check error: NACK, timeout or all ok and ack
    
    if (I2C.error != I2C_ACK && I2C.error != I2C_OK){ // if previous was reading skip further code
        I2C.error = I2C_BAD_ACTION;
        return 0;
    }
    if ((TWI0.MSTATUS & TWI_BUSSTATE_gm) != TWI_BUSSTATE_OWNER_gc) { //if error in bus (something crash, shorted and so on)
        I2C.error = I2C_BUS_ERROR;
        return 0;
    }
    
    TWI0.MDATA = data; //easiest part- write data   
    bool ok_flag = false;
    
    TCB1_ON(I2C.wait_tx_time); //start timer
    while (TCB1_Checker()) {
        if (TWI0.MSTATUS & (TWI_BUSERR_bm | TWI_ARBLOST_bm)) {
            I2C.error = (TWI0.MSTATUS & TWI_BUSERR_bm) ? I2C_BUS_ERROR : I2C_ARB_LOST;
            TCB1_OFF();
            return 0;
        }
        if (TWI0.MSTATUS & TWI_WIF_bm) {
            ok_flag = true;
            break;
        }
    }
    TCB1_OFF();
    
    if (!ok_flag) { //if timeout
        I2C.error = I2C_TIMEOUT;
        return 0;
    }
    
    if (TWI0.MSTATUS & TWI_RXACK_bm) { //if nack
        I2C.error = I2C_NACK;
        return 0;
    }
    
    I2C.error = I2C_ACK; //if ack- all ok
    
    if(repstart) //if need to read reg (add was send, then write to reg, then repstart, then add+read, ...)
        TWI0.MCTRLB = TWI_MCMD_REPSTART_gc;
    
    return 1;
    
}

bool I2C_read(uint8_t *data, bool ack){
    
    if (I2C.error != I2C_OK){ // if not received ack from add+read
        I2C.error = I2C_BAD_ACTION;
        return 0;
    }
    if ((TWI0.MSTATUS & TWI_BUSSTATE_gm) != TWI_BUSSTATE_OWNER_gc) { //if error in bus (something crash, shorted and so on)
        I2C.error = I2C_BUS_ERROR;
        return 0;
    }
       
    bool ok_flag = false; //prepare flag
    TCB1_ON(I2C.wait_rx_time); //star timer
    while (TCB1_Checker()) {
    if (TWI0.MSTATUS & (TWI_BUSERR_bm | TWI_ARBLOST_bm)) {
        I2C.error = (TWI0.MSTATUS & TWI_BUSERR_bm) ? I2C_BUS_ERROR : I2C_ARB_LOST;
        TCB1_OFF();
        return 0;
    }
    if (TWI0.MSTATUS & TWI_RIF_bm) {
        ok_flag = true;
        break;
    }
}
    TCB1_OFF();
    if (!ok_flag){ // if timeout
        I2C.error = I2C_TIMEOUT; //set error
        return 0; //return 0
    }
    I2C.error = I2C_OK; //if add+read return all ok and received data ack or nack from master should be sended after this function
    
    *data = TWI0.MDATA;
    
    TWI0.MCTRLB = TWI_MCMD_RECVTRANS_gc | (ack ? TWI_ACKACT_ACK_gc : TWI_ACKACT_NACK_gc); //set ack or noack
    
    return 1;

}

bool I2C_stop(){

    if ((TWI0.MSTATUS & TWI_BUSSTATE_gm) != TWI_BUSSTATE_OWNER_gc) { //if bus not owned return
        I2C.error = I2C_BAD_ACTION;
        return 0;
    }

    TWI0.MCTRLB = TWI_MCMD_STOP_gc; //set stop command

    bool ok_flag = false;
    TCB1_ON(I2C.wait_rx_time);

    while (TCB1_Checker()) {
        if (TWI0.MSTATUS & (TWI_BUSERR_bm | TWI_ARBLOST_bm)) {
            I2C.error = (TWI0.MSTATUS & TWI_BUSERR_bm) ? I2C_BUS_ERROR : I2C_ARB_LOST;
            TCB1_OFF();
            return 0;
        }
        if ((TWI0.MSTATUS & TWI_BUSSTATE_gm) == TWI_BUSSTATE_IDLE_gc) { //wait until bus is idle
            ok_flag = true;
            break;
        }
    }

    TCB1_OFF();

    if (!ok_flag) {
        I2C.error = I2C_TIMEOUT;
        return 0;
    }

    I2C.error = I2C_OK;
    return 1;
}



