/*
 * pwm.h
 *
 * Created: 9/3/2017 3:01:39 PM
 *  Author: ozzie
 */ 


#ifndef PWM_H_
#define PWM_H_

#include "tc.h"

#define PWM_RN_STDBY_TRUE	1
#define PWM_RN_STDBY_FALSE	0

void pwm_init(PORT_REG * port, int pin, int perph, int tc, int clk_source, int pwm_mode, int run_stdby, uint16_t duty_cycle);

void init_pwm(PORT_REG * port, int pin, uint8_t gclk, uint8_t clk_sel, TC_16bit_t * tc, int pm);
void pwm_set_duty_cycle(TC_16bit_t * tc, uint16_t pwm_value);

#endif /* PWM_H_ */