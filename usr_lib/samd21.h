/*
 * samd21.h
 *
 * Created: 8/27/2017 12:13:34 PM
 *  Author: ozzie
 */ 


#ifndef SAMD21_H_
#define SAMD21_H_

#include "stdint-gcc.h"

#define _REG volatile
#define FCPU 1000000

#define DIR_OUTPUT	1
#define DIR_INPUT	0

#define HIGH	1
#define LOW		0

#define ON		1
#define OFF   	0

#define PIN0	(1<<0)
#define PIN1	(1<<1)
#define PIN2	(1<<2)
#define PIN3	(1<<3)
#define PIN4	(1<<4)
#define PIN5	(1<<5)
#define PIN6	(1<<6)
#define PIN7	(1<<7)
#define PIN8	(1<<8)
#define PIN9	(1<<9)
#define PIN10	(1<<10)
#define PIN11	(1<<11)
#define PIN12	(1<<12)
#define PIN13	(1<<13)
#define PIN14	(1<<14)
#define PIN15	(1<<15)
#define PIN16	(1<<16)
#define PIN17	(1<<17)
#define PIN18	(1<<18)
#define PIN19	(1<<19)
#define PIN20	(1<<20)
#define PIN21	(1<<21)
#define PIN22	(1<<22)
#define PIN23	(1<<23)
#define PIN24	(1<<24)
#define PIN25	(1<<25)
#define PIN26	(1<<26)
#define PIN27	(1<<27)
#define PIN28	(1<<28)
#define PIN29	(1<<29)
#define PIN30	(1<<30)
#define PIN31	(1<<31)

#define PIN_VAR(x, y)	(x<<y)							//Use for things like 0x3 (11) shifted by however many bits
#define CLR32(x)		(!PIN(x) ^ 0xFFFFFFFF)			//Use to clr selected bits in 32bits of data, simply &= with data
#define CLR32_VAR(x, y)	(!PIN_VAR(x,y) ^ 0xFFFFFFFF)	//Use to clr selected bits in 32bits of data, simply &= with data
#define CLR16_VAR(x, y) (!PIN_VAR(x,y) ^ 0xFFFF)
#define CLR8_VAR(x,y)	(!PIN_VAR(x,y) ^ 0xFF)

//#define ENABLE		1
//#define DISABLE		0

#define TRUE	1
#define FALSE	0

#define	RXPAD0	0
#define RXPAD1	1
#define RXPAD2	2
#define RXPAD3	3

#define TXPAD0	0
#define TXPAD1	1
#define TXPAD2	2
#define TXPAD3	3

#define PERPHA	0
#define PERPHB	1
#define PERPHC	2
#define PERPHD	3
#define PERPHE	4
#define PERPHF	5
#define PERPHG	6
#define PERPHH	7

#define MUX_INPUT_ENABLE	1
#define MUX_INPUT_DISABLE	0
#define MUX_PULLUP_ENABLE	1
#define MUX_PULLUP_DISABLE	0
#define MUX_DRV_NORMAL		0
#define MUX_DRV_STRONG		1

/*
#define SERCOM0		0
#define SERCOM1		1
#define SERCOM2		2
#define SERCOM3		3
#define SERCOM4		4
#define SERCOM5		5*/

#define SERCOMBASE_OFFSET (0x42000800)
/*
#define TC0		0
#define TC1		1
#define TC2		2
#define TC3		3
#define TC4		4
#define	TC5		5
#define TC6		6
#define	TC7		7*/

#define ERROR_NONE				0
#define ERROR_I2C_COLLISION		1
#define ERROR_I2C_NO_ACK		2

typedef enum {
	NONE = 0,
	I2C_COLLISIONS,
	I2C_NO_ACK
}errors;

typedef union {
	uint8_t		reg8;
	uint16_t	reg16;
	uint32_t	reg32;
}data_type;

//Should be used to pass data and check errors, reduces risk of false positives
typedef struct {
	_REG errors error;		//Pass error if any
	_REG data_type data;	//Pass data
	_REG uint32_t * reg;	//Pass base register
}data_t;

typedef enum pins{
	Pin0 = 0,
	Pin1,
	Pin2,
	Pin3,
	Pin4,
	Pin5,
	Pin6,
	Pin7,
	Pin8,
	Pin9,
	Pin10,
	Pin11,
	Pin12,
	Pin13,
	Pin14,
	Pin15,
	Pin16,
	Pin17,
	Pin18,
	Pin19,
	Pin20,
	Pin21,
	Pin22,
	Pin23,
	Pin24,
	Pin25,
	Pin26,
	Pin27,
	Pin28,
	Pin29,
	Pin30,
	Pin31
}pin_t;

typedef enum pads {
	PAD0 = 0x0,
	PAD1 = 0x1,
	PAD2 = 0x2,
	PAD3 = 0x4
}pads_t;

typedef enum bits {
	BIT0 = 0x0,
	BIT1 = 0x1
}bits_t;

uint16_t get_baud_rate_async(int fbaud, int fref, int samples);
int convert_pin_number(int pin);

#endif /* SAMD21_H_ */