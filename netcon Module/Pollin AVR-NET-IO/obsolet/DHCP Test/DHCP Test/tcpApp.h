#ifndef __TCPAPP_H__
#define __TCPAPP_H__

struct uipTcpAppState
{
    uint8_t state;
};

void tcpAppInit(void);
void tcpAppCall(void);

#endif
