/*
 * uart.c
 *
 * Created: 8/27/2017 12:09:58 PM
 *  Author: ozzie
 */ 

#include "uart.h"

/**************************************************
*	Function Name : UART SETUP
*
*	Description : Pass pre-configured UART register and pass it
*		to the proper base address.
*
***************************************************/
void uart_setup(int comm, UART_REG * uart) {
	
	UART_REG * reg;
	switch(comm) {
		case 0 : reg = UART0_REG;
		break;
		
		case 1 : reg = UART1_REG;
		break;
		
		case 2 : reg = UART2_REG;
		break;
		
		case 3 : reg = UART3_REG;
		break;
		
		case 4 : reg = UART4_REG;
		break;
		
		case 5 : reg = UART5_REG;
		break;
		
		default:
		return;
	}
	*reg = *uart;
}

/**************************************************
*	Function Name : UART Enable
*
*	Description : Enable UART
*
***************************************************/
void  uart_enable(int uart_comm_port) {
	switch(uart_comm_port) {
		case 0: UART0_REG->CNTRLA.bits.EN = 1;
		break;
		
		case 1: UART1_REG->CNTRLA.bits.EN = 1;
		break;

		case 2: UART2_REG->CNTRLA.bits.EN = 1;
		break;

		case 3: UART3_REG->CNTRLA.bits.EN = 1;
		break;

		case 4: UART4_REG->CNTRLA.bits.EN = 1;
		break;

		case 5: UART5_REG->CNTRLA.bits.EN = 1;
		break;

		default:
		return;	//ERROR
	}
}

/**************************************************
*	Function Name : UART INITIALIZE
*
*	Description : Pass UART_PARAMETERS
*
***************************************************/
void uart_initialize(UART_PARAMETERS uart) {
	
	UART_REG * reg;
	
	GCLK_REG->CLKCNTRL.bits.GEN = 0;

	switch(uart.COM_PORT_NUM) {
		case 0 : GCLK_REG->CLKCNTRL.bits.ID = GCLK_SERCOM0;
			PM_REG->AHBCMASK.bits.PMSERCOM0 = 1;
			reg = UART0_REG;
			break;
		case 1 : GCLK_REG->CLKCNTRL.bits.ID = GCLK_SERCOM1;
			PM_REG->AHBCMASK.bits.PMSERCOM1 = 1;
			reg = UART1_REG;
			break;
		case 2 : GCLK_REG->CLKCNTRL.bits.ID = GCLK_SERCOM2;
			PM_REG->AHBCMASK.bits.PMSERCOM2 = 1;
			reg = UART2_REG;
			break;
		case 3 : GCLK_REG->CLKCNTRL.bits.ID = GCLK_SERCOM3;
			PM_REG->AHBCMASK.bits.PMSERCOM3 = 1;
			reg = UART3_REG;
			break;
		case 4 : GCLK_REG->CLKCNTRL.bits.ID = GCLK_SERCOM4;
			PM_REG->AHBCMASK.bits.PMSERCOM4 = 1;
			reg = UART4_REG;
			break;
		case 5 : GCLK_REG->CLKCNTRL.bits.ID = GCLK_SERCOM5;
			PM_REG->AHBCMASK.bits.PMSERCOM5 = 1;
			reg = UART5_REG;
			break;
		default :
		return; //ERROR
	}
	
	gpio_set_mux(uart.TX_PORT, uart.TX_PIN, PERPHC, MUX_INPUT_ENABLE, MUX_PULLUP_ENABLE, MUX_DRV_NORMAL);
	gpio_set_mux(uart.RX_PORT, uart.RX_PIN, PERPHC, MUX_INPUT_ENABLE, MUX_PULLUP_ENABLE, MUX_DRV_NORMAL);
	
	GCLK_REG->CLKCNTRL.bits.CLKEN = 1;
	
	reg->CNTRLA.bits.MODE = uart.CMODE;
	
	reg->CNTRLA.bits.RXPO = uart.RX_PAD;
	reg->CNTRLA.bits.TXPO = uart.TX_PAD;
	
	reg->CNTRLA.bits.RUNSTDBY = 1;
	
	reg->CNTRLA.bits.DORD = uart.DATA_ORDER;
	
	reg->BAUD = uart.BAUD;
	
	reg->INTENSET.bits.RXC = uart.RX_INT;
	
	reg->CNTRLB.bits.RXEN = 1;
	reg->CNTRLB.bits.TXEN = 1;
}

int uart_send_data_echo(UART_REG * reg, uint8_t data) {
	
}

//Checks the received data, if ECHO is enabled then send data back
//Check if user has pressed enter or not
int uart_rx_interrupt_handler(UART_REG * reg, uint8_t echo) {	//Echo = 0 (off), Echo = 1 (on)
	char data = reg->DATA.bits.DATA8;
	
	if(echo == 1) {
		if(uart_send_data(reg, data) == ENTER_PRESSED) {
			return UART_USER_INPUT_COMPLETE;
		}
	} else {
		if (data = '\r') {
			return UART_USER_INPUT_COMPLETE;
		}
	}
	
	return data;
}

void uart_send_string(UART_REG * reg, char data[]) {
	int i = 0;
	
	while(uart_send_data(reg, data[i]) != UART_END_OF_MESSAGE) {
		i++;
	}
}

