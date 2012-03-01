/*
 * Datei:               netcon.h
 * Author:              dev00
 * Beschreibung:
 *
 * Aenderungsdatum:     Di, 07. Feb 2012 13:45:50
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
void send_error(void);
void parse_request(void);
void netcon_app_call(void);

#endif

