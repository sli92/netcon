/*
 * Datei:               netcon.h
 * Author:              dev00
 * Beschreibung:
 *
 * Aenderungsdatum:     Do, 26. Jän 2012 10:27:58
 *
 */

#ifndef __NETFIND_H__
#define __NETFIND_H__

#include <stdint.h>
#include <avr/pgmspace.h>

#define NETCON_CLIENT_PORT 50003

#define MAX_KEY_LEN 32
#define MAX_VAL_LEN 32

void netcon_init(void);
void parse_request(void);
void netcon_app_call(void);

#endif

