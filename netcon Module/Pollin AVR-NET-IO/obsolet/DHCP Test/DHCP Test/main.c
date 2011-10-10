/*
 * main.c
 * UART-Ethernet Bridge
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <inttypes.h>

#include "uart.h"
#include "uip/uip.h"
#include "uip/uip_arp.h"
#include "enc28j60.h"
#include "main.h"

#define UIP_BUFFER ((struct uip_eth_hdr *)uip_buf)

// MAC Adresse: 02-00-00-00-00-01
const uint8_t MACAddr[6] = {0x02, 0x00, 0x00, 0x00, 0x00, 0x45};

uint8_t IPAddr[4] = {0, 0, 0, 0};
uint8_t Subnetmask[4] = {0, 0, 0, 0};
uint8_t Gateway[4] = {0, 0, 0, 0};

int main(void)
{
    uip_ipaddr_t ipaddr;
    uint16_t arpTimer = 1000;       // in 1/100 s ^= 10 s
    uint8_t periodicTimer = 50;     // in 1/100 s ^= 0.5 s
    uint8_t temp;

    uartInit();
	
    enc28j60Init(MACAddr);
    uip_init();
    uip_arp_init();
    
    memcpy(uip_ethaddr.addr, MACAddr, sizeof(MACAddr));

    uip_ipaddr(ipaddr, IPAddr[0], IPAddr[1], IPAddr[2], IPAddr[3]);
    uip_sethostaddr(ipaddr);

    uip_ipaddr(ipaddr, Subnetmask[0], Subnetmask[1], Subnetmask[2], Subnetmask[3]);
    uip_setnetmask(ipaddr);

    uip_ipaddr(ipaddr, Gateway[0], Gateway[1], Gateway[2], Gateway[3]);
    uip_setdraddr(ipaddr);

    // 2 Sekunden warten
    _delay_ms(200);
    _delay_ms(200);
    _delay_ms(200);
    _delay_ms(200);
    _delay_ms(200);
    _delay_ms(200);
    _delay_ms(200);
    _delay_ms(200);
    _delay_ms(200);
    _delay_ms(200);
	
    // tcpAppInit();
	udpAppInit();
        
    // --- Timer setup --------------------------------------------
    TCNT0 = 100;
    TCCR0 |= (1 << CS02) | (1 << CS00);
    // ------------------------------------------------------------

    while(1 > 0)
    {
        uip_len = enc28j60ReceivePacket(uip_buf, UIP_CONF_BUFFER_SIZE);
        if(uip_len > 0)
        {
            if(UIP_BUFFER->type == HTONS(UIP_ETHTYPE_IP))
            {
                uip_arp_ipin();
                uip_input();

                if(uip_len > 0)
                {
                    uip_arp_out();
                    enc28j60TransmitPacket(uip_buf, uip_len, 0x00);
                }
            }
            else if(UIP_BUFFER->type == HTONS(UIP_ETHTYPE_ARP))
            {
                uip_arp_arpin();
                if(uip_len > 0)
                    enc28j60TransmitPacket(uip_buf, uip_len, 0x00);
            }
        }

        if(TIFR & (1 << TOV0))
        {
            TCNT0 = 100;
            TIFR |= (1 << TOV0);
            arpTimer--;
            periodicTimer--;
        }
        if(arpTimer == 0)
        {
            arpTimer = 1000;
            uip_arp_timer();
        }
        if(periodicTimer == 0)
        {
            periodicTimer = 50;
			
            for(temp = 0; temp < UIP_CONNS; temp++)
            {
                uip_periodic(temp);				
                if(uip_len > 0)
                {
                    uip_arp_out();
                    enc28j60TransmitPacket(uip_buf, uip_len, 0x00);
                }
            }
			
            for(temp = 0; temp < UIP_UDP_CONNS; temp++)
            {
                uip_udp_periodic(temp);				
                if(uip_len > 0)
                {
                    uip_arp_out();
                    enc28j60TransmitPacket(uip_buf, uip_len, 0x00);
                }
            }
        }
    }

    return 0;
}

void uip_log(char *m)
{
    uartPutStringP(PSTR("uIP: "));
    uartPutStringP(m);
    uartPutChar('\n');
}
