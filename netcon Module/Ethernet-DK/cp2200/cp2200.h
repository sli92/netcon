#ifndef __CP2200_H__
#define __CP2200_H__

#include <stdint.h>

#define MAX_FRAME_LENGTH    400

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
 * INT0EN
 *
 */
#define _ERXINT         0x00

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
#define _PSAVED         0x07

/*
 * TXCN
 *
 */
#define _TXGO           0x00
#define _FDPLXOV        0x01
#define _BCKPRES        0x02
#define _TXPPKT         0x03
#define _PADENOV        0x04
#define _CRCENOV        0x05
#define _OVERRIDE       0x07

/*
 * CPINFOH
 *
 */
#define _MCAST          0x00
#define _BCAST          0x01
#define _RXDATA         0x02
#define _RXCF           0x03
#define _RXPCF          0x04
#define _RXUCF          0x05
#define _RXVLAN         0x06
#define _RXVALID        0x07

/*
 * CPINFOL
 *
 */
#define _RXDROP         0x00
#define _RXLEN          0x01
#define _CRCERR         0x04
#define _LENERR         0x05
#define _LENGTH         0x06
#define _RXOK           0x07

/*
 * RXCN
 *
 */
#define _RXCLEAR        0x00
#define _RXSKIP         0x01
#define _RXCLRV         0x02
#define _RXINH          0x03

/*
 * RXSTA
 *
 */
#define _RXBUSY         0x00
#define _CPEND          0x01

/*
 * TXBUSY
 *
 */
#define _TXBUSY         0x00

extern uint8_t mac_addr[6];

void cp2200_init(void);
void cp2200_transmit(const uint8_t *_data, uint16_t len);
uint16_t cp2200_receive(uint8_t *_data, uint16_t max_len);

void cp2200_write_mac_register(uint8_t reg_addr, uint16_t value);

uint8_t cp2200_read_flash_byte(uint16_t addr);
void cp2200_read_flash_sequence(uint16_t start_addr, uint8_t *dest, uint16_t len);

#endif
