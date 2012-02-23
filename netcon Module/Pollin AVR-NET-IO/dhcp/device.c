#include <stdint.h>
#include <stdio.h>
#include "device.h"
#include "netcon_types.h"
#include "adc.h"

struct device device_list[NUMBER_OF_DEVICES];

void devices_init(void)
{
        device_list[0].value[0] = '\0';
        device_list[0].type = NETCON_TYPE_VOLTAGE;
        device_list[0].dtype = 'f';

        device_list[1].value[0] = '\0';
        device_list[1].type = NETCON_TYPE_VOLTAGE;
        device_list[1].dtype = 'f';
}

void devices_update(void)
{
        sprintf(device_list[0].value, "%.4f", adc_get_value(0) * (5.0 / 1024));
        sprintf(device_list[1].value, "%.4f", adc_get_value(1) * (5.0 / 1024));
}

