/*
 * i2c.c
 *
 * Created: 9/2/2017 10:54:06 PM
 *  Author: ozzie
 */ 

#include "i2c.h"

/**************************************************
*	Function Name : i2c Initialization 
*
*	Description : Configure i2c
*
***************************************************/
void i2c_init(	int comm, PORT_REG * sclk_port, PORT_REG * sda_port, int sclk_pin, \
				int sda_pin, int clk_source, int speed, int inactive_bus, uint16_t baud) {
	
	I2C_MASTER_REG_t * reg;
	
	switch(comm) {
		case 0 : GCLK_REG->CLKCNTRL.bits.ID = GCLK_SERCOM0;
		PM_REG->AHBCMASK.bits.PMSERCOM0 = 1;
		reg = I2C0_REG;
		break;
		
		case 1 : GCLK_REG->CLKCNTRL.bits.ID = GCLK_SERCOM1;
		PM_REG->AHBCMASK.bits.PMSERCOM1 = 1;
		reg = I2C1_REG;
		break;
		
		case 2 : GCLK_REG->CLKCNTRL.bits.ID = GCLK_SERCOM2;
		PM_REG->AHBCMASK.bits.PMSERCOM2 = 1;
		reg = I2C2_REG;
		break;
		
		case 3 : GCLK_REG->CLKCNTRL.bits.ID = GCLK_SERCOM3;
		PM_REG->AHBCMASK.bits.PMSERCOM3 = 1;
		reg = I2C3_REG;
		break;
		
		case 4 : GCLK_REG->CLKCNTRL.bits.ID = GCLK_SERCOM4;
		PM_REG->AHBCMASK.bits.PMSERCOM4 = 1;
		reg = I2C4_REG;
		break;
		
		case 5 : GCLK_REG->CLKCNTRL.bits.ID = GCLK_SERCOM5;
		PM_REG->AHBCMASK.bits.PMSERCOM5 = 1;
		reg = I2C5_REG;
		break;
		
		default :
		return; //ERROR
		
	}
	
	gpio_set_mux(sclk_port, sclk_pin, PERPHC, MUX_INPUT_DISABLE, MUX_PULLUP_DISABLE, MUX_DRV_NORMAL);
	gpio_set_mux(sda_port, sda_pin, PERPHC, MUX_INPUT_DISABLE, MUX_PULLUP_DISABLE, MUX_DRV_NORMAL);
	
	GCLK_REG->CLKCNTRL.bits.GEN = clk_source;
	GCLK_REG->CLKCNTRL.bits.CLKEN = 1;
	
	
	reg->CNTRLA.bits.MODE = 0x5;	//Has to be set to 0x5 in order to enter i2c mode
	
	reg->CNTRLA.bits.SPEED = speed;
	reg->CNTRLA.bits.INACTOUT = inactive_bus;
	
	
	reg->BAUD.bits.BAUDLOW = 0xF & baud;
	reg->BAUD.bits.BAUD = (baud >> 4);

	reg->CNTRLA.bits.EN = 1;
	
	while(reg->SYNCBUSY.bits.ENABLE);
	
	reg->STATUS.bits.BUSSTATE = 1;
}

void i2c_set_ack(I2C_MASTER_REG_t * reg, int set) {
	reg->CNTRLB.bits.ACKACT = set;
}

/**************************************************
*	Function Name : i2C Set Command
*
*	Description : Set command for repeat starts, reading data after sending an ACK/NACK
*				  or sending a stop condition.
*
***************************************************/
void i2c_set_cmd(I2C_MASTER_REG_t * reg, int set) {
	reg->CNTRLB.bits.CMD = set;
}

/**************************************************
*	Function Name : i2C Send Address
*
*	Description : Send address and read write data to slave
*
***************************************************/
void i2c_send_address(I2C_MASTER_REG_t * reg, int address, int read_write) {
	while(reg->SYNCBUSY.bits.ENABLE);
	reg->ADDR.bits.ADDR = (address << 1) | read_write;
}

/**************************************************
*	Function Name : i2c Check Address
*
*	Description : Check if the address has been completed via a loop.  Use only if an 
*				interrupt is not being used, otherwise the code will sit and wait
*
***************************************************/
int i2c_check_address(I2C_MASTER_REG_t * reg) {
	//If both MB and SB = 0 then loop and wait.
	while(reg->INTFLAG.bits.MB == 0 && reg->INTFLAG.bits.SB == 0) {
	}
	
	//Check for any collisions
	if(reg->STATUS.bits.ARBLOST == 1) {
		return I2C_COLLISION;
	}
	
	//If set to 0 then ACK received
	if(reg->STATUS.bits.RXNACK == 0) {
		return ACK_RCVD;
	} else {
		return ACK_NOT_RCVD;
	}
}

/**************************************************
*	Function Name : i2c Send Data w/o interrupt
*
*	Description : Send data and wait for it to send.  Use only if an
*				interrupt is not being used, otherwise the code will sit and wait
*
***************************************************/
int i2c_send_data(I2C_MASTER_REG_t * reg, uint8_t data) {
	
	//TODO: Might have to check Status.CLKHOLD signal first?
	reg->DATA.bits.DATA = data;
	//If MB = 0 then loop
	while(reg->INTFLAG.bits.MB == 0) {
	}
	
	//Check for any collisions
	if(reg->STATUS.bits.ARBLOST == 1) {
		return I2C_COLLISION;
	}
	
	if(reg->STATUS.bits.RXNACK == 0) {
		return ACK_RCVD;
	} else {
		return ACK_NOT_RCVD;
	}
	
}

/**************************************************
*	Function Name : i2c Send Data w/ interrupt
*
*	Description : Send data 
*
***************************************************/
void i2c_send_data_int(I2C_MASTER_REG_t * reg, uint8_t data) {
	reg->DATA.bits.DATA = data;
}

/**************************************************
*	Function Name : i2c Read Data
*
*	Description : Read Data
*
***************************************************/
int i2c_read_data(I2C_MASTER_REG_t * reg) {
	while (reg->INTFLAG.bits.SB == 0) { }
	
	//Check for any collisions
	if(reg->STATUS.bits.ARBLOST == 1) {
		return I2C_COLLISION;
	}
	
	int data = reg->DATA.bits.DATA;
	
	if (data == I2C_COLLISION) {
		return data + 1;
	}
	
	return data;
}

void i2c_enable(I2C_MASTER_REG_t * reg) {
	reg->CNTRLA.bits.EN = 1;
	reg->STATUS.bits.BUSSTATE = 1;
}

void i2c_disable(I2C_MASTER_REG_t * reg) {
	reg->CNTRLA.bits.EN = 0;
}

