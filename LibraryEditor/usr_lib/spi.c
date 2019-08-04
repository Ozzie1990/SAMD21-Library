/*
 * spi.c
 *
 * Created: 1/21/2018 12:27:54 PM
 *  Author: ozzie
 */ 


#include "spi.h"

void spi_drone_setup() {
	/*
	//Enable GCLK for SERCOM3
	GCLK_REG->CLKCNTRL.bits.GEN = 0;
	GCLK_REG->CLKCNTRL.bits.ID = GCLK_SERCOM0;
	GCLK_REG->CLKCNTRL.bits.CLKEN = 1;
		
	//Enable PM Bus for SERCOM3
	PM_REG->AHBCMASK.bits.PMSERCOM0 = 1;
		
	//Enable Global Interrupt
	INTERRUPT_SAMD21->ISER_SAMD21.bits.SERCOM0 = 1;
		
	//Set UART MODE ASYNC
	SERCOM0_REG->CNTRLA.bits.MODE = 0x2;
		
	//Set RX PAD1 and TX PAD0
	SERCOM0_REG->CNTRLA.bits.RXPO = 1;
	SERCOM0_REG->CNTRLA.bits.TXPO = 0;
		
	//No idea
	SERCOM0_REG->CNTRLA.bits.RUNSTDBY = 1;
		
	//LSB First
	SERCOM0_REG->CNTRLA.bits.DORD = 1;
		
	//Set Baud 9600
	SERCOM0_REG->BAUD = 0xD8AE;
		
	//ENABLE RX and TX
	SERCOM0_REG->CNTRLB.bits.RXEN = 1;
	SERCOM0_REG->CNTRLB.bits.TXEN = 1;
		
	//ENABLE
	SERCOM0_REG->CNTRLA.bits.EN = 1;*/
}