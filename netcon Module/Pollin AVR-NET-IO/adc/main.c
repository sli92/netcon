/*
 * Datei:               main.c
 * Author:              dev00
 * Beschreibung:        Testprogramm fuer die ADC Bibilothek.
 *
 * Aenderungsdatum:     Do, 12. Jän 2012 14:59:19
 *
 */

#include "uart.h"
#include "adc.h"

int main(void)
{
        char buffer[32];

        uart_init();
        uart_puts("Program started\n");
        adc_init();

        while(1 > 0) {
                sprintf(buffer, "ADC0: %d\n", adc_get_value(0));
                uart_puts(buffer);
        }

        return 0;
}

