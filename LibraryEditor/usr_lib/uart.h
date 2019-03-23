/*
 * uart.h
 *
 * Created: 8/27/2017 12:09:46 PM
 *  Author: ozzie
 */ 


#ifndef UART_H_
#define UART_H_

#include "clock.h"
#include "pm.h"
#include "samd21.h"
#include "UART.h"
#include "gpio.h"
#include "stdint-gcc.h"
#include "interrupt.h"

typedef union {
	struct {
		uint32_t SWRST:		1;
		uint32_t EN:		1;
		uint32_t MODE:		3;
		uint32_t:			2;
		uint32_t RUNSTDBY:	1;
		
		uint32_t IBON:		1;
		uint32_t:			4;
		uint32_t SAMPR:		3;
		
		uint32_t TXPO:		2;
		uint32_t:			2;
		uint32_t RXPO:		2;
		uint32_t SAMPA:		2;
		
		uint32_t FORM:		4;
		uint32_t CMODE:		1;
		uint32_t CPOL:		1;
		uint32_t DORD:		1;
		uint32_t:			1;
	} bits;
	uint32_t reg;
} UART_CTRLA;

typedef union {
	struct {
		uint32_t CHSIZE:	3;
		uint32_t:			3;
		uint32_t SBMODE:	1;
		uint32_t:			1;
		
		uint32_t COLDEN:	1;
		uint32_t SFDE:		1;
		uint32_t ENC:		1;
		uint32_t:			2;
		uint32_t PMODE:		1;
		uint32_t:			2;
		
		uint32_t TXEN:		1;
		uint32_t RXEN:		1;
		uint32_t:			6;
		
		uint32_t:			8;
	}bits;
	uint32_t reg;
} UART_CTRLB;

typedef union {
	struct {
		uint8_t DRE:	1;
		uint8_t TXC:	1;
		uint8_t RXC:	1;
		uint8_t RXS:	1;
		uint8_t CTSIC:	1;
		uint8_t RXBRK:	1;
		uint8_t:		1;
		uint8_t ERROR:	1;
	} bits;
	uint8_t reg;
} UART_INTENCLR, UART_INTENSET, UART_INTFLAG;

typedef union {
	struct {
		uint16_t DATA8:	8;
		
		uint16_t DATA9:	1;
		uint16_t:		7;
	} bits;
	uint16_t reg;
}UART_DATA;

typedef struct {
	_REG UART_CTRLA				CNTRLA;
	_REG UART_CTRLB				CNTRLB;
	_REG uint32_t				reserved0;
	_REG uint16_t				BAUD;
	_REG uint8_t				RXPL;
	_REG uint32_t				reserved1;
	_REG UART_INTENCLR			INTENCLR;
	_REG uint8_t				reserved3;
	_REG UART_INTENSET			INTENSET;
	_REG uint8_t				reserved4;
	_REG UART_INTFLAG			INTFLAG;
	_REG uint8_t				reserved5;
	_REG uint16_t				STATUS;
	_REG uint32_t				SYNCBUSY;
	_REG uint32_t				reserved6;
	_REG uint32_t				reserved7;
	_REG UART_DATA				DATA;
	_REG uint32_t				reserved8;
	_REG uint16_t				reserved9;
	_REG uint8_t				DBGCTRL;
}UART_REG;

typedef struct {
	uint8_t COM_PORT_NUM;
	UART_REG* RX_PORT;
	UART_REG* TX_PORT;
	uint8_t RX_PAD;
	uint8_t TX_PAD;
	uint32_t RX_PIN;
	uint32_t TX_PIN;
	uint16_t BAUD;
	uint8_t DATA_ORDER;
	uint8_t CMODE;
	uint8_t RX_INT;
} UART_PARAMETERS;

#define UART5_REG ((UART_REG *)0x42001C00)
#define UART4_REG ((UART_REG *)0x42001800)
#define UART3_REG ((UART_REG *)0x42001400)
#define UART2_REG ((UART_REG *)0x42001000)
#define UART1_REG ((UART_REG *)0x42000C00)
#define UART0_REG ((UART_REG *)0x42000800)
#define UARTBASE_OFFSET (0x42000800)

#define GETBAUD(x)	((uint16_t)(65536*(1-16*(x/FCPU))))	//Doesn't work but the formula is correct (I think)

#define UART_USER_INPUT_COMPLETE	4
#define UART_SEND_COMPLETE			3
#define UART_END_OF_MESSAGE			2
#define ENTER_PRESSED				1
#define ENTER_NOT_PRESSED			0


void uart_setup(int comm, UART_REG * uart);
void uart_enable(int uart_comm_port);
void uart_initialize(UART_PARAMETERS uart);
int uart_send_data(UART_REG * reg, uint8_t data);
int uart_send_data_echo(UART_REG * reg, uint8_t data);
void uart_send_string(UART_REG * reg, char data[]);
uint8_t uart_read_data(UART_REG * reg);
int uart_rx_interrupt_handler(UART_REG * reg, uint8_t echo);

void uart_quick_en();

#endif /* UART_H_ */