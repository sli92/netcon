/*
 * Datei:               netfind.c
 * Author:              dev00
 * Beschreibung:
 *
 * Aenderungsdatum:     Do, 01. Dez 2011 11:02:56
 *
 */

#include <stdlib.h>
#include <string.h>
#include <avr/pgmspace.h>

#include "netfind.h"
#include "uip/uip.h"
#include "clock.h"
#include "main.h"

static struct netfind_state netfind_s;

void netfind_init(void)
{
        struct uip_udp_conn *local_conn;
        uip_ipaddr_t ipaddr;
        uip_ipaddr(&ipaddr, 255, 255, 255, 255);

        local_conn = uip_udp_new(&ipaddr, HTONS(NETFIND_SERVER_PORT));
        uip_udp_bind(local_conn, HTONS(NETFIND_CLIENT_PORT));

        netfind_s.state = NETFIND_STATE_IDLE;
}

void netfind_app_call(void)
{
        if(uip_poll()) {
                switch(netfind_s.state) {
                        case NETFIND_STATE_WAIT:
                                if(get_clock() > netfind_s.send_answer_time)
                                        netfind_s.state = NETFIND_STATE_SEND_ANSWER;
                                break;

                        case NETFIND_STATE_SEND_ANSWER:
                                netfind_send_answer();
                                break;
                }
        }

        if(uip_newdata()) {
                switch(netfind_s.state) {
                        case NETFIND_STATE_IDLE:
                                netfind_handle_request();
                                break;
                }
        }
}

int filter_ethaddr(uint8_t *addr1, uint8_t *addr2)
{
        if(addr2[0] == 0xFF && addr2[1] == 0xFF &&
           addr2[2] == 0xFF && addr2[3] == 0xFF &&
           addr2[4] == 0xFF && addr2[5] == 0xFF)
                return 1;

        if(addr1[0] == addr2[0] && addr1[1] == addr2[1] &&
           addr1[2] == addr2[2] && addr1[3] == addr2[3] &&
           addr1[4] == addr2[4] && addr1[5] == addr2[5])
                return 1;

        return 0;
}

void netfind_handle_request(void)
{
        char *appdata = (char *)uip_appdata;

        // Check if device has a valid IP-Address
        if((uip_hostaddr[0] | uip_hostaddr[1]) == 0)
                return;

        if(strcmp_P(appdata, PSTR("netfind")) != 0)
                return;

        // Check version
        if(appdata[8] > NETFIND_VERSION)
                return;

        if(appdata[9] != 0 &&
           appdata[9] != pgm_read_byte(device_type))
                return;

        if(filter_ethaddr(uip_ethaddr.addr, appdata + 10) == 0)
                return;


        netfind_s.send_answer_time = get_clock() + (random() %
                                 (2 * CLOCK_TICKS_PER_SECOND));

        netfind_s.state = NETFIND_STATE_WAIT;
}

void netfind_send_answer(void)
{
        uint32_t uptime = get_clock();
        char *appdata = (char *)uip_appdata;

        strcpy_P(appdata, PSTR("netdiscover"));
        appdata += 12;

        memcpy(appdata, uip_ethaddr.addr, 6);
        appdata += 6;

        memcpy(appdata, &uptime, 4);
        appdata += 4;

        *appdata++ = pgm_read_byte(device_type);

        strcpy_P(appdata, hostname);
        while(*appdata++);

        uip_send(uip_appdata, appdata - (char *)uip_appdata);

        netfind_s.state = NETFIND_STATE_IDLE;
}

