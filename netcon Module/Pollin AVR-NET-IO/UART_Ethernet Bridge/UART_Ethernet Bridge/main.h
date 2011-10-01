#ifndef __MAIN_H__
#define __MAIN_H__

#define UART_LINE_BUFFER_READY_TO_SEND 0x01
#define UART_LINE_BUFFER_READY_TO_FILL 0x02

#define UART_LINE_BUFFER_SIZE 32

extern char uartLineBuffer[UART_LINE_BUFFER_SIZE];
// free position after the last char
extern uint8_t uartLineBufferPos;
extern uint8_t uartLineBufferFlags;

#endif
