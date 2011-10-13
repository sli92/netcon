/*
 * Datei:               main.c
 * Author:              dev00
 * Beschreibung:        Testprogramm fuer die UART Bibilothek.
 *
 * Aenderungsdatum:     Do, 13. Okt 2011 23:57:51
 *
 */

#include "uart.h"

int main(void)
{
        uart_init();
        uart_puts("Hallo Welt!\n");
        uart_puts("Das ist ein Test\n");

        while(1) {

        }

        return 0;
}

