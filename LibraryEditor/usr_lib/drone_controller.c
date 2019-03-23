/*
 * drone_controller.c
 *
 * Created: 9/3/2017 3:07:45 PM
 *  Author: Ozzie
 * 
 * Directions:  To use the code you need to initialize the drone via initialize_drone()
 * 				Modify the PID gains defined in function mentioned above as needed.
 * 				
 * 				Currently the code only stabilizes.  Need to add features for moving in various directions
 * 				and taking in user input.
 */ 

#include "drone_controller.h"
#include "i2c.h"

uint16_t pwm_values[] = {(0xFFFF / 2),(0xFFFF / 4),(0xFFFF / 8),(0xFFFF / 16), (0xFFFF / 32), (0xFFFF / 64)};

/**************************************************
*	Function Name :  Initialize Drone 
*
***************************************************/
void initialize_drone() {
	
	//Set up TC MUX for pins
	//PORTB_REG->DIR.reg = 0x0000300C;  
	
	//Initialize PWM
	pwm_init(PORTB_REG, PIN8,  PERPHE, 4, GCLK0, NPWM, PWM_RN_STDBY_TRUE, (0xFFFF/8));
	pwm_init(PORTB_REG, PIN10, PERPHE, 5, GCLK0, NPWM, PWM_RN_STDBY_TRUE, (0xFFFF/8));
	pwm_init(PORTA_REG, PIN18, PERPHE, 3, GCLK0, NPWM, PWM_RN_STDBY_TRUE, (0xFFFF/8));
	pwm_init(PORTA_REG, PIN20, PERPHE, 7, GCLK0, NPWM, PWM_RN_STDBY_TRUE, (0xFFFF/8));

	//Baud rate seems to work but doesn't mean it's right
	i2c_init(0, PORTA_REG, PORTA_REG, PIN9, PIN8, GCLK0, I2C_SPEED_SM_FM, I2C_INACT_DISABLE, 0x0B16); //TODO: FIX BAUD RATE	
	
	//Enable Accelerometer
	accel_cmd(0x2A, 0x01);
	
	motor1Speed = 0xFFFF;
	motor2Speed = 0xFFFF;
	motor3Speed = 0xFFFF;
	motor4Speed = 0xFFFF;

	//PID X Gain
	pidXDiffGain = 1;
	pidXIntGain = 1;
	pidXPropGain = 1;

	//PID Y Gain
	pidYDiffGain = 1;
	pidYIntGain = 1;
	pidYPropGain = 1;

	//PID Z Gain
	pidZDiffGain = 0;
	pidZIntGain = 0;
	pidZPropGain = 0;

	//PID X Input and Output
	pidXInput = 0;
	pidXOutput = 0;

	//PID Y Input and Output
	pidYInput = 0;
	pidYOutput = 0;

	delay = 0;

	diff_accel_val = 0;
	int_accel_val  = 0;
	prop_accel_val = 0;
	
	drone_xInput = 0;
	drone_yInput = 0;
	
	drone_plant_x_output = 0;
	drone_plant_y_output = 0;
}

/**************************************************
*	Function Name :  Set Motor 1 Speed
*
***************************************************/
void setMotor1Speed(uint16_t speed) {
	pwm_set_duty_cycle(TC4_REG, speed);
}

/**************************************************
*	Function Name :  Set Motor 2 Speed
*
***************************************************/
void setMotor2Speed(uint16_t speed) {
	pwm_set_duty_cycle(TC5_REG, speed);
}

/**************************************************
*	Function Name :  Set Motor 3 Speed
*
***************************************************/
void setMotor3Speed(uint16_t speed) {
	pwm_set_duty_cycle(TC3_REG, speed);
}

/**************************************************
*	Function Name :  Set Motor 4 Speed
*
***************************************************/
void setMotor4Speed(uint16_t speed) {
	pwm_set_duty_cycle(TC7_REG, speed);
}

/**************************************************
*	Function Name :  Auto Stabalizer
*
*   Description :	For Testing
*
***************************************************/
void drone_auto_stabilizer() {

	while(1) {
		drone_x_system(65500);
		drone_y_system(0);	//Might be backwards with X axis
		//updateMotors();	//Update motor speed
		//resetMotorSpeed();	//Reset motor speed variable (does not stop motors)
	}
	

}

/**************************************************
*	Function Name :  Derivative Function
*
*   Description :	For PID
*
***************************************************/
int diff_accel(int diff_in) {
	diff_accel_val = (diff_in - delay);
	return diff_accel_val;
}

