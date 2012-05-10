/*
 * Datei:               main.c
 * Author:              dev00
 * Beschreibung:        Testet den ENC28J60 Treiber und den uIP Stack. Das
 *                      AVR-NET-IO Board laesst sich anpingen.
 *
 * Aenderungsdatum:     Do, 10. Mai 2012 09:31:52
 *
 */

#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdint.h>
#include <avr/pgmspace.h>

extern char hostname[20];
extern char place[20];
extern const uint8_t mac_addr[] PROGMEM;

int main(void);
uint16_t get_seed(void);
void uip_log(const prog_char *m);

#endif

