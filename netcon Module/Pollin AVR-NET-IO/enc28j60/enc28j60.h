/*
 * Datei:               enc28j60.h
 * Author:              dev00
 * Beschreibung:        Stellt Funktionen fuer die Benutzung des ENC28J60
 *                      Netzwerkcontrollers zur Verfuegung.
 *
 * Aenderungsdatum:     Do, 13. Okt 2011 23:59:56
 *
 */

#ifndef __ENC28J60_H__
#define __ENC28J60_H__

#include <inttypes.h>

/* Siehe ENC28J60 Errata, Seite 3, Issue 5 */
#define RECEIVE_BUFFER_START    0x0000
#define RECEIVE_BUFFER_END      0x19FF
#define TRANSMIT_BUFFER_START   0x1A00

/* Falls DHCP verwendet wird muss dieser Wert eventuell angepasst werden. */
#define MAX_FRAME_LENGTH        400

/* Register ab dieser Adresse sind in allen Baenken vorhanden. */
#define KEY_REGISTERS_START     0x1B

#define BANK_0                  0x00
#define BANK_1                  0x20
#define BANK_2                  0x40
#define BANK_3                  0x60

#define ADDR_MASK               0x1F
#define BANK_MASK               0x60

/*
 * ENC28J60 Registeradressen Definitionen:
 *      Bit 0-4         Register Addresse
 *      Bit 5-6         Bank Nummer
 *      Bit 7           Lesen des Registers benoetigt einen "dummy" Transfer.
 *
 */
#define DUMMY_TRANSFER          0x80

/*
 * Alle Baenke
 *
 */
#define EIE                     0x1B
#define EIR                     0x1C
#define ESTAT                   0x1D
#define ECON2                   0x1E
#define ECON1                   0x1F

/*
 * Bank 0
 *
 */
#define ERDPTL                  0x00
#define ERDPTH                  0x01
#define EWRPTL                  0x02
#define EWRPTH                  0x03
#define ETXSTL                  0x04
#define ETXSTH                  0x05
#define ETXNDL                  0x06
#define ETXNDH                  0x07
#define ERXSTL                  0x08
#define ERXSTH                  0x09
#define ERXNDL                  0x0A
#define ERXNDH                  0x0B
#define ERXRDPTL                0x0C
#define ERXRDPTH                0x0D
#define ERXWRPTL                0x0E
#define ERXWRPTH                0x0F
#define EDMASTL                 0x10
#define EDMASTH                 0x11
#define EDMANDL                 0x12
#define EDMANDH                 0x13
#define EDMADSTL                0x14
#define EDMADSTH                0x15
#define EDMACSL                 0x16
#define EDMACSH                 0x17

/*
 * Bank 1
 *
 */
#define ETH0                    (0x00 | BANK_1)
#define ETH1                    (0x01 | BANK_1)
#define ETH2                    (0x02 | BANK_1)
#define ETH3                    (0x03 | BANK_1)
#define ETH4                    (0x04 | BANK_1)
#define ETH5                    (0x05 | BANK_1)
#define ETH6                    (0x06 | BANK_1)
#define ETH7                    (0x07 | BANK_1)
#define EPMM0                   (0x08 | BANK_1)
#define EPMM1                   (0x09 | BANK_1)
#define EPMM2                   (0x0A | BANK_1)
#define EPMM3                   (0x0B | BANK_1)
#define EPMM4                   (0x0C | BANK_1)
#define EPMM5                   (0x0D | BANK_1)
#define EPMM6                   (0x0E | BANK_1)
#define EPMM7                   (0x0F | BANK_1)
#define EPMCSL                  (0x10 | BANK_1)
#define EPMCSH                  (0x11 | BANK_1)
#define EPMOL                   (0x14 | BANK_1)
#define EPMOH                   (0x15 | BANK_1)
#define ERXFCON                 (0x18 | BANK_1)
#define EPKTCNT                 (0x19 | BANK_1)

/*
 * Bank 2
 *
 */
