/*
 * Datei:               main.c
 * Author:              dev00
 * Beschreibung:        Testet den ENC28J60 Treiber und den uIP Stack. Das
 *                      AVR-NET-IO Board laesst sich anpingen.
 *
 * Aenderungsdatum:     Di, 27. Mär 2012 16:39:02
 *
 */

#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdint.h>
#include <avr/pgmspace.h>

extern const char hostname[] PROGMEM;
extern const char place[] PROGMEM;
extern const uint8_t mac_addr[] PROGMEM;

int main(void);
uint16_t get_seed(void);
void uip_log(const prog_char *m);

#endif

