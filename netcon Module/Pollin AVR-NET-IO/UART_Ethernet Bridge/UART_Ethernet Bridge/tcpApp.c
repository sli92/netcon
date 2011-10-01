#include "uip/uip.h"
#include "uart.h"
#include "main.h"
#include "tcpApp.h"

void tcpAppInit(void)
{
    uip_listen(HTONS(23));
}

void tcpAppCall(void)
{
    if(uip_newdata())
    {
		*((char *)uip_appdata + uip_datalen()) = '\0';
		uartPutString(uip_appdata);	
	}

	if(uip_poll() && (uartLineBufferFlags & (1 << UART_LINE_BUFFER_READY_TO_SEND)))
	{
		uip_send(uartLineBuffer, uartLineBufferPos);
	}
	
    if(uip_acked())
    {
		// reenable uart buffer
		uartLineBufferPos = 0;
		uartLineBufferFlags |= 1 << UART_LINE_BUFFER_READY_TO_FILL;
		uartLineBufferFlags &= ~(1 << UART_LINE_BUFFER_READY_TO_SEND);
    }
}

