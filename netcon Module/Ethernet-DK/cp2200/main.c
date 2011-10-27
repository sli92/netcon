#include "compiler_defs.h"
#include "C8051F340_defs.h"

#include <stdint.h>

#include "uart.h"
#include "main.h"
#include "clock.h"
#include "cp2200.h"

void timer2_overflow(void) __interrupt(5);

int main(void)
{
    uint32_t lasttick = 0x00;
    uint32_t current = 0x00;

    /* Watchdog deaktivieren */
    PCA0MD &= ~(1 << 6);

    sysclk_init();
    io_init();
    emif_init();
    clock_init();
    uart_init();

    uart_puts("Preinit\n");
    cp2200_init();
    uart_puts("Postinit\n");

    IE |= (1 << _EA);

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
    OSCICN |= (1 << _IFCN1) | (1 << _IFCN0);

    /* Siehe C8051F340 Datenblatt, Seite 138 */
    CLKMUL = 0x00;

    CLKMUL |= (1 << _MULEN);

    delay_40us();

    CLKMUL |= (1 << _MULINIT);

    while(!(CLKMUL & (1 << _MULRDY)));

    CLKSEL |= (1 << _CLKSL1) | (1 << _CLKSL0);

    FLSCL |= (1 << _FLRT);
}

/*
 * Initialisiert die verschiedenen Ports.
 *
 */
void io_init(void)
{
    /* TX0 als Push-Pull Ausgang */
    P0MDOUT |= (1 << 0);

    XBR0 |= (1 << 0);
    XBR1 |= (1 << 6);


    /* /WR und /RD als Push-Pull Ausgang */
    P1MDOUT |= (1 << 7) | (1 << 6);

    /*
     * Code ist direkt aus dem SiLabs Beispiel, schöner machen.
     *
     */
    // Configure External Memory Interface Pins to push-pull except for
    // P2.0 and P2.1 which are used for target board switches.
    P2MDOUT |= 0xFC;                    // ADDR[15:8]
    P3MDOUT |= 0xFF;                    // ADDR[7:0]
    P4MDOUT |= 0xFF;                    // DATA[7:0]

    // Set initial values of External Memory Interface Pins
    P1 = 0xFE;                          // /WR, /RD, are high, RESET is low
    P1 &= ~0x18;                        // LEDs off
    P2 = 0xFF;                          // ADDR[15:8] initially high 
    P3 = 0xFF;                          // ADDR[7:0] initially high
    P4 = 0xFF;                          // DATA[7:0] intially high
}

/*
 * Code ist direkt aus dem SiLabs Beispiel, schöner machen.
 *
 */
void emif_init(void)
{
   EMI0CF = 0x1B;                      // non-muxed mode; split mode 
                                       // with bank select

   EMI0TC = 0xFF;                      // slowest timing (4-cycle MOVX)

   EMI0CN = 0x20;                      // Off-chip
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

void delay_1ms(void)
{
}
