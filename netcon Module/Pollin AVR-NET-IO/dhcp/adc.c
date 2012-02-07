
/*
 * Datei:               adc.c
 * Author:              dev00
 * Beschreibung:
 *
 * Aenderungsdatum:     Di, 07. Feb 2012 13:27:19
 *
 */

#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>

#include "adc.h"

uint8_t current_channel = 0;

void adc_init(void)
{
        // Externe Referenzspannung (5V)
        ADMUX &= ~((1 << REFS1) | (1 << REFS0));

        ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint16_t adc_get_value(uint8_t channel)
{
        if(current_channel != channel) {
                channel &= 0x07;

                ADMUX &= 0xE0;
                ADMUX |= channel;
        }

        ADCSRA |= (1 << ADSC);

        while(ADCSRA & (1 << ADSC));

        return ADC;
}

