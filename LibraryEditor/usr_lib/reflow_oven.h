/*
 * reflow_oven.h
 *
 * Created: 10/22/2018 7:11:28 PM
 *  Author: ozzie
 */ 


#ifndef REFLOW_OVEN_H_
#define REFLOW_OVEN_H_

#include "samd21.h"
#include "uart.h"
#include "gpio.h"
#include "adc.h"
#include "string.h"
#include "pwm.h"

typedef enum {
	IDLE,
	ZONE1,
	ZONE2,
	ZONE3,
	ZONE4
} rfl_ovn_states_t;

typedef struct {
	rfl_ovn_states_t	State;
	uint8_t				RelayState;
	uint16_t			Temp;
}rfl_ovn_status_t;

rfl_ovn_status_t rfl_ovn_status;

volatile uint16_t rfl_ovn_time;
volatile uint16_t rfl_ovn_time_tmp;

//UART Commands
#define REFLOW_OVEN_STATUS_CMD		0x00
#define REFLOW_OVEN_START_CMD		0x01
#define REFLOW_OVEN_STOP_CMD		0x02
#define REFLOW_OVEN_GET_TEMP_CMD	0x03

static char REFLOW_OVEN_COMMANDS[][3] = {"STS","STR","STP","TMP"};
	
#define REFLOW_OVEN_UART		UART3_REG

//Temperature thresholds
#define REFLOW_OVEN_ZONE_1		0x0139	//25C			:Initial
#define REFLOW_OVEN_ZONE_2		0x0735	//150C			:90s stable
#define REFLOW_OVEN_ZONE_3		0x0C35  //245C ~ 250C	:30s stable 
#define REFLOW_OVEN_ZONE_4		0x0177  //30C	

//Time constants for zone 2 and 3
#define REFLOW_OVEN_ZONE_2_TIME 10 //90	//seconds
#define REFLOW_OVEN_ZONE_3_TIME 30	//seconds

#define REFLOW_OVEN_MESSAGE_WELCOME			"Welcome to Easy Reflow\nVersion 1.00\r\n\r\n"
#define REFLOW_OVEN_MESSAGE_INIT_START		"Initialization process starting...\r\n\r\n"
#define REFLOW_OVEN_MESSAGE_INIT_ADC		"Initializing ADC.\r\n"
#define REFLOW_OVEN_MESSAGE_INIT_RELAY		"Initializing relay control.\r\n"
#define REFLOW_OVEN_MESSAGE_INIT_COMPLETE	"Initialization process complete!\r\n\r\n"
#define REFLOW_OVEN_MESSAGE_INIT_FAILED		"Initialization process failed!\r\n\r\n"
#define REFLOW_OVEN_MESSAGE_START_PROFILE	"Starting reflow oven profile...\r\n\r\n"
#define REFLOW_OVEN_MESSAGE_PROFILE_ZONE1	"ZONE 1 Start.\r\n"
#define REFLOW_OVEN_MESSAGE_PROFILE_ZONE2	"ZONE 2 Start.\r\n"
#define REFLOW_OVEN_MESSAGE_PROFILE_ZONE3	"ZONE 3 Start.\r\n"
#define REFLOW_OVEN_MESSAGE_PROFILE_ZONE4	"ZONE 4 Start, open oven door to rapidly cool oven!\r\n"
#define REFLOW_OVEN_MESSAGE_PROFILE_DONE	"Profile complete!\r\n"

#define REFLOW_OVEN_MESSAGE_TEMP_STATUS(x)  ("The temperature is: " #x "C\r\n")
#define REFLOW_OVEN_MESSAGE_RLY_STATUS(x)	("Relay is " x)

//char REFLOW_OVEN_START[] = "STR";
//char REFLOW_OVEN_STOP[] = "STP";
//char REFLOW_OVEN_TEMP[] = "TMP";

char reflow_oven_user_input[100];
static int reflow_oven_user_input_cnt = 0;
static int rfl_uart_enable = 0;

void reflow_oven_init();
void reflow_oven_send_command(uint8_t cmd);
uint16_t reflow_oven_read_temp();
void reflow_oven_start_profile();
void reflow_oven_stop_profile();
int reflow_oven_get_temperature();
void reflow_oven_set(int on);
void reflow_oven_check_user_input();
void reflow_oven_timer();
void reflow_oven_set_timer();
void reflow_oven_reset_timer();
void reflow_oven_start_timer(void);
void reflow_oven_turn_off();
void reflow_oven_toggle_heater();

void reflowOvenZone1(void);
void reflowOvenZone2(void);
void reflowOvenZone3(void);
void reflowOvenZone4(void);

#endif /* REFLOW_OVEN_H_ */
