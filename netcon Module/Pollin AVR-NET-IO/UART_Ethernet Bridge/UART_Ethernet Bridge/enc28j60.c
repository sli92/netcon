#include <avr/io.h>
#include <util/delay.h>
#include "enc28j60.h"

uint8_t enc28j60CurrentBank = 0;
uint16_t enc28j60NextPacketAdress = RECEIVE_BUFFER_START;

void enc28j60Init(const uint8_t *MACAddr)
{
    // Ausgänge:   SS           MOSI          SCK
    DDRB |= (1 << DDB4) | (1 << DDB5) | (1 << DDB7);

    // Eingange:   MISO
    DDRB &= ~(1 << DDB6);

    //     SCK = F_CPU/16    Master     SPI-Enable
    // SPCR |= (1 << SPR0) | (1 << MSTR) | (1 << SPE);
	
	//     SCK = F_CPU/2    Master     SPI-Enable
    SPCR |= (1 << SPI2X) | (1 << MSTR) | (1 << SPE);


    enc28j60SystemResetCommand();
	
	// ENC28J60 errata, page 3, issue 2
	_delay_ms(1);
    // while(!(enc28j60ReadControlRegister(ESTAT) & (1 << ESTAT_CLKRDY)));


    // Setup Receive buffer
    enc28j60WriteControlRegister(ERXSTL, RECEIVE_BUFFER_START);
    enc28j60WriteControlRegister(ERXSTH, RECEIVE_BUFFER_START >> 8);
    enc28j60WriteControlRegister(ERXNDL, RECEIVE_BUFFER_END);
    enc28j60WriteControlRegister(ERXNDH, RECEIVE_BUFFER_END >> 8);

    // MAC Setup
    enc28j60BitFieldSet(MACON1, (1 << MACON1_MARXEN) | (1 << MACON1_TXPAUS) | (1 << MACON1_RXPAUS));
    enc28j60BitFieldClear(MACON2, 1 << MACON2_MARST);
    enc28j60BitFieldSet(MACON3, (1 << MACON3_FULDPX) | (1 << MACON3_FRMLNEN) | (1 << MACON3_TXCRCEN) | (1 << MACON3_PADCFG0));

    enc28j60WriteControlRegister(MAMXFLL, MAX_FRAME_LENGTH + 4);
    enc28j60WriteControlRegister(MAMXFLH, (MAX_FRAME_LENGTH + 4) >> 8);

    // See ENC28J60 data sheet, page 34
    enc28j60WriteControlRegister(MABBIPG, 0x15);
    enc28j60WriteControlRegister(MAIPGL, 0x12);


    enc28j60WriteControlRegister(MAADR1, MACAddr[0]);
    enc28j60WriteControlRegister(MAADR2, MACAddr[1]);
    enc28j60WriteControlRegister(MAADR3, MACAddr[2]);
    enc28j60WriteControlRegister(MAADR4, MACAddr[3]);
    enc28j60WriteControlRegister(MAADR5, MACAddr[4]);
    enc28j60WriteControlRegister(MAADR6, MACAddr[5]);

    // Set PHY in full duplex mode
    enc28j60WritePHYRegister(PHCON1, 1 << PHCON1_PDPXMD);

    enc28j60BitFieldSet(ECON1, 1 << ECON1_RXEN);
}

uint8_t enc28j60ReadControlRegister(uint8_t regAddr)
{
    uint8_t data;

    // Switch bank if necessary
    enc28j60SetBank(regAddr);

    // Set chip select
    PORTB &= ~(1 << PB4);

    // Write
    SPDR = OPCODE_RCR | (regAddr & ADDR_MASK);
    while(!(SPSR & (1 << SPIF)));

    // Read
    SPDR = 0x00;
    while(!(SPSR & (1 << SPIF)));

    // Reread if chip sends dummy byte first
    if(regAddr & DUMMY_TRANSFER)
    {
        SPDR = 0x00;
        while(!(SPSR & (1 << SPIF)));
    }

    data = SPDR;


    // Remove chip select
    PORTB |= (1 << PB4);

    return data;
}

void enc28j60ReadBufferMemory(uint8_t *data, uint16_t len)
{
    // Set chip select
    PORTB &= ~(1 << PB4);

    SPDR = OPCODE_RBM | 0x1A;
    while(!(SPSR & (1 << SPIF)));
    
    while(len--)
    {
        SPDR = 0x00;
        while(!(SPSR & (1 << SPIF)));
        *data++ = SPDR;
    }

    // Remove chip select
    PORTB |= (1 << PB4);
}

void enc28j60WriteControlRegister(uint8_t regAddr, uint8_t data)
{

    // Switch bank if necessary
    enc28j60SetBank(regAddr);

    // Set chip select
    PORTB &= ~(1 << PB4);

    SPDR = OPCODE_WCR | (regAddr & ADDR_MASK);
    while(!(SPSR & (1 << SPIF)));

    SPDR = data;

    while(!(SPSR & (1 << SPIF)));

    // Remove chip select
    PORTB |= (1 << PB4);
}

void enc28j60WriteBufferMemory(uint8_t *data, uint16_t len)
{
    // Set chip select
    PORTB &= ~(1 << PB4);

    SPDR = OPCODE_WBM | 0x1A;
    while(!(SPSR & (1 << SPIF)));
    
    while(len--)
    {
        SPDR = *data++;
        while(!(SPSR & (1 << SPIF)));
    }

    // Remove chip select
    PORTB |= (1 << PB4);
}

