/*
* Datei:               netcon.c
* Author:              dev00
* Beschreibung:
*
* Aenderungsdatum:     Sa, 03. Mär 2012 18:03:46
*
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <avr/pgmspace.h>
#include "uip-conf.h"
#include "uip/uip.h"
#include "clock.h"
#include "uart.h"
#include "device.h"
#include "main.h"
#include "dhcp.h"

#include "netcon.h"

void netcon_init(void)
{
        uip_listen(HTONS(NETCON_CLIENT_PORT));
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
                                sprintf_P(uip_appdata, PSTR("OK\r\n%s\r\n"), device_list[temp].value);
                                uip_send(uip_appdata, strlen(uip_appdata));
                                return;
                        }

                } else if(strcasecmp_P(bufptr, PSTR("UPTIME")) == 0) {
                        sprintf_P(uip_appdata, PSTR("OK\r\n%lu\r\n"), get_clock());

                        uip_send(uip_appdata, strlen(uip_appdata));
                        return;
                } else if(strcasecmp_P(bufptr, PSTR("NAME")) == 0) {
                        char buffer[32];
                        strcpy_P(buffer, hostname);
                        sprintf_P(uip_appdata, PSTR("OK\r\n%s\r\n"), buffer);
                        uip_send(uip_appdata, strlen(uip_appdata));
                        return;
                } else if(strcasecmp_P(bufptr, PSTR("PLACE")) == 0) {
                        char buffer[32];
                        strcpy_P(buffer, place);
                        sprintf_P(uip_appdata, PSTR("OK\r\n%s\r\n"), buffer);
                        uip_send(uip_appdata, strlen(uip_appdata));
                        return;
                } else if(strcasecmp_P(bufptr, PSTR("DEVICECOUNT")) == 0) {
                        sprintf_P(uip_appdata, PSTR("OK\r\n%d\r\n"), NUMBER_OF_DEVICES);
                        uip_send(uip_appdata, strlen(uip_appdata));
                        return;
                } else if(strcasecmp_P(bufptr, PSTR("TYPE")) == 0) {
                        bufptr = strtok(NULL, " \r\n");

                        if(bufptr[0] >= '0' && bufptr[0] <= '8' && (bufptr[0] - '0') < NUMBER_OF_DEVICES) {
                                temp = bufptr[0] - '0';
                                sprintf_P(uip_appdata, PSTR("OK\r\n%d\r\n"), device_list[temp].type);
                                uip_send(uip_appdata, strlen(uip_appdata));
                                return;
                        }

                } else if(strcasecmp_P(bufptr, PSTR("DTYPE")) == 0) {
                        bufptr = strtok(NULL, " \r\n");

                        if(bufptr[0] >= '0' && bufptr[0] <= '8' && (bufptr[0] - '0') < NUMBER_OF_DEVICES) {
                                temp = bufptr[0] - '0';
                                sprintf_P(uip_appdata, PSTR("OK\r\n%c\r\n"), device_list[temp].dtype);
                                uip_send(uip_appdata, strlen(uip_appdata));
                                return;
                        }

                } else if(strcasecmp_P(bufptr, PSTR("MIN")) == 0) {
                        bufptr = strtok(NULL, " \r\n");

                        if(bufptr[0] >= '0' && bufptr[0] <= '8' && (bufptr[0] - '0') < NUMBER_OF_DEVICES) {
                                temp = bufptr[0] - '0';
                                sprintf_P(uip_appdata, PSTR("OK\r\n%s\r\n"), device_list[temp].min);
                                uip_send(uip_appdata, strlen(uip_appdata));
                                return;
                        }

                } else if(strcasecmp_P(bufptr, PSTR("MAX")) == 0) {
                        bufptr = strtok(NULL, " \r\n");

                        if(bufptr[0] >= '0' && bufptr[0] <= '8' && (bufptr[0] - '0') < NUMBER_OF_DEVICES) {
                                temp = bufptr[0] - '0';
                                sprintf_P(uip_appdata, PSTR("OK\r\n%s\r\n"), device_list[temp].max);
                                uip_send(uip_appdata, strlen(uip_appdata));
                                return;
                        }

                } else if(strcasecmp_P(bufptr, PSTR("MAC")) == 0) {
                        sprintf_P(uip_appdata, PSTR("OK\r\n%02X:%02X:%02X:%02X:%02X:%02X\r\n"), uip_ethaddr.addr[5], uip_ethaddr.addr[4],
                                                                                                uip_ethaddr.addr[3], uip_ethaddr.addr[2],
                                                                                                uip_ethaddr.addr[1], uip_ethaddr.addr[0]);
                        uip_send(uip_appdata, strlen(uip_appdata));
                        return;
                }

        } else if(strcasecmp_P(bufptr, PSTR("SET")) == 0) {

        }

        strcpy_P(uip_appdata, PSTR("ERROR\r\n"));
        uip_send(uip_appdata, 7);
}

void netcon_app_call(void)
{
        if(uip_newdata()) {
                parse_request();
        }

        if(uip_rexmit()) {
                // Psst no one saw this :D
                strcpy_P(uip_appdata, PSTR("ERROR\r\n"));
                uip_send(uip_appdata, 7);
        }
}

