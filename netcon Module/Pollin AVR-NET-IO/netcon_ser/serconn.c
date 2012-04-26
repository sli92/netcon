#include <stdlib.h>
#include <stdio.h>
#include "serconn.h"
#include "uart.h"

char name[STRING_LENGTH];
char location[STRING_LENGTH];
uint8_t device_count;
struct device *device_list;


uint8_t read_answer(char *dst)
{
        uint8_t len = 0;

        while(len < STRING_LENGTH) {
                dst[len] = uart_getchar();

                if(dst[len] == '\r' || dst[len] == '\n') {
                        dst[len] = '\0';
                        return len;
                }

                len++;
        }

        dst[len] = '\0';
        return len;
}

void serconn_init(void)
{
        uint8_t i = 0;
        char ch = '\0';
        char buffer[STRING_LENGTH];

        name[0] = '\0';
        location[0] = '\0';
        device_count = 0;

        uart_putchar('n');
        read_answer(name);

        uart_putchar('o');
        read_answer(location);


        uart_putchar('a');
        read_answer(buffer);
        sscanf(buffer, "%X", &device_count);

        device_list = (struct device *)malloc(device_count * sizeof(struct device));
        for(i = 0; i < device_count; i++) {
                sprintf(buffer, "f%02X", i);
                uart_puts(buffer);
                read_answer(buffer);
                device_list[i].dtype = buffer[0];

                sprintf(buffer, "t%02X", i);
                uart_puts(buffer);
                read_answer(buffer);
                sscanf(buffer, "%X", &device_list[i].type);

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

void update_values(void)
{
        uint8_t i = 0;
        char buffer[STRING_LENGTH];

        for(i = 0; i < device_count; i++) {
                sprintf(buffer, "w%02X", i);
                uart_puts(buffer);
                read_answer(device_list[i].value);
        }
}

