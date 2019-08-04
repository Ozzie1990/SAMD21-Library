

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include "samd21.h"

#define CMD_SYSTEM_STATUS 0x00
#define CMD_TURN_OFF_LIGHT 0x01

typedef struct {
    uint16_t STATUS = 0x00;
    uint16_t HEALTH_CHECK = 0x01;       //0x01
    uint16_t TURN_OFF_LIGHT;     //0x02
    uint16_t TURN_ON_LIGHT;      //0x03
    uint16_t OPEN_DOOR;
    uint16_t CLOSE_DOOR;
}commands;

void send_command(commands cmd);



#endif  //COMMANDS_H_