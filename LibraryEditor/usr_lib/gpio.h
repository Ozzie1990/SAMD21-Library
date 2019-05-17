/*
 * gpio.h
 *
 * Created: 8/23/2017 9:24:46 PM
 *  Author: ozzie
 */ 


#ifndef GPIO_H_
#define GPIO_H_

#include "stdint-gcc.h"
#include "samd21.h"

#define DIROUT		1
#define DIRIN		0


#define PERPH_UART	((PERPHC << 4) | PERPHC)

typedef union {
	struct {
		uint32_t bit0:	1;
		uint32_t bit1:	1;
		uint32_t bit2:	1;
		uint32_t bit3:	1;
		uint32_t bit4:	1;
		uint32_t bit5:	1;
		uint32_t bit6:	1;
		uint32_t bit7:	1;
		uint32_t bit8:	1;
		uint32_t bit9:	1;
		uint32_t bit10:	1;
		uint32_t bit11:	1;
		uint32_t bit12:	1;
		uint32_t bit13:	1;
		uint32_t bit14:	1;
		uint32_t bit15:	1;
		uint32_t bit16:	1;
		uint32_t bit17:	1;
		uint32_t bit18:	1;
		uint32_t bit19:	1;
		uint32_t bit20:	1;
		uint32_t bit21:	1;
		uint32_t bit22:	1;
		uint32_t bit23:	1;
		uint32_t bit24:	1;
		uint32_t bit25:	1;
		uint32_t bit26:	1;
		uint32_t bit27:	1;
		uint32_t bit28:	1;
		uint32_t bit29:	1;
		uint32_t bit30:	1;
		uint32_t bit31:	1;
	} bit;
	uint32_t reg;
}PORT_DIR, PORT_TGL, PORT_OUT;

typedef union {
	struct {
		uint32_t PINMASK:	16;
		uint32_t PINMUXEN:	1;
		uint32_t INEN:		1;
		uint32_t PULLEN:	1;
		uint32_t:			3;
		uint32_t DVRSTR:	1;
		uint32_t PMUX:		4;
		uint32_t WRPMUX:	1;
		uint32_t:			1;
		uint32_t WRPINCNFG:	1;
		uint32_t HWSEL:		1;
	}bits;
	uint32_t reg;
} PORT_WRCONFIG;

typedef union {
	struct {
		uint8_t	PMUXE:	4;	//bits 0..3 set even mux value
		uint8_t PMUXO:	4;	//bits 4..7 set odd mux value
	}bits;
	uint8_t reg;
} PORT_MUX;

typedef union {
	struct {
		uint8_t	PMUXEN:	1;
		uint8_t INEN:	1;
		uint8_t PULLEN:	1;
		uint8_t:		3;
		uint8_t DVRSTR:	1;
		uint8_t:		1;
	}bits;
	uint8_t reg;
} PORT_CFG;


typedef struct{
	_REG	PORT_DIR		DIR;
	_REG	uint32_t		DIRCLR;
	_REG	uint32_t		DIRSET;
	_REG	uint32_t		DIRTGL;
	_REG	PORT_OUT		OUT;
	_REG	uint32_t		OUTCLR;
	_REG	uint32_t		OUTSET;
	_REG	uint32_t		OUTTGL;
	_REG	uint32_t		IN;
	_REG	uint32_t		CTRL;
	_REG	PORT_WRCONFIG	WRCONFIG;
	_REG	uint32_t		reserved;
	_REG	PORT_MUX		PINMUX[16];
	_REG	PORT_CFG		PINCFG[31];
} PORT_REG;

#define PORTB_REG ((PORT_REG *) 0x41004480)
#define PORTA_REG ((PORT_REG *) 0x41004400)

#define PORTB_OFFSET ((port_reg_t *) 0x41004480)

void gpio_set_dir(PORT_REG * reg, int pin, int dir);
void gpio_toggle(PORT_REG * port, int pin);
void gpio_set_out(PORT_REG * port, int pin, int data);
void gpio_set_mux(PORT_REG * port, int pin, int perph, int input_enable, int pullup, int drive_strength);
void gpio_set_mux_fast(PORT_REG * port, pin_t pin, int perph, int input_enable, int pullup, int drive_strength);
void gpio_mux_disable(PORT_REG * port, int pin);
void gpio_set_out(PORT_REG * port, int pin, int data);
int  gpio_read(PORT_REG * port, int pin);

void led_initialize();
void led_toggle();

void usart3_pad01_set();
void spi0_set();
//void led_on();
//void led_off();

#endif /* GPIO_H_ */