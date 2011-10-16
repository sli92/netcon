/*
 * Datei:               uip-conf.h
 * Author:              dev00
 * Beschreibung:        Konfiguration fuer den uIP TCP/IP Stack,
 *
 * Aenderungsdatum:     Sa, 15. Okt 2011 12:28:21
 *
 */

#ifndef __UIP_CONF_H__
#define __UIP_CONF_H__

#include <stdint.h>
#include "enc28j60.h"
#include "tcp_app.h"

typedef uint8_t uip_tcp_appstate_t;

#define UIP_APPCALL tcp_app_call

typedef uint8_t u8_t;
typedef uint16_t u16_t;
typedef uint16_t uip_stats_t;

#define UIP_CONF_BUFFER_SIZE                    MAX_FRAME_LENGTH
#define UIP_CONF_MAX_CONNECTIONS                5
#define UIP_CONF_MAX_LISTENPORTS                5
#define UIP_CONF_ARPTAB_SIZE                    5
#define UIP_CONF_LOGGING                        1

#endif

