/*
 * spi.h
 *
 * Created: 1/21/2018 11:48:27 AM
 *  Author: ozzie
 */ 


#ifndef SPI_H_
#define SPI_H_

#include "clock.h"
#include "pm.h"
#include "samd21.h"
#include "sercom.h"
#include "stdint-gcc.h"

//void spi_initialize(SERCOM comm, SERCOM_REG reg);
//void spi_en(SERCOM comm);

void spi_drone_setup();



#endif /* SPI_H_ */