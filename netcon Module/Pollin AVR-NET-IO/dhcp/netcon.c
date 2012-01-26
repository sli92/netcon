/*
* Datei:               netcon.c
* Author:              dev00
* Beschreibung:
*
* Aenderungsdatum:     Do, 26. Jän 2012 13:14:43
*
*/

#include <stdlib.h>
#include <string.h>
#include <avr/pgmspace.h>
#include "uip/uip.h"
#include "uart.h"

#include "netcon.h"

void netcon_init(void)
{
        uip_listen(HTONS(NETCON_CLIENT_PORT));
}

void parse_request(void)
{
}

void netcon_app_call(void)
{
        if(uip_newdata()) {
                parse_request();
        }
}

