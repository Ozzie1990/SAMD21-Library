/*
 * reflow_oven.c
 *
 * Created: 10/22/2018 8:22:00 PM
 *  Author: ozzie
 */ 

#include "reflow_oven.h"

void reflow_oven_init(int uart_enable) {
	uart_quick_en();	//Enable UART for communication
	
	//Messages
	if(uart_enable) {
		uart_send_string(REFLOW_OVEN_UART, REFLOW_OVEN_MESSAGE_WELCOME);
		uart_send_string(REFLOW_OVEN_UART, REFLOW_OVEN_MESSAGE_INIT_START);
		uart_send_string(REFLOW_OVEN_UART, REFLOW_OVEN_MESSAGE_INIT_RELAY);
	}

	
	gpio_set_dir(PORTB_REG, PIN7, DIROUT);	//Set relay GPIO port
	
	//Message
	if(uart_enable) {
		uart_send_string(REFLOW_OVEN_UART, REFLOW_OVEN_MESSAGE_INIT_ADC);
	}
	
	adc_init(GCLK0, 0, 0x08);	//Initialize ADC 
	
	//TODO: Start a timer for counting

	reflow_oven_set_timer();
  
	rfl_ovn_status.State = IDLE;
	rfl_ovn_status.Temp = 0x00;
	
	if(uart_enable) {
		uart_send_string(REFLOW_OVEN_UART, REFLOW_OVEN_MESSAGE_INIT_COMPLETE);
	}
	
	//If no communication just run the profile.  Might add button to read instead in the future
	if(uart_enable) {	//TODO REVERSE LATER, CHANGED FOR DEBUGGING
		reflow_oven_start_profile(0);
	}
}

void reflow_oven_send_command(uint8_t cmd) {
	
	switch(cmd) {
		case REFLOW_OVEN_STATUS_CMD :
			//ADD
			rfl_ovn_status.Temp = adc_read();	//TODO: Convert to C
			uart_send_string(REFLOW_OVEN_UART, REFLOW_OVEN_MESSAGE_TEMP_STATUS(temp));	//Send temp
			
			//Send relay status
			(rfl_ovn_status.RelayState) ? uart_send_string(REFLOW_OVEN_UART, REFLOW_OVEN_MESSAGE_RLY_STATUS("On")) : uart_send_string(REFLOW_OVEN_UART, REFLOW_OVEN_MESSAGE_RLY_STATUS("Off"));
			break;
		
		case REFLOW_OVEN_START_CMD :
			//ADD
			break;
		
		case REFLOW_OVEN_STOP_CMD :
			//ADD
			break;
		
		case REFLOW_OVEN_GET_TEMP_CMD :
			//ADD
			break;
			
		default	:
			//NOT A VALID COMMAND
			break;
	}
}

uint16_t reflow_oven_read_temp() {
	adc_start();					//Start conversion
	
	while(!adc_ready()) {	/*Wait for data to be ready */}
	
	adc_ready_clr();	//Clear Ready
	
	return adc_read();				//Return data
}

void reflow_oven_start_profile(int uart_enable) {
	
	/*****************ZONE 1**********************/
	reflow_oven_set(ON);	
	
	if(uart_enable) {
		uart_send_string(REFLOW_OVEN_UART, REFLOW_OVEN_MESSAGE_START_PROFILE);
		uart_send_string(REFLOW_OVEN_UART, REFLOW_OVEN_MESSAGE_PROFILE_ZONE1);
	}
	
	while(adc_read() <= REFLOW_OVEN_ZONE_2) { /**WAIT**/ }
		
	/*****************ZONE 1 END*******************/
		
		
	/*****************ZONE 2**********************/
	reflow_oven_set(OFF);
	//TODO: Enable Timer
	
	if(uart_enable) {
		uart_send_string(REFLOW_OVEN_UART, REFLOW_OVEN_MESSAGE_PROFILE_ZONE2);
	}

	//TODO: Add counter for 90s
	while(rfl_ovn_time < REFLOW_OVEN_ZONE_2_TIME) { /**WAIT**/ }
	rfl_ovn_time = 0;
	//TODO: Disable Timer
	
	/*****************ZONE 2 END*******************/
	
	
	/*****************ZONE 3**********************/
	reflow_oven_set(ON);
	//TODO: Enable Timer
	
	if(uart_enable) {
		uart_send_string(REFLOW_OVEN_UART, REFLOW_OVEN_MESSAGE_PROFILE_ZONE3);
	}
	
	while(adc_read() <= REFLOW_OVEN_ZONE_3) { /**WAIT**/ }
	
	reflow_oven_set(OFF);	//Turn off to keep at temperature
	
	//TODO: Add counter for 30s
	while(rfl_ovn_time < REFLOW_OVEN_ZONE_3_TIME) { /**WAIT**/ }
	rfl_ovn_time = 0;
	//TODO: Disable Timer

	/*****************ZONE 3 END*******************/
	
	
	/*****************ZONE 4**********************/
	//Start zone 4, turn off the oven and open it to cool off
	reflow_oven_set(OFF);
	
	if(uart_enable) {
		uart_send_string(REFLOW_OVEN_UART, REFLOW_OVEN_MESSAGE_PROFILE_ZONE4);
	}
	
	while(adc_read() >= REFLOW_OVEN_ZONE_4) { /**WAIT**/ }
	
	if(uart_enable) {
		uart_send_string(REFLOW_OVEN_UART, REFLOW_OVEN_MESSAGE_PROFILE_DONE);
	}
	/*****************ZONE 4 END*******************/
}

void reflow_oven_set(int on) {
	if (on) {
		gpio_set_out(PORTB_REG, PIN7, HIGH);
		rfl_ovn_status.RelayState = 1;
	} else {
		gpio_set_out(PORTB_REG, PIN7, LOW);
		rfl_ovn_status.RelayState = 0;
	}
}

void reflow_oven_check_user_input() {
	char response = uart_rx_interrupt_handler(REFLOW_OVEN_UART, 1);
	
	if(response == UART_USER_INPUT_COMPLETE) {
		//Decode user input and send
		reflow_oven_cmd_decoder(reflow_oven_user_input);
		reflow_oven_user_input_cnt = 0;
	} else {
		//Append character and increment character count
		reflow_oven_user_input[reflow_oven_user_input_cnt] += response;
		reflow_oven_user_input_cnt += 1;
	}
}

//Decodes user input and send it to the reflow oven controller
void reflow_oven_cmd_decoder(char data[]) {
	
	uint8_t cmd_cnt = 0;
	
	for(int i = 0; i < 100; i++) {
		//Check CMD
		for(int j = 0; j < 100; j++) {
			
			//Check if strings match
			if(REFLOW_OVEN_COMMANDS[i][j] == data[j]) {
				
				//Check if we're at the last letter
				if(j == 2) {
					
					//Command matches, send it
					reflow_oven_send_command(cmd_cnt);
					return;
				}
			} else {
				cmd_cnt += 1;	//Move to next command
				break;
			}
		}
		
		//Check if we've passed the last command
		if(cmd_cnt == 4) {
			return;
		}
	}
}

//Timer interrupt handler
//Used to increment timer integer
//Implement in main.c file
void reflow_oven_timer() {
	rfl_ovn_time += 1;
}

void reflow_oven_set_timer() {

	tc_init(TC3, GCLK0, MFRQ, 0xC350) //50,000 timer
}
