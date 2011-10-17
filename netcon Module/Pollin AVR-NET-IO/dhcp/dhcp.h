/*
 * Datei:               dhcp.h
 * Author:              dev00
 * Beschreibung:        DHCP Client fuer den uIP Stack.
 *
 * Aenderungsdatum:     Mo, 17. Okt 2011 23:39:04
 *
 */

#ifndef __DHCP_H__
#define __DHCP_H__

#include <stdint.h>
#include <avr/pgmspace.h>

#include "uip/uip.h"

#define DHCP_SERVER_PORT                67
#define DHCP_CLIENT_PORT                68

#define DHCP_STATE_WAIT                 0
#define DHCP_STATE_DISCOVER             1
#define DHCP_STATE_OFFER                2
#define DHCP_STATE_REQUEST              3
#define DHCP_STATE_ACK                  4
#define DHCP_STATE_WAIT_RENEW           5

#define DHCP_FLAGS_BROADCAST            0x8000

#define DHCP_OP_REQUEST                 1
#define DHCP_OP_REPLY                   2

#define DHCP_TYPE_ETHERNET              1

#define DHCP_OPTION_MSG_TYPE            53
#define DHCP_OPTION_MSG_TYPE_LEN        1

#define DHCPDISCOVER                    1
#define DHCPOFFER                       2
#define DHCPREQUEST                     3
#define DHCPDECLINE                     4
#define DHCPACK                         5
#define DHCPNAK                         6
#define DHCPRELEASE                     7
#define DHCPINFORM                      8

#define DHCP_OPTION_SUBNETMASK          1
#define DHCP_OPTION_ROUTER              3
#define DHCP_OPTION_REQ_LIST            55

#define DHCP_OPTION_REQ_ADDR            50
#define DHCP_OPTION_REQ_ADDR_LEN        4

#define DHCP_OPTION_ADDR_TIME           51
#define DHCP_OPTION_ADDR_TIME_LEN       4

#define DHCP_OPTION_DHCP_SERVERID       54
#define DHCP_OPTION_DHCP_SERVERID_LEN   4

#define DHCP_OPTION_HOSTNAME            12

#define DHCP_OPTION_END                 255

struct dhcp_message {
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

struct dhcp_state {
        uint8_t state;

        struct uip_udp_conn *conn;

        uint32_t lease_time;
        uint32_t xid;

        uint32_t dhcp_renew_time;

        uip_ipaddr_t dhcp_server_addr;

        uip_ipaddr_t ip_addr;
        uip_ipaddr_t gateway_addr;
        uip_ipaddr_t subnet_mask;
};

#define ntohl(x) __builtin_bswap32(x)

void dhcp_init(void);
void dhcp_app_call(void);
void dhcp_send_discover(void);
void dhcp_init_message(struct dhcp_message *message);
uint8_t *dhcp_add_msg_type_option(uint8_t *opt_ptr, uint8_t msg_type);
uint8_t *dhcp_add_parameter_list_option(uint8_t *opt_ptr);
uint8_t *dhcp_add_req_addr_option(uint8_t *opt_ptr);
uint8_t *dhcp_add_dhcp_serverid_option(uint8_t *opt_ptr);
uint8_t *dhcp_add_hostname_option_P(uint8_t *opt_ptr, const prog_char *hostname);
uint8_t *dhcp_add_end(uint8_t *opt_ptr);
void dhcp_parse_offer(void);
void dhcp_send_request(void);
void dhcp_parse_ack(void);

#endif

