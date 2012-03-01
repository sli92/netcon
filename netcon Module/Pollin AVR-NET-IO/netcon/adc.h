/*
 * Datei:               adc.h
 * Author:              dev00
 * Beschreibung:
 *
 * Aenderungsdatum:     Do, 12. Jän 2012 08:39:16
 *
 */

#ifndef __ADC_H__
#define __ADC_H__

#define BAUD 9600UL

#include <stdint.h>

void adc_init(void);
uint16_t adc_get_value(uint8_t channel);

#endif

