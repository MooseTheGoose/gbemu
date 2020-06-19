#ifndef GBCPU_H
#define GBCPU_H

#include <stdint.h>

/* 
 * Only works on little endian systems.
 * Either change this as necessary or introduce
 * a macro to adapt to big endian systems.
 */

struct GBMEM;

typedef struct GBCPU 
{
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

    uint32_t TICKS;
    struct GBMEM *MEMORY;
}
GBCPU;

/*
 *  Memory convenience structure
 */
typedef struct GBMEM
{
    struct GBCART 
    {
        uint8_t *ROM;
        uint8_t *EXRAM;

        uint8_t (*MBC_READ)(struct GBCART *mem, uint16_t addr);
        void (*MBC_WRITE)(struct GBCART *mem, uint16_t addr, 
                          uint8_t byte);
        void *MBC_DATA;
    }
    CARTRIDGE;

    uint8_t VRAM[2][0x2000];
    uint8_t WRAM[8][0x1000];
    uint8_t ECHO[0x1DFF];
    uint8_t OAM[0xA0];
    uint8_t NO_USE[0x60];
    uint8_t IO[0x80];
    uint8_t HRAM[0x7F];
    uint8_t IE;
}
GBMEM;

#define CC_C 0x10
#define CC_Z 0x80
#define CC_H 0x20
#define CC_N 0x40

#endif