/**************************************************
*	Function Name :  Integral Function
*
*   Description :	For PID
*
***************************************************/
int int_accel(int int_in) {
	int_accel_val = int_accel_val + (int_in - delay);
	return diff_accel_val;
}

/**************************************************
*	Function Name :  PID Handler
*
*   Description :	Pass input/gains to the system and the PID handler will provide plant input from it
*
***************************************************/
int PID_Handler(int diffGain, int intGain, int propGain, int PID_In) {
	int data = (diffGain * diff_accel(PID_In)) + (int_accel(PID_In) * intGain) + (propGain * PID_In);
	delay = PID_In;

	return data;
}

/**************************************************
*	Function Name :  X Axis System
*
*   Description :	Provide user input to system and subtract it from the previous plant output to then 
*					be used by the setPlant function for use in the PID/Plant model
*
***************************************************/
void drone_x_system(int input) {
	int systemInput = input - drone_plant_x_output;
	drone_plant_x_output = setPlant(X_AXIS, systemInput, pidXDiffGain, pidXIntGain, pidXPropGain);
}

/**************************************************
*	Function Name :  Y Axis System
*
*   Description :	Provide user input to system and subtract it from the previous plant output to then 
*					be used by the setPlant function for use in the PID/Plant model
*
***************************************************/
void drone_y_system(int input) {
	int systemInput = input - drone_plant_y_output;
	drone_plant_y_output = setPlant(Y_AXIS, systemInput, pidYDiffGain, pidYIntGain, pidYPropGain);
}

/**************************************************
*	Function Name :  Plant Model
*
*   Description :	Take input (speed) and adjust the PWM speed given this input.  Update speed
*					variables to be used later and provide axis accelerometer data.
*
***************************************************/
int plant(int axis, int input) {
	
	int tSpeed = input;
	uint16_t offset = 0x1000;	//Offset for constant flying	TODO: Might have to cut in half due to two additions
	int axisData = 0;
	
	if (tSpeed < 0) {
		
		//Change tSpeed to be positive plus offset
		tSpeed = (-1 * tSpeed) + offset;	//TODO: If min is 0xFFFF change to "- offset"
		
		//Check if speed rolled over and if so then make it equal the max
		if (tSpeed < (offset * 2)) {
			tSpeed = 0xFFFFFFFF;
		}
		
		//Only 16 bits of PWM
		uint16_t speed = tSpeed;
		
		/*TODO:
		Need to have variables for each motor that each axis adds.  Without that then stabilization doesn't
		work as well since one axis over takes the other ones input. 
		*/
		if (axis == X_AXIS) {	
			setMotor1Speed(speed);	
			setMotor2Speed(speed);	
			setMotor3Speed(speed/8);
			setMotor4Speed(speed/8);
			axisData = getXAxis();
		} else if (axis == Y_AXIS) {	
			setMotor1Speed(speed);
			setMotor2Speed(speed/8);
			setMotor3Speed(speed/8);
			setMotor4Speed(speed);
			axisData = getYAxis();
		}

		//DO NOT DELETE, THIS WILL REPLACE PREVIOUS CODE
		//TODO: IF MAX SPEED IS 0 THEN UPDATE TO -=
		/*
		if (axis == X_AXIS) {
			motor1Speed += speed/2;
			motor2Speed += speed/2;
			motor3speed += speed/8;
			motor4Speed += speed/8;
			axisData = getXAxis();
		} else if (axis == Y_AXIS) {
			motor1Speed += speed/2;
			motor2Speed += speed/8;
			motor3Speed += speed/8;
			motor4Speed += speed/2;
			axisData = getYAxis();
		}*/
		
	} else {
		
		//Apply offset
		tSpeed += offset;	//TODO: If min is 0xFFFF change to "- offset"
		
		//Check if speed rolled over and if so then make it equal the max
		if (tSpeed < (offset * 2)) {
			tSpeed = 0xFFFFFFFF;
		}
		
		//Only 16 bits of PWM
		uint16_t speed = tSpeed;

		if (axis == X_AXIS) {	//X
			setMotor1Speed(speed/8);
			setMotor2Speed(speed/8);
			setMotor3Speed(speed);
			setMotor4Speed(speed);
			axisData = getXAxis();
			} else if (axis == Y_AXIS) {	//Y
			setMotor1Speed(speed/8);
			setMotor2Speed(speed);
			setMotor3Speed(speed);
			setMotor4Speed(speed/8);
			axisData = getYAxis();
		}

		//DO NOT DELETE, THIS WILL REPLACE PREVIOUS CODE
		//TODO: IF MAX SPEED IS 0 THEN UPDATE TO -=
		/*
		if (axis == X_AXIS) {
			motor1Speed += speed/8;
			motor2Speed += speed/8;
			motor3speed += speed/2;
			motor4Speed += speed/2;
			axisData = getXAxis();
		} else if (axis == Y_AXIS) {
			motor1Speed += speed/8;
			motor2Speed += speed/2;
			motor3Speed += speed/2;
			motor4Speed += speed/8;
			axisData = getYAxis();
		}*/
	}
	
	return axisData;	
}

