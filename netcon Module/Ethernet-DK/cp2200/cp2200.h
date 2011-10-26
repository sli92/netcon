#ifndef __CP2200_H__
#define __CP2200_H__

#include <stdint.h>

#define MACCN   0x00
#define MACCF   0x01
#define IPGT    0x02
#define IPGR    0x03
#define CWMAXR  0x04
#define MAXLEN  0x05
#define MACAD0  0x10
#define MACAD1  0x11
#define MACAD2  0x12

void cp2200_init(void);
void write_mac_register(uint8_t reg_addr, uint16_t value);

#endif
