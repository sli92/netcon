/*
 * Datei:               dhcp.c
 * Author:              dev00
 * Beschreibung:        DHCP Client fuer den uIP Stack.
 *
 * Aenderungsdatum:     Do, 01. Dez 2011 10:29:22
 *
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <avr/pgmspace.h>
#include "uip/uip.h"

#include "main.h"
#include "clock.h"
#include "dhcp.h"

static struct dhcp_state dhcp_s;

void dhcp_init(void)
{
        uip_ipaddr_t ipaddr;
        uip_ipaddr(&ipaddr, 255, 255, 255, 255);

        dhcp_s.conn = uip_udp_new(&ipaddr, HTONS(DHCP_SERVER_PORT));
        uip_udp_bind(dhcp_s.conn, HTONS(DHCP_CLIENT_PORT));

        dhcp_s.xid = random();

        dhcp_s.dhcp_renew_time = get_clock() + 2 * CLOCK_TICKS_PER_SECOND +
                                 (random() % (3 * CLOCK_TICKS_PER_SECOND));

        dhcp_s.state = DHCP_STATE_BOOT_WAIT;
}

void dhcp_app_call(void)
{
        if(uip_newdata()) {
                switch(dhcp_s.state) {
                        case DHCP_STATE_OFFER:
                                dhcp_parse_offer();
                                break;

                        case DHCP_STATE_ACK:
                                dhcp_parse_ack();
                                break;
                }
        }

        if(uip_poll()) {
                switch(dhcp_s.state) {
                        case DHCP_STATE_BOOT_WAIT:
                                if(get_clock() > dhcp_s.dhcp_renew_time)
                                        dhcp_s.state = DHCP_STATE_DISCOVER;
                                break;

                        case DHCP_STATE_DISCOVER:
                                dhcp_send_discover();
                                break;

                        case DHCP_STATE_REQUEST:
                                dhcp_send_request();
                                break;

                        case DHCP_STATE_WAIT_RENEW:
                                if(get_clock() > dhcp_s.dhcp_renew_time)
                                        dhcp_s.state = DHCP_STATE_REQUEST;
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
        opt_ptr = dhcp_add_hostname_option_P(opt_ptr, hostname);
        opt_ptr = dhcp_add_end(opt_ptr);

        uip_send(uip_appdata, opt_ptr - (uint8_t *)uip_appdata);

        dhcp_s.state = DHCP_STATE_OFFER;
}

void dhcp_init_message(struct dhcp_message *message)
{
        message->op = DHCP_OP_REQUEST;
        message->htype = DHCP_TYPE_ETHERNET;
        message->hlen = sizeof(uip_ethaddr.addr);
        message->hops = 0;
        message->xid = dhcp_s.xid;
        message->secs = htons(get_clock() / CLOCK_TICKS_PER_SECOND);
        message->flags = HTONS(DHCP_FLAGS_BROADCAST);

        memset(message->ciaddr, 0, sizeof(message->ciaddr));
        memset(message->yiaddr, 0, sizeof(message->yiaddr));
        memset(message->siaddr, 0, sizeof(message->siaddr));
        memset(message->giaddr, 0, sizeof(message->giaddr));

        memcpy(message->chaddr, uip_ethaddr.addr, sizeof(uip_ethaddr.addr));
        memset(&(message->chaddr[sizeof(uip_ethaddr.addr)]), 0,
               sizeof(message->chaddr) - sizeof(uip_ethaddr.addr));

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
        memcpy(opt_ptr, &dhcp_s.ip_addr, DHCP_OPTION_REQ_ADDR_LEN);

        return opt_ptr + DHCP_OPTION_REQ_ADDR_LEN;
}

uint8_t *dhcp_add_dhcp_serverid_option(uint8_t *opt_ptr)
{
        *opt_ptr++ = DHCP_OPTION_DHCP_SERVERID;
        *opt_ptr++ = DHCP_OPTION_DHCP_SERVERID_LEN;
        memcpy(opt_ptr, &dhcp_s.dhcp_server_addr,
               DHCP_OPTION_DHCP_SERVERID_LEN);

        return opt_ptr + DHCP_OPTION_DHCP_SERVERID_LEN;
}

uint8_t *dhcp_add_hostname_option_P(uint8_t *opt_ptr, const prog_char *hostname)
{
        uint8_t *len_ptr;

        *opt_ptr++ = DHCP_OPTION_HOSTNAME;
        len_ptr = opt_ptr++;

        strcpy_P(opt_ptr, hostname);
        *len_ptr = strlen(opt_ptr);

        return opt_ptr + *len_ptr;
}

uint8_t *dhcp_add_end(uint8_t *opt_ptr)
{
        *opt_ptr++ = DHCP_OPTION_END;

        return opt_ptr;
}

void dhcp_parse_offer(void)
{
        struct dhcp_message *message = (struct dhcp_message *)uip_appdata;

        if(message->op != DHCP_OP_REPLY || message->xid != dhcp_s.xid) {
                return;
        }

        memcpy(&dhcp_s.ip_addr, message->yiaddr, sizeof(dhcp_s.ip_addr));
        memcpy(&dhcp_s.dhcp_server_addr, message->siaddr,
               sizeof(dhcp_s.dhcp_server_addr));

        dhcp_s.state = DHCP_STATE_REQUEST;
}

void dhcp_send_request(void)
{
        uint8_t *opt_ptr;
        struct dhcp_message *message = (struct dhcp_message *)uip_appdata;

        dhcp_init_message(message);
        memcpy(message->siaddr, &dhcp_s.dhcp_server_addr, sizeof(message->siaddr));

        /* Skip magic cookie */
        opt_ptr = message->options + 4;

        opt_ptr = dhcp_add_msg_type_option(opt_ptr, DHCPREQUEST);
        opt_ptr = dhcp_add_req_addr_option(opt_ptr);
        opt_ptr = dhcp_add_hostname_option_P(opt_ptr, hostname);
        opt_ptr = dhcp_add_dhcp_serverid_option(opt_ptr);
        opt_ptr = dhcp_add_end(opt_ptr);

        uip_send(uip_appdata, opt_ptr - (uint8_t *)uip_appdata);

        dhcp_s.state = DHCP_STATE_ACK;
}

