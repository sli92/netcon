#include "compiler_defs.h"
#include "C8051F340_defs.h"

#include <stdint.h>

#include "uart.h"
#include "main.h"
#include "clock.h"

void timer2_overflow(void) __interrupt(5);

int main(void)
{
    uint32_t lasttick = 0x00;
    uint32_t current = 0x00;

    /* Watchdog deaktivieren */
    PCA0MD &= ~(1 << 6);

    sysclk_init();
    io_init();
    uart_init();
    clock_init();

    IE |= (1 << 7);

    while(1 > 0) {
        current = get_clock();
        if((current - lasttick) > CLOCK_TICKS_PER_SECOND) {
            lasttick = current;
            uart_puts("Second\n");
        }
    }

    return 0;
}

/*
 * Initialisiert den Takt auf die maximale Frequenz, beim C8051F340 sind dies
 * 48MHz.
 *
 */
void sysclk_init(void)
{
    OSCICN |= (1 << 1) | (1 << 0);

    /* Siehe C8051F340 Datenblatt, Seite 138 */
    CLKMUL = 0x00;

    CLKMUL |= (1 << 7);
    delay_40us();

    CLKMUL |= (1 << 6);

    while(!(CLKMUL & (1 << 5)));

    CLKSEL |= (1 << 1) | (1 << 0);
}

/*
 * Initialisiert die verschiedenen Ports.
 *
 */
void io_init(void)
{
    /* TX0 als Push-Pull Ausgang */
    P0MDOUT |= (1 << 4);

    XBR0 |= (1 << 0);
    XBR1 |= (1 << 6);
}

/*
 * Wartet ungefaehr 40us, Code aus dem SiLabs UART Beispiel.
 *
 */
void delay_40us(void)
{
    int x;
    for(x = 0;x < 500;x)
        x++;
}
