

#ifndef RC_CONTROLLER_H_
#define RC_CONTROLLER_H_

#include "pwm.h"
#include "adc.h"
#include "gpio.h"
#include "samd21.h"

#define ERROR	0x00

//Motor Speed
uint16_t leftMtrSpd;	//Left Motor
uint16_t rightMtrSpd;	//Right Motor

//Distance 
uint32_t frntDist;	//Front (incase more sensors are added in the future)

//Speed
uint16_t rcSpeed;

void initRC();
data_t readSonar();
void turnRight(int degrees);
void turnLeft(int degrees);
void setRightSpeed(int speed);
void setLeftSpeed(int speed);
void moveForward(int distance, int speed);
void moveBackwards(int distance);
void setSpeed(uint16_t speed);
void stopRC();

#endif