/*
 * Datei:               clock.h
 * Author:              dev00
 * Beschreibung:        Architekturspezifische Implementierung einer Funktion,
 *                      welche die Zeit seit Systemstart in "Ticks"
 *                      zurueckliefert.
 *
 */

#ifndef __CLOCK_H__
#define __CLOCK_H__

#include <stdint.h>

/*
 * Gilt nur fuer F_CPU = 16MHz.
 *
 */
#define CLOCK_TICKS_PER_SECOND (100)

/*
 * Initialisiert den Counter fuer die weitere Verwendung. Interrupts muessen
 * aber manuell aktiviert werden.
 *
 */
void init_clock(void);

/*
 * Liefert die vergangenen Ticks seit Systemstart.
 *
 */
uint32_t get_clock(void);

#endif

