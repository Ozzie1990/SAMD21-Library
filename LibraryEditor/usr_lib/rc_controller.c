
#include "rc_controller.h"

void initRC() {

	//Initialize PWM
	pwm_init(PORTB_REG, PIN10, PERPHE, 5, GCLK0, NPWM, PWM_RN_STDBY_TRUE, (0xFFFF / 8));	//Left Motor
	pwm_init(PORTA_REG, PIN18, PERPHE, 3, GCLK0, NPWM, PWM_RN_STDBY_TRUE, (0xFFFF / 8));	//Right Motor
	
	//Initialize ADC
	adc_init(GCLK0, 0, 0xA); //TODO: TEST IF THIS WORKS
}

//Quick Test, handles everything just run
void rcTestRun() {

	//Initialize PWM and ADC
	initRC();	

	//Set speed for left turn
	setRightSpeed(0xFFFF);
	setLeftSpeed(0xFFFF);
	for (int i = 0; i < 10000; i++) {	}

	//Stop RC
	stopRC();

	//Turn Test
	turnRight(90);
	turnLeft(90);
	for (int i = 0; i < 10000; i++) {	}

	turnLeft(90);
	turnRight(90);
	
	for (int i = 0; i < 10000; i++) {	}
}

data_t readSonar()
{
	data_t data;
	data.error = 0;
	data.data.reg16 = 0x0045;
	return data;
}

void turnRight(int degrees)
{
	setLeftSpeed(0);
	setRightSpeed(0xFFFF);

	for (int i = 0; i < (degrees * 100); i++) {	}

	setRightSpeed(0);

	//TODO: Add interrupt timer that varies with degrees.
}

void turnLeft(int degrees)
{
	setRightSpeed(0);
	setLeftSpeed(0xFFFF);

	for (int i; i < (degrees * 100); i++) { }
	
	setLeftSpeed(0);
}

void setRightSpeed(int speed)
{
	pwm_set_duty_cycle(TC3_REG, speed);
}

void setLeftSpeed(int speed)
{
	pwm_set_duty_cycle(TC5_REG, speed);
}

void stopRC() {
	pwm_set_duty_cycle(TC3_REG, 0);
	pwm_set_duty_cycle(TC5_REG, 0);
}

void moveForward(int distance, int speed){
	pwm_set_duty_cycle(TC3_REG, speed);
	pwm_set_duty_cycle(TC5_REG, speed);
}

void moveBackwards(int distance)
{
	//Currently the pcb doesn't support this, need an H-Bridge circuit to support 
}

void setSpeed(uint16_t speed)
{
}
