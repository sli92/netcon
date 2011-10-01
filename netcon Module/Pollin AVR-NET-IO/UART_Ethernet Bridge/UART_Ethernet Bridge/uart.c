#include <avr/io.h>

// include 'custom' header first, for the BAUD definition used by setbaud.h
#include "uart.h"
#include <util/setbaud.h>

void uartInit(void)
{
    UCSRB = (1 << RXEN) | (1 << TXEN);                      // UART aktivieren für Senden und Empfangen

    UBRRH = UBRRH_VALUE;
    UBRRL = UBRRL_VALUE;
    

#if USE_2X
    UCSRA |= (1 << U2X);
#endif

    UCSRC = (1 << UCSZ0) | (1 << UCSZ1) | (1 << URSEL);     // 8-Bit, synchrone Übertragung
}

void uartPutChar(const char ch)
{
    if(ch == '\n')
        uartPutChar('\r');

    while(!(UCSRA & (1 << UDRE)));                          // Wartet bis das Bit UDRE 1 ist
    UDR = ch;
}

char uartCharIn(void)
{
	if(UCSRA & (1 << RXC))
	    return 1;
		
	return 0;
}

char uartGetChar(void)
{
    char ch;

    while(!(UCSRA & (1 << RXC)));                           // Wartet bis das Bit RXC 1 ist

    ch = UDR;
	
	if(ch == '\r')
	    ch = '\n';

	uartPutChar(ch);

    return ch;
}

void uartPutString(const char *string)
{
    while(*string)
        uartPutChar(*string++);
}
