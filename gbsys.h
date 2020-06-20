#ifndef GBSYS_H
#define GBSYS_H

#include <stdint.h>

typedef struct GBSYS
{
    /* Internal Memory */

    uint8_t VRAM[2][0x2000];
    uint8_t WRAM[8][0x1000];
    uint8_t ECHO[0x1DFF];
    uint8_t OAM[0xA0];
    uint8_t NO_USE[0x60];
    uint8_t IO[0x80];
    uint8_t HRAM[0x7F];
    uint8_t IE;

    /* Cartridge Memory & MBC */

    uint8_t *ROM;
    uint8_t *EXRAM;

    uint8_t (*MBC_READ)(struct GBSYS *sys, uint16_t addr);
    void (*MBC_WRITE)(struct GBSYS *sys, uint16_t addr, 
                      uint8_t code);
    void *MBC_DATA;

    /* Registers */
    union
    {
        uint16_t BC;
        struct
        {
            uint8_t C;
            uint8_t B;
        };
    };
    union
    {
        uint16_t DE;
        struct
        {
            uint8_t E;
            uint8_t D;
        };
    };
    union
    {
        uint16_t HL;
        struct
        {
            uint8_t L;
            uint8_t H;
        };
    };
    union
    {
        uint16_t AF;
        struct
        {
            uint8_t F;
            uint8_t A;
        };
    };

    uint16_t PC;
    uint16_t SP;

    /* Internal variables */
    uint32_t TICKS;
    uint8_t SYSFLAGS;
    uint8_t BUS_READ, BUS_WRITE;
}
GBSYS;

#define CC_C 0x10
#define CC_Z 0x80
#define CC_H 0x20
#define CC_N 0x40

#define SYS_HALT 0x1
#define SYS_IME  0x2

void GBSysUpdate(GBSYS *SYSTEM);
uint8_t GBSysReadByte(GBSYS *SYSTEM, uint16_t addr);
void GBSysWriteByte(GBSYS *SYSTEM, uint16_t addr, uint8_t code);

uint8_t NoMBCRead(GBSYS *SYSTEM, uint16_t addr);
void    NoMBCWrite(GBSYS *SYSTEM, uint16_t addr, uint8_t code);

#endif