void dhcp_parse_ack(void)
{
        uint8_t got_ack = 0;
        uint8_t op, len;
        uint8_t *opt_ptr;
        struct dhcp_message *message = (struct dhcp_message *)uip_appdata;

        if(message->op != DHCP_OP_REPLY || message->xid != dhcp_s.xid) {
                return;
        }


        /* Skip magic cookie */
        opt_ptr = message->options + 4;

        do {
                op = *opt_ptr++;
                len = *opt_ptr++;

                switch(op) {
                        case DHCP_OPTION_SUBNETMASK:
                                memcpy(&dhcp_s.subnet_mask, opt_ptr,
                                       sizeof(dhcp_s.subnet_mask));
                                break;

                        case DHCP_OPTION_ROUTER:
                                memcpy(&dhcp_s.gateway_addr, opt_ptr,
                                       sizeof(dhcp_s.gateway_addr));
                                break;

                        case DHCP_OPTION_ADDR_TIME:
                                memcpy(&dhcp_s.lease_time, opt_ptr,
                                       sizeof(dhcp_s.lease_time));

                                dhcp_s.lease_time = ntohl(dhcp_s.lease_time);
                                break;

                        case DHCP_OPTION_MSG_TYPE:
                                if(*opt_ptr == DHCPACK)
                                        got_ack = 1;
                                break;
                }

                opt_ptr += len;
        } while(op != DHCP_OPTION_END);

        if(got_ack) {
                uip_sethostaddr(&dhcp_s.ip_addr);
                uip_setnetmask(&dhcp_s.subnet_mask);
                uip_setdraddr(&dhcp_s.gateway_addr);

                dhcp_s.dhcp_renew_time = get_clock() + dhcp_s.lease_time *
                                         CLOCK_TICKS_PER_SECOND / 2;

                dhcp_s.state = DHCP_STATE_WAIT_RENEW;
        }
}

