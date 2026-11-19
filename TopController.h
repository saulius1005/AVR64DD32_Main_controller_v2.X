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
	uint16_t Elevation;
	uint16_t HPElevation;
	uint16_t Azimuth;
	uint16_t HPAzimuth;
	uint16_t PVU;
	uint16_t PVI;
	bool ElMin;
	bool ElMax;
	bool AzMin;
	bool AzMax;
}top_cont_t;

extern top_cont_t sensor_data;

#ifdef	__cplusplus
}
#endif

#endif	/* TOPCONTROLLER_H */

