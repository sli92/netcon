/*
* Datei:               netcon.c
* Author:              dev00
* Beschreibung:
*
* Aenderungsdatum:     Di, 07. Feb 2012 03:12:08
*
*/

#include <stdlib.h>
#include <string.h>
#include <avr/pgmspace.h>
#include "uip-conf.h"
#include "uip/uip.h"
#include "uart.h"

#include "netcon.h"

void netcon_init(void)
{
        uip_listen(HTONS(NETCON_CLIENT_PORT));
}

void parse_request(void)
{
        char *bufptr;

        if(uip_len >= UIP_CONF_BUFFER_SIZE) {
                strcpy_P(uip_appdata, PSTR("ERROR\r\n"));
                uip_send(uip_appdata, strlen(uip_appdata));
                return;
        }

        ((char *)uip_appdata)[uip_len] = '\0';

        bufptr = strtok(uip_appdata, " \r\n");
        while(bufptr != NULL) {
                bufptr = strtok(NULL, " \r\n");
        }
}

void netcon_app_call(void)
{
        if(uip_newdata()) {
                parse_request();
        }
}