/**************************************************
*	Function Name :  Update Motor Speed
*
***************************************************/
void updateMotors() {
	setMotor1Speed(motor1Speed);
	setMotor2Speed(motor2Speed);
	setMotor3Speed(motor3Speed);
	setMotor4Speed(motor4Speed);
}

/**************************************************
*	Function Name :  Reset Motor Speed
*
***************************************************/
void resetMotorSpeed() {
	motor1Speed = MIN_SPEED;
	motor2Speed = MIN_SPEED;
	motor3Speed = MIN_SPEED;
	motor4Speed = MIN_SPEED;
}

/**************************************************
*	Function Name :  Set Plant
*
*   Description :	Take input to PID, pass it through PID with set gains
*					and provide that output to the plant.  Plant will respond and will provide the 
*					output of the accelerometer.
*
***************************************************/
int setPlant(int axis, int pidInput, int diffGain, int intGain, int propGain) {
	int plantInput = PID_Handler(diffGain, intGain, propGain, pidInput);	//Get input for plant
	int plantOutput = plant(axis, plantInput);	//Provide input from previous function into plant model

	//Return provided plant output for use in next setPlant call
	return plantOutput;	
}

/**************************************************
*	Function Name :  Read X Axis Data
*
***************************************************/
int getXAxis() {
	
	uint16_t data = 0;
	
	//MSB Data
	data = read_ADC(OUT_X_MSB);
	
	//LSB Data
	//TODO: This doesn't seem right, why only 4 bits and why is the MSB not being shifted?
	//data = (data | (read_ADC(OUT_X_LSB) << 4));
	data = ((data << 8) | read_ADC(OUT_X_LSB));
	
	return data;
}

/**************************************************
*	Function Name :  Read Y Axis Data
*
***************************************************/
int getYAxis() {
	
	uint16_t data = 0;
	
	//MSB Data
	data = read_ADC(OUT_Y_MSB);
	
	//LSB Data
	//data = (data | (read_ADC(OUT_Y_LSB) << 4));
	data = ((data << 8) | read_ADC(OUT_Y_LSB));
	
	return data;
}

/**************************************************
*	Function Name :  Read Z Axis Data
*
***************************************************/
int getZAxis() {
	
	uint16_t data = 0;
	
	//MSB Data
	data = read_ADC(OUT_Z_MSB);
	
	//LSB Data
	//data = (data | (read_ADC(OUT_Z_LSB) << 4));
	data = ((data << 8) | read_ADC(OUT_Z_LSB));
	
	return data;
}

/**************************************************
*	Function Name :  Read All Axis Data
*
*	Description : Pass pointers to read all data, return status
*				  If ACCEL_ERROR passed the scrap all data
*
***************************************************/
int getAllAxis(int * x, int * y, int * z) {
	return read_accel_multi(x, y, z);
}

/**************************************************
*	Function Name :  Read Accelerometer 
*
*	Description : Steps for reading are as follows:
*				1. Start bit w/ Address (set to Write)
*				2. After AK is received send register to slave
*				3. After AK send repeated start with address (set to Read)
*				4. After AK is received and data buffer is full read data
*				5. Send NACK and Stop Bit
*
***************************************************/
int read_ADC(int data) {

	/***************Step 1**********************/
	I2C0_REG->CNTRLB.bits.QCEN = 0;	//Clear quick commands
	i2c_send_address(I2C0_REG, 0x1D, I2C_WRITE);
	
	int status = i2c_check_address(I2C0_REG);
	if (status == ACK_NOT_RCVD || status == I2C_COLLISION) { return ACCEL_ERROR; } //ERROR
	/*******************************************/
	
	
	/***************Step 2**********************/
	status = i2c_send_data(I2C0_REG, data);
	if (status == ACK_NOT_RCVD || status == I2C_COLLISION) { return ACCEL_ERROR; } //ERROR
	/*******************************************/
		
	/***************Step 3**********************/
	//Writing new address should generate a repeat start
	i2c_send_address(I2C0_REG, 0x1D, I2C_READ);
	
	status = i2c_check_address(I2C0_REG);
	if (status == ACK_NOT_RCVD || status == I2C_COLLISION) { return ACCEL_ERROR; } //ERROR
	/*******************************************/
	
	/***************Step 4 & 5**********************/
	i2c_set_ack(I2C0_REG, I2C_ACKACT_NACK);
	int axis_data = i2c_read_data(I2C0_REG);
	
	//a completely different method
	i2c_set_cmd(I2C0_REG, I2C_CMD_STOP);
	/*******************************************/
	
	
	return axis_data;
}

