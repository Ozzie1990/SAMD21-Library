/*
 * adc.c
 *
 * Created: 7/26/2018 9:47:11 PM
 *  Author: ozzie
 */ 

#include "adc.h"
#include "gpio.h"

void adc_init(int clk_src, int freerun, int pin) {
	
	//PM_REG->AHBCMASK.bits.PMADC = 0;
	
	//Enable power manager for ADC
	PM_REG->AHBCMASK.bits.PMADC = 1;
	//Set clock source
	GCLK_REG->CLKCNTRL.bits.GEN = clk_src;
	GCLK_REG->CLKCNTRL.bits.ID = GCLK_ADC;
	
	//Enable Clock
	GCLK_REG->CLKCNTRL.bits.CLKEN = 1;
	
	//Set mux for ADC
	gpio_set_mux(PORTB_REG, PIN0, PERPHB, MUX_INPUT_ENABLE, MUX_PULLUP_ENABLE, MUX_DRV_NORMAL);
	
	//Negative is set to ground, positive pin set by user
	ADC_REG->INPUTCTRL.bit.MUXNEG = 0x18;
	ADC_REG->INPUTCTRL.bit.MUXPOS = pin;
	
	//Set Ref to (.5 * VANNA)
	ADC_REG->REFCTRL.bit.REFSEL = 2;
	
	//Set free run
	ADC_REG->CTRLB.bit.FREERUN = freerun;	//0: Single Conversion, 1: Freerun (i.e. continuous)

	//Enable
	ADC_REG->CTRLA.bit.ENABLE = 1;
}

//Start ADC
void adc_start() {
	ADC_REG->SWTRIG.bit.START = 1;
}

uint8_t adc_ready() {
	return ADC_REG->INTFLAG.bit.RESRDY;
}

void adc_ready_clr() {
	ADC_REG->INTFLAG.bit.RESRDY = 1;
}

//Read value of ADC
uint16_t adc_read() {
	return ADC_REG->RESULT.reg;
}
