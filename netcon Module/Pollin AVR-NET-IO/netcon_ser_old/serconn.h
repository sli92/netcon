/*
 * Datei:               serconn.h
 * Author:              dev00
 * Beschreibung:
 *
 * Aenderungsdatum:     Do, 03. Mai 2012 13:39:27
 *
 */

#ifndef __SERCONN_H__
#define __SERCONN_H__

#include <stdint.h>

#define STRING_LENGTH 20

struct device {
        char    value[STRING_LENGTH];
        char    min[STRING_LENGTH];
        char    max[STRING_LENGTH];
        uint8_t type;
        char    dtype;
};

extern char modulname[STRING_LENGTH];
extern char location[STRING_LENGTH];
extern uint8_t device_count;
extern struct device *device_list;

#include <stdint.h>

uint8_t read_answer(char *dst);
void serconn_init(void);
void update_values(void);

#endif

