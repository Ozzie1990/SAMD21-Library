/*
 * i2c.h
 *
 * Created: 9/2/2017 10:54:42 PM
 *  Author: ozzie
 */ 


#ifndef I2C_H_
#define I2C_H_

#include "clock.h"
#include "gpio.h"
#include "pm.h"
#include "samd21.h"
#include "stdint-gcc.h"

#define ACK_RCVD		0
#define ACK_NOT_RCVD	1
#define I2C_COLLISION	0xF

#define I2C_READ		1
#define I2C_WRITE		0

//Speed States
#define I2C_SPEED_SM_FM		0	//Standard and Fast Speed (100k and 400k)
#define I2C_SPEED_FM_PLUS	1	//Fast+ (1M)
#define I2C_SPEED_HS		2	//High Speed (3.4M)

//Inactive Time-Out States
#define I2C_INACT_DISABLE	0
#define I2C_INACT_55US		1
#define I2C_INACT_105US		2
#define I2C_INACT_205US		3

//Ack/Nack set
#define I2C_ACKACT_ACK		0
#define I2C_ACKACT_NACK		1

//Command Actions
#define I2C_CMD_NONE			0
#define I2C_CMD_REPEAT_START	1
#define I2C_CMD_BYTE_READ		2
#define I2C_CMD_STOP			3

typedef union {
	struct {
		uint32_t SWRST:		1;
		uint32_t EN:		1;
		uint32_t MODE:		3;
		uint32_t:			2;
		uint32_t RUNSTDBY:	1;
		
		uint32_t:			8;
		
		uint32_t PINOUT:	1;
		uint32_t:			3;
		uint32_t SDAHOLD:	2;
		uint32_t MEXTTOEN:	1;
		uint32_t SEXTTOEN:	1;
		
		uint32_t SPEED:		2;
		uint32_t:			1;
		uint32_t SCLSM:		1;
		uint32_t INACTOUT:	2;
		uint32_t LOWTOUT:	1;
		uint32_t:			1;
		
	} bits;
	uint32_t reg;
} I2C_MASTER_CTRLA;

typedef union {
	struct {
		uint32_t:			8;
		
		uint32_t SMEN:		1;
		uint32_t QCEN:		1;
		uint32_t:			6;
		
		uint32_t CMD:		2;
		uint32_t ACKACT:	1;
		uint32_t:			5;
		
		uint32_t:			8;
	}bits;
	uint32_t reg;
} I2C_MASTER_CTRLB;

typedef union {
	struct {
		uint32_t BAUD:		8;
		uint32_t BAUDLOW:	8;
		uint32_t HSBAUD:	8;
		uint32_t HSBAUDLOW:	8;
	}bits;
	uint32_t reg;
} I2C_MASTER_BAUD;

typedef union {
	struct {
		uint8_t MB:		1;
		uint8_t SB:		1;
		uint8_t:		5;
		uint8_t ERROR:	1;
	} bits;
	uint8_t reg;
} I2C_MASTER_INTENCLR, I2C_MASTER_INTENSET, I2C_MASTER_INTFLAG;

typedef union {
	struct {
		uint16_t DATA:	8;
		
		//uint16_t:		8;
	} bits;
	uint8_t reg;
} I2C_MASTER_DATA;

typedef union {
	struct {
		uint16_t BUSERR:		1;
		uint16_t ARBLOST:		1;
		uint16_t RXNACK:		1;
		uint16_t:				1;
		uint16_t BUSSTATE:		2;
		uint16_t LOWTOUT:		1;
		uint16_t CLKHOLD:		1;
		
		uint16_t MEXTTOUT:		1;
		uint16_t SEXTTOUT:		1;
		uint16_t LENERR:		1;	
		uint16_t:				5;
	} bits;
	uint16_t reg;
} I2C_MASTER_STATUS;

