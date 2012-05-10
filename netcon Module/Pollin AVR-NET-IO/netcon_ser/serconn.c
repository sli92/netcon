#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include "netcon_types.h"
#include "serconn.h"
#include "main.h"
#include "uart.h"

struct device device_list[MAX_DEVICES];
uint32_t device_count = 0;

void serconn_init(void)
{
        uint8_t i = 0;
        uint32_t tmp;
        char ch = '\0';
        char buffer[STRING_LENGTH];

        // Flush the UART buffer
        while(uart_char_in())
                uart_getchar_ne();

        uart_putchar('n');
        read_answer(hostname);

        uart_putchar('o');
        read_answer(place);

        uart_putchar('a');
        read_answer(buffer);
        sscanf(buffer, "%X", &device_count);

        if(device_count > MAX_DEVICES) {
                device_count = 0;
                return;
        }

        for(i = 0; i < device_count; i++) {
                sprintf(buffer, "f%02X", i);
                uart_puts(buffer);
                read_answer(buffer);
                device_list[i].dtype = buffer[0];

                sprintf(buffer, "t%02X", i);
                uart_puts(buffer);
                read_answer(buffer);
                sscanf(buffer, "%X", &tmp);
                device_list[i].type = tmp;

                sprintf(buffer, "m%02X", i);
                uart_puts(buffer);
                read_answer(device_list[i].min);

                sprintf(buffer, "x%02X", i);
                uart_puts(buffer);
                read_answer(device_list[i].max);

                sprintf(buffer, "w%02X", i);
                uart_puts(buffer);
                read_answer(device_list[i].value);
        }
}

/*
void serconn_init(void)
{
        uart_puts("serconn_init()\n");

        read_answer(hostname);
}
*/

void serconn_update(void)
{
        uint8_t i = 0;
        char buffer[STRING_LENGTH];

        for(i = 0; i < device_count; i++) {
                sprintf(buffer, "w%02X", i);
                uart_puts(buffer);
                read_answer(device_list[i].value);
        }
}

uint8_t read_answer(char *dst)
{
        uint8_t len = 0;

        while(len < STRING_LENGTH) {
                dst[len] = uart_getchar_ne();

                if(dst[len] == '\r' || dst[len] == '\n') {
                        dst[len] = '\0';

                        while(uart_char_in())
                                uart_getchar_ne();

                        return len;
                }

                len++;
        }

        dst[len] = '\0';
        return len;
}

