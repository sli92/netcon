#include "i2c.h"

void i2c_init(void)
{
        // 400kHz
        TWBR = 3;
        TWSR |= (1 << TWPS0);

        TWCR |= (1 << TWEA) | (1 << TWEN);
}

