#ifndef __CP2200_H__
#define __CP2200_H__

#include <stdint.h>

#define MACCN           0x00
#define MACCF           0x01
#define IPGT            0x02
#define IPGR            0x03
#define CWMAXR          0x04
#define MAXLEN          0x05
#define MACAD0          0x10
#define MACAD1          0x11
#define MACAD2          0x12

/*
 * INT0
 *
 */
#define _RXINT          0x00
#define _RXFINT         0x01
#define _TXINT          0x02
#define _FLWEINT        0x03
#define _OSCINT         0x04
#define _SELFINT        0x05
#define _RXEINT         0x06
#define _EOPINT         0x07

/*
 * PHYCN
 *
 */
#define _LINKSTA        0x00
#define _POLREV         0x01
#define _LPRFAULT       0x02
#define _LBMD           0x03
#define _DPLXMD         0x04
#define _RXEN           0x05
#define _TXEN           0x06
#define _PHYEN          0x07

/*
 * PHYCF
 *
 */
#define _REVPOL         0x00
#define _AUTOPOL        0x01
#define _ADPAUSE        0x02
#define _AUTONEG        0x04
#define _JABBER         0x05
#define _LINKINTG       0x06
#define _SMSQ           0x07

/*
 * IOPWR
 *
 */
#define _WEAKD          0x01
#define _LINKEN         0x02
#define _ACTEN          0x03

/*
 * TXPWR
 *
 */
#define _PSAVED      0x07

void cp2200_init(void);
void write_mac_register(uint8_t reg_addr, uint16_t value);

#endif
