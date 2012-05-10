/*
 * Datei:               netcon.h
 * Author:              dev00
 * Beschreibung:
 *
 * Aenderungsdatum:     Do, 10. Mai 2012 11:06:46
 *
 */

#ifndef __DEVICE_H__
#define __DEVICE_H__

#include <stdint.h>

#define STRING_LENGTH 20
#define MAX_DEVICES   04

struct device{
        char    value[STRING_LENGTH];
        char    min[STRING_LENGTH];
        char    max[STRING_LENGTH];
        uint8_t type;
        char    dtype;
};

void serconn_init(void);
void serconn_update(void);
uint8_t read_answer(char *dst);

// Device specific
extern struct device device_list[MAX_DEVICES];
extern uint32_t device_count;

#endif

