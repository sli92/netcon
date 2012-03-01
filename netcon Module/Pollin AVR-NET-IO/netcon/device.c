#include <stdint.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include "device.h"
#include "netcon_types.h"
#include "adc.h"

struct device device_list[NUMBER_OF_DEVICES];

void devices_init(void)
{
        device_list[0].value[0] = '\0';
        device_list[0].type = NETCON_TYPE_VOLTAGE;
        device_list[0].dtype = 'f';
        strcpy_P(device_list[0].min, PSTR("0.0"));
        strcpy_P(device_list[0].max, PSTR("5.0"));

        device_list[1].value[0] = '\0';
        device_list[1].type = NETCON_TYPE_VOLTAGE;
        device_list[1].dtype = 'f';
        strcpy_P(device_list[1].min, PSTR("0.0"));
        strcpy_P(device_list[1].max, PSTR("5.0"));
}

void devices_update(void)
{
        sprintf(device_list[0].value, "%.2f", adc_get_value(0) * 0.0048828125);
        sprintf(device_list[1].value, "%.2f", adc_get_value(1) * 0.0048828125);
}

