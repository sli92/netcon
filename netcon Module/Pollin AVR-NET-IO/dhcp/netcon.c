/*
* Datei:               netcon.c
* Author:              dev00
* Beschreibung:
*
* Aenderungsdatum:     Di, 07. Feb 2012 14:23:08
*
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <avr/pgmspace.h>
#include "uip-conf.h"
#include "uip/uip.h"
#include "uart.h"
#include "device.h"

#include "netcon.h"

void netcon_init(void)
{
        uip_listen(HTONS(NETCON_CLIENT_PORT));
}

void send_error(void)
{
        strcpy_P(uip_appdata, PSTR("ERROR\r\n"));
        uip_send(uip_appdata, 7);
}

void parse_request(void)
{
        uint8_t temp;
        char *bufptr;

        ((char *)uip_appdata)[uip_len] = '\0';

        bufptr = strtok(uip_appdata, " \r\n");

        if(strcasecmp_P(bufptr, PSTR("GET")) == 0) {
                bufptr = strtok(NULL, " \r\n");

                if(strcasecmp_P(bufptr, PSTR("VALUE")) == 0) {
                        bufptr = strtok(NULL, " \r\n");

                        if(bufptr[0] >= '0' && bufptr[0] <= '8' && (bufptr[0] - '0') < NUMBER_OF_DEVICES) {
                                temp = bufptr[0] - '0';
                                sprintf(uip_appdata, "OK\r\n%s\r\n", device_list[temp].value);
                                uip_send(uip_appdata, strlen(uip_appdata));
                                return;
                        }
                }

        } else if(strcasecmp_P(bufptr, PSTR("SET")) == 0) {

        }

        send_error();
}

void netcon_app_call(void)
{
        if(uip_newdata()) {
                parse_request();
        }
}