#define MACON1                  (0x00 | BANK_2 | DUMMY_TRANSFER)
#define MACON2                  (0x01 | BANK_2 | DUMMY_TRANSFER)
#define MACON3                  (0x02 | BANK_2 | DUMMY_TRANSFER)
#define MACON4                  (0x03 | BANK_2 | DUMMY_TRANSFER)
#define MABBIPG                 (0x04 | BANK_2 | DUMMY_TRANSFER)
#define MAIPGL                  (0x06 | BANK_2 | DUMMY_TRANSFER)
#define MAIPGH                  (0x07 | BANK_2 | DUMMY_TRANSFER)
#define MACLCON1                (0x08 | BANK_2 | DUMMY_TRANSFER)
#define MACLCON2                (0x09 | BANK_2 | DUMMY_TRANSFER)
#define MAMXFLL                 (0x0A | BANK_2 | DUMMY_TRANSFER)
#define MAMXFLH                 (0x0B | BANK_2 | DUMMY_TRANSFER)
#define MICMD                   (0x12 | BANK_2 | DUMMY_TRANSFER)
#define MIREGADR                (0x14 | BANK_2 | DUMMY_TRANSFER)
#define MIWRL                   (0x16 | BANK_2 | DUMMY_TRANSFER)
#define MIWRH                   (0x17 | BANK_2 | DUMMY_TRANSFER)
#define MIRDL                   (0x18 | BANK_2 | DUMMY_TRANSFER)
#define MIRDH                   (0x19 | BANK_2 | DUMMY_TRANSFER)

/*
 * Bank 3
 *
 */
#define MAADR5                  (0x00 | BANK_3 | DUMMY_TRANSFER)
#define MAADR6                  (0x01 | BANK_3 | DUMMY_TRANSFER)
#define MAADR3                  (0x02 | BANK_3 | DUMMY_TRANSFER)
#define MAADR4                  (0x03 | BANK_3 | DUMMY_TRANSFER)
#define MAADR1                  (0x04 | BANK_3 | DUMMY_TRANSFER)
#define MAADR2                  (0x05 | BANK_3 | DUMMY_TRANSFER)
#define EBSTSD                  (0x06 | BANK_3) 
#define EBSTCON                 (0x07 | BANK_3) 
#define EBSTCSL                 (0x08 | BANK_3) 
#define EBSTCSH                 (0x09 | BANK_3) 
#define MISTAT                  (0x0A | BANK_3 | DUMMY_TRANSFER)
#define EREVID                  (0x12 | BANK_3) 
#define ECOCON                  (0x15 | BANK_3) 
#define EFLOCON                 (0x17 | BANK_3) 
#define EPAUSL                  (0x18 | BANK_3) 
#define EPAUSH                  (0x19 | BANK_3)

/*
 * Bitnamen (nicht vollstaendig)
 *
 */

/*
 * ECON1
 *
 */
#define ECON1_BSEL0             0x00
#define ECON1_BSEL1             0x01
#define ECON1_RXEN              0x02
#define ECON1_TXRTS             0x03
#define ECON1_CSUMEN            0x04
#define ECON1_DMAST             0x05
#define ECON1_RXRST             0x06
#define ECON1_TXRST             0x07

/*
 * ECON2
 *
 */
#define ECON2_VRPS              0x03
#define ECON2_PWRSV             0x05
#define ECON2_PKTDEC            0x06
#define ECON2_AUTOINC           0x07

/*
 * ESTAT
 *
 */
#define ESTAT_CLKRDY            0x00
#define ESTAT_TXABRT            0x01
#define ESTAT_LATECOL           0x04
#define ESTAT_BUFFER            0x06
#define ESTAT_INT               0x07

/*
 * EIE
 *
 */
#define EIE_RXERIE              0x00
#define EIE_TXERIE              0x01
#define EIE_TXIE                0x03
#define EIE_LINKIE              0x04
#define EIE_DMAIE               0x05
#define EIE_PKTIE               0x06
#define EIE_INTIE               0x07

/*
 * EIR
 *
 */
#define EIR_RXERIF              0x00
#define EIR_TXERIF              0x01
#define EIR_TXIF                0x03
#define EIR_LINKIF              0x04
#define EIR_DMAIF               0x05
#define EIR_PKTIF               0x06

/*
 * MICMD
 *
 */
#define MICMD_MIIRD             0x00
#define MICMD_MIISCAN           0x01

/*
 * MISTAT
 *
 */
