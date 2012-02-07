/*
 * Datei:               netcon.h
 * Author:              dev00
 * Beschreibung:
 *
 * Aenderungsdatum:     Di, 07. Feb 2012 14:07:23
 *
 */

#ifndef __DEVICE_H__
#define __DEVICE_H__

#include <stdint.h>

struct device{
        char    value[16];
        uint8_t type;
        char    dtype;
};

void device_init(void);
void device_update(void);

// Device specific
#define NUMBER_OF_DEVICES 2
extern struct device device_list[NUMBER_OF_DEVICES];

#endif

