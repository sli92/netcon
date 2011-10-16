/*
 * Datei:               main.c
 * Author:              dev00
 * Beschreibung:        Testet die Funktionen der "clock" Bibilothek.
 *
 * Aenderungsdatum:     Sa, 15. Okt 2011 12:28:48
 *
 */

#include <stdint.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "clock.h"

int main(void)
{
        uint32_t lasttick;
        uint32_t current;

        uart_init();
        clock_init();
        lasttick = get_clock();

        sei();
        while(1) {
                current = get_clock();
                if((current - lasttick) > CLOCK_TICKS_PER_SECOND) {
                        lasttick = current;
                        uart_puts("Second\n");
                }
        }

        return 0;
}

