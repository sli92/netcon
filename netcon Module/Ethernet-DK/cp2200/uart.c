#include "compiler_defs.h"
#include "C8051F340_defs.h"

#include "uart.h"

/*
 * Initialisiert den UART(9600 BAUD).
 *
 */
void uart_init(void)
{
    SCON0 |= (1 << _REN0);

    TMOD |= (1 << _T1M1);

    TH1 = 0x30;
    TL1 = TH1;

    TCON |= (1 << _TR1);
    SCON0 |= (1 << _TI0);
}

/*
 * Gibt ein Zeichen ueber die serielle Schnittstelle aus.
 *
 */
void uart_putchar(const char ch)
{
    if(ch == '\n')
        uart_putchar('\r');

    while(!(SCON0 & (1 << _TI0)));
    SCON0 &= ~(1 << _TI0);
    SBUF0 = ch;
}

/*
 * Gibt eine nullterminierte Zeichenkette ueber die serielle Schnittstelle
 * aus.
 *
 */
void uart_puts(const char *s)
{
    while(*s)
        uart_putchar(*s++);
}

/*
 * Liefert 1, wenn sich ein empfangenes Zeichem im UART Register des AVR
 * befindet, ansonten 0.
 *
 */
char uart_char_in(void)
{
    if(SCON0 & (1 << _RI0))
        return 1;

    return 0;
}

/*
 * Wartet auf ein Zeichen, und gibt dieses zurueck.
 *
 */
char uart_getchar(void)
{
    char ch;

    while(!(SCON0 & (1 << _RI0)));
    SCON0 &= ~(1 << _RI0);

    ch = SBUF0;

    if(ch == '\r')
        ch = '\n';

    uart_putchar(ch);

    return ch;
}
