/*
 * reflow_oven.c
 *
 * Created: 10/22/2018 8:22:00 PM
 *  Author: ozzie
 */ 

#include "reflow_oven.h"
//NOTE: CURRENTLY UART COMMUNICATION WITH THIS SYSTEM IS NOT TESTED/COMPLETE, DO NOT RECOMMEND USING
//NEEDS TO BE FIXED WITH INTERRUPTS NOT WITH THE CURRENT SET UP OTHERWISE THE SYSTEM COULD LATCH!!!!!

/*
 * Function:  reflow_oven_init
 * --------------------
 * initalizes the reflow oven by 
 * 1) Setting UART (if inputed)
 * 2) Setting the GPIO
 * 3) Setting the Timer
 * 4) Setting the ADC
 * 5) Setting the LEDs
 *
 *
 *  returns: null
 */
void reflow_oven_init() {
	
	gpio_set_dir(PORTA_REG, PIN4, DIRIN);	//Auto = 1	Manual = 0
	gpio_set_dir(PORTA_REG, PIN5, DIRIN);	//On = 1	Off = 0
	//TODO: Set up interrupt for the on/off switch.
	
	rfl_uart_enable = 0; //gpio_read(PORTA_REG, PIN4);	//Read switch
	int start = 0;	//Start bit, read later

	if(rfl_uart_enable) {
		uart_quick_en();//Enable UART for communication
		
		uart_send_string(REFLOW_OVEN_UART, REFLOW_OVEN_MESSAGE_WELCOME);
		uart_send_string(REFLOW_OVEN_UART, REFLOW_OVEN_MESSAGE_INIT_START);
		uart_send_string(REFLOW_OVEN_UART, REFLOW_OVEN_MESSAGE_INIT_RELAY);
	}

	gpio_set_dir(PORTB_REG, PIN7, DIROUT);	//Set relay GPIO port
	
	//Message
	if(rfl_uart_enable) {
		uart_send_string(REFLOW_OVEN_UART, REFLOW_OVEN_MESSAGE_INIT_ADC);
	}
	
	adc_init(GCLK0, 0, 0x08);	//Initialize ADC 
	
	reflow_oven_set_timer();
  
	rfl_ovn_status.State = IDLE;
	rfl_ovn_status.Temp = 0x00;
	
	if(rfl_uart_enable) {
		uart_send_string(REFLOW_OVEN_UART, REFLOW_OVEN_MESSAGE_INIT_COMPLETE);
	}
	
	gpio_set_dir(PORTB_REG, PIN11 | PIN12 | PIN13 | PIN14 | PIN15, DIROUT);	//Zone LEDs
	gpio_set_out(PORTB_REG, PIN11 | PIN12 | PIN13 | PIN14 | PIN15, LOW);		//Zone LEDs set low
	
	start = gpio_read(PORTA_REG, PIN5);	//Read start switch
	
	//Attempt to start after initialization
	if(start) {
		reflow_oven_start_profile(rfl_uart_enable);
	}
	
	//If this is reached then manually start after in the main.c file by toggling the on/off switch
}

/*
 * Function:  rerflow_oven_send_command
 * -------------------------------------
 *  Command decoder
 *
 *  cmd: command that will be decoded
 *
 *  returns: null
 */
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
	int temp = adc_read();
	return temp;			//Return data
}

