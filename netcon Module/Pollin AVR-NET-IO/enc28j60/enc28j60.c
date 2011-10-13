/*
 * Datei:               enc28j60.c
 * Author:              dev00
 * Beschreibung:        Stellt Funktionen fuer die Benutzung des ENC28J60
 *                      Netzwerkcontrollers zur Verfuegung.
 *
 * Aenderungsdatum:     Fr, 14. Okt 2011 01:01:04
 *
 */

#include <avr/io.h>
#include <util/delay.h>
#include <inttypes.h>

#include "enc28j60.h"

uint8_t current_bank = 0;
uint16_t next_packet_addr = RECEIVE_BUFFER_START;

/*
 * Interne Funktionen
 *
 */
void get_cs(void)
{
        PORTB &= ~(1 << PB4);
}

void release_cs(void)
{
        PORTB |= (1 << PB4);
}

void enc28j60_set_bank(uint8_t reg_addr)
{
        if(((reg_addr & ADDR_MASK) < KEY_REGISTERS_START) &&
            current_bank != (reg_addr & BANK_MASK)) {
                current_bank = (reg_addr & BANK_MASK);

                enc28j60_bit_field_clear(ECON1, (1 << ECON1_BSEL1) |
                                                (1 << ECON1_BSEL0));

                enc28j60_bit_field_set(ECON1, (current_bank >> 5));
        }
}

void enc28j60_system_reset(void)
{
        get_cs();

        SPDR = OPCODE_SRC | 0x1F;
        while(!(SPSR & (1 << SPIF)));

        release_cs();
}

uint8_t enc28j60_read_control_register(uint8_t reg_addr)
{
        uint8_t data;

        enc28j60_set_bank(reg_addr);

        get_cs();

        SPDR = OPCODE_RCR | (reg_addr & ADDR_MASK);
        while(!(SPSR & (1 << SPIF)));

        SPDR = 0x00;
        while(!(SPSR & (1 << SPIF)));

        if(reg_addr & DUMMY_TRANSFER) {
                SPDR = 0x00;
                while(!(SPSR & (1 << SPIF)));
        }

        data = SPDR;

        release_cs();

        return data;
}

void enc28j60_write_control_register(uint8_t reg_addr, uint8_t data)
{
        enc28j60_set_bank(reg_addr);

        get_cs();

        SPDR = OPCODE_WCR | (reg_addr & ADDR_MASK);
        while(!(SPSR & (1 << SPIF)));

        SPDR = data;

        while(!(SPSR & (1 << SPIF)));

        release_cs();
}

void enc28j60_read_buffer_memory(uint8_t *data, uint16_t len)
{
        get_cs();

        SPDR = OPCODE_RBM | 0x1A;
        while(!(SPSR & (1 << SPIF)));

        while(len--) {
                SPDR = 0x00;
                while(!(SPSR & (1 << SPIF)));
                *data++ = SPDR;
        }

        release_cs();
}

void enc28j60_write_buffer_memory(const uint8_t *data, uint16_t len)
{
        get_cs();

        SPDR = OPCODE_WBM | 0x1A;
        while(!(SPSR & (1 << SPIF)));

        while(len--) {
                SPDR = *data++;
                while(!(SPSR & (1 << SPIF)));
        }

        release_cs();
}

void enc28j60_bit_field_set(uint8_t reg_addr, uint8_t bitfield)
{
        enc28j60_set_bank(reg_addr);

        get_cs();

        SPDR = OPCODE_BFS | (reg_addr & ADDR_MASK);
        while(!(SPSR & (1 << SPIF)));

        SPDR = bitfield;
        while(!(SPSR & (1 << SPIF)));

        release_cs();
}

void enc28j60_bit_field_clear(uint8_t reg_addr, uint8_t bitfield)
{
        enc28j60_set_bank(reg_addr);

        get_cs();

        SPDR = OPCODE_BFC | (reg_addr & ADDR_MASK);
        while(!(SPSR & (1 << SPIF)));

        SPDR = bitfield;
        while(!(SPSR & (1 << SPIF)));

        release_cs();
}

uint16_t enc28j60_read_PHY_register(uint8_t reg_addr)
{
        enc28j60_write_control_register(MIREGADR, reg_addr);
        enc28j60_write_control_register(MICMD, (1 << MICMD_MIIRD));

        while(enc28j60_read_control_register(MISTAT) & (1 << MISTAT_BUSY));

        enc28j60_write_control_register(MICMD, (0 << MICMD_MIIRD));

        return enc28j60_read_control_register(MIRDL) | (enc28j60_read_control_register(MIRDH) << 8);
}

void enc28j60_write_PHY_register(uint8_t reg_addr, uint16_t data)
{
        enc28j60_write_control_register(MIREGADR, reg_addr);
        enc28j60_write_control_register(MIWRL, data);
        enc28j60_write_control_register(MIWRH, data >> 8);

        while(enc28j60_read_control_register(MISTAT) & (1 << MISTAT_BUSY));
}

/*
 * Benutzerfunktionen
 *
 */
