/*
 * Datei:               dhcp.c
 * Author:              dev00
 * Beschreibung:        DHCP Client fuer den uIP Stack.
 *
 * Aenderungsdatum:     Sa, 15. Okt 2011 18:04:06
 *
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "uip/uip.h"

#include "main.h"
#include "clock.h"
#include "dhcp.h"

struct dhcp_state state;

uint32_t ntohl(uint32_t x)
{
        return ((x & 0xFF000000UL) >> 24) | ((x & 0x00FF0000UL) >>  8) |
               ((x & 0x0000FF00UL) <<  8) | ((x & 0x000000FFUL) << 24);
}

void dhcp_init(void)
{
        uip_ipaddr_t ipaddr;
        uip_ipaddr(ipaddr, 255, 255, 255, 255);

        state.conn = uip_udp_new(&ipaddr, HTONS(DHCP_SERVER_PORT));
        uip_udp_bind(state.conn, HTONS(DHCP_CLIENT_PORT));

        state.xid = random();

        state.dhcp_renew_time = get_clock() + 2 * CLOCK_TICKS_PER_SECOND +
                                (random() % (3 * CLOCK_TICKS_PER_SECOND));

        state.state = DHCP_STATE_WAIT;
}

void dhcp_app_call(void)
{
        if(uip_newdata()) {
                switch(state.state) {
                        case DHCP_STATE_OFFER:
                                dhcp_parse_offer();
                                break;

                        case DHCP_STATE_ACK:
                                dhcp_parse_ack();
                                break;
                }
        }

        if(uip_poll()) {
                switch(state.state) {
                        case DHCP_STATE_WAIT:
                                if(get_clock() > state.dhcp_renew_time)
                                        state.state = DHCP_STATE_DISCOVER;
                                break;

                        case DHCP_STATE_DISCOVER:
                                dhcp_send_discover();
                                break;

                        case DHCP_STATE_REQUEST:
                                dhcp_send_request();
                                break;

                        case DHCP_STATE_WAIT_RENEW:
                                if(get_clock() > state.dhcp_renew_time)
                                        state.state = DHCP_STATE_REQUEST;
                                break;
                }
        }
}

void dhcp_send_discover(void)
{
        uint8_t *opt_ptr;
        struct dhcp_message *message = (struct dhcp_message *)uip_appdata;

        dhcp_init_message(message);

        /* Skip magic cookie */
        opt_ptr = message->options + 4;

        opt_ptr = dhcp_add_msg_type_option(opt_ptr, DHCPDISCOVER);
        opt_ptr = dhcp_add_parameter_list_option(opt_ptr);
        opt_ptr = dhcp_add_hostname_option(opt_ptr);
        opt_ptr = dhcp_add_end(opt_ptr);

        uip_send(uip_appdata, opt_ptr - (uint8_t *)uip_appdata);

        state.state = DHCP_STATE_OFFER;
}

void dhcp_init_message(struct dhcp_message *message)
{
        message->op = DHCP_OP_REQUEST;
        message->htype = DHCP_TYPE_ETHERNET;
        message->hlen = sizeof(mac_addr);
        message->hops = 0;
        message->xid = state.xid;
        message->secs = htons(get_clock() / CLOCK_TICKS_PER_SECOND);
        message->flags = HTONS(DHCP_FLAGS_BROADCAST);

        memset(message->ciaddr, 0, sizeof(message->ciaddr));
        memset(message->yiaddr, 0, sizeof(message->yiaddr));
        memset(message->siaddr, 0, sizeof(message->siaddr));
        memset(message->giaddr, 0, sizeof(message->giaddr));

        memcpy(message->chaddr, mac_addr, sizeof(mac_addr));
        memset(&(message->chaddr[sizeof(mac_addr)]), 0,
               sizeof(message->chaddr) - sizeof(mac_addr));

        memset(message->sname, 0, sizeof(message->sname));
        memset(message->file, 0, sizeof(message->file));

        /* Magic cookie */
        message->options[0] = 0x63;
        message->options[1] = 0x82;
        message->options[2] = 0x53;
        message->options[3] = 0x63;
}

uint8_t *dhcp_add_msg_type_option(uint8_t *opt_ptr, uint8_t msg_type)
{
        *opt_ptr++ = DHCP_OPTION_MSG_TYPE;
        *opt_ptr++ = DHCP_OPTION_MSG_TYPE_LEN;
        *opt_ptr++ = msg_type;

        return opt_ptr;
}

uint8_t *dhcp_add_parameter_list_option(uint8_t *opt_ptr)
{
        *opt_ptr++ = DHCP_OPTION_REQ_LIST;
        *opt_ptr++ = 2;
        *opt_ptr++ = DHCP_OPTION_SUBNETMASK;
        *opt_ptr++ = DHCP_OPTION_ROUTER;

        return opt_ptr;
}

uint8_t *dhcp_add_req_addr_option(uint8_t *opt_ptr)
{
        *opt_ptr++ = DHCP_OPTION_REQ_ADDR;
        *opt_ptr++ = DHCP_OPTION_REQ_ADDR_LEN;
        memcpy(opt_ptr, state.ip_addr, sizeof(state.ip_addr));

        return opt_ptr + DHCP_OPTION_REQ_ADDR_LEN;
}