void reflow_oven_start_profile(int rfl_uart_enable) {
	
	/*****************ZONE 1**********************/
	reflow_oven_set(ON);
	gpio_set_out(PORTB_REG, PIN11, HIGH);	//LED
	
	if(rfl_uart_enable) {
		uart_send_string(REFLOW_OVEN_UART, REFLOW_OVEN_MESSAGE_START_PROFILE);
		uart_send_string(REFLOW_OVEN_UART, REFLOW_OVEN_MESSAGE_PROFILE_ZONE1);
	}
	
	while(reflow_oven_read_temp() <= REFLOW_OVEN_ZONE_2) { /**WAIT**/ }		
	/*****************ZONE 1 END*******************/
		
		
	/*****************ZONE 2**********************/
	reflow_oven_set(OFF);		//Turn off to keep at temperature
	reflow_oven_start_timer();	//Set timer for 90s
	gpio_set_out(PORTB_REG, PIN12, HIGH);	//LED
	
	if(rfl_uart_enable) {
		uart_send_string(REFLOW_OVEN_UART, REFLOW_OVEN_MESSAGE_PROFILE_ZONE2);
	}

	while(rfl_ovn_time < REFLOW_OVEN_ZONE_2_TIME) { /**WAIT**/ }
	reflow_oven_reset_timer();
	/*****************ZONE 2 END*******************/
	
	
	/*****************ZONE 3**********************/
	reflow_oven_set(ON);	//Increase reflow oven temp
	gpio_set_out(PORTB_REG, PIN13, HIGH);	//LED
	
	if(rfl_uart_enable) {
		uart_send_string(REFLOW_OVEN_UART, REFLOW_OVEN_MESSAGE_PROFILE_ZONE3);
	}
	
	while(reflow_oven_read_temp() <= REFLOW_OVEN_ZONE_3) { /**WAIT**/ }
	
	reflow_oven_start_timer();	//Set timer for 30s
	reflow_oven_set(OFF);	//Turn off to keep at temperature
	
	while(rfl_ovn_time < REFLOW_OVEN_ZONE_3_TIME) { /**WAIT**/ }
	reflow_oven_reset_timer();
	/*****************ZONE 3 END*******************/
	
	
	/*****************ZONE 4**********************/
	//Start zone 4, turn off the oven and open it to cool off
	reflow_oven_set(OFF);
	gpio_set_out(PORTB_REG, PIN14, HIGH);	//LED
	
	if(rfl_uart_enable) {
		uart_send_string(REFLOW_OVEN_UART, REFLOW_OVEN_MESSAGE_PROFILE_ZONE4);
	}
	
	while(reflow_oven_read_temp() >= REFLOW_OVEN_ZONE_4) { /**WAIT**/ }
	
	if(rfl_uart_enable) {
		uart_send_string(REFLOW_OVEN_UART, REFLOW_OVEN_MESSAGE_PROFILE_DONE);
	}
	
	/*****************ZONE 4 END*******************/
	
	gpio_set_out(PORTB_REG, PIN15, HIGH);
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

/*
 * Function:  reflow_oven_timer
 * --------------------
 *
 *  PUT THIS CODE IN THE TC3_Handler() FUNCTION IN YOUR MAIN.C FILE
 *  Handles the increment of every second.
 *	After a count of 20, increment rfl_ovn_time by 1
 *
 *  returns: null
 */
void reflow_oven_timer() {

	if(rfl_ovn_time_tmp >= 19) {	//Assuming FCLKD = 1MHz clock, if not then change!
		rfl_ovn_time += 1;
		rfl_ovn_time_tmp = 0;
	} else {
		rfl_ovn_time_tmp += 1;
	}
	
	TC3_REG->INTFLAG.bits.MC0 = 1;	//Must reset interrupt
}

void reflow_oven_set_timer() {

	tc_init(3, GCLK0, MFRQ, 0xC350); //50,000 timer (1 second = 20 counts if FCLK = 1MHz)
	INTERRUPT_SAMD21->ISER_SAMD21.bits.TC3 = 1;	//Enable Interrupt
}

/*
 * Function:  reflow_oven_reset_timer
 * --------------------
 *
 *  Turn off timer, reset the tc count and the reflow oven count
 *
 *  returns: null
 */
void reflow_oven_reset_timer() {
	tc_en(TC3_REG, OFF);		//TODO: Verify disabling the tc doesn't mess up the interrupt enable too
	tc_reset_cnt16(TC3_REG);	//Might be cleared after disabling the tc, but not sure.  Need to test
	rfl_ovn_time = 0;
}

inline void reflow_oven_start_timer() {
	tc_en(TC3_REG, ON);
}

/*
 * Function:  reflow_oven_reset_timer
 * --------------------
 *
 *  Turn off oven and wait until turned back on.  If you want to reset the system then use the reset button
 *  This is more of pause then a real turn off of the system
 *
 *  returns: null
 */
void reflow_oven_turn_off() {
	
	while(!gpio_read(PORTA_REG, PIN5)) {	//TODO: Test to make sure nested interrupts are acceptable.  If not then rethink this function
		reflow_oven_set(OFF);
	};
}

/* CODE TO COPY INTO MAIN.C

#include "usr_lib/reflow_oven.h"
#include "samd21.h"				//Needed for the TC3_Hanlder function

void TC3_Hanlder() { reflow_oven_timer() }	//Increments counter when enabled
	
//TODO: Add interrupt handler for UART, once it's been properly coded

int main(void) {
	
	reflow_oven_init();		//Initialize and start, if switch is set to "On" otherwise just initialize
	
	//Wait for the switch to be toggled first off then on, this avoid a situation where the reflow oven starts from
	//the init function and keeps going.  The user must reset the switch before the oven turns on again.
	while(1) {
		if(!gpio_read(PORTA_REG, PIN5)) {
			while(1) {
				if(gpio_read(PORTA_REG, PIN5) {
					reflow_oven_start_profile(rfl_uart_enable);
					break;	//End of cycle, wait for switch toggle again
				}
			}
		}
	}
}

*/