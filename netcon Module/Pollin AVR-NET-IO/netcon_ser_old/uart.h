/*
 * Datei:               uart.h
 * Author:              dev00
 * Beschreibung:        Stellt Funktionen für eine Kommunikation über die
 *                      serielle Schnittstelle zur Verfuegung. (9600 Baud)
 *
 * Aenderungsdatum:     Do, 03. Mai 2012 11:27:59
 *
 */

#ifndef __UART_H__
#define __UART_H__

#define BAUD 9600UL

/*
 * Initialisiert den UART.
 *
 */
void uart_init(void);

/*
 * Gibt ein Zeichen ueber die serielle Schnittstelle aus.
 *
 */
void uart_putchar(const char ch);

/*
 * Gibt eine nullterminierte Zeichenkette ueber die serielle Schnittstelle
 * aus.
 *
 */
void uart_puts(const char *s);

/*
 * Gibt eine nullterminierte Zeichenkette aus dem Programmspeicher ueber
 * die serielle Schnittstelle aus.
 *
 */
void uart_puts_P(const char *s);

/*
 * Liefert 1, wenn sich ein empfangenes Zeichem im UART Register des AVR
 * befindet, ansonten 0.
 *
 */
char uart_char_in(void);

/*
 * Wartet auf ein Zeichen, und gibt dieses zurueck.
 *
 */
char uart_getchar(void);

char uart_getchar_ne(void);

#endif

