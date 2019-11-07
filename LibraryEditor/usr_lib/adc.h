/*
 * adc.h
 *
 * Created: 7/26/2018 9:26:16 PM
 *  Author: ozzie
 */ 


#ifndef ADC_H_
#define ADC_H_

#include "samd21.h"
#include "clock.h"
#include "pm.h"

typedef union {
	struct {
		uint8_t SWRST:		1;
		uint8_t ENABLE:		1;
		uint8_t	RUNSTDBY:	1;
		uint8_t :			5;
		
	}bit;
	
	uint8_t reg;
}ADC_CTRLA;

typedef union {
	struct {
		uint8_t REFSEL:		3;
		uint8_t:			1;
		uint8_t REFCOMP:	1;
	}bit;
	
	uint8_t reg;
}ADC_REFCTRL;

typedef union {
	struct {
		uint8_t SAMPLENUM:	4;
		uint8_t ADJRES:		3;
		uint8_t:			1;
	}bit;
	
	uint8_t reg;
}ADC_AVGCTRL;

typedef union {
	struct {
		uint8_t SAMPLEN:	6;
		uint8_t:			2;
	}bit;
	
	uint8_t reg;
}ADC_SAMPCTRL;

typedef union {
	struct {
		uint16_t DIFFMODE:		1;
		uint16_t LEFTADJ:		1;
		uint16_t FREERUN:		1;
		uint16_t CORREN:		1;
		uint16_t RESSEL:		2;
		uint16_t:				2;
		
		uint16_t PRESCALER:		3;
		uint16_t:				5;
		
	}bit;
	
	uint16_t reg;
}ADC_CTRLB;

typedef union {
	struct {
		uint8_t WINMODE:	3;	
		uint8_t:			5;
	}bit;
	
	uint8_t reg;
}ADC_WINCTRL;

typedef union {
	struct {
		uint8_t FLUSH:		1;
		uint8_t START:		1;
		uint8_t :			6;
	}bit;
	
	uint8_t reg;
}ADC_SWTRIG;

typedef union {
	struct {
		uint32_t MUXPOS:		5;
		uint32_t :				3;
		
		uint32_t MUXNEG:		5;
		uint32_t :				3;
		
		uint32_t INPUTSCAN:		4;
		uint32_t INPUTOFFSET:	4;
		
		uint32_t GAIN:			4;
		uint32_t :				4;
	}bit;
	
	uint32_t reg;
	
}ADC_INPUTCTRL;

typedef union {
	struct {
		uint8_t STARTEI:	1;
		uint8_t SYNCEI:		1;
		uint8_t:			2;
		uint8_t RESRDYEO:	1;
		uint8_t WINMONEO:	1;
		uint8_t:			2;
	}bit;
	
	uint8_t reg;
}ADC_EVCTRL;

typedef union {
	struct {
		uint8_t RESRDY:		1;	
		uint8_t OVERRUN:	1;
		uint8_t WINMON:		1;
		uint8_t SYNCRDY:	1;
		uint8_t:			4;
	}bit;
	
	uint8_t reg;
}ADC_INTEN;

typedef union {
	struct {
		uint8_t:			7;
		uint8_t SYNCBUSY:	1;
	}bit;
	
	uint8_t reg;
}ADC_STATUS;

typedef union {
	struct {
		uint16_t RESULT:	16;
	}bit;
	
	uint16_t reg;
}ADC_RESULT;

typedef union {
	struct{
		uint8_t DBGRUN:		1;
		uint8_t :			7;
	}bit;
	
	uint8_t reg;
}ADC_DEBUG;

typedef struct {
	
	_REG ADC_CTRLA CTRLA;			//0x00
	_REG ADC_REFCTRL REFCTRL;		//0x01
	_REG ADC_AVGCTRL AVGCTRL;		//0x02
	_REG ADC_SAMPCTRL SAMPCTRL;		//0x03
	_REG ADC_CTRLB CTRLB;			//0x04-5
	_REG uint8_t reserved0[2];		//0x06-7
	_REG ADC_WINCTRL WINCTRL;		//0x08
	_REG uint8_t reserved1[3];		//0x09-B
	_REG ADC_SWTRIG SWTRIG;			//0x0C
	_REG uint8_t reserved2[3];		//0x0D-F
	_REG ADC_INPUTCTRL INPUTCTRL;	//0x10-3
	_REG ADC_EVCTRL EVCTRL;			//0x14
	_REG uint8_t reserved3;			//0x15
	_REG ADC_INTEN INTENCLR;		//0x16
	_REG ADC_INTEN  INTENSET;		//0x17
	_REG ADC_INTEN  INTFLAG;		//0x18
	_REG ADC_STATUS STATUS;			//0x19
	_REG ADC_RESULT RESULT;			//0x1A-B
	_REG uint8_t reserved4[14];		//0x1C-29 //NOT REALLY RESERVED BUT DON'T NEED IT FOR NOW
	_REG ADC_DEBUG DBGCTRL;			//0x2A
	
}adc_t;

#define ADC_REG ((adc_t *)0x42004000)

void adc_init(int clk_src, int freerun, int pin);
void adc_start();
uint8_t adc_ready();
void adc_ready_clr();
uint16_t adc_read();



#endif /* ADC_H_ */
