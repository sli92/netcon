/*
 * Datei:               main.c
 * Author:              dev00
 * Beschreibung:        DHCP Test fuer den uIP Stack.
 *
 * Aenderungsdatum:     Do, 15. Mär 2012 11:06:32
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
// #include "adc.h"
// #include "device.h"
#include "serconn.h"

#include "enc28j60.h"
#include "tcp_app.h"
#include "uip/uip.h"
#include "uip/uip_arp.h"

#define UIP_BUFFER ((struct uip_eth_hdr *)uip_buf)

// const char hostname[] PROGMEM = "AVR-NET-IO-Lipp";
// const char place[] PROGMEM = "Wohnzimmer";
const uint8_t mac_addr[] PROGMEM = {0x02, 0x00, 0x00, 0x00, 0x00, 0x02};

/*
uint8_t IPAddr[4] = {192, 168, 1, 8};
uint8_t Subnetmask[4] = {255, 255, 255, 0};
uint8_t Gateway[4] = {192, 168, 1, 1};
*/

int main(void)
{
        uint32_t lastperiodic = 0;
        uint32_t lastarp = 0;
        uint32_t lastupdate = 0;

        uint8_t i; // , x;
        char buffer[64];

        uart_init();
        clock_init();
        // adc_init();
        // devices_init();

        serconn_init();
        lastupdate = get_clock();

/*
        uart_puts("\ninfo:\n");
        uart_puts(name);
        uart_putchar('\n');
        uart_puts(location);

        uart_putchar('\n');
        sprintf(buffer, "devnum: %d\n", device_count);
        uart_puts(buffer);

        for(i = 0; i < device_count; i++) {
                sprintf(buffer, "f: %c, t: %X, m:%s, x:%s, w:%s\n", device_list[i].dtype, device_list[i].type, device_list[i].min,
                                                                    device_list[i].max, device_list[i].value);

                uart_puts(buffer);
        }
*/

        memcpy_P(uip_ethaddr.addr, mac_addr, sizeof(uip_ethaddr.addr));

/*
        uip_ipaddr_t ipaddr;
        uip_ipaddr(ipaddr, IPAddr[0], IPAddr[1], IPAddr[2], IPAddr[3]);
        uip_sethostaddr(ipaddr);

        uip_ipaddr(ipaddr, Subnetmask[0], Subnetmask[1], Subnetmask[2], Subnetmask[3]);
        uip_setnetmask(ipaddr);

        uip_ipaddr(ipaddr, Gateway[0], Gateway[1], Gateway[2], Gateway[3]);
        uip_setdraddr(ipaddr);
*/

        enc28j60_init(uip_ethaddr.addr);

        uip_init();
        uip_arp_init();

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

                } else if((get_clock() - lastperiodic) > CLOCK_TICKS_PER_SECOND / 2) {
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

                if((get_clock() - lastarp) > CLOCK_TICKS_PER_SECOND * 10) {
                        lastarp = get_clock();
                        uip_arp_timer();

/*
                        x = 0;
                        for(i = 0; i < UIP_CONNS; i++) {
                                if(uip_conn_active(i))
                                        x++;
                        }

                        sprintf(buffer, "Active connections: %d\n", x);
                        uart_puts(buffer);
*/
                }

                if((get_clock() - lastupdate) > CLOCK_TICKS_PER_SECOND / 2){
                    update_values();
                    lastupdate = get_clock();
                }
        }
}

/*
 * http://www.rn-wissen.de/index.php/Zufallszahlen_mit_avr-gcc
 *
 */
uint16_t get_seed(void)
{
        uint16_t seed = 0;
        uint16_t *p = (uint16_t *) (RAMEND + 1);
        extern uint16_t __heap_start;

        while (p >= &__heap_start + 1)
                seed ^= * (--p);

        return seed;
}

void uip_log(const prog_char *m)
{
    uart_puts_P(PSTR("uIP: "));
    uart_puts_P(m);
    uart_putchar('\n');
}

