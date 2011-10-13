/*
 * Datei:               clock.c
 * Author:              dev00
 * Beschreibung:        Architekturspezifische Implementierung einer Funktion,
 *                      welche die Zeit seit Systemstart in "Ticks"
 *                      zurueckliefert.
 *
 * Aenderungsdatum:     Do, 13. Okt 2011 23:59:08
 *
 */

#include <stdint.h>
#include <avr/io.h>
#include <util/atomic.h>

#include "clock.h"

volatile uint32_t clock_counter = 0;

/*
 * Interruptroutine.
 *
 */
ISR(TIMER0_OVF_vect)
{
        clock_counter++;

        TCNT0 = 100;
}

/*
 * Initialisiert den Counter fuer die weitere Verwendung.
 *
 */
void clock_init(void)
{
        TCNT0 = 100;

        /* 1024stel des Taktes */
        TCCR0 |= (1 << CS02) | (1 << CS00);

        /* Timer Interrupt aktivieren */
        TIMSK |= (1 << TOIE0);
}

/*
 * Initialisiert den Counter fuer die weitere Verwendung. Interrupts muessen
 * aber manuell aktiviert werden.
 *
 */
uint32_t get_clock(void)
{
        uint32_t t;
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
                t = clock_counter;
        }

        return t;
}

