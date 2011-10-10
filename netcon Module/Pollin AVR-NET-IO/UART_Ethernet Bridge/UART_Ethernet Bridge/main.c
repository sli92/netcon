/*
 * main.c
 * UART-Ethernet Bridge
 */

#include <avr/io.h>
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
const uint8_t MACAddr[6] = {0x02, 0x00, 0x00, 0x00, 0x00, 0x01};

uint8_t IPAddr[4] = {192, 168, 1, 8};
uint8_t Subnetmask[4] = {255, 255, 255, 0};
uint8_t Gateway[4] = {192, 168, 1, 1};

// Fürs Schulnetz
/*
const uint8_t IPAddr[4] = {172, 100, 190, 45};
const uint8_t Subnetmask[4] = {255, 255, 0, 0};
const uint8_t Gateway[4] = {172, 100, 1, 200};
*/

char uartLineBuffer[UART_LINE_BUFFER_SIZE];
uint8_t uartLineBufferPos = 0;
uint8_t uartLineBufferFlags = 0x00;

// Todo: move constant stuff like strings, etc. to program-memory

int main(void)
{
    uip_ipaddr_t ipaddr;
    uint16_t arpTimer = 1000;       // in 1/100 s ^= 10 s
    uint8_t periodicTimer = 50;     // in 1/100 s ^= 0.5 s
    uint8_t temp, activeConns;

    uartInit();

    enc28j60Init(MACAddr);
    uip_init();
    uip_arp_init();
    
    memcpy(uip_ethaddr.addr, MACAddr, 6);

    uip_ipaddr(ipaddr, IPAddr[0], IPAddr[1], IPAddr[2], IPAddr[3]);
    uip_sethostaddr(ipaddr);

    uip_ipaddr(ipaddr, Subnetmask[0], Subnetmask[1], Subnetmask[2], Subnetmask[3]);
    uip_setnetmask(ipaddr);

    uip_ipaddr(ipaddr, Gateway[0], Gateway[1], Gateway[2], Gateway[3]);
    uip_setdraddr(ipaddr);

    tcpAppInit();
        
    // --- Timer setup --------------------------------------------
    TCNT0 = 100;
    TCCR0 |= (1 << CS02) | (1 << CS00);
    // ------------------------------------------------------------

    uartLineBufferFlags |= (1 << UART_LINE_BUFFER_READY_TO_FILL);
    while(1 > 0)
    {
        if(uartCharIn() && (uartLineBufferFlags & (1 << UART_LINE_BUFFER_READY_TO_FILL)))
        {
            temp = uartGetChar();
            
            if(temp == '\n' || uartLineBufferPos > (UART_LINE_BUFFER_SIZE - 3))
            {
                if(temp == '\n')
                {
                    uartLineBuffer[uartLineBufferPos++] = '\r';
                    uartLineBuffer[uartLineBufferPos++] = '\n';
                }
                else
                    uartLineBuffer[uartLineBufferPos++] = temp;
                
                uartLineBufferFlags |= 1 << UART_LINE_BUFFER_READY_TO_SEND;
                uartLineBufferFlags &= ~(1 << UART_LINE_BUFFER_READY_TO_FILL);
				
				// Fire periodic timer instantly to send the new line, maximum delay = 10 ms
				periodicTimer = 1;
            }
            else
                uartLineBuffer[uartLineBufferPos++] = temp;
        }
        
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
			
			activeConns = 0;
            for(temp = 0; temp < UIP_CONNS; temp++)
            {
                uip_periodic(temp);
				
				if(uip_conn_active(temp))
				    activeConns++;
				
                if(uip_len > 0)
                {
                    uip_arp_out();
                    enc28j60TransmitPacket(uip_buf, uip_len, 0x00);
                }
            }
			
			if((activeConns == 0) && (uartLineBufferFlags & (1 << UART_LINE_BUFFER_READY_TO_SEND)))
			{
                uartLineBufferPos = 0;
                uartLineBufferFlags |= 1 << UART_LINE_BUFFER_READY_TO_FILL;
                uartLineBufferFlags &= ~(1 << UART_LINE_BUFFER_READY_TO_SEND);
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
