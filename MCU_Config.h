/* 
 * File:   MCU_Config.h
 * Author: Saulius
 *
 * Created on Antradienis, 2026, lapkritis 17, 22.49
 */

#ifndef MCU_CONFIG_H
#define	MCU_CONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif

FUSES = {
	.WDTCFG = 0x00, // WDTCFG {PERIOD=OFF, WINDOW=OFF}
	.BODCFG = 0x00, // BODCFG {SLEEP=DISABLE, ACTIVE=DISABLE, SAMPFREQ=128Hz, LVL=BODLEVEL0}
	.OSCCFG = 0x00, // OSCCFG {CLKSEL=OSCHF}
	.SYSCFG0 = 0xD0, // SYSCFG0 {EESAVE=CLEAR, RSTPINCFG=GPIO, UPDIPINCFG=UPDI, CRCSEL=CRC16, CRCSRC=NOCRC}
	.SYSCFG1 = 0x17, // SYSCFG1 {SUT=64MS, MVSYSCFG=SINGLE}
	.CODESIZE = 0x00, // CODESIZE {CODESIZE=User range:  0x0 - 0xFF}
	.BOOTSIZE = 0x00, // BOOTSIZE {BOOTSIZE=User range:  0x0 - 0xFF}
};

LOCKBITS = 0x5CC5C55C; // {KEY=NOLOCK}


#ifdef	__cplusplus
}
#endif

#endif	/* MCU_CONFIG_H */

