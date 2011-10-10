#ifndef __UIP_CONF_H__
#define __UIP_CONF_H__

#include <inttypes.h>
#include "enc28j60.h"
#include "tcpApp.h"
#include "udpApp.h"

typedef struct uipTcpAppState uip_tcp_appstate_t;
typedef struct uipUdpAppState uip_udp_appstate_t;

#define UIP_APPCALL tcpAppCall
#define UIP_UDP_APPCALL udpAppCall

typedef uint8_t u8_t;
typedef uint16_t u16_t;
typedef uint16_t uip_stats_t;

#define UIP_CONF_BUFFER_SIZE        MAX_FRAME_LENGTH
#define UIP_CONF_MAX_CONNECTIONS    5
#define UIP_CONF_MAX_LISTENPORTS    5
#define UIP_CONF_ARPTAB_SIZE        5
#define UIP_CONF_LOGGING            1

#define UIP_CONF_UDP                1
#define UIP_CONF_UDP_CONNS          5
#define UIP_CONF_BROADCAST          1

#endif
