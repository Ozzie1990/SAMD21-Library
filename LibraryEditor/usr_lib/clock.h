/*
 * clock.h
 *
 * Created: 8/27/2017 3:56:45 PM
 *  Author: ozzie
 */ 


#ifndef CLOCK_H_
#define CLOCK_H_

#include "samd21.h"

#define GCLK0	0
#define GCLK1	1
#define GCLK2	2
#define GCLK3	3
#define GCLK4	4
#define GCLK5	5
#define GCLK6	6
#define	GCLK7	7
#define GCLK8	8

#define GCLK_WDT		0x03
#define GCLK_RTC		0x04
#define GCLK_EIC		0x05
#define GCLK_USB		0x06
#define GCLK_SERCOM0	0x14
#define GCLK_SERCOM1	0x15
#define GCLK_SERCOM2	0x16
#define GCLK_SERCOM3	0x17
#define GCLK_SERCOM4	0x18
#define GCLK_SERCOM5	0x19
#define GCLK_TCC2_TC3   0x1B
#define GCLK_TC4_TC5	0x1C
#define GCLK_TC6_TC7	0x1D
#define GCLK_ADC		0x1E
#define GCLK_AC_DIG		0x1F
#define GCLK_AC_ANA		0x21
#define GCLK_DAC		0x23
#define GCLK_PTC		0x24
#define GCLK_I2S_0		0x25
#define GCLK_I2S_1		0x26

typedef union {
	struct {
		uint16_t ID:		6;
		uint16_t:			2;
		
		uint16_t GEN:		4;
		uint16_t:			2;
		uint16_t CLKEN:		1;
		uint16_t WRTLOCK:	1;
	} bits;
	uint16_t reg;
} GCLK_CLKCNTRL;

typedef union {
	struct {
		uint32_t ID:		4;
		uint32_t:			4;
		uint32_t SRC:		5;
		uint32_t:			3;
		uint32_t GENEN:		1;
		uint32_t IDC:		1;
		uint32_t OOV:		1;
		uint32_t OE:		1;
		uint32_t DIVSEL:	1;
		uint32_t RUNSTDBY:	1;
		uint32_t:			10;
	} bits;
	uint32_t reg;
}GCLK_GENCTRL;

typedef struct {
	_REG uint8_t		CNTRL;
	_REG uint8_t		STATUS;
	_REG GCLK_CLKCNTRL	CLKCNTRL;
	_REG GCLK_GENCTRL	GENCNTRL;
	_REG uint32_t		GENDIV;
} GCLK_REG_BASE;

#define GCLK_REG ((GCLK_REG_BASE *)0x40000C00)

typedef struct gclk_reg {
	volatile uint8_t CNTRL;
	volatile uint8_t STATUS;
	volatile uint16_t CLKCNTRL;
	volatile uint32_t GENCNTRL;
	volatile uint32_t GENDIV;
} gclk_reg_t;

#define GCLK_OFFSET ((gclk_reg_t *) 0x40000C00)

void gclk_set(uint8_t val);
void gclk_en(int en);



#endif /* CLOCK_H_ */
