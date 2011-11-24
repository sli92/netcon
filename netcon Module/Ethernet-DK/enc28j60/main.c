/*
 * Datei:               main.c
 * Author:              dev00
 * Beschreibung:        Testet den CP2200 Treiber und den uIP Stack. Das
 *                      Ethernet-DK Board laesst sich anpingen. (192.168.1.8)
 *
 * Aenderungsdatum:     Do, 27. Okt 2011 23:54:56
 *
 */
#include "compiler_defs.h"
#include "C8051F340_defs.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "uart.h"
#include "main.h"
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

void timer2_overflow(void) __interrupt(5);

unsigned char _sdcc_external_startup(void)
{
    /* Watchdog deaktivieren */
    PCA0MD &= ~(1 << 6);

    return 0;
}

int main(void)
{
    uint32_t lastperiodic = 0;
    uint32_t lastarp = 0;

    uint8_t i;
    uip_ipaddr_t ipaddr;

    sysclk_init();

    clock_init();

    /* Interrupts aktivieren. */
    IE |= (1 << _EA);

    io_init();
    uart_init();

    enc28j60_init(mac_addr);

    uip_init();
    uip_arp_init();

    memcpy(uip_ethaddr.addr, mac_addr, sizeof(uip_ethaddr.addr));

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

                } else if((get_clock() - lastperiodic) > CLOCK_TICKS_PER_SECOND / 2) {
                        lastperiodic = get_clock();
                        
                        for(i = 0; i < UIP_CONNS; i++) {
                                uip_periodic(i);

                                if(uip_len > 0) {
                                        uip_arp_out();
                                        enc28j60_transmit(uip_buf, uip_len);
                                }
                        }
                }

                if((get_clock() - lastarp) > CLOCK_TICKS_PER_SECOND * 10) {
                        lastarp = get_clock();
                        uip_arp_timer();
                }
        }

    return 0;
}

/*
 * Initialisiert den Takt auf die maximale Frequenz, beim C8051F340 sind dies
 * 48MHz.
 *
 */
void sysclk_init(void)
{
    OSCICN |= (1 << _IFCN1) | (1 << _IFCN0);

    /* Siehe C8051F340 Datenblatt, Seite 138 */
    CLKMUL = 0x00;

    CLKMUL |= (1 << _MULEN);

    delay_40us();

    CLKMUL |= (1 << _MULINIT);

    while(!(CLKMUL & (1 << _MULRDY)));

    CLKSEL |= (1 << _CLKSL1) | (1 << _CLKSL0);

    FLSCL |= (1 << _FLRT);
}

/*
 * Initialisiert die verschiedenen Ports.
 *
 */
void io_init(void)
{
    /* SCK, MOSI, NSS, TX0 als Push-Pull Ausgang */
    P0MDOUT |= (1 << 0) | (1 << 2) | (1 << 3) | (1 << 4);

    XBR0 |= (1 << _UART0E) | (1 << _SPI0E);
    XBR1 |= (1 << _XBARE);
}

/*
 * Wartet ungefaehr 40us, Code aus dem SiLabs UART Beispiel.
 * (Nur in der sysclk_init Funktion verwenden)
 *
 */
void delay_40us(void)
{
    int x;
    for(x = 0;x < 500;x)
        x++;
}

/*
 * Wartet zwischen 10ms und 20ms.
 *
 */
void delay_20ms(void)
{
    uint32_t start = get_clock();

    while((get_clock() - start) < 2);
}

void uip_log(char *m)
{
    uart_puts("uIP: ");
    uart_puts(m);
    uart_putchar('\n');
}
