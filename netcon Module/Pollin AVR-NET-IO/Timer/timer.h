/*
 * Datei:               timer.h
 * Author:              dev00
 * Beschreibung:        Stellt einen Timer zur Verfuegung.
 *
 */

#ifndef __TIMER_H__
#define __TIMER_H__

#include <stdint.h>

struct timer {
        uint32_t start;
        uint32_t interval;
};

/*
 * Diese Funktion sollte in regemlaessiegen Zeitabstaenden aufgerufen werden,
 * die Dauer dieser Zeitabstaende bestimmt die Aufloesung des Timers.
 *
 */
void timer_tick(void);

/*
 * Liefert die Anzahl der Ticks seit dem Systemstart.
 *
 */
uint32_t get_tick_count(void);

void timer_set(struct timer *t);
uint8_t timer_expired(struct timer *t);

#endif /*__TIMER_H__*/

