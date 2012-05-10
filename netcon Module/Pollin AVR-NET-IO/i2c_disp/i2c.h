#ifndef __I2C_H__
#define __I2C_H__

#include <stdint.h>

void i2c_init(void);
void i2c_start(uint8_t addr);
void i2c_send(uint8_t data);
void i2c_stop(void);










#endif

