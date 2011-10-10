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
    uint8_t temp, allAcked;
    struct uipTcpAppState *state = &(uip_conn->appstate);
    
    if(uip_connected())
    {
        state->state = STATE_IDLE;
    }
        
    if(uip_newdata())
    {
        *((char *)uip_appdata + uip_datalen()) = '\0';
        uartPutString(uip_appdata);    
    }

    if(uip_poll())
    {
	    if(uartLineBufferFlags & (1 << UART_LINE_BUFFER_READY_TO_SEND))
        {
            if(state->state == STATE_IDLE)
            {
                uip_send(uartLineBuffer, uartLineBufferPos);
            
                state->state = STATE_SENT;
            }
		}		
    }
    
    if(uip_rexmit())
    {
        uip_send(uartLineBuffer, uartLineBufferPos);
    }
    
    if(uip_closed() || uip_aborted() || uip_timedout())
    {
        state->state = STATE_ACKED;
    }
    
    if(uip_acked())
    {
        if(state->state == STATE_SENT)
        {
            state->state = STATE_ACKED;
        }
        
        allAcked = 1;
        for(temp = 0; temp < UIP_CONNS; temp++)
        {
            if(uip_conn_active(temp) && uip_conns[temp].appstate.state != STATE_ACKED)
            {
                allAcked = 0;
                break;
            }
        }
        
        // reenable uart buffer
        if(allAcked == 1)
        {
            for(temp = 0; temp < UIP_CONNS; temp++)
                if(uip_conn_active(temp))
                    uip_conns[temp].appstate.state = STATE_IDLE;
            
            uartLineBufferPos = 0;
            uartLineBufferFlags |= 1 << UART_LINE_BUFFER_READY_TO_FILL;
            uartLineBufferFlags &= ~(1 << UART_LINE_BUFFER_READY_TO_SEND);
        }
    }
}


