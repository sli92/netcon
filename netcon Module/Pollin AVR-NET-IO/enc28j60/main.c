/*
 * Datei:               main.c
 * Author:              dev00
 * Beschreibung:        Testet den ENC28J60 Treiber und den uIP Stack. Das
 *                      AVR-NET-IO Board laesst sich anpingen.
 *
 * Aenderungsdatum:     Mo, 17. Okt 2011 19:12:43
 *
 */

#include <stdint.h>
#include <string.h>
#include <avr/pgmspace.h>

#include "uart.h"
#include "clock.h"

#include "enc28j60.h"
#include "tcp_app.h"
#include "uip/uip.h"
#include "uip/uip_arp.h"

#define UIP_BUFFER ((struct uip_eth_hdr *)uip_buf)

const uint8_t mac_addr[6] = {0x02, 0x00, 0x00, 0x00, 0x00, 0x01};

uint8_t ip_addr[4] = {192, 168, 1, 8};
uint8_t gateway_addr[4] = {192, 168, 1, 1};
uint8_t subnet_mask[4] = {255, 255, 255, 0};

int main(void)
{
        uint32_t lastperiodic = 0;
        uint32_t lastarp = 0;

        uint8_t i;
        uip_ipaddr_t ipaddr;

        uart_init();
        clock_init();

        enc28j60_init(mac_addr);
        uip_init();
        uip_arp_init();

        memcpy(uip_ethaddr.addr, mac_addr, sizeof(mac_addr));

        uip_ipaddr(&ipaddr, ip_addr[0], ip_addr[1], ip_addr[2], ip_addr[3]);
        uip_sethostaddr(&ipaddr);

        uip_ipaddr(&ipaddr, subnet_mask[0], subnet_mask[1], subnet_mask[2], subnet_mask[3]);
        uip_setnetmask(&ipaddr);

        uip_ipaddr(&ipaddr, gateway_addr[0], gateway_addr[1], gateway_addr[2], gateway_addr[3]);
        uip_setdraddr(&ipaddr);

        tcp_app_init();

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
                }

                if((get_clock() - lastarp) > 1000) {
                        lastarp = get_clock();
                        uip_arp_timer();
                }
        }
}

void uip_log(char *m)
{
    uart_puts_p(PSTR("uIP: "));
    uart_puts_p(m);
    uart_putchar('\n');
}