int uart_send_data(UART_REG * reg, uint8_t data) {
		
	//If enter is pressed send two commands to get new line
	if(data == '\r') {
		while (!reg->INTFLAG.bits.DRE & 1) { }	//Loop while DRE is not 1
		reg->DATA.bits.DATA8 = '\r';			//Set Data
		while (!reg->INTFLAG.bits.TXC & 1) { }
				
		while (!reg->INTFLAG.bits.DRE & 1) { }	//Loop while DRE is not 1
		reg->DATA.bits.DATA8 = '\n';			//Set Data
		while (!reg->INTFLAG.bits.TXC & 1) { }
			
		return ENTER_PRESSED;
	}
	
	if(data  == '\0') {
		return UART_END_OF_MESSAGE;
	}
		
	while (!reg->INTFLAG.bits.DRE & 1) { }	//Loop while DRE is not 1
	reg->DATA.bits.DATA8 = data;			//Set Data
	while (!reg->INTFLAG.bits.TXC & 1) { }	//Loop while TX is not complete
	return UART_SEND_COMPLETE;
}

uint8_t uart_read_data(UART_REG * reg) {
	while (!(reg->INTFLAG.bits.RXC & 1)) { }		//Loop while waiting for input
	uint8_t data = reg->DATA.bits.DATA8;			//Read Data
	return data;
}


/**************************************************
*	Function Name : UART Quick Enable 
*
*	Description : Quickly enable UART for debugging, works
*				  on all versions of SAMD21.
*				  Port A - Pin 22 (TX), 23 (RX)
*				  SAMD21E: 21, 22
*				  SAMD21G: 31, 32
*				  SAMD21J: 43, 44		
*				  
***************************************************/
void uart_quick_en() {
	uint16_t baud = get_baud_rate_async(9600, FCPU, 16);
	INTERRUPT_SAMD21->ISER_SAMD21.bits.SERCOM0 = 1;
	UART_PARAMETERS uart;
	uart.CMODE = 1;
	uart.DATA_ORDER = 1;
	uart.COM_PORT_NUM = 0;
	uart.RX_PORT = PORTA_REG;
	uart.TX_PORT = PORTA_REG;
	uart.RX_PAD = 0;
	uart.TX_PAD = 1;
	uart.RX_PIN = PIN23;
	uart.TX_PIN = PIN22;
	uart.BAUD = baud;
	//uart_init(0, PORTA_REG, PORTA_REG, 1, 0, PIN8, PIN9, baud, 0, 0);	//TODO: Fix later

	uart_initialize(uart);
	uart_enable(0);
}

/*
*	Function: uart_get_reg
*	
*	Description:  returns a pre-configured uart register for use by the user 
*   
*/

t_uart_reg * uart_get_reg(int comm_port, t_pin_info rx, t_pin_info tx) {
	
	t_uart_reg * uart;
	
	//Set Input Clock
	GCLK_REG->CLKCNTRL.bits.GEN = 0;
	
	//Set UART SERCOM address and initialize power management
	switch(comm_port) {
		case 0:
			uart = UART0_REG;
			PM_REG->AHBCMASK.bits.PMSERCOM0 = 1;
			break;
		
		case 1:
			uart = UART1_REG;
			PM_REG->AHBCMASK.bits.PMSERCOM1 = 1;
			break;
			
		case 2:
			uart = UART2_REG;
			PM_REG->AHBCMASK.bits.PMSERCOM2 = 1;
			break;
			
		case 3:
			uart = UART3_REG;
			PM_REG->AHBCMASK.bits.PMSERCOM3 = 1;
			break;
			
		case 4:
			uart = UART4_REG;
			PM_REG->AHBCMASK.bits.PMSERCOM4 = 1;			
			break;
			
		case 5:
			uart = UART5_REG;
			PM_REG->AHBCMASK.bits.PMSERCOM5 = 1;
			break;
			
		default:
			return;	//ERROR
	}
	
	//Set GPIO MUX for UART use
	gpio_set_mux(tx.port, tx.pin, tx.perh, MUX_INPUT_ENABLE, MUX_PULLUP_ENABLE, MUX_DRV_NORMAL);
	gpio_set_mux(rx.port, rx.pin, rx.perh, MUX_INPUT_ENABLE, MUX_PULLUP_ENABLE, MUX_DRV_NORMAL);
	
	//Enable clock 
	GCLK_REG->CLKCNTRL.bits.CLKEN = 1;
	
	//Get Baud Rate
	uint16_t baud = get_baud_rate_async(9600, FCPU, 16);
	
	//Set UART
	uart->CNTRLA.bits.MODE = 1;	//Set Mode
	uart->CNTRLA.bits.DORD = 1;	//Set Data Order
	uart->CNTRLA.bits.RXPO = 0;	//RX Pad Set
	uart->CNTRLA.bits.TXPO = 1;	//TX Pad Set
	uart->CNTRLA.bits.RUNSTDBY = 1;
	uart->BAUD = baud;
	uart->CNTRLB.bits.RXEN = 1;
	uart->CNTRLB.bits.TXEN = 1;
	
	return uart;
}

void uart_en(t_uart_reg * uart) {
	uart->CNTRLA.bits.EN = 1;
}

void uart_dis(t_uart_reg * uart) {
	uart->CNTRLA.bits.EN = 0;
}

t_pin_info uart_rx_pin() {
	t_pin_info rx;
	
	rx.port = PORTA_REG;
	rx.pin = PIN23;
	rx.perh = PERPHC;
	
	return rx;	
}

t_pin_info uart_tx_pin() {
	t_pin_info tx;
	
	tx.port = PORTA_REG;
	tx.pin = PIN22;
	tx.perh - PERPHC;
	
	return tx;
}