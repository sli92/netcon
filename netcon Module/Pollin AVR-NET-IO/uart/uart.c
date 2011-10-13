/*
 * Datei:               uart.c
 * Author:              dev00
 * Beschreibung:        Stellt Funktionen für eine Kommunikation über die
 *                      serielle Schnittstelle zur Verfuegung. (9600 Baud)
 *
 */

#include <avr/io.h>
#include <avr/pgmspace.h>

#include "uart.h"
#include <util/setbaud.h>

/*
 * Initialisiert den UART.
 *
 */
void uart_init(void)
{
        UCSRB = (1 << RXEN) | (1 << TXEN);

        UBRRH = UBRRH_VALUE;
        UBRRL = UBRRL_VALUE;

        #if USE_2X
                UCSRA |= (1 << U2X);
        #else
                UCSRA &= ~(1 << U2X);
        #endif

        UCSRC = (1 << UCSZ0) | (1 << UCSZ1) | (1 << URSEL);
}

/*
 * Gibt ein Zeichen ueber die serielle Schnittstelle aus.
 *
 */
void uart_putchar(const char ch)
{
        if(ch == '\n')
                uart_putchar('\r');

        while(!(UCSRA & (1 << UDRE)));

        UDR = ch;
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
 * Gibt eine nullterminierte Zeichenkette aus dem Programmspeicher ueber
 * die serielle Schnittstelle aus.
 *
 */
void uart_puts_p(const char *s)
{
        while(pgm_read_byte(s))
                uart_putchar(pgm_read_byte(s++));
}

/*
 * Liefert 1, wenn sich ein empfangenes Zeichem im UART Register des AVR
 * befindet, ansonten 0.
 *
 */
char uart_char_in(void)
{
        if(UCSRA & (1 << RXC))
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

        while(!(UCSRA & (1 << RXC)));

        ch = UDR;

        if(ch == '\r')
                ch = '\n';

        uart_putchar(ch);

        return ch;
}