#define MISTAT_BUSY             0x00
#define MISTAT_SCAN             0x01
#define MISTAT_NVALID           0x02

/*
 * MACON1
 *
 */
#define MACON1_MARXEN           0x00
#define MACON1_PASSALL          0x01
#define MACON1_RXPAUS           0x02
#define MACON1_TXPAUS           0x03

/*
 * MACON2
 *
 */
#define MACON2_TFUNRST          0x00
#define MACON2_MATXRST          0x01
#define MACON2_RFUNRST          0x02
#define MACON2_MARXRST          0x03
#define MACON2_RNDRST           0x06
#define MACON2_MARST            0x07

/*
 * MACON3
 *
 */
#define MACON3_FULDPX           0x00
#define MACON3_FRMLNEN          0x01
#define MACON3_HFRMEN           0x02
#define MACON3_PHDREN           0x03
#define MACON3_TXCRCEN          0x04
#define MACON3_PADCFG0          0x05
#define MACON3_PADCFG1          0x06
#define MACON3_PADCFG2          0x07

/*
 * MACON4
 *
 */
#define MACON4_NOBKOFF          0x04
#define MACON4_BPEN             0x05
#define MACON4_DEFER            0x06

/*
 * PHY-Registeradressen
 *
 */
#define PHCON1                  0x00
#define PHSTAT1                 0x01
#define PHID1                   0x02
#define PHID2                   0x03
#define PHCON2                  0x10
#define PHSTAT2                 0x11
#define PHIE                    0x12
#define PHIR                    0x13
#define PHLCON                  0x14

/*
 * PHY-Bitnamen
 *
 */

/*
 * PHCON1
 *
 */
#define PHCON1_PDPXMD           0x08
#define PHCON1_PPWRSV           0x0A
#define PHCON1_PLOOPBK          0x0E
#define PHCON1_PRST             0x0F

/*
 * PHSTAT1
 *
 */
#define PHSTAT1_JBSTAT          0x01
#define PHSTAT1_LLSTAT          0x02
#define PHSTAT1_PHDPX           0x0B
#define PHSTAT1_PFDPX           0x0C

/*
 * PHCON2
 *
 */
#define PHCON2_HDLDIS           0x08
#define PHCON2_JABBER           0x0A
#define PHCON2_TXDIS            0x0E
#define PHCON2_FRCLNK           0x0D

/*
 * PHSTAT2
 *
 */
#define PHSTAT2_PLRITY          0x05
#define PHSTAT2_DPXSTAT         0x09
#define PHSTAT2_LSTAT           0x0A
#define PHSTAT2_COLSTAT         0x0B
#define PHSTAT2_RXSTAT          0x0C
#define PHSTAT2_TXSRAR          0x0D

/*
 * Opcodes
 *
 */
#define OPCODE_RCR              0x00
#define OPCODE_RBM              0x20
#define OPCODE_WCR              0x40
#define OPCODE_WBM              0x60
#define OPCODE_BFS              0x80
#define OPCODE_BFC              0xA0
#define OPCODE_SRC              0xE0

/*
 * Interne Funktionen
 *
 */
void get_cs(void);
void release_cs(void);

void enc28j60_set_bank(uint8_t reg_addr);

void enc28j60_system_reset(void);

uint8_t enc28j60_read_control_register(uint8_t reg_addr);
void enc28j60_write_control_register(uint8_t reg_addr, uint8_t data);

void enc28j60_read_buffer_memory(uint8_t *data, uint16_t len);
void enc28j60_write_buffer_memory(const uint8_t *data, uint16_t len);

void enc28j60_bit_field_set(uint8_t reg_addr, uint8_t bitfield);
void enc28j60_bit_field_clear(uint8_t reg_addr, uint8_t bitfield);

uint16_t enc28j60_read_PHY_register(uint8_t reg_addr);
void enc28j60_write_PHY_register(uint8_t reg_addr, uint16_t data);

/*
 * Benutzerfunktionen
 *
 */
void enc28j60_init(const uint8_t *mac_addr);
void enc28j60_transmit(const uint8_t *data, uint16_t len);
uint16_t enc28j60_receive(uint8_t *data, uint16_t max_len);

#endif

