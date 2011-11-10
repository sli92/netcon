#include "compiler_defs.h"
#include "C8051F340_defs.h"

#include <stdint.h>

#include "cp2200.h"
#include "main.h"

/*
 * Registerdefinitionen, aus dem SiLabs CP2200 Beispiel.
 *
 */
volatile __pdata __at 0x2008 uint8_t RAMADDRH;      // RAM Address Pointer High Byte
volatile __pdata __at 0x2009 uint8_t RAMADDRL;      // RAM Address Pointer Low Byte
volatile __pdata __at 0x2002 uint8_t RAMRXDATA;     // RXFIFO RAM Data Register
volatile __pdata __at 0x2004 uint8_t RAMTXDATA;     // TXBUFF RAM Data Register
volatile __pdata __at 0x2069 uint8_t FLASHADDRH;    // Flash Address Pointer High Byte
volatile __pdata __at 0x2068 uint8_t FLASHADDRL;    // Flash Address Pointer Low Byte
volatile __pdata __at 0x2005 uint8_t FLASHAUTORD;   // Flash Autoread w/ increment
volatile __pdata __at 0x2006 uint8_t FLASHDATA;     // Flash Read/Write Data Register
volatile __pdata __at 0x2067 uint8_t FLASHKEY;      // Flash Lock and Key
volatile __pdata __at 0x206A uint8_t FLASHERASE;    // Flash Erase
volatile __pdata __at 0x207B uint8_t FLASHSTA;      // Flash Status
volatile __pdata __at 0x200A uint8_t MACADDR;       // MAC Address Pointer
volatile __pdata __at 0x200B uint8_t MACDATAH;      // MAC Data Register High Byte
volatile __pdata __at 0x200C uint8_t MACDATAL;      // MAC Data Register Low Byte
volatile __pdata __at 0x200D uint8_t MACRW;         // MAC Read/Write Initiate
volatile __pdata __at 0x2063 uint8_t INT0;          // Interrupt Status Register 0 (Self-Clearing)
volatile __pdata __at 0x2076 uint8_t INT0RD;        // Interrupt Status Register 0 (Read-Only)
volatile __pdata __at 0x2064 uint8_t INT0EN;        // Interrupt Enable Register 0
volatile __pdata __at 0x207B uint8_t FLASHSTA;      // Flash Status
volatile __pdata __at 0x200A uint8_t MACADDR;       // MAC Address Pointer
volatile __pdata __at 0x200B uint8_t MACDATAH;      // MAC Data Register High Byte
volatile __pdata __at 0x200C uint8_t MACDATAL;      // MAC Data Register Low Byte
volatile __pdata __at 0x200D uint8_t MACRW;         // MAC Read/Write Initiate
volatile __pdata __at 0x2063 uint8_t INT0;          // Interrupt Status Register 0 (Self-Clearing)
volatile __pdata __at 0x2076 uint8_t INT0RD;        // Interrupt Status Register 0 (Read-Only)
volatile __pdata __at 0x2064 uint8_t INT0EN;        // Interrupt Enable Register 0
volatile __pdata __at 0x207F uint8_t INT1;          // Interrupt Status Register 1 (Self-Clearing)
volatile __pdata __at 0x207E uint8_t INT1RD;        // Interrupt Status Register 1 (Read-Only)
volatile __pdata __at 0x207D uint8_t INT1EN;        // Interrupt Enable Register 1
volatile __pdata __at 0x2013 uint8_t VDMCN;         // VDD Monitor Control Register
volatile __pdata __at 0x2075 uint8_t SWRST;         // Software Reset Register
volatile __pdata __at 0x2073 uint8_t RSTSTA;        // Reset Source Status Register
volatile __pdata __at 0x2072 uint8_t RSTEN;         // Reset Enable Register
volatile __pdata __at 0x2070 uint8_t IOPWR;         // Port Input/Output Power
volatile __pdata __at 0x207C uint8_t OSCPWR;        // Oscillator Power
volatile __pdata __at 0x2010 uint8_t RXFILT;        // Receive Filter Configuraton
volatile __pdata __at 0x2011 uint8_t RXCN;          // Receive Control
volatile __pdata __at 0x2012 uint8_t RXSTA;         // Receive Status
volatile __pdata __at 0x2001 uint8_t RXAUTORD;      // Receive Autoread w/ increment
volatile __pdata __at 0x200E uint8_t RXHASHH;       // Receive Hash Table High Byte
volatile __pdata __at 0x200F uint8_t RXHASHL;       // Receive Hash Table Low Byte
volatile __pdata __at 0x201D uint8_t CPINFOH;       // Current RX Packet Information High Byte
volatile __pdata __at 0x201E uint8_t CPINFOL;       // Current RX Packet Information Low Byte
volatile __pdata __at 0x201F uint8_t CPLENH;        // Current RX Packet Length High Byte
volatile __pdata __at 0x2020 uint8_t CPLENL;        // Current RX Packet Length Low Byte
volatile __pdata __at 0x2021 uint8_t CPADDRH;       // Current RX Packet Address High Byte
volatile __pdata __at 0x2022 uint8_t CPADDRL;       // Current RX Packet Address Low Byte
volatile __pdata __at 0x201A uint8_t CPTLB;         // Current RX Packet TLB Number
volatile __pdata __at 0x201C uint8_t TLBVALID;      // TLB Valid Indicators
volatile __pdata __at 0x2023 uint8_t TLB0INFOH;     // TLB0 Information High Byte
volatile __pdata __at 0x2024 uint8_t TLB0INFOL;     // TLB0 Information Low Byte
volatile __pdata __at 0x2025 uint8_t TLB0LENH;      // TLB0 Length High Byte
volatile __pdata __at 0x2026 uint8_t TLB0LEN;       // TLB0 Length Low Byte
volatile __pdata __at 0x2027 uint8_t TLB0ADDRH;     // TLB0 Address High Byte
volatile __pdata __at 0x2028 uint8_t TLB0ADDRL;     // TLB0 Address Low Byte
volatile __pdata __at 0x2029 uint8_t TLB1INFOH;     // TLB1 Information High Byte
volatile __pdata __at 0x202A uint8_t TLB1INFOL;     // TLB1 Information Low Byte
volatile __pdata __at 0x202B uint8_t TLB1LENH;      // TLB1 Length High Byte
volatile __pdata __at 0x202C uint8_t TLB1LENL;      // TLB1 Length Low Byte
volatile __pdata __at 0x202D uint8_t TLB1ADDRH;     // TLB1 Address High Byte
volatile __pdata __at 0x202E uint8_t TLB1ADDRL;     // TLB1 Address Low Byte
volatile __pdata __at 0x202F uint8_t TLB2INFOH;     // TLB2 Information High Byte
volatile __pdata __at 0x2030 uint8_t TLB2INFOL;     // TLB2 Information Low Byte
volatile __pdata __at 0x2031 uint8_t TLB2LENH;      // TLB2 Length High Byte
volatile __pdata __at 0x2032 uint8_t TLB2LENL;      // TLB2 Length Low Byte
volatile __pdata __at 0x2033 uint8_t TLB2ADDRH;     // TLB2 Address High Byte
volatile __pdata __at 0x2034 uint8_t TLB2ADDRL;     // TLB2 Address Low Byte
volatile __pdata __at 0x2035 uint8_t TLB3INFOH;     // TLB3 Information High Byte
volatile __pdata __at 0x2036 uint8_t TLB3INFOL;     // TLB3 Information Low Byte
volatile __pdata __at 0x2037 uint8_t TLB3LENH;      // TLB3 Length High Byte
volatile __pdata __at 0x2038 uint8_t TLB3LENL;      // TLB3 Length Low Byte
volatile __pdata __at 0x2039 uint8_t TLB3ADDRH;     // TLB3 Address High Byte
volatile __pdata __at 0x203A uint8_t TLB3ADDRL;     // TLB3 Address Low Byte
volatile __pdata __at 0x203B uint8_t TLB4INFOH;     // TLB4 Information High Byte
volatile __pdata __at 0x203C uint8_t TLB4INFOL;     // TLB4 Information Low Byte
volatile __pdata __at 0x203D uint8_t TLB4LENH;      // TLB4 Length High Byte
volatile __pdata __at 0x203E uint8_t TLB4LENL;      // TLB4 Length Low Byte
volatile __pdata __at 0x203F uint8_t TLB4ADDRH;     // TLB4 Address High Byte
volatile __pdata __at 0x2040 uint8_t TLB4ADDRL;     // TLB4 Address Low Byte
volatile __pdata __at 0x2041 uint8_t TLB5INFOH;     // TLB5 Information High Byte
volatile __pdata __at 0x2042 uint8_t TLB5INFOL;     // TLB5 Information Low Byte
volatile __pdata __at 0x2043 uint8_t TLB5LENH;      // TLB5 Length High Byte
volatile __pdata __at 0x2044 uint8_t TLB5LENL;      // TLB5 Length Low Byte
volatile __pdata __at 0x2045 uint8_t TLB5ADDRH;     // TLB5 Address High Byte
volatile __pdata __at 0x2046 uint8_t TLB5ADDRL;     // TLB5 Address Low Byte
volatile __pdata __at 0x2047 uint8_t TLB6INFOH;     // TLB6 Information High Byte
volatile __pdata __at 0x2048 uint8_t TLB6INFOL;     // TLB6 Information Low Byte
volatile __pdata __at 0x2049 uint8_t TLB6LENH;      // TLB6 Length High Byte
volatile __pdata __at 0x204A uint8_t TLB6LENL;      // TLB6 Length Low Byte
volatile __pdata __at 0x204B uint8_t TLB6ADDRH;     // TLB6 Address High Byte
volatile __pdata __at 0x204C uint8_t TLB6ADDRL;     // TLB6 Address Low Byte
volatile __pdata __at 0x204D uint8_t TLB7INFOH;     // TLB7 Information High Byte
volatile __pdata __at 0x204E uint8_t TLB7INFOL;     // TLB7 Information Low Byte
volatile __pdata __at 0x204F uint8_t TLB7LENH;      // TLB7 Length High Byte
volatile __pdata __at 0x2050 uint8_t TLB7LENL;      // TLB7 Length Low Byte
volatile __pdata __at 0x2051 uint8_t TLB7ADDRH;     // TLB7 Address High Byte
volatile __pdata __at 0x2052 uint8_t TLB7ADDRL;     // TLB7 Address Low Byte
volatile __pdata __at 0x2017 uint8_t RXFIFOHEADH;   // Receive Buffer Head Pointer High Byte
volatile __pdata __at 0x2018 uint8_t RXFIFOHEADL;   // Receive Buffer Head Pointer Low Byte
volatile __pdata __at 0x2015 uint8_t RXFIFOTAILH;   // Receive Buffer Tail Pointer High Byte
volatile __pdata __at 0x2016 uint8_t RXFIFOTAILL;   // Receive Buffer Tail Pointer Low Byte
volatile __pdata __at 0x205B uint8_t RXFIFOSTA;     // Receive Buffer Status
volatile __pdata __at 0x2059 uint8_t TXSTARTH;      // Transmit Data Starting Address High Byte
volatile __pdata __at 0x205A uint8_t TXSTARTL;      // Transmit Data Starting Address Low Byte
volatile __pdata __at 0x2003 uint8_t TXAUTOWR;      // Transmit Data Autowrite
volatile __pdata __at 0x2057 uint8_t TXENDH;        // Transmit Data Ending Address High Byte
volatile __pdata __at 0x2058 uint8_t TXENDL;        // Transmit Data Ending Address Low Byte
volatile __pdata __at 0x2053 uint8_t TXCN;          // Transmit Control
volatile __pdata __at 0x2055 uint8_t TXPAUSEH;      // Transmit Pause High Byte
volatile __pdata __at 0x2056 uint8_t TXPAUSEL;      // Transmit Pause Low Byte
volatile __pdata __at 0x2054 uint8_t TXBUSY;        // Transmit Busy Indicator
volatile __pdata __at 0x205C uint8_t TXSTA6;        // Transmit Status Vector 6
volatile __pdata __at 0x205D uint8_t TXSTA5;        // Transmit Status Vector 5
volatile __pdata __at 0x205E uint8_t TXSTA4;        // Transmit Status Vector 4
volatile __pdata __at 0x205F uint8_t TXSTA3;        // Transmit Status Vector 3
volatile __pdata __at 0x2060 uint8_t TXSTA2;        // Transmit Status Vector 2
volatile __pdata __at 0x2061 uint8_t TXSTA1;        // Transmit Status Vector 1
volatile __pdata __at 0x2062 uint8_t TXSTA0;        // Transmit Status Vector 0
volatile __pdata __at 0x2078 uint8_t PHYCN;         // Physical Layer Control
volatile __pdata __at 0x2079 uint8_t PHYCF;         // Physical Layer Configuration
volatile __pdata __at 0x2080 uint8_t PHYSTA;        // Physical Layer Status
volatile __pdata __at 0x207A uint8_t TXPWR;         // Transmitter Power Register

