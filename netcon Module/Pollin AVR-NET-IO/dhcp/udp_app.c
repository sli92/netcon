/*
 * Datei:               udp_app.c
 * Author:              dev00
 * Beschreibung:        Stellt die Callback-Funktion fuer den uIP Stack zur
 *                      Verfuegung.
 *
 * Aenderungsdatum:     Sa, 15. Okt 2011 15:12:21
 *
 */

#include "dhcp.h"

void udp_app_init(void)
{
        dhcp_init();
}

void udp_app_call(void)
{
        switch(uip_udp_conn->lport) {
                case HTONS(DHCP_CLIENT_PORT):
                        dhcp_app_call();
                        break;
        }
}

