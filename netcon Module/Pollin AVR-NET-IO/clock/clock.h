/*
 * Datei:               clock.h
 * Author:              dev00
 * Beschreibung:        Architekturspezifische Implementierung einer Funktion,
 *                      welche die Zeit seit Systemstart in "Ticks"
 *                      zurueckliefert.
 *
 * Aenderungsdatum:     Di, 25. Okt 2011 23:29:41
 *
 */

#ifndef __CLOCK_H__
#define __CLOCK_H__

#include <stdint.h>

/*
 * Gilt nur fuer F_CPU = 16MHz.
 *
 */
#define CLOCK_TICKS_PER_SECOND  100

/*
 * Initialisiert den Counter fuer die weitere Verwendung. Interrupts muessen
 * aber manuell aktiviert werden.
 *
 */
void clock_init(void);

/*
 * Liefert die vergangenen Ticks seit Systemstart.
 *
 */
uint32_t get_clock(void);

#endif