uint8_t *dhcp_add_dhcp_serverid_option(uint8_t *opt_ptr)
{
        *opt_ptr++ = DHCP_OPTION_DHCP_SERVERID;
        *opt_ptr++ = DHCP_OPTION_DHCP_SERVERID_LEN;
        memcpy(opt_ptr, state.dhcp_server_addr, sizeof(state.ip_addr));

        return opt_ptr + DHCP_OPTION_DHCP_SERVERID_LEN;
}

uint8_t *dhcp_add_hostname_option(uint8_t *opt_ptr)
{
        char *hostname_tmp = hostname;
        uint8_t *len_ptr;

        *opt_ptr++ = DHCP_OPTION_HOSTNAME;
        len_ptr = opt_ptr++;

        while(*hostname_tmp)
                *opt_ptr++ = *hostname_tmp++;

        *len_ptr = hostname_tmp - hostname;

        return opt_ptr;
}

uint8_t *dhcp_add_end(uint8_t *opt_ptr)
{
        *opt_ptr++ = DHCP_OPTION_END;

        return opt_ptr;
}

void dhcp_parse_offer(void)
{
        uint8_t op, len;
        uint8_t *opt_ptr;
        struct dhcp_message *message = (struct dhcp_message *)uip_appdata;

        if(message->op != DHCP_OP_REPLY || message->xid != state.xid) {
                return;
        }

        memcpy(state.ip_addr, message->yiaddr, sizeof(state.ip_addr));
        memcpy(state.dhcp_server_addr, message->siaddr, sizeof(state.dhcp_server_addr));

        /* Skip magic cookie */
        opt_ptr = message->options + 4;

        do {
                op = *opt_ptr++;
                len = *opt_ptr++;

                switch(op) {
                        case DHCP_OPTION_SUBNETMASK:
                                memcpy(state.subnet_mask, opt_ptr,
                                       sizeof(state.subnet_mask));
                                break;

                        case DHCP_OPTION_ROUTER:
                                memcpy(state.gateway_addr, opt_ptr,
                                       sizeof(state.gateway_addr));
                                break;

                        case DHCP_OPTION_ADDR_TIME:
                                memcpy(&state.lease_time, opt_ptr,
                                       sizeof(state.lease_time));

                                state.lease_time = ntohl(state.lease_time);
                                break;
                }

                opt_ptr += len;
        } while(op != DHCP_OPTION_END);

        state.state = DHCP_STATE_REQUEST;
}

void dhcp_send_request(void)
{
        uint8_t *opt_ptr;
        struct dhcp_message *message = (struct dhcp_message *)uip_appdata;

        dhcp_init_message(message);
        memcpy(message->siaddr, state.dhcp_server_addr, sizeof(message->siaddr));

        /* Skip magic cookie */
        opt_ptr = message->options + 4;

        opt_ptr = dhcp_add_msg_type_option(opt_ptr, DHCPREQUEST);
        opt_ptr = dhcp_add_req_addr_option(opt_ptr);
        opt_ptr = dhcp_add_hostname_option(opt_ptr);
        opt_ptr = dhcp_add_dhcp_serverid_option(opt_ptr);
        opt_ptr = dhcp_add_end(opt_ptr);

        uip_send(uip_appdata, opt_ptr - (uint8_t *)uip_appdata);

        state.state = DHCP_STATE_ACK;
}

void dhcp_parse_ack(void)
{
        uint8_t got_ack = 0;
        uint8_t op, len;
        uint8_t *opt_ptr;
        struct dhcp_message *message = (struct dhcp_message *)uip_appdata;

        if(message->op != DHCP_OP_REPLY || message->xid != state.xid) {
                return;
        }


        /* Skip magic cookie */
        opt_ptr = message->options + 4;

        do {
                op = *opt_ptr++;
                len = *opt_ptr++;

                switch(op) {
                        case DHCP_OPTION_SUBNETMASK:
                                memcpy(state.subnet_mask, opt_ptr,
                                       sizeof(state.subnet_mask));
                                break;

                        case DHCP_OPTION_ROUTER:
                                memcpy(state.gateway_addr, opt_ptr,
                                       sizeof(state.gateway_addr));
                                break;

                        case DHCP_OPTION_ADDR_TIME:
                                memcpy(&state.lease_time, opt_ptr,
                                       sizeof(state.lease_time));

                                state.lease_time = ntohl(state.lease_time);
                                break;

                        case DHCP_OPTION_MSG_TYPE:
                                if(*opt_ptr == DHCPACK)
                                        got_ack = 1;
                                break;
                }

                opt_ptr += len;
        } while(op != DHCP_OPTION_END);

        if(got_ack) {
                uip_sethostaddr(state.ip_addr);
                uip_setnetmask(state.subnet_mask);
                uip_setdraddr(state.gateway_addr);

                state.dhcp_renew_time = get_clock() + state.lease_time *
                                        CLOCK_TICKS_PER_SECOND / 2;

                state.state = DHCP_STATE_WAIT_RENEW;
        }
}

