/*
 * Datei:               main.c
 * Author:              dev00
 * Beschreibung:        Testet den ENC28J60 Treiber und den uIP Stack. Das
 *                      AVR-NET-IO Board laesst sich anpingen.
 *
 * Aenderungsdatum:     Sa, 15. Okt 2011 12:38:13
 *
 */

#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdint.h>

extern const uint8_t mac_addr[6];

int main(void);
uint16_t get_seed(void);
void uip_log(char *m);

#endif

