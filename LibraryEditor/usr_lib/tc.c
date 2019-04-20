/*
 * tc.c
 *
 * Created: 11/25/2018 12:47:55 PM
 *  Author: ozzie
 */ 

#include "tc.h"
#include "interrupt.h"

void tc_init(int tc, int clk_src, int mode, uint16_t cc0) {
	TC_16bit_t * reg;
	
	switch(tc) {
		
		case 3 : PM_REG->AHBCMASK.bits.PMTC3 = 1;
		GCLK_REG->CLKCNTRL.bits.ID = GCLK_TCC2_TC3;
		reg = TC3_REG;
		break;
		
		case 4 : PM_REG->AHBCMASK.bits.PMTC4 = 1;
		GCLK_REG->CLKCNTRL.bits.ID = GCLK_TC4_TC5;
		reg = TC4_REG;
		break;
		
		//TODO: UPDATE WITH PROPER TC5 REGISTER WHEN CREATED
		case 5 : PM_REG->AHBCMASK.bits.PMTC5 = 1;
		GCLK_REG->CLKCNTRL.bits.ID = GCLK_TC4_TC5;
		reg = TC5_REG;
		break;
		
		case 6 : PM_REG->AHBCMASK.bits.PMTC6 = 1;
		GCLK_REG->CLKCNTRL.bits.ID = GCLK_TC6_TC7;
		reg = TC6_REG;
		break;
		
		//TODO: UPDATE WITH PROPER TC7 REGISTER WHEN CREATED
		case 7 : PM_REG->AHBCMASK.bits.PMTC7 = 1;
		GCLK_REG->CLKCNTRL.bits.ID = GCLK_TC6_TC7;
		reg = TC7_REG;

		break;
		
		default:
		return;
	}
	
	GCLK_REG->CLKCNTRL.bits.GEN = clk_src;
	GCLK_REG->CLKCNTRL.bits.CLKEN = 1;
	reg->CTRLA.bits.WAVEGEN = mode;
	reg->CC0 = cc0;
	reg->INTENSET.bits.MC0 = 1;	//DEFAULTED TO ENABLE INTERRUPT
}

void tc_en(TC_16bit_t * tc, int enable) {
	tc->CTRLA.bits.ENABLE = enable;
}

void tc_reset_cnt16(TC_16bit_t * tc) {
	tc->COUNT = 0;
}

