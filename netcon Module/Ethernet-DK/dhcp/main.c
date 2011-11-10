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
#include "cp2200.h"
#include "tcp_app.h"
#include "uip/uip.h"
#include "uip/uip_arp.h"

#define UIP_BUFFER ((struct uip_eth_hdr *)uip_buf)

const char *hostname = "Ethernet-DK";

uint8_t ip_addr[4] = {192, 168, 1, 8};
uint8_t gateway_addr[4] = {192, 168, 1, 1};
uint8_t subnet_mask[4] = {255, 255, 255, 0};

void timer2_overflow(void) __interrupt(5);
void external_interrupt(void) __interrupt(0);

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

    sysclk_init();

    clock_init();

    /* Interrupts aktivieren. */
    IE |= (1 << _EA);

    io_init();
    emif_init();
    uart_init();
    cp2200_init();

    uip_init();
    uip_arp_init();


    memcpy(uip_ethaddr.addr, mac_addr, sizeof(uip_ethaddr.addr));

    tcp_app_init();
    udp_app_init();


    while(1 > 0) {
        uip_len = cp2200_receive(uip_buf, UIP_CONF_BUFFER_SIZE);

        if(uip_len > 0) {
            if(UIP_BUFFER->type == HTONS(UIP_ETHTYPE_IP)) {
                uip_arp_ipin();
                uip_input();

                if(uip_len > 0) {
                    uip_arp_out();
                    cp2200_transmit(uip_buf, uip_len);
                }
            } else if(UIP_BUFFER->type == HTONS(UIP_ETHTYPE_ARP)) {
                uip_arp_arpin();
                if(uip_len > 0) {
                    cp2200_transmit(uip_buf, uip_len);
                }
            }
        } else if((get_clock() - lastperiodic) > CLOCK_TICKS_PER_SECOND / 2) {
            lastperiodic = get_clock();

            for(i = 0; i < UIP_CONNS; i++) {
                uip_periodic(i);

                if(uip_len > 0) {
                    uip_arp_out();
                    cp2200_transmit(uip_buf, uip_len);
                }
            }

            for(i = 0; i < UIP_UDP_CONNS; i++) {
                uip_udp_periodic(i);

                if(uip_len > 0) {
                    uip_arp_out();
                    cp2200_transmit(uip_buf, uip_len);
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
    /* TX0 als Push-Pull Ausgang */
    P0MDOUT |= (1 << 0);

    XBR0 |= (1 << _UART0E);
    XBR1 |= (1 << _XBARE);


    /* /WR, /RD, LED1 und LED2 als Push-Pull Ausgang */
    P1MDOUT |= (1 << 7) | (1 << 6) | (1 << 3) | (1 << 4);


    /* Nachfolgender Code ist aus dem SiLabs Beispiel. */
       
    /*
     * Configure External Memory Interface Pins to push-pull except for
     * P2.0 and P2.1 which are used for target board switches.
     *
     */
    P2MDOUT |= 0xFC;        // ADDR[15:8]
    P3MDOUT |= 0xFF;        // ADDR[7:0]
    P4MDOUT |= 0xFF;        // DATA[7:0]

    /* /WR, /RD auf 1 */
    P1 |= (1 << 7) | (1 << 6);
    
    /* LED1, LED2 und /RST auf 0 */
    P1 &= ~((1 << 3) | (1 << 4) | (1 << 0));

    P2 = 0xFF;              /* ADDR[15:8] initially high */
    P3 = 0xFF;              /* ADDR[7:0] initially high */
    P4 = 0xFF;              /* DATA[7:0] intially high */

    /* Port 0.3 als Externer-Interrupt 0 Quelle. */
    IT01CF = 0x03;
}

void emif_init(void)
{
    /*
     * Nicht gemultiplexter Splitmode.
     * Siehe C8051F340 Datenblatt, Seite 118.
     *
     */
    EMI0CF |= (1 << _EMD2) | (1 << _EMD1);

    /* Laut SiLabs Beispielcode, Timing für bis zu 50MHz */
    EMI0TC = 0x9E;

    /* Addresse des höhren Bytes */
    EMI0CN = 0x20;
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

void uip_log(const char *m)
{
    uart_puts("uIP: ");
    uart_puts(m);
    uart_putchar('\n');
}
