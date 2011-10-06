#ifndef __UIP_CONF_H__
#define __UIP_CONF_H__

#include <inttypes.h>
#include "enc28j60.h"
#include "tcpApp.h"

#define STATE_IDLE  0
#define STATE_SENT  1
#define STATE_ACKED 2

struct uipTcpAppState
{
    uint8_t state;
};


typedef struct uipTcpAppState uip_tcp_appstate_t;

#define UIP_APPCALL tcpAppCall

typedef uint8_t u8_t;
typedef uint16_t u16_t;
typedef uint16_t uip_stats_t;

#define UIP_CONF_BUFFER_SIZE        MAX_FRAME_LENGTH
#define UIP_CONF_MAX_CONNECTIONS    5
#define UIP_CONF_MAX_LISTENPORTS    5
#define UIP_CONF_ARPTAB_SIZE        5
#define UIP_CONF_LOGGING            1

#endif
