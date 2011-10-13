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
        uip_udp_bind(dhcpState.connection, HTONS(DHCP_CLIENT_PORT));
	
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
			    dhcpParseOffer();
			    break;
				
			case UDP_DHCP_STATE_REQUEST:
			    dhcpFinish();
			    break;
		}			
	}
}

void dhcpFinish(void)
{
    uint8_t *optPtr;
    struct dhcpMessage *message = (struct dhcpMessage *)uip_appdata;
	
	if((message->op != 2) || (message->xid != dhcpState.xid))
	{
		uartPutString("Packet was not a DHCP response.\n");
		return;
	}
	
	optPtr = message->options;
	optPtr += 4;
	
    uint8_t op = 0;
    uint8_t len = 0;
	
	while(op != 255)
	{
		op = *optPtr++;
		len = *optPtr++;
		
		if(op == 53 && optPtr[0] == 5)
		{
			uartPutString("ACK get!\n");
			
            uip_sethostaddr(&dhcpState.ipAddr);
            uip_setnetmask(&dhcpState.subnetMask);
            uip_setdraddr(&dhcpState.gatewayAddr);
			
			dhcpState.state = UDP_DHCP_STATE_FINISH;
			break;
		}
		
		optPtr += len;
	}
}

void dhcpParseOffer(void)
{
	uint8_t *optPtr;
	struct dhcpMessage *message = (struct dhcpMessage *)uip_appdata;
	
	if((message->op != 2) || (message->xid != dhcpState.xid))
	{
		uartPutString("Packet was not a DHCP response.\n");
		return;
	}

    memcpy(&dhcpState.ipAddr, message->yiaddr, sizeof(dhcpState.ipAddr));

	optPtr = message->options;
	
	
	char buffer[40];
	
	// Skip magic cookie
	optPtr += 4;
	
	uint8_t len = 0;
	uint8_t op = 0;
	
	while(op != 255)
	{
		op = *optPtr++;
		len = *optPtr++;
		
		switch(op)
		{
			case 1:
			    uip_ipaddr(&dhcpState.subnetMask, optPtr[0], optPtr[1], optPtr[2], optPtr[3]);
			    break;

			case 3:
			    uip_ipaddr(&dhcpState.gatewayAddr, optPtr[0], optPtr[1], optPtr[2], optPtr[3]);
			    break;
				
			case 51:
			    dhcpState.leaseTime = (optPtr[3]) | (optPtr[2] << 8) | (optPtr[1] << 16) | (optPtr[0] << 24);
			    break;
				
			case 54:
			    uip_ipaddr(&dhcpState.dhcpServerAddr, optPtr[0], optPtr[1], optPtr[2], optPtr[3]);
			    break;
		}
		
		optPtr += len;
	}
	
	sprintf(buffer, "IP: %d.%d.%d.%d\n", uip_ipaddr1(&dhcpState.ipAddr),
                                         uip_ipaddr2(&dhcpState.ipAddr),
                                         uip_ipaddr3(&dhcpState.ipAddr),
                                         uip_ipaddr4(&dhcpState.ipAddr));
    uartPutString(buffer);
	
	sprintf(buffer, "Subnetmask: %d.%d.%d.%d\n", uip_ipaddr1(&dhcpState.subnetMask),
                                                 uip_ipaddr2(&dhcpState.subnetMask),
                                                 uip_ipaddr3(&dhcpState.subnetMask),
                                                 uip_ipaddr4(&dhcpState.subnetMask));
    uartPutString(buffer);
	
    sprintf(buffer, "Gateway: %d.%d.%d.%d\n", uip_ipaddr1(&dhcpState.gatewayAddr),
                                              uip_ipaddr2(&dhcpState.gatewayAddr),
                                              uip_ipaddr3(&dhcpState.gatewayAddr),
                                              uip_ipaddr4(&dhcpState.gatewayAddr));
    uartPutString(buffer);
	
	sprintf(buffer, "DHCP Server: %d.%d.%d.%d\n", uip_ipaddr1(&dhcpState.dhcpServerAddr),
                                                  uip_ipaddr2(&dhcpState.dhcpServerAddr),
                                                  uip_ipaddr3(&dhcpState.dhcpServerAddr),
                                                  uip_ipaddr4(&dhcpState.dhcpServerAddr));
    uartPutString(buffer);
	
	sprintf(buffer, "Lease time: %u s\n", dhcpState.leaseTime);
	uartPutString(buffer);

    /// Make packet -.-
	message->op = 1;
	message->htype = 1;
	message->hlen = 6;
	message->hops = 0;
	message->xid = dhcpState.xid;
	message->secs = 0;
	message->flags = 0;
	
	memset(message->ciaddr, 0, sizeof(message->ciaddr));
	memset(message->yiaddr, 0, sizeof(message->yiaddr));
	// siaddr is already set right
	// memset(message->siaddr, 0, sizeof(message->siaddr));
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
	
	// REQUEST
	*optPtr++ = 53;
	*optPtr++ = 1;
	*optPtr++ = 3;
	
    *optPtr++ = 50;
    *optPtr++ = 4;
    *optPtr++ = uip_ipaddr1(&dhcpState.ipAddr);
    *optPtr++ = uip_ipaddr2(&dhcpState.ipAddr);
    *optPtr++ = uip_ipaddr3(&dhcpState.ipAddr);
	*optPtr++ = uip_ipaddr4(&dhcpState.ipAddr);
	
	*optPtr++ = 54;
	*optPtr++ = 4;
    *optPtr++ = uip_ipaddr1(&dhcpState.dhcpServerAddr);
    *optPtr++ = uip_ipaddr2(&dhcpState.dhcpServerAddr);
    *optPtr++ = uip_ipaddr3(&dhcpState.dhcpServerAddr);
	*optPtr++ = uip_ipaddr4(&dhcpState.dhcpServerAddr);
	
		// Hostname
	*optPtr++ = 12;
	*optPtr++ = 3;
	*optPtr++ = 'A';
	*optPtr++ = 'V';
	*optPtr++ = 'R';
	
	*optPtr++ = 255;
	
	uip_send(uip_appdata, optPtr - (uint8_t *)uip_appdata);

	dhcpState.state = UDP_DHCP_STATE_REQUEST;
}

void dhcpSendDiscover(void)
{
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
	
	// Hostname
	*optPtr++ = 12;
	*optPtr++ = 3;
	*optPtr++ = 'A';
	*optPtr++ = 'V';
	*optPtr++ = 'R';
	
	*optPtr++ = 255;
	
	uip_send(uip_appdata, optPtr - (uint8_t *)uip_appdata);
    
	dhcpState.state = UDP_DHCP_STATE_DISCOVER;
}
