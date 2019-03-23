/*
 * pm.c
 *
 * Created: 8/27/2017 4:26:01 PM
 *  Author: ozzie
 */ 

#include "pm.h"

void pm_set_maskc(int pin) {
	PM_OFFSET->AHBCMASK |= (1<<5);
}