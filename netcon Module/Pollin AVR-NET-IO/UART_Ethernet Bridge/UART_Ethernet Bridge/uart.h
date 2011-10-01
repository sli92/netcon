#ifndef __UART_H__
#define __UART_H__

#define BAUD 9600UL

void uartInit(void);

void uartPutChar(const char ch);
void uartPutString(const char *string);

char uartCharIn(void);
char uartGetChar(void);


#endif