typedef union {
	struct {
		uint32_t SWRST:			1;
		uint32_t ENABLE:		1;
		uint32_t SYSOP:			1;
		uint32_t:				5;
		
		uint32_t:				8;
		uint32_t:				8;
		uint32_t:				8;
	} bits;
	uint32_t reg;
} I2C_MASTER_SYNCBUSY;

typedef union {
	struct {
		//uint32_t:			8;
		
		uint32_t ADDR:		11;
		uint32_t:			2;
		uint32_t LENEN:		1;
		uint32_t HS:		1;
		uint32_t TENBITEN:	1;
		
		uint32_t LEN:		8;
		
		uint32_t:			8;
	} bits;
	uint32_t reg;	
} I2C_MASTER_ADDR;

typedef union {
	struct {
		uint8_t	DBGSTOP:	1;
		uint8_t:			7;
	} bits;
	uint8_t reg;
} I2C_MASTER_DBGCTRL;

typedef struct {
	_REG I2C_MASTER_CTRLA				CNTRLA;
	_REG I2C_MASTER_CTRLB				CNTRLB;
	_REG uint32_t						reserved0;
	_REG I2C_MASTER_BAUD				BAUD;
	_REG uint32_t						reserved1;
	_REG I2C_MASTER_INTENCLR			INTENCLR;
	_REG uint8_t						reserved2;
	_REG I2C_MASTER_INTENSET			INTENSET;
	_REG uint8_t						reserved3;
	_REG I2C_MASTER_INTFLAG				INTFLAG;
	_REG uint8_t						reserved7;
	//_REG I2C_MASTER_DATA				DATA;
	_REG I2C_MASTER_STATUS				STATUS;
	_REG I2C_MASTER_SYNCBUSY			SYNCBUSY;
	_REG uint32_t						reserved4;
	//_REG uint8_t						reserved5;
	_REG I2C_MASTER_ADDR				ADDR;
	_REG I2C_MASTER_DATA				DATA;
	_REG uint32_t						reserved8;
	_REG uint16_t						reserved9;
	_REG uint8_t						reserved10;
	_REG I2C_MASTER_DBGCTRL				DBGCTRL;
}I2C_MASTER_REG_t;

#define I2C5_REG ((I2C_MASTER_REG_t *)0x42001C00)
#define I2C4_REG ((I2C_MASTER_REG_t *)0X42001800)
#define I2C3_REG ((I2C_MASTER_REG_t *)0x42001400)
#define I2C2_REG ((I2C_MASTER_REG_t *)0x42001000)
#define I2C1_REG ((I2C_MASTER_REG_t *)0x42000C00)
#define I2C0_REG ((I2C_MASTER_REG_t *)0x42000800)
#define I2CBASE_OFFSET (0x42000800)

void i2c_init(	int comm, PORT_REG * sclk_port, PORT_REG * sda_port, int sclk_pin, \
				int sda_pin, int clk_source, int speed, int inactive_bus, uint16_t baud);
void i2c_set_cmd(I2C_MASTER_REG_t * reg, int set);
void i2c_set_ack(I2C_MASTER_REG_t * reg, int set);
void i2c_send_address(I2C_MASTER_REG_t * reg, int address, int read_write);
int i2c_check_address(I2C_MASTER_REG_t * reg);
int i2c_send_data(I2C_MASTER_REG_t * reg, uint8_t data);
void i2c_send_data_int(I2C_MASTER_REG_t * reg, uint8_t data);
int i2c_read_data(I2C_MASTER_REG_t * reg);
void i2c_disable(I2C_MASTER_REG_t * reg);
void i2c_enable(I2C_MASTER_REG_t * reg);


void i2c_set_master();
//void i2c_init(I2C comm, I2C_REG reg);
//void i2c_en(I2C comm);

void init_i2c_master(PORT_REG * port, int pin, uint8_t gclk, uint8_t clk_sel, I2C_MASTER_REG_t * reg, int pm);

void i2c_drone_initialize();

#endif /* I2C_H_ */