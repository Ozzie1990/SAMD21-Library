/*
 * LibraryEditor.c
 *
 * Created: 3/22/2019 11:04:19 PM
 * Author : ozzie
 */ 

#include "usr_lib/reflow_oven.h"
#include "samd21.h"

void TC3_Handler() {
	
	if(rfl_ovn_time_tmp >= 19) {	//Assuming FCLKD = 1MHz clock, if not then change!
		rfl_ovn_time += 1;
		rfl_ovn_time_tmp = 0;
		} else {
		rfl_ovn_time_tmp += 1;
	}
	
	TC3_REG->INTFLAG.bits.MC0 = 1;	
}


int main(void)
{
    /* Initialize the SAM system */
    //SystemInit();
	
	//TEST CODE DELETE LATER//
	reflow_oven_init();
	//END TEST CODE//

    /* Replace with your application code */
    while (1) 
    {
    }
}