void enc28j60_init(const uint8_t *mac_addr)
{
        /* Ausgaenge:  SS           MOSI          SCK */
        DDRB |= (1 << DDB4) | (1 << DDB5) | (1 << DDB7);

        /* Eingang:    MISO */
        DDRB &= ~(1 << DDB6);

        /*      SCK = F_CPU/2  Master        SPI-Enable */
        SPCR |= (1 << SPI2X) | (1 << MSTR) | (1 << SPE);

        enc28j60_system_reset();

        /* Siehe ENC28J60 Errata, Seite 3, Issue 2 */
        _delay_ms(1);

        /* Empfangspuffer einstellen */
        enc28j60_write_control_register(ERXSTL, RECEIVE_BUFFER_START);
        enc28j60_write_control_register(ERXSTH, RECEIVE_BUFFER_START >> 8);
        enc28j60_write_control_register(ERXNDL, RECEIVE_BUFFER_END);
        enc28j60_write_control_register(ERXNDH, RECEIVE_BUFFER_END >> 8);

        /* MAC initialisieren */
        enc28j60_bit_field_set(MACON1, (1 << MACON1_MARXEN) |
                                       (1 << MACON1_TXPAUS) |
                                       (1 << MACON1_RXPAUS));

        enc28j60_bit_field_clear(MACON2, 1 << MACON2_MARST);
        enc28j60_bit_field_set(MACON3, (1 << MACON3_FULDPX) |
                                       (1 << MACON3_FRMLNEN) |
                                       (1 << MACON3_TXCRCEN) |
                                       (1 << MACON3_PADCFG0));

        enc28j60_write_control_register(MAMXFLL, MAX_FRAME_LENGTH + 4);
        enc28j60_write_control_register(MAMXFLH, (MAX_FRAME_LENGTH + 4) >> 8);

        /* Siehe ENC28J60 Datenblatt, Seite 34 */
        enc28j60_write_control_register(MABBIPG, 0x15);
        enc28j60_write_control_register(MAIPGL, 0x12);

        enc28j60_write_control_register(MAADR1, mac_addr[0]);
        enc28j60_write_control_register(MAADR2, mac_addr[1]);
        enc28j60_write_control_register(MAADR3, mac_addr[2]);
        enc28j60_write_control_register(MAADR4, mac_addr[3]);
        enc28j60_write_control_register(MAADR5, mac_addr[4]);
        enc28j60_write_control_register(MAADR6, mac_addr[5]);

        enc28j60_write_PHY_register(PHCON1, 1 << PHCON1_PDPXMD);

        enc28j60_bit_field_set(ECON1, 1 << ECON1_RXEN);
}

void enc28j60_transmit(const uint8_t *data, uint16_t len)
{
        uint8_t per_packet_control_byte = 0;
        uint16_t adress = TRANSMIT_BUFFER_START;

        while(enc28j60_read_control_register(ECON1) & (1 << ECON1_TXRTS));

        enc28j60_write_control_register(ETXSTL, adress);
        enc28j60_write_control_register(ETXSTH, adress >> 8);

        enc28j60_write_control_register(EWRPTL, adress);
        enc28j60_write_control_register(EWRPTH, adress >> 8);

        enc28j60_write_buffer_memory(&per_packet_control_byte, 1);
        enc28j60_write_buffer_memory(data, len);

        adress += len;

        enc28j60_write_control_register(ETXNDL, adress);
        enc28j60_write_control_register(ETXNDH, adress >> 8);

        enc28j60_bit_field_clear(EIR, 1 << EIR_TXIF);
        enc28j60_bit_field_set(EIE, 1 << EIE_TXIE);

        enc28j60_bit_field_set(ECON1, 1 << ECON1_TXRTS);
}

uint16_t enc28j60_receive(uint8_t *data, uint16_t max_len)
{
        uint16_t length, temp;
        uint8_t header[6];

        if(!enc28j60_read_control_register(EPKTCNT))
                return 0;

        enc28j60_write_control_register(ERDPTL, next_packet_addr);
        enc28j60_write_control_register(ERDPTH, next_packet_addr >> 8);

        enc28j60_read_buffer_memory(header, 6);
        next_packet_addr = header[0] | (header[1] << 8);

        length = (header[2] | (header[3] << 8)) - 4;

        length = length > max_len ? max_len : length;

        enc28j60_read_buffer_memory(data, length);

        /* ENC28J60 Errata, Seite 6, Issue 14 */
        temp = enc28j60_read_control_register(ERXSTL) | (enc28j60_read_control_register(ERXSTH) << 8);

        if(next_packet_addr == temp) {
                enc28j60_write_control_register(ERXRDPTL, RECEIVE_BUFFER_END);
                enc28j60_write_control_register(ERXRDPTH, RECEIVE_BUFFER_END >> 8);
        } else {
                temp = next_packet_addr - 1;
                enc28j60_write_control_register(ERXRDPTL, temp);
                enc28j60_write_control_register(ERXRDPTH, temp >> 8);
        }

        enc28j60_bit_field_set(ECON2, 1 << ECON2_PKTDEC);

        return length;
}

