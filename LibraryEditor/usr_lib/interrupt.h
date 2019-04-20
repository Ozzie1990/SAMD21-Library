/*
 * interrupt.h
 *
 * Created: 8/30/2017 8:56:43 PM
 *  Author: ozzie
 */ 


#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#include "stdint-gcc.h"
#include "samd21.h"

//Need to set to 1 to enable interrupt
typedef union {
	struct {
		uint32_t PM:		1;
		uint32_t SYSCTRL:	1;
		uint32_t WDT:	1;
		uint32_t RTC:	1;
		uint32_t EIC:	1;
		uint32_t NVMCTRL:	1;
		uint32_t DMAC:	1;
		uint32_t USB:	1;
		uint32_t EVSYS:	1;
		uint32_t SERCOM0:	1;
		uint32_t SERCOM1:	1;
		uint32_t SERCOM2:	1;
		uint32_t SERCOM3:	1;
		uint32_t SERCOM4:	1;
		uint32_t SERCOM5:	1;
		uint32_t TCC0:	1;
		uint32_t TCC1:	1;
		uint32_t TCC2:	1;
		uint32_t TC3:	1;
		uint32_t TC4:	1;
		uint32_t TC5:	1;
		uint32_t TC6:	1;
		uint32_t TC7:	1;
		uint32_t ADC:	1;
		uint32_t AC:	1;
		uint32_t DAC:	1;
		uint32_t PTC:	1;
		uint32_t I2S:	1;
		uint32_t bit28:	1;
		uint32_t bit29:	1;
		uint32_t bit30:	1;
		uint32_t bit31:	1;
	} bits;
	uint32_t reg;
} INTERRUPT;

//User ISER_SAMD21 to set interrupt bit
typedef struct {
	_REG INTERRUPT ISER_SAMD21;
	_REG INTERRUPT ICER_SAMD21;
	_REG INTERRUPT ISPR_SAMD21;
	_REG INTERRUPT ICPR_SAMD21;
}INT_SAMD21;

#define INTERRUPT_SAMD21 ((INT_SAMD21 *) 0xE000E100)

/*typedef struct {
	_REG uint32_t PM;
	_REG uint32_t SYSCTRL;
	_REG uint32_t WDT;
	_REG uint32_t RTC;
	_REG uint32_t EIC;
	_REG uint32_t NVMCTRL;
	_REG uint32_t DMAC;
	_REG uint32_t USB;
	_REG uint32_t EVSYS;
	_REG uint32_t SERCOM0;
	_REG uint32_t SERCOM1;
	_REG uint32_t SERCOM2;
	_REG uint32_t SERCOM3;
	_REG uint32_t SERCOM4;
	_REG uint32_t SERCOM5;
	_REG uint32_t TCC0;
	_REG uint32_t TCC1;
	_REG uint32_t TCC2;
	_REG uint32_t TC3;
	_REG uint32_t TC4;
	_REG uint32_t TC5;
	_REG uint32_t TC6;
	_REG uint32_t TC7;
	_REG uint32_t ADC;
	_REG uint32_t AC;
	_REG uint32_t DAC;
	_REG uint32_t PTC;
	_REG uint32_t I2S;
}INT_TABLE;*/

#define INT_VEC ((INT_TABLE *) 0x20000050)

typedef struct {
	_REG uint32_t t;
}placeholder;

#define INT_MASK ((placeholder *) 0x200000A4)
#define INT_EN	 ((placeholder *) 0x200000A5)


void SERCOM3_Handler();


#endif /* INTERRUPT_H_ */