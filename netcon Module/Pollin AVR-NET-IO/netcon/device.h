/*
 * Datei:               netcon.h
 * Author:              dev00
 * Beschreibung:
 *
 * Aenderungsdatum:     Do, 01. Mär 2012 08:18:33
 *
 */

#ifndef __DEVICE_H__
#define __DEVICE_H__

#include <stdint.h>

struct device{
        char    value[16];
        char    min[16];
        char    max[16];
        uint8_t type;
        char    dtype;
};

void device_init(void);
void device_update(void);

// Device specific
#define NUMBER_OF_DEVICES 2
extern struct device device_list[NUMBER_OF_DEVICES];

#endif

