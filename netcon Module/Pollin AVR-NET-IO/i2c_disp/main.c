/*
 * Datei:               main.c
 * Author:              dev00
 * Beschreibung:        Testprogramm fuer die I2C Display Bibilothek.
 *
 * Aenderungsdatum:     Fr, 11. Mai 2012 00:33:42
 *
 */

#include <avr/io.h>
#include "uart.h"
#include "i2c.h"

int main(void)
{
        uart_init();
        i2c_init();

        i2c_start(0x74);
        i2c_send(0x00);
        i2c_send(0x25);
        i2c_send(0x06);
        i2c_send(0x24);
        i2c_send(0x0F);
        i2c_stop();

        uart_getchar();

        i2c_start(0x74);
        i2c_send(0x40);
        i2c_send(0xC1);
        i2c_send(0xC2);
        i2c_stop();

        while(1 > 0) {

        }

        return 0;
}

