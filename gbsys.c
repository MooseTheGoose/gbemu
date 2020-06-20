#include "gbsys.h"

uint8_t NoMBCRead(GBSYS *SYSTEM, uint16_t addr)
{
    uint8_t code;

    if(addr >= 0 && addr <= 0x8000)
    { code = SYSTEM->ROM[addr]; }
    else
    { code = SYSTEM->EXRAM[addr-0xA000]; }

    return code;
}

void NoMBCWrite(GBSYS *SYSTEM, uint16_t addr, uint8_t code)
{
    if(addr >= 0xA000 && addr <= 0xBFFF)
    { SYSTEM->EXRAM[addr-0xA000] = code; }
}

uint8_t GBSysReadByte(GBSYS *SYSTEM, uint16_t addr)
{
    uint8_t code;

    if((addr >= 0xA000 && addr <= 0xBFFF)
       || (addr >= 0 && addr <= 0x8000))
    { code = SYSTEM->MBC_READ(SYSTEM, addr); }
    else if(addr >= 0x8000 && addr <= 0x9FFF)
    { code = SYSTEM->VRAM[0][addr - 0x8000]; }
    else if(addr >= 0xC000 && addr <= 0xCFFF)
    { code = SYSTEM->WRAM[0][addr - 0xC000]; }
    else if(addr >= 0xD000 && addr <= 0xDFFF)
    { code = SYSTEM->WRAM[1][addr - 0xD000]; }
    else if(addr >= 0xE000 && addr <= 0xFDFF)
    { code = SYSTEM->ECHO[addr - 0xE000]; }
    else if(addr >= 0xFE00 && addr <= 0xFE9F)
    { code = SYSTEM->OAM[addr - 0xFE00]; }
    else if(addr >= 0xFEA0 && addr <= 0xFEFF)
    { code = SYSTEM->NO_USE[addr - 0xFEA0]; }
    else if(addr >= 0xFF00 && addr <= 0xFF7F)
    { code = SYSTEM->IO[addr-0xFF00]; }
    else if(addr >= 0xFF80 && addr <= 0xFFFE)
    { code = SYSTEM->IO[addr-0xFF80]; }
    else
    { code = SYSTEM->IE; }

    SYSTEM->BUS_READ = code;

    return code;
}

void GBSysWriteByte(GBSYS *SYSTEM, uint16_t addr, uint8_t code)
{
    if((addr >= 0xA000 && addr <= 0xBFFF)
       || (addr >= 0 && addr <= 0x8000))
    { SYSTEM->MBC_WRITE(SYSTEM, addr, code); }
    else if(addr >= 0x8000 && addr <= 0x9FFF)
    { SYSTEM->VRAM[0][addr - 0x8000] = code; }
    else if(addr >= 0xC000 && addr <= 0xCFFF)
    { SYSTEM->WRAM[0][addr - 0xC000] = code; }
    else if(addr >= 0xD000 && addr <= 0xDFFF)
    { SYSTEM->WRAM[1][addr - 0xD000] = code; }
    else if(addr >= 0xE000 && addr <= 0xFDFF)
    { SYSTEM->ECHO[addr - 0xE000] = code; }
    else if(addr >= 0xFE00 && addr <= 0xFE9F)
    { SYSTEM->OAM[addr - 0xFE00] = code; }
    else if(addr >= 0xFEA0 && addr <= 0xFEFF)
    { SYSTEM->NO_USE[addr - 0xFEA0] = code; }
    else if(addr >= 0xFF00 && addr <= 0xFF7F)
    { SYSTEM->IO[addr-0xFF00] = code; }
    else if(addr >= 0xFF80 && addr <= 0xFFFE)
    { SYSTEM->IO[addr-0xFF80] = code; }
    else
    { SYSTEM->IE = code; }

    SYSTEM->BUS_WRITE = code;
}

void GBSysExecNextInst(GBSYS *SYSTEM)
{
    uint8_t code = SYSTEM->MBC_READ(SYSTEM, SYSTEM->PC++);

    switch(code & 0xC0)
    {
        case 0x00:
        case 0x40:
        case 0x80:
        case 0xC0:
            break;
    }
}

void GBSysUpdate(GBSYS *SYSTEM)
{
    GBSysExecNextInst(SYSTEM);
}
