/*
 * Datei:               main.c
 * Author:              dev00
 * Beschreibung:        Testet die Funktionen der "clock" Bibilothek.
 *
 * Aenderungsdatum:     Fr, 21. Okt 2011 00:47:10
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
        while(1 > 0) {
                current = get_clock();
                if((current - lasttick) > CLOCK_TICKS_PER_SECOND) {
                        lasttick = current;
                        uart_puts("Second\n");
                }
        }

        return 0;
}

