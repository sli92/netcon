/*
 * Datei:               netcon.h
 * Author:              dev00
 * Beschreibung:
 *
 * Aenderungsdatum:     Sa, 03. Mär 2012 17:18:23
 *
 */

#ifndef __NETCON_H__
#define __NETCON_H__

#include <stdint.h>
#include <avr/pgmspace.h>

#define NETCON_CLIENT_PORT 50003

#define MAX_KEY_LEN 32
#define MAX_VAL_LEN 32

void netcon_init(void);
void parse_request(void);
void netcon_app_call(void);

#endif

