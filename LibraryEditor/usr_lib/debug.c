/*
 * debug.c
 *
 * Created: 6/11/2018 3:22:40 PM
 *  Author: ozzie
 */ 

#include "debug.h"

void debug_setTestLED() {
	gpio_set_dir(PORTA_REG, PIN18, DIROUT);
	gpio_set_dir(PORTB_REG, PIN7, DIROUT);
}

void debug_testLED() {	
	gpio_toggle(PORTA_REG, PIN18);
	gpio_toggle(PORTB_REG, PIN7);
	
	for(int i = 0; i < 500000; i++) {
		//Wait Loop
	}
	
}

void debug_delay() {
	for(int i  = 0; i < 500000; i++) {
		//wait
	}
}