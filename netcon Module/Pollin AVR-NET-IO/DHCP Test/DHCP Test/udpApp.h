#ifndef __UDPAPP_H_
#define __UDPAPP_H_

#define DHCP_SERVER_PORT 67
#define DHCP_CLIENT_PORT 68

#define UDP_DHCP_STATE_INITIAL  0
#define UDP_DHCP_STATE_DISCOVER 1

struct dhcpMessage
{
	uint8_t op;
	uint8_t htype;
	uint8_t hlen;
	uint8_t hops;
	
	uint32_t xid;
	
	uint16_t secs;
	uint16_t flags;
	
	uint8_t ciaddr[4];
	uint8_t yiaddr[4];
	uint8_t siaddr[4];
	uint8_t giaddr[4];
	
	uint8_t chaddr[16];
	
	uint8_t sname[64];
	
	uint8_t file[128];
	
	uint8_t options[312];		
};

struct udpDhcpClientState
{
	uint8_t state;
	struct uip_udp_conn *connection;
	
	uint32_t xid;
};

struct uipUdpAppState
{
    uint8_t state;
};

void udpAppInit(void);
void udpAppCall(void);
void dhcpSendDiscover(void);

#endif