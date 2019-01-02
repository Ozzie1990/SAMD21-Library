/*
 * tc.h
 *
 * Created: 11/25/2018 12:43:01 PM
 *  Author: ozzie
 */ 


#ifndef TC_H_
#define TC_H_

#include "samd21.h"
#include "gpio.h"
#include "clock.h"
#include "stdint-gcc.h"
#include "pm.h"

typedef union {
	struct {
		uint16_t SWRST:		1;
		uint16_t ENABLE:	1;
		uint16_t MODE:		2;
		uint16_t:			1;
		uint16_t WAVEGEN:	2;
		uint16_t:			1;
		
		uint16_t PRESCALER:	3;
		uint16_t RUNSTDBY:	1;
		uint16_t PRESCSYNC:	2;
		uint16_t:			2;
	}bits;
	uint16_t reg;
}CTRLA_REG;

typedef union {
	struct {
		uint16_t ADDR:		5;
		uint16_t:			3;
		
		uint16_t:			6;
		uint16_t RCONT:		1;
		uint16_t RREQ:		1;
	}bits;
	uint16_t reg;
}READREQ_REG;

typedef union {
	struct {
		uint8_t	DIR:		1;
		uint8_t:			1;
		uint8_t ONESHOT:	1;
		uint8_t:			3;
		uint8_t CMD:		2;
	}bits;
	uint8_t reg;
}CTRLBCLR_REG, CTRLBSET_REG;

typedef union {
	struct {
		uint8_t INVEN0:		1;
		uint8_t INVEN1:		1;
		uint8_t:			2;
		uint8_t CPTEN0:		1;
		uint8_t CPTEN1:		1;
		uint8_t:			2;
	}bits;
	uint8_t reg;
}CTRLC_REG;

typedef union {
	struct {
		uint8_t DBGRUN:		1;
		uint8_t:			7;
	}bits;
	uint8_t reg;
}DBGCTRL_REG;

typedef union {
	struct {
		uint16_t EVACT:		3;
		uint16_t:			1;
		uint16_t TCINV:		1;
		uint16_t TCEI:		1;
		uint16_t:			2;
		
		uint16_t OVFEO:		1;
		uint16_t:			3;
		uint16_t MCEO0:		1;
		uint16_t MCEO1:		1;
		uint16_t:			2;
	}bits;
	uint16_t reg;
}EVCTRL_REG;

typedef union {
	struct {
		uint8_t OVF:		1;
		uint8_t ERR:		1;
		uint8_t:			1;
		uint8_t SYNCRDY:	1;
		uint8_t MC0:		1;
		uint8_t MC1:		1;
		uint8_t:			2;
	}bits;
	uint8_t reg;
}INTENCLR_REG, INTENSET_REG, INTFLAG_REG;

typedef union {
	struct {
		uint8_t:			3;
		uint8_t STOP:		1;
		uint8_t SLAVE:		1;
		uint8_t:			2;
		uint8_t SYNCBUSY:	1;
	}bits;
	uint8_t reg;
}STATUS_REG;

typedef struct {
	_REG CTRLA_REG		CTRLA;
	_REG READREQ_REG	READREQ;
	_REG CTRLBCLR_REG	CTRLBCLR;
	_REG CTRLBSET_REG	CTRLBSET;
	_REG CTRLC_REG		CTRLC;
	_REG uint8_t		reserved0;
	_REG DBGCTRL_REG	DBGCTRL;
	_REG uint8_t		reserved1;
	_REG EVCTRL_REG		EVCTRL;
	_REG INTENCLR_REG	INTENCLR;
	_REG INTENSET_REG	INTENSET;
	_REG INTFLAG_REG	INTFLAG;
	_REG STATUS_REG		STATUS;
	_REG uint16_t		COUNT;
	_REG uint8_t		reserved2[6];
	_REG uint16_t		CC0;
	_REG uint16_t		CC1;
	_REG uint8_t		reserved3[4];
}TC_16bit_t;

typedef struct {
	_REG CTRLA_REG		CTRLA;
	_REG READREQ_REG	READREQ;
	_REG CTRLBCLR_REG	CTRLBCLR;
	_REG CTRLBSET_REG	CTRLBSET;
	_REG CTRLC_REG		CTRLC;
	_REG uint8_t		reserved0;
	_REG DBGCTRL_REG	DBGCTRL;
	_REG uint8_t		reserved1;
	_REG EVCTRL_REG		EVCTRL;
	_REG INTENCLR_REG	INTENCLR;
	_REG INTENSET_REG	INTENSET;
	_REG INTFLAG_REG	INTFLAG;
	_REG STATUS_REG		STATUS;
	_REG uint8_t		COUNT;
	_REG uint8_t		reserved2[3];
	_REG uint8_t		PER;
	_REG uint8_t		reserved4[3];
	_REG uint8_t		CC0;
	_REG uint8_t		CC1;
}TC_8bit_t;

#define TC3_REG ((TC_16bit_t *)0x42002C00)
#define TC4_REG ((TC_16bit_t *)0x42003000)
#define TC5_REG ((TC_16bit_t *)0x42003400)
#define	TC6_REG ((TC_16bit_t *)0x42003800)
#define TC7_REG ((TC_16bit_t *)0x42003C00)

//Modes for TC
#define NFRQ	0x0
#define MFRQ	0x1
#define NPWM	0x2
#define MPWM	0x3

void tc_init(int tc, int clk_src, int mode, uint16_t cc0);
void tc_set_timer(int seconds);

#endif /* TC_H_ */