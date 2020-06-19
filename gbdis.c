#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define ARRLEN(arr) (sizeof(arr) / sizeof(*(arr)))

const char *rnames[8] = 
{ "B", "C", "D", "E", "H", "L", "(HL)", "A" };

const char *pnames[4] = 
{ "BC", "DE", "HL", "SP" };

void DisExtInst(char *buffer, unsigned char code)
{
    if(code & 0xC0)
    {
        static const char *bit_nmemonics[3] = 
        { "BIT", "RES", "SET" };

        sprintf(buffer
                "%s %d, %s", 
                bit_nmemonics[code - 0x40 >> 6 & 3],
                code >> 3 & 7,
                rnames[code & 7]);
    }
    else
    {
        static const char *rot_nmemonics[8] = 
        { "RLC", "RRC", "RL", "RR",
          "SLA", "SRA", "SWAP", "SRL" };

        sprintf(buffer,
                "%s %s", 
                 rot_nmemonics[code >> 3 & 7],
                 rnames[code & 7]); 
    }
}

void DisNormInst(char *buffer, unsigned char *mem,
                 uint16_t *memptr)
{
    unsigned char code = mem[(*memptr)++];

    switch(code & 0xC0)
    {
        case 0x0:
        {
        }
        case 0x40:
        {
            if(code == 0x76)
            { strcpy(buffer, "HALT"); }
            else
            { 
                sprintf(buffer,
                        "LD %s, %s", 
                        rnames[code >> 3 & 7],
                        rnames[code & 7]);
            }
        }
        case 0x80:
        {
            static const char *alu_nmemonics[8] = 
            { "ADD", "ADC", "SUB", "SBC",
              "AND", "XOR", "OR", "CP" }

           unsigned char alu_code = code >> 3 & 7;
           if(alu_code == 0 || alu_code == 1 
              || alu_code == 3)
           {
               sprintf(buffer,
                       "%s A, %s",
                       alu_nmemonics[alu_code],
                       rnames[code & 7]);
           }
           else
           {
               sprintf(buffer,
                       "%s %s",
                       alu_nmemonics[alu_code],
                       rnames[code & 7]);
           }
        }
        case 0xC0:
        {
            if(code == 0xCB) 
            { DisExtInst(buffer, mem[(*memptr)++]); }
            else if((code & 0xC7) == 0xC7)
            { sprintf(buffer, "RST %02XH", code & 0x38); } 
        }
    }
}
