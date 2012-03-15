#include <stdlib.h>
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
        ch = uart_getchar();

        if(ch >= '0' && ch <= '9') {
                device_count = ch - '0';
        }


        device_list = (struct device *)malloc(device_count * sizeof(struct device));
        for(i = 0; i < device_count; i++) {
                sprintf(buffer, "f%d", i);
                uart_puts(buffer);
                ch = uart_getchar();
                device_list[i].dtype = ch;

                sprintf(buffer, "t%d", i);
                uart_puts(buffer);
                read_answer(buffer);
                device_list[i].type = strtoul(buffer, NULL, 16);

                sprintf(buffer, "m%d", i);
                uart_puts(buffer);
                read_answer(device_list[i].min);

                sprintf(buffer, "x%d", i);
                uart_puts(buffer);
                read_answer(device_list[i].max);

                sprintf(buffer, "w%d", i);
                uart_puts(buffer);
                read_answer(device_list[i].value);
        }
}

void update_values(void)
{
        uint8_t i = 0;
        char buffer[STRING_LENGTH];

        for(i = 0; i < device_count; i++) {
                sprintf(buffer, "w%d", i);
                uart_puts(buffer);
                read_answer(device_list[i].value);
        }
}

