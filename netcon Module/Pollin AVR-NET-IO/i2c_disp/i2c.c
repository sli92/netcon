#include <stdint.h>
#include <avr/io.h>
#include <util/twi.h>
#include "i2c.h"

void i2c_init(void)
{
        // 400kHz
        TWBR = 0x0C;
        TWSR = 0x00;
}

void i2c_start(uint8_t addr)
{
        TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
        while(!(TWCR & (1 << TWINT)));

        TWDR = addr;
        TWCR = (1 << TWINT) | (1 << TWEN);
        while(!(TWCR & (1 << TWINT)));
}

void i2c_send(uint8_t data)
{
        TWDR = data;
        TWCR = (1 << TWINT) | (1 << TWEN);

        while(!(TWCR & (1 << TWINT)));
}

void i2c_stop(void)
{
        TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
        while(!(TWCR & (1 << TWINT)));
}

