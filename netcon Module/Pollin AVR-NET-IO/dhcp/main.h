/*
 * Datei:               main.c
 * Author:              dev00
 * Beschreibung:        Testet den ENC28J60 Treiber und den uIP Stack. Das
 *                      AVR-NET-IO Board laesst sich anpingen.
 *
 * Aenderungsdatum:     Di, 18. Okt 2011 00:10:38
 *
 */

#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdint.h>

extern const prog_char hostname[];
extern const prog_uint8_t mac_addr[];

int main(void);
uint16_t get_seed(void);
void uip_log(const prog_char *m);

#endif

