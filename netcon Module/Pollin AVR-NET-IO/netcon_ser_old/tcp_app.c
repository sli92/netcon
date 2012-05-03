/*
 * Datei:               tcp_app.c
 * Author:              dev00
 * Beschreibung:        Stellt die Callback-Funktion fuer den uIP Stack zur
 *                      Verfuegung.
 *
 * Aenderungsdatum:     Do, 26. Jän 2012 08:35:50
 *
 */

#include "tcp_app.h"
#include "netcon.h"

#include "uip/uip.h"

void tcp_app_init(void)
{
        netcon_init();
}

void tcp_app_call(void)
{
        switch(uip_conn->lport) {
                case HTONS(NETCON_CLIENT_PORT):
                        netcon_app_call();
                        break;
        }
}

