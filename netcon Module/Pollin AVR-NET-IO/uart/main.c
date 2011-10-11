/*
 * Datei:               main.c
 * Author:              dev00
 * Beschreibung:        Testprogramm fuer die UART Bibilothek.
 *
 */

#include "uart.h"

int main(void)
{
        init_uart();
        uart_puts("Hallo Welt!\n");

        while(1) {

        }

        return 0;
}

