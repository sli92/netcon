#ifndef __UDPAPP_H_
#define __UDPAPP_H_

struct uipUdpAppState
{
    uint8_t state;
};

void udpAppInit(void);
void udpAppCall(void);

#endif