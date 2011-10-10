#ifndef __TCPAPP_H__
#define __TCPAPP_H__

#define TCP_UART_STATE_IDLE  0
#define TCP_UART_STATE_SENT  1
#define TCP_UART_STATE_ACKED 2

struct uipTcpAppState
{
    uint8_t state;
};

void tcpAppInit(void);
void tcpAppCall(void);

#endif
