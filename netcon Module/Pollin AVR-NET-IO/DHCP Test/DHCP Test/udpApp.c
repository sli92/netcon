#include <stdlib.h>

#include "uip/uip.h"
#include "udpApp.h"
#include "uart.h"
#include "main.h"

struct udpDhcpClientState dhcpState;

void udpAppInit(void)
{
    uip_ipaddr_t ipaddr;
	
	uip_ipaddr(ipaddr, 255, 255, 255, 255);
	dhcpState.connection = uip_udp_new(&ipaddr, HTONS(DHCP_SERVER_PORT));
	if(dhcpState.connection != NULL)
	{
        uip_udp_bind(dhcpState.connection, HTONS(DHCP_CLIENT_PORT));
        uartPutString("UDP Socket bound\n");
	}
	
	// Use random number later
	dhcpState.xid = 0xDEADBEEF;
	
	dhcpState.state = UDP_DHCP_STATE_INITIAL;
}

void udpAppCall(void)
{
	if(uip_poll())
	{
		switch(dhcpState.state)
		{
			case UDP_DHCP_STATE_INITIAL:
			    dhcpSendDiscover();
			    break;
		}
	}
	
	if(uip_newdata())
	{
        switch(dhcpState.state)
        {
			case UDP_DHCP_STATE_DISCOVER:
			    uartPutString("Some packet joined the party\n");
			    break;
		}			
	}
}

void dhcpSendDiscover(void)
{
	uartPutString("Sending Discover\n");
	
	uint8_t *optPtr;
	struct dhcpMessage *message = (struct dhcpMessage *)uip_appdata;
	
	message->op = 1;
	message->htype = 1;
	message->hlen = 6;
	message->hops = 0;
	message->xid = dhcpState.xid;
	message->secs = 0;
	message->flags = 0;
	
	memset(message->ciaddr, 0, sizeof(message->ciaddr));
	memset(message->yiaddr, 0, sizeof(message->yiaddr));
	memset(message->siaddr, 0, sizeof(message->siaddr));
	memset(message->giaddr, 0, sizeof(message->giaddr));
	
	memcpy(message->chaddr, MACAddr, sizeof(MACAddr));
	// I lolled, this fills of the other bytes of the device address with 0
	memset(&(message->chaddr[sizeof(MACAddr)]), 0, sizeof(message->chaddr) - sizeof(MACAddr));
	
	memset(message->sname, 0, sizeof(message->sname));
	memset(message->file, 0, sizeof(message->file));
	
	optPtr = message->options;
	
	// magic cookie
    *optPtr++ = 0x63;
    *optPtr++ = 0x82;
    *optPtr++ = 0x53;
    *optPtr++ = 0x63;
	
	// DISCOVER
	*optPtr++ = 53;
	*optPtr++ = 1;
	*optPtr++ = 1;
	
    *optPtr++ = 55;
    *optPtr++ = 3;
    *optPtr++ = 1;
    *optPtr++ = 3;
    *optPtr++ = 6;
	
	*optPtr++ = 255;
	
	
	
	uartPutString("Data dump:\n");
	
	char buffer[5];
	for(int i = 0; i < (optPtr - (uint8_t *)uip_appdata); i++)
	{
		sprintf(buffer, "%d ", ((char *)uip_appdata)[i]);
	    uartPutString(buffer);
	}
	
    uartPutString("Discover send\n");
	
	uip_send(uip_appdata, optPtr - (uint8_t *)uip_appdata);
    
	dhcpState.state = UDP_DHCP_STATE_DISCOVER;
}
