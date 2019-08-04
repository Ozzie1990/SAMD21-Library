/*
 * LibraryEditor.c
 *
 * Created: 3/22/2019 11:04:19 PM
 * Author : ozzie
 */ 

#include "usr_lib/gpio.h"
#include "usr_lib/tc.h"
#include "usr_lib/interrupt.h"
#include "system_samd21.h"

static volatile int count = 0;
static int tmpCount = 0;

void TC3_Handler() {
	tempFunc();
}

void tempFunc() {
	
	if(tmpCount >=19) {
		tmpCount = 0;
		count++;
		} else {
		tmpCount++;
	}
	
	TC3_REG->INTFLAG.bits.MC0 = 1;
}


int main(void)
{
    /* Initialize the SAM system */
    //SystemInit();
	
	//TEST CODE DELETE LATER//
	PORTA_REG->DIR.bit.bit18 = 1;
	
	PORTA_REG->OUT.bit.bit18 = HIGH;
	int state = 1;
	
	tc_init(3, 0, MFRQ, 0xC350);
	INTERRUPT_SAMD21->ISER_SAMD21.bits.TC3 = 1;
	tc_en(TC3_REG, 1);
	
	//reflow_oven_init();
	//END TEST CODE//
	
	volatile int a = 0;

    /* Replace with your application code */
    while (1) 
    {
		if(count >= 5 && state == 1) {
			PORTA_REG->OUT.bit.bit18 = LOW;
			state = 0;
			count = 0;
		} else if (count >= 5 && state == 0) {
			PORTA_REG->OUT.bit.bit18 = HIGH;	
			state = 1;
			count = 0;
		}
		
		a++;
    }
}