uint8_t mac_addr[6] = {0, 0, 0, 0, 0, 0};

/*
 * Interruptroutine.
 *
 */
void external_interrupt(void) __interrupt(0)
{
    __critical {
        uint8_t valid_bits;
        uint8_t num_packets;
        uint8_t int_state;

        int_state = INT1;
        int_state = INT0;
        
        if(int_state & (1 << _RXINT)) {
            valid_bits = TLBVALID;

            for(num_packets = 0; valid_bits; num_packets++)
                valid_bits &= valid_bits - 1;

            if(num_packets >= 7)
                RXCN |= 1 << _RXINH;
        }
    }
}

void cp2200_init(void)
{
    uint8_t int_state;

    /* /RST auf Low und nach 20ms auf High. */
    P1 &= ~(1 << 0);
    delay_20ms();
    P1 |= 1 << 0;

    /* Warten bis /RST auf 1 ist. */
    while(!(P1 & (1 << 0)));

    /* Warten bis OSCINT(Oszillator bereit) und SELFINT(Selbstinitialisierung) gesetzt sind. */
    do {
        int_state = INT0RD;
    } while (((int_state & ((1 << _SELFINT) | (1 << _OSCINT))) != 
			 ((1 << _SELFINT) | (1 << _OSCINT))) ||
			 (int_state == 0xFF));

    if(RXHASHH != 0x04)
        return;

    /* Alle Interrupts deaktivieren. */
    INT0EN |= 1 << _ERXINT;
    INT1EN = 0x00;

    /* Stellt sicher, dass die Interruptregister geleert werden. */
    int_state = INT0;
    int_state = INT1;

    /* PHY-Initialisierung */
    PHYCN = 0x00;
    
    PHYCN |= (1 << _DPLXMD);

    TXPWR |= (1 << _PSAVED);

    PHYCF |= (1 << _SMSQ) | (1 << _JABBER) | (1 << _LINKINTG) | (1 << _AUTOPOL);

    PHYCN |= (1 << _PHYEN);

    delay_20ms();

    PHYCN |= (1 << _TXEN) | (1 << _RXEN);

    IOPWR |= (1 << _ACTEN) | (1 << _LINKEN);


    while(!(PHYCN & (1 << _LINKSTA)));


    /* MAC Initialisierung */
    /* CP2200 Datenblatt, Seite 78. */
    cp2200_write_mac_register(MACCF, 0x40B3);
    cp2200_write_mac_register(IPGT, 0x0015);
    cp2200_write_mac_register(IPGR, 0x0C12);

    cp2200_write_mac_register(MAXLEN, MAX_FRAME_LENGTH);

    cp2200_read_flash_sequence(0x1FFA, mac_addr, 6);

    cp2200_write_mac_register(MACAD0, ((mac_addr[5]) << 8) | mac_addr[4]);
    cp2200_write_mac_register(MACAD1, ((mac_addr[3]) << 8) | mac_addr[2]);
    cp2200_write_mac_register(MACAD2, ((mac_addr[1]) << 8) | mac_addr[0]);

    cp2200_write_mac_register(MACCN, 0x0001);

    
    IE |= (1 << _EX0);
}

