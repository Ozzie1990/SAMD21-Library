/*
 * gpio.c
 *
 * Created: 8/25/2017 9:05:00 AM
 *  Author: ozzie
 */ 

#include "gpio.h"

void gpio_set_dir(PORT_REG * port, int pin, int dir) {
	if(dir == DIROUT) {
		port->DIR.reg |= pin;
	} else {
		port->DIR.reg &= !pin;
	}
}

//Need to fix this code, idea is ok but Assembly conversion makes it turn off/on all LEDs?  Might be an
//anomaly due to a bad/damaged chip.
void gpio_set_out(PORT_REG * port, int pin, int data) {
	if(data == 0) {
		port->OUT.reg &= !pin;
	} else {
		port->OUT.reg |= pin;
	}
}

void gpio_toggle(PORT_REG * port, int pin) {
	port->OUTTGL |= pin;
}

/**************************************************
*	Function Name : GPIO SET MUX
*
*	Description : Set GPIO MUX ports, cannot set
*		multiple pins at the same time. Pin must be passed with the (1<<x) format
*
***************************************************/
void gpio_set_mux(PORT_REG * port, int pin, int perph, int input_enable, int pullup, int drive_strength) {
	
	pin = convert_pin_number(pin);	//Configure pin to proper format
	
	if(pin%2 == 0) {
		port->PINMUX[pin/2].bits.PMUXE = perph;
	} else {
		port->PINMUX[pin/2].bits.PMUXO = perph;
	}
	
	port->PINCFG[pin].bits.PMUXEN = 1;
	port->PINCFG[pin].bits.INEN = input_enable;
	port->PINCFG[pin].bits.DVRSTR = drive_strength;
	port->PINCFG[pin].bits.PULLEN = pullup;
}

/**************************************************
*	Function Name : GPIO SET MUX FAST
*
*	Description : Set GPIO MUX ports, cannot set
*		multiple pins at the same time. Used without GPIO conversion
*		Pins must be defined by basic integers (PIN3 = int 3, not 0b1000)
*
***************************************************/
void gpio_set_mux_fast(PORT_REG * port, pin_t pin, int perph, int input_enable, int pullup, int drive_strength) {
	
	if(pin%2 == 0) {
		port->PINMUX[pin/2].bits.PMUXE = perph;
		} else {
		port->PINMUX[pin/2].bits.PMUXO = perph;
	}
	
	port->PINCFG[pin].bits.PMUXEN = 1;
	port->PINCFG[pin].bits.INEN = input_enable;
	port->PINCFG[pin].bits.DVRSTR = drive_strength;
	port->PINCFG[pin].bits.PULLEN = pullup;
}

void gpio_mux_disable(PORT_REG * port, int pin) {
	pin = convert_pin_number(pin);
	port->PINCFG[pin].bits.PMUXEN = 0;
}

int gpio_read(PORT_REG * port, int pin) {
	if(port->IN & pin == 0) {
		return 0;
	} 
	return 1;
}

void led_initialize() {
	gpio_set_dir(PORTB_REG, PIN30, DIROUT);
}

void led_toggle() {
	gpio_toggle(PORTB_REG, PIN30);
}

void usart3_pad01_set() {
	PORTA_REG->PINMUX[11].bits.PMUXE = PERPHC;
	PORTA_REG->PINMUX[11].bits.PMUXO = PERPHC;
	PORTA_REG->PINCFG[22].bits.PMUXEN = 1;
	PORTA_REG->PINCFG[22].bits.INEN = 1;
	PORTA_REG->PINCFG[23].bits.PMUXEN = 1;
	PORTA_REG->PINCFG[23].bits.INEN = 1;
}

void spi0_set() {
	PORTA_REG->PINMUX[2].bits.PMUXE = PERPHC;
	PORTA_REG->PINMUX[2].bits.PMUXO = PERPHC;
	PORTA_REG->PINMUX[3].bits.PMUXE = PERPHC;
	PORTA_REG->PINMUX[3].bits.PMUXO = PERPHC;
	PORTA_REG->PINCFG[4].bits.PMUXEN = 1;
	PORTA_REG->PINCFG[4].bits.INEN = 1;
	PORTA_REG->PINCFG[5].bits.PMUXEN = 1;
	PORTA_REG->PINCFG[5].bits.INEN = 1;
	PORTA_REG->PINCFG[6].bits.PMUXEN = 1;
	PORTA_REG->PINCFG[6].bits.INEN = 1;
	PORTA_REG->PINCFG[7].bits.PMUXEN = 1;
	PORTA_REG->PINCFG[7].bits.INEN = 1;
}