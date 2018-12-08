/*
 * pwm.c
 *
 * Created: 9/3/2017 3:01:48 PM
 *  Author: Ozzie
 */ 

#include "pwm.h"

/**************************************************
*	Function Name : PWM Initialization
*
*	Description : Configure PWM
*
***************************************************/
void pwm_init(PORT_REG * port, int pin, int perph, int tc, int clk_source, int pwm_mode, int run_stdby, uint16_t duty_cycle) {
	
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
	
	GCLK_REG->CLKCNTRL.bits.GEN = clk_source;
	GCLK_REG->CLKCNTRL.bits.CLKEN = 1;
	
	//gpio_set_mux(port, pin, perph, MUX_INPUT_ENABLE, MUX_PULLUP_ENABLE, MUX_DRV_NORMAL);
	gpio_set_mux(port, pin, perph, MUX_INPUT_ENABLE, MUX_PULLUP_DISABLE, MUX_DRV_NORMAL);
	
	reg->CTRLA.bits.WAVEGEN = pwm_mode;
	reg->CTRLA.bits.RUNSTDBY = run_stdby;
	pwm_set_duty_cycle(reg, duty_cycle);
	reg->CTRLA.bits.ENABLE = 1;
}

/**************************************************
*	Function Name : PWM Set Duty Cycle
*
*	Description : Configure PWM value
*
***************************************************/
void pwm_set_duty_cycle(TC_16bit_t * tc, uint16_t pwm_value) {
	tc->CC0 = pwm_value;
}