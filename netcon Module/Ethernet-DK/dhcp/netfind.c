/*
 * Datei:               netfind.c
 * Author:              dev00
 * Beschreibung:
 *
 * Aenderungsdatum:     Do, 27. Okt 2011 08:28:52
 *
 */

#include <stdlib.h>
#include <string.h>

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
                if(netfind_s.state == NETFIND_STATE_IDLE &&
                   (uip_hostaddr[0] | uip_hostaddr[1]) != 0x00) {
                        netfind_s.send_answer_time = get_clock() + (random() %
                                                 (2 * CLOCK_TICKS_PER_SECOND));

                        netfind_s.state = NETFIND_STATE_WAIT;
                }
        }
}

void netfind_send_answer(void)
{
        strcpy(uip_appdata, hostname);

        uip_send(uip_appdata, strlen(uip_appdata) + 1);

        netfind_s.state = NETFIND_STATE_IDLE;
}