/**************************************************
*	Function Name :  Read Accelerometer
*
*	Description : Steps for reading are as follows:
*				1. Start bit w/ Address (set to Write)
*				2. After AK is received send register to slave
*				3. After AK send repeated start with address (set to Read)
*				4. After AK is received and data buffer is full read data
*				5. Keep reading data until all Axis data is received.
*				6. Send NACK and Stop Bit
*
***************************************************/
int read_accel_multi(int * x, int * y, int * z) {

	/***************Step 1**********************/
	I2C0_REG->CNTRLB.bits.QCEN = 0;	//Clear quick commands
	i2c_send_address(I2C0_REG, 0x1D, I2C_WRITE);
	
	int status = i2c_check_address(I2C0_REG);
	if (status == ACK_NOT_RCVD || status == I2C_COLLISION) { return ACCEL_ERROR; } //ERROR
	/*******************************************/
	
	/***************Step 2**********************/
	status = i2c_send_data(I2C0_REG, 0x01);	//Always start with X axis MSB
	if (status == ACK_NOT_RCVD || status == I2C_COLLISION) { return ACCEL_ERROR; } //ERROR
	/*******************************************/
	
	/***************Step 3**********************/
	//Writing new address should generate a repeat start
	i2c_send_address(I2C0_REG, 0x1D, I2C_READ);
	
	status = i2c_check_address(I2C0_REG);
	if (status == ACK_NOT_RCVD || status == I2C_COLLISION) { return ACCEL_ERROR; } //ERROR
	/*******************************************/
	
	/***************Step 4-6 **********************/
	i2c_set_ack(I2C0_REG, I2C_ACKACT_ACK);
	
	//Data holder for all axis
	int data[] = {0,0,0,0,0,0};
	
	for(int i = 0; i < 6; i++) {
		
		//Final piece of data, set to NACK and then stop
		if (i == 5) {
			i2c_set_ack(I2C0_REG, I2C_ACKACT_NACK);
		}
		
		data[i] = i2c_read_data(I2C0_REG);
		
		if (data == I2C_COLLISION) {return I2C_COLLISION;}
		
		//Send ACK
		I2C0_REG->CNTRLB.bits.CMD = 0x2;
		
		//Delay needed for accelerometer
		for(int j = 0; j < 50; j++) {}
	}
	
	//Send stop command
	i2c_set_cmd(I2C0_REG, I2C_CMD_STOP);
	/*******************************************/
	
	*x = (0x3FFF & ((data[0] << 8) | data[1]));
	*y = (0x3FFF & ((data[2] << 8) | data[3]));
	*z = (0x3FFF & ((data[4] << 8) | data[5]));
	
	return ACCEL_READ_COMPLETE;
}

int accel_cmd(int addr, int cmd) {
	/***************Step 1**********************/
	I2C0_REG->CNTRLB.bits.QCEN = 0;	//Clear quick commands
	i2c_send_address(I2C0_REG, 0x1D, I2C_WRITE);
	
	int status = i2c_check_address(I2C0_REG);
	if (status == ACK_NOT_RCVD || status == I2C_COLLISION) { return ACCEL_ERROR; } //ERROR
	/*******************************************/
	
	
	/***************Step 2**********************/
	status = i2c_send_data(I2C0_REG, addr);	
	if (status == ACK_NOT_RCVD || status == I2C_COLLISION) { return ACCEL_ERROR; } //ERROR
	/*******************************************/
	
	/***************Step 3**********************/
	status = i2c_send_data(I2C0_REG, cmd);	
	if (status == ACK_NOT_RCVD || status == I2C_COLLISION) { return ACCEL_ERROR; } //ERROR
	/*******************************************/
	
	//Send stop command
	i2c_set_cmd(I2C0_REG, I2C_CMD_STOP);
}

//I2C0_REG->CNTRLB.bits.QCEN = 1;
//i2c_set_cmd(I2C0_REG, I2C_CMD_REPEAT_START);