void enc28j60BitFieldSet(uint8_t regAddr, uint8_t data)
{
    // Switch bank if necessary
    enc28j60SetBank(regAddr);

    // Set chip select
    PORTB &= ~(1 << PB4);

    SPDR = OPCODE_BFS | (regAddr & ADDR_MASK);
    while(!(SPSR & (1 << SPIF)));

    SPDR = data;
    while(!(SPSR & (1 << SPIF)));

    // Remove chip select
    PORTB |= (1 << PB4);
}

void enc28j60BitFieldClear(uint8_t regAddr, uint8_t data)
{
    // Switch bank if necessary
    enc28j60SetBank(regAddr);

    // Set chip select
    PORTB &= ~(1 << PB4);

    SPDR = OPCODE_BFC | (regAddr & ADDR_MASK);
    while(!(SPSR & (1 << SPIF)));

    SPDR = data;
    while(!(SPSR & (1 << SPIF)));

    // Remove chip select
    PORTB |= (1 << PB4);
}

void enc28j60SystemResetCommand(void)
{
    // Set chip select
    PORTB &= ~(1 << PB4);

    SPDR = OPCODE_SRC | 0x1F;
    while(!(SPSR & (1 << SPIF)));

    // Remove chip select
    PORTB |= (1 << PB4);
}

void enc28j60SetBank(uint8_t regAddr)
{
    if(((regAddr & ADDR_MASK) < KEY_REGISTERS_START) && enc28j60CurrentBank != (regAddr & BANK_MASK))
    {
        enc28j60CurrentBank = (regAddr & BANK_MASK);

        enc28j60BitFieldClear(ECON1, (1 << ECON1_BSEL1) | (1 << ECON1_BSEL0));
        enc28j60BitFieldSet(ECON1, (enc28j60CurrentBank >> 5));
    }
}

uint16_t enc28j60ReadPHYRegister(uint8_t regAddr)
{
    enc28j60WriteControlRegister(MIREGADR, regAddr);
    enc28j60WriteControlRegister(MICMD, (1 << MICMD_MIIRD));
    
    while(enc28j60ReadControlRegister(MISTAT) & (1 << MISTAT_BUSY));

    enc28j60WriteControlRegister(MICMD, (0 << MICMD_MIIRD));

    return enc28j60ReadControlRegister(MIRDL) | (enc28j60ReadControlRegister(MIRDH) << 8);
}

void enc28j60WritePHYRegister(uint8_t regAddr, uint16_t data)
{
    enc28j60WriteControlRegister(MIREGADR, regAddr);
    enc28j60WriteControlRegister(MIWRL, data);
    enc28j60WriteControlRegister(MIWRH, data >> 8);

    while(enc28j60ReadControlRegister(MISTAT) & (1 << MISTAT_BUSY));
}

void enc28j60TransmitPacket(uint8_t *data, uint16_t length, uint8_t perPacketControlByte)
{
    if(enc28j60ReadControlRegister(ECON1) & (1 << ECON1_TXRTS))
        return;

    uint16_t adress = TRANSMIT_BUFFER_START;

    enc28j60WriteControlRegister(ETXSTL, adress);
    enc28j60WriteControlRegister(ETXSTH, adress >> 8);


    enc28j60WriteControlRegister(EWRPTL, adress);
    enc28j60WriteControlRegister(EWRPTH, adress >> 8);

    enc28j60WriteBufferMemory(&perPacketControlByte, 1);
    enc28j60WriteBufferMemory(data, length);

    adress += length;

    enc28j60WriteControlRegister(ETXNDL, adress);
    enc28j60WriteControlRegister(ETXNDH, adress >> 8);


    enc28j60BitFieldClear(EIR, 1 << EIR_TXIF);
    enc28j60BitFieldSet(EIE, 1 << EIE_TXIE);

    enc28j60BitFieldSet(ECON1, 1 << ECON1_TXRTS);
}

uint16_t enc28j60ReceivePacket(uint8_t *data, uint16_t maxlength)
{
    uint16_t length, temp;
    uint8_t header[6];

    if(!enc28j60ReadControlRegister(EPKTCNT))
        return 0;
    
    enc28j60WriteControlRegister(ERDPTL, enc28j60NextPacketAdress);
    enc28j60WriteControlRegister(ERDPTH, enc28j60NextPacketAdress >> 8);

    enc28j60ReadBufferMemory(header, 6);
    enc28j60NextPacketAdress = header[0] | (header[1] << 8);
    length = (header[2] | (header[3] << 8)) - 4;
    // status = header[4] | (header[5] << 8);

    length = length > maxlength ? maxlength : length;

    enc28j60ReadBufferMemory(data, length);

    // ENC28J60 errata, page 6, issue 14
	// The receive hardware may corrupt the circular receive buffer
	// when an even value is programmed into the ERXRDPTH:ERXRDPTL registers.
    temp = enc28j60ReadControlRegister(ERXSTL) | (enc28j60ReadControlRegister(ERXSTH) << 8);
    if(enc28j60NextPacketAdress == temp)
	{
		// ERXND = RECEIVE_BUFFER_END
        enc28j60WriteControlRegister(ERXRDPTL, RECEIVE_BUFFER_END);
        enc28j60WriteControlRegister(ERXRDPTH, RECEIVE_BUFFER_END >> 8);
	}
	else
	{
		temp = enc28j60NextPacketAdress - 1;
		enc28j60WriteControlRegister(ERXRDPTL, temp);
        enc28j60WriteControlRegister(ERXRDPTH, temp >> 8);
	}



/*
    enc28j60WriteControlRegister(ERXRDPTL, enc28j60NextPacketAdress & 0xFF);
    enc28j60WriteControlRegister(ERXRDPTH, (enc28j60NextPacketAdress >> 8) & 0xFF);
*/

    enc28j60BitFieldSet(ECON2, 1 << ECON2_PKTDEC);

    return length;
}
