/*
 * samd21.c
 *
 * Created: 4/21/2018 1:59:38 PM
 *  Author: ozzie
 */ 

#include "samd21.h"

/**************************************************
*	Function Name : GET BAUD RATE ASYNC
*
*	Description : Returns 16bit value for baud rate for Async mode of any system.  NEED TO VERIFY IF IT WORKS
*
***************************************************/
uint16_t get_baud_rate_async(int fbaud, int fref, int samples) {
	float temp = 65536 * (1 - (samples*(fbaud / fref)));
	uint16_t baud = (uint16_t) temp;
	
	return baud;
}

/**************************************************
*	Function Name : CONVERT PIN NUMBER
*
*	Description : Used to get a standard pin number from the predefined pin numbers that has
*	been created.  Ex.  get_pin_number(PIN4) returns int 4 whereas PIN4 equals b'1000' (an 8 integer)
*
***************************************************/
int convert_pin_number(int pin) {
	if(pin == 0) {
		return 0;
	}
	
	int count = 0;
	while(count < 32) {
		if(pin == 1) {
			break;
		}
		pin = pin>>1;
		count++;
	}
	return count;
}