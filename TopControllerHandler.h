/* 
 * File:   TopController.h
 * Author: Saulius
 *
 * Created on Ketvirtadienis, 2026, lapkritis 19, 16.07
 */

#ifndef TOPCONTROLLER_H
#define	TOPCONTROLLER_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#define TOP_MESSAGE_LENGTH 17
    
typedef enum {
    TOP_READY = 0,
    TOP_BUSY,
    TOP_CRC_ERROR,
    TOP_TIMEOUT_ERROR
} top_status_t;    

typedef struct {
    top_status_t TopStatus;
	uint16_t EL;
	uint16_t HR_EL;
	uint16_t AZ;
	uint16_t HR_AZ;
	uint16_t PVU;
	uint16_t PVI;
	bool ElMin;
	bool ElMax;
	bool AzMin;
	bool AzMax;
}top_cont_t;

extern top_cont_t sensor_data;

uint16_t fast_atoi_hex(const char *p, uint8_t digits); // char to uint16_t
void ReadTopController(); //reads all sensor data from TopController


#ifdef	__cplusplus
}
#endif

#endif	/* TOPCONTROLLER_H */

