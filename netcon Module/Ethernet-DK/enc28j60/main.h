#ifndef __MAIN_H__
#define __MAIN_H__

int main(void);
void sysclk_init(void);
void io_init(void);
void delay_40us(void);
void delay_20ms(void);
void uip_log(char *m);

#endif
