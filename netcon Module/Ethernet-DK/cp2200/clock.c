#include "compiler_defs.h"
#include "C8051F340_defs.h"

#include <stdint.h>

#include "clock.h"

static volatile uint32_t clock_counter = 0;

/*
 * Interruptroutine.
 *
 */
void timer2_overflow(void) __interrupt(5)
{
    __critical {
        clock_counter++;

        TF2H = 0x00;
    }
}

/*
 * Initialisiert den Counter fuer die weitere Verwendung. Interrupts muessen
 * aber manuell aktiviert werden.
 *
 */
void clock_init(void)
{
    TMR2RL = 25536;
    TMR2 = TMR2RL;

    TMR2CN |= (1 << 2);
    IE |= (1 << 5);
}

/*
 * Liefert die vergangenen Ticks seit Systemstart.
 *
 */
uint32_t get_clock(void)
{
    uint32_t t;

    __critical {
        t = clock_counter;
    }

    return t;
}

