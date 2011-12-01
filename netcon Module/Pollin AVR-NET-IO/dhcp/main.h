/*
 * Datei:               main.c
 * Author:              dev00
 * Beschreibung:        Testet den ENC28J60 Treiber und den uIP Stack. Das
 *                      AVR-NET-IO Board laesst sich anpingen.
 *
 * Aenderungsdatum:     Do, 01. Dez 2011 10:29:23
 *
 */

#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdint.h>
#include <avr/pgmspace.h>

extern const prog_char hostname[];
extern const prog_uint8_t mac_addr[];
extern const prog_uint8_t device_type;

int main(void);
uint16_t get_seed(void);
void uip_log(const prog_char *m);

#endif

