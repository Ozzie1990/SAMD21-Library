/*
 * pm.h
 *
 * Created: 8/27/2017 4:25:53 PM
 *  Author: ozzie
 */ 


#ifndef PM_H_
#define PM_H_

#include "samd21.h"

#define PM_SERCOM3 5

typedef union {
	struct {
		uint32_t PMPAC2:	1;
		uint32_t PMEVSYS:	1;
		uint32_t PMSERCOM0:	1;
		uint32_t PMSERCOM1:	1;
		uint32_t PMSERCOM2:	1;
		uint32_t PMSERCOM3:	1;
		uint32_t PMSERCOM4:	1;
		uint32_t PMSERCOM5:	1;
		
		uint32_t PMTCC0:	1;
		uint32_t PMTCC1:	1;
		uint32_t PMTCC2:	1;
		uint32_t PMTC3:		1;
		uint32_t PMTC4:		1;
		uint32_t PMTC5:		1;
		uint32_t PMTC6:		1;
		uint32_t PMTC7:		1;
		
		uint32_t PMADC:		1;
		uint32_t PMAC:		1;
		uint32_t PMDAC:		1;
		uint32_t PMPTC:		1;
		uint32_t PMI2C:		1;
		uint32_t:			3;
		
		uint32_t:			8;
	}bits;
	uint32_t reg;
} PM_AHBCMASK;



typedef struct {
	_REG uint8_t		CNTRL;
	_REG uint8_t		SLEEP;
	_REG uint32_t		reserved0;
	_REG uint16_t		reserved1;
	_REG uint8_t		CPUSEL;
	_REG uint8_t		APBASEL;
	_REG uint8_t		APBBSEL;
	_REG uint8_t		APBCSEL;
	_REG uint32_t		reserved2;
	_REG uint32_t		AHBMASK;
	_REG uint32_t		AHBAMASK;
	_REG uint32_t		AHBBMASK;
	_REG PM_AHBCMASK	AHBCMASK;
	_REG uint32_t		reserved4;
	_REG uint32_t		reserved5;
}PM_REG_BASE;

#define PM_REG ((PM_REG_BASE *) 0x40000400)

typedef struct pm_reg {
	volatile uint8_t CNTRL;
	volatile uint8_t SLEEP;
	volatile uint32_t reserved0;
	volatile uint16_t reserved1;
	volatile uint8_t CPUSEL;
	volatile uint8_t APBASEL;
	volatile uint8_t APBBSEL;
	volatile uint8_t APBCSEL;
	volatile uint32_t reserved2;
	//volatile uint32_t reserved3;
	volatile uint32_t AHBMASK;
	volatile uint32_t AHBAMASK;
	volatile uint32_t AHBBMASK;
	volatile uint32_t AHBCMASK;
	volatile uint32_t reserved4;
	volatile uint32_t reserved5;
}pm_reg_t;

#define PM_OFFSET ((pm_reg_t *) 0x40000400)
#define PM_OFFSET_TEST ((pm_reg_t *) 0x40000420)

void pm_set_maskc(int pin);


#endif /* PM_H_ */
