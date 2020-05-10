#ifndef GBCPU_H
#define GBCPU_H

#include <stdint.h>

/* 
 * Only works on little endian systems.
 * Either change this as necessary or introduce
 * a macro to adapt to big endian systems.
 */

typedef struct RFile
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
}
RFile;

typedef union Opcode
{
    uint8_t code;
    struct
    {
        uint8_t main : 2;
        uint8_t f1   : 3;
        uint8_t f2   : 3;
    };
}
Opcode;

#define CC_C 0x10
#define CC_Z 0x80
#define CC_H 0x20
#define CC_N 0x40

#endif