void cp2200_transmit(const uint8_t *_data, uint16_t len)
{
    uint16_t ram_addr = 0;

    while(TXBUSY & (1 << _TXBUSY));

    TXSTARTH = 0x00;
    TXSTARTL = 0x00;

    RAMADDRH = 0x00;
    RAMADDRL = 0x00;

    while(ram_addr < len) {
        RAMTXDATA = _data[ram_addr];

        ram_addr++;
        RAMADDRH = (ram_addr >> 8) & 0xFF;
        RAMADDRL = ram_addr & 0xFF;
    }

    while(ram_addr < 64) {
        RAMTXDATA = 0;

        ram_addr++;
        RAMADDRH = (ram_addr >> 8) & 0xFF;
        RAMADDRL = ram_addr & 0xFF;
    }

    ram_addr--;
    TXENDH = (ram_addr >> 8) & 0xFF;
    TXENDL = ram_addr & 0xFF;

    TXSTARTH = 0x00;
    TXSTARTL = 0x00;

    TXCN |= (1 << _TXGO);
}

uint16_t cp2200_receive(uint8_t *_data, uint16_t max_len)
{
    uint16_t i = 0, len = 0;

    if(!(CPINFOH & (1 << _RXVALID))) {
        return 0;
    }

    if(!CPINFOL & (1 << _RXOK)) {
        RXCN |= (1 << _RXSKIP);
    } else {
        len = ((CPLENH << 8) | CPLENL);
        if(len > max_len) {
            while(i < max_len) {
                _data[i++] = RXAUTORD;
            }

            len = max_len;
            RXCN |= 1 << _RXSKIP;
        } else {
            while(i < len) {
                _data[i++] = RXAUTORD;
            }

            RXCN |= 1 << _RXCLRV;
        }
    }


    if(TLBVALID == 0x00)
        RXCN &= ~(1 << _RXINH);


    return len;
}


void cp2200_write_mac_register(uint8_t reg_addr, uint16_t value)
{
    MACADDR = reg_addr;

    MACDATAH = (value >> 8) & 0xFF;
    MACDATAL = value & 0xFF;

    MACRW = 0;
}

uint8_t cp2200_read_flash_byte(uint16_t addr)
{
    FLASHADDRH = (addr >> 8) & 0xFF;
    FLASHADDRL = addr & 0xFF;

    return FLASHDATA;
}

void cp2200_read_flash_sequence(uint16_t start_addr, uint8_t *dest, uint16_t len)
{
    uint16_t i = 0;

    FLASHADDRH = (start_addr >> 8) & 0xFF;
    FLASHADDRL = start_addr & 0xFF;
    
    while(i < len)
        dest[i++] = FLASHAUTORD;
}
