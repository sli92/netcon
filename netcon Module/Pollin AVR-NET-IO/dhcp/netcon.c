/*
* Datei:               netcon.c
* Author:              dev00
* Beschreibung:
*
* Aenderungsdatum:     Do, 23. Feb 2012 13:10:19
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
#include "main.h"

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
                } else if(strcasecmp_P(bufptr, PSTR("NAME")) == 0) {
                                char buffer[32];
                                strcpy_P(buffer, hostname);
                                sprintf(uip_appdata, "OK\r\n%s\r\n", buffer);
                                uip_send(uip_appdata, strlen(uip_appdata));
                                return;
                } else if(strcasecmp_P(bufptr, PSTR("PLACE")) == 0) {
                                char buffer[32];
                                strcpy_P(buffer, place);
                                sprintf(uip_appdata, "OK\r\n%s\r\n", buffer);
                                uip_send(uip_appdata, strlen(uip_appdata));
                                return;
                } else if(strcasecmp_P(bufptr, PSTR("DEVICECOUNT")) == 0) {
                                sprintf(uip_appdata, "OK\r\n%d\r\n", NUMBER_OF_DEVICES);
                                uip_send(uip_appdata, strlen(uip_appdata));
                                return;
                } else if(strcasecmp_P(bufptr, PSTR("TYPE")) == 0) {
                        bufptr = strtok(NULL, " \r\n");

                        if(bufptr[0] >= '0' && bufptr[0] <= '8' && (bufptr[0] - '0') < NUMBER_OF_DEVICES) {
                                temp = bufptr[0] - '0';
                                sprintf(uip_appdata, "OK\r\n%d\r\n", device_list[temp].type);
                                uip_send(uip_appdata, strlen(uip_appdata));
                                return;
                        }
                } else if(strcasecmp_P(bufptr, PSTR("DTYPE")) == 0) {
                        bufptr = strtok(NULL, " \r\n");

                        if(bufptr[0] >= '0' && bufptr[0] <= '8' && (bufptr[0] - '0') < NUMBER_OF_DEVICES) {
                                temp = bufptr[0] - '0';
                                sprintf(uip_appdata, "OK\r\n%c\r\n", device_list[temp].dtype);
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
        } else if(uip_rexmit()) {
                send_error();
                uart_puts("uip_rexmit()\n");
        } else if(uip_timedout()) {
                uart_puts("uip_timedout()\n");
        } else if(uip_closed()) {
                uart_puts("uip_closed()\n");
        } else if(uip_aborted()) {
                uart_puts("uip_aborted()\n");
        }
}

