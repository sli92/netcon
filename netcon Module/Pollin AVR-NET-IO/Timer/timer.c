/*
 * Datei:               timer.c
 * Author:              dev00
 * Beschreibung:        Stellt einen Timer zur Verfuegung.
 *
 */

#include <stdint.h>
#include "timer.h"

uint32_t tick_counter = 0;

/*
 * Diese Funktion sollte in regemlaessiegen Zeitabstaenden aufgerufen werden,
 * die Dauer dieser Zeitabstaende bestimmt die Aufloesung des Timers.
 *
 */
void timer_tick(void)
{
        tick_counter++;
}

/*
 * Liefert die Anzahl der Ticks seit dem Systemstart.
 *
 */
uint32_t get_tick_count(void)
{
        return tick_counter;
}

