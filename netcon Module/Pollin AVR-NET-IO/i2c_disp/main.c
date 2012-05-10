/*
 * Datei:               main.c
 * Author:              dev00
 * Beschreibung:        Testprogramm fuer die I2C Display Bibilothek.
 *
 * Aenderungsdatum:     Do, 10. Mai 2012 13:55:53
 *
 */

#include "uart.h"
#include "i2c.h"

int main(void)
{
        uart_init();
        uart_puts("Hallo Welt!\n");
        uart_puts("Das ist ein Test\n");

        while(1 > 0) {

        }

        return 0;
}

