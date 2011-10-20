/*
 * Datei:               main.c
 * Author:              dev00
 * Beschreibung:        Testprogramm fuer die UART Bibilothek.
 *
 * Aenderungsdatum:     Fr, 21. Okt 2011 00:47:00
 *
 */

#include "uart.h"

int main(void)
{
        uart_init();
        uart_puts("Hallo Welt!\n");
        uart_puts("Das ist ein Test\n");

        while(1 > 0) {

        }

        return 0;
}

