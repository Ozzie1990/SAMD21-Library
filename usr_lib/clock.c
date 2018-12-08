/*
 * clock.c
 *
 * Created: 8/27/2017 4:17:25 PM
 *  Author: ozzie
 */ 

#include "clock.h"

void gclk_set(uint8_t val) {
	//GCLK_OFFSET->CLKCNTRL |= (0x0000 | val);
	GCLK_REG->CLKCNTRL.bits.ID = val;
}

void gclk_en(int en) {
	if(en == 1) {
		GCLK_OFFSET->CLKCNTRL |= (1<<14);
	} else {
		
	}
}