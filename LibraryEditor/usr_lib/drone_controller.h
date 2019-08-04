/*
 * drone_controller.h
 *
 * Created: 9/3/2017 3:07:31 PM
 *  Author: ozzie
 */ 


#ifndef DRONE_CONTROLLER_H_
#define DRONE_CONTROLLER_H_

#include "pwm.h"
#include "clock.h"
#include "gpio.h"
#include "pm.h"

//Custom commands
#define ACCEL_ERROR				0x00
#define ACCEL_READ_COMPLETE		0x01

//Motor Speeds
#define MAX_SPEED           0x0000
#define MIN_SPEED           0xFFFF

//Axis Declaration
#define X_AXIS              0
#define Y_AXIS              1
#define Z_AXIS              2

//Command list for the Adafruit accelerometer
#define OUT_X_MSB			0x01
#define OUT_X_LSB			0x02
#define OUT_Y_MSB			0x03
#define OUT_Y_LSB			0x04
#define OUT_Z_MSB			0x05
#define OUT_Z_LSB			0x06
#define F_SETUP				0x0A
#define TRIG_CFG			0x0B
#define SYSMOD				0x0C
#define INT_SOURCE			0x0D
#define WHO_AM_I			0x0E
#define XYZ_DATA_CFG		0x0F
#define HP_FILTER_CUTOFF	0x10
#define PL_STATUS			0x11
#define PL_CFG				0x12
#define PL_COUNT			0x13
#define PL_BF_ZCOMP			0x14
#define P_L_THS_REG			0x15
#define FF_MT_CFG			0x16
#define FF_MT_SRC			0x17
#define FF_MT_THS			0x18
#define FF_MT_COUNT			0x19

//Motor speed
uint16_t motor1Speed;
uint16_t motor2Speed;
uint16_t motor3Speed;
uint16_t motor4Speed;

//PID X Gain
uint16_t pidXDiffGain;
uint16_t pidXIntGain;
uint16_t pidXPropGain;

//PID Y Gain
uint16_t pidYDiffGain;
uint16_t pidYIntGain;
uint16_t pidYPropGain;

//PID Z Gain
uint16_t pidZDiffGain;
uint16_t pidZIntGain;
uint16_t pidZPropGain;

//PID X Input and Output
uint16_t pidXInput;
uint16_t pidXOutput;

//PID Y Input and Output
uint16_t pidYInput;
uint16_t pidYOutput;

uint32_t delay;

uint32_t diff_accel_val;
uint32_t int_accel_val;
uint32_t prop_accel_val;

int drone_xInput;
int drone_yInput;

int drone_plant_x_output;
int drone_plant_y_output;


/**Primary Functions**/

void initialize_drone();    
void drone_auto_stabilizer();   //Test code
void setMotor1Speed(uint16_t speed);
void setMotor2Speed(uint16_t speed);
void setMotor3Speed(uint16_t speed);
void setMotor4Speed(uint16_t speed);

void updateMotors();
void resetMotorSpeed();
int diff_accel(int diff_in);
int int_accel(int int_in);
int PID_Handler(int diffGain, int intGain, int propGain, int PID_In);
void drone_x_system(int input); 
void drone_y_system(int input);
int plant(int axis, int input);
int setPlant(int axis, int pidInput, int diffGain, int intGain, int propGain);
int getAllAxis(int * x, int * y, int * z);
int getXAxis();
int getYAxis();
int getZAxis();
int read_ADC(int data);
int read_accel_multi(int * x, int * y, int * z);
int accel_cmd(int addr, int cmd);

#endif /* DRONE_CONTROLLER_H_ */