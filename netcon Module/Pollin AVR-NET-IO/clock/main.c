/*
 * Datei:               main.c
 * Author:              dev00
 * Beschreibung:        Testet die Funktionen der "clock" Bibilothek.
 *
 * Aenderungsdatum:     Do, 13. Okt 2011 23:58:51
 *
 */

#include <avr/interrupt.h>
#include "clock.h"

int main(void)
{
        uint32_t lasttick;
        uint32_t current;


        clock_init();
        lasttick = get_clock();

        sei();
        while(1) {
                current = get_clock();
                if((current - lasttick) > CLOCK_TICKS_PER_SECOND) {

                }
        }

        return 0;
}

