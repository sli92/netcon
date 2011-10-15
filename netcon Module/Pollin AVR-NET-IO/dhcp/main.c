/*
 * Datei:               main.c
 * Author:              dev00
 * Beschreibung:        DHCP Test fuer den uIP Stack.
 *
 * Aenderungsdatum:     Sa, 15. Okt 2011 17:48:06
 *
 */

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "main.h"
#include "uart.h"
#include "clock.h"

#include "enc28j60.h"
#include "tcp_app.h"
#include "uip/uip.h"
#include "uip/uip_arp.h"

#define UIP_BUFFER ((struct uip_eth_hdr *)uip_buf)

const uint8_t mac_addr[6] = {0x02, 0x00, 0x00, 0x00, 0x00, 0x01};

int main(void)
{
        uint32_t lastperiodic = 0;
        uint32_t lastarp = 0;

        uint8_t i;

        uart_init();
        clock_init();

        enc28j60_init(mac_addr);
        uip_init();
        uip_arp_init();

        memcpy(uip_ethaddr.addr, mac_addr, sizeof(mac_addr));

        /* Seed fuer den Zufallsgenerator setzen. */
        srandom(get_seed());

        sei();

        tcp_app_init();
        udp_app_init();

        while(1 > 0) {
                uip_len = enc28j60_receive(uip_buf, UIP_CONF_BUFFER_SIZE);
                if(uip_len > 0) {
                        if(UIP_BUFFER->type == HTONS(UIP_ETHTYPE_IP)) {
                                uip_arp_ipin();
                                uip_input();

                                if(uip_len > 0) {
                                        uip_arp_out();
                                        enc28j60_transmit(uip_buf, uip_len);
                                }

                        } else if(UIP_BUFFER->type == HTONS(UIP_ETHTYPE_ARP)) {
                                uip_arp_arpin();
                                if(uip_len > 0)
                                        enc28j60_transmit(uip_buf, uip_len);
                        }

                } else if((get_clock() - lastperiodic) > 50) {
                        lastperiodic = get_clock();

                        for(i = 0; i < UIP_CONNS; i++) {
                                uip_periodic(i);

                                if(uip_len > 0) {
                                        uip_arp_out();
                                        enc28j60_transmit(uip_buf, uip_len);
                                }
                        }


                        for(i = 0; i < UIP_UDP_CONNS; i++) {
                                uip_udp_periodic(i);

                                if(uip_len > 0) {
                                        uip_arp_out();
                                        enc28j60_transmit(uip_buf, uip_len);
                                }
                        }
                }

                if((get_clock() - lastarp) > 1000) {
                        lastarp = get_clock();
                        uip_arp_timer();
                }
        }
}

uint16_t get_seed(void)
{
        uint16_t seed = 0;
        uint16_t *p = (uint16_t *) (RAMEND + 1);
        extern uint16_t __heap_start;

        while (p >= &__heap_start + 1)
                seed ^= * (--p);

        return seed;
}

void uip_log(char *m)
{
    uart_puts_p(PSTR("uIP: "));
    uart_puts_p(m);
    uart_putchar('\n');
}

