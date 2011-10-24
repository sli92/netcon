#include <stdint.h>

#include "clock.h"

static volatile uint32_t clock_counter = 0;

/*
 * Interruptroutine.
 *
 */
/*
ISR(TIMER0_OVF_vect)
{
        clock_counter++;

        TCNT0 = 100;
}
*/

/*
 * Initialisiert den Counter fuer die weitere Verwendung.
 *
 */
void clock_init(void)
{
}

/*
 * Initialisiert den Counter fuer die weitere Verwendung. Interrupts muessen
 * aber manuell aktiviert werden.
 *
 */
uint32_t get_clock(void)
{
        uint32_t t;

        // ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        // {
                t = clock_counter;
        // }

        return t;
}

