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

uint8_t GBSysReadReg(GBSYS *SYSTEM, int index)
{
    uint8_t reg;

    switch(index)
    {
        case 0:
        { reg = SYSTEM->B; break; }
        case 1:
        { reg = SYSTEM->C; break; }
        case 2:
        { reg = SYSTEM->D; break; }
        case 3:
        { reg = SYSTEM->E; break; }
        case 4:
        { reg = SYSTEM->H; break; }
        case 5:
        { reg = SYSTEM->L; break; }
        case 6:
        { reg = GBSysReadByte(SYSTEM, SYSTEM->HL); break; }
        case 7:
        { reg = SYSTEM->A; break; }
    }

    return reg;
}

void GBSysWriteReg(GBSYS *SYSTEM, int index, uint8_t byte)
{
    switch(index)
    {
        case 0:
        { SYSTEM->B = byte; break; }
        case 1:
        { SYSTEM->C = byte; break; }
        case 2:
        { SYSTEM->D = byte; break; }
        case 3:
        { SYSTEM->E = byte; break; }
        case 4:
        { SYSTEM->H = byte; break; }
        case 5:
        { SYSTEM->L = byte; break; }
        case 6:
        { GBSysWriteByte(SYSTEM, SYSTEM->HL, byte); break; }
        case 7:
        { SYSTEM->A = byte; break; }
    }
}

int GBSysReadPair(GBSYS *SYSTEM, int index)
{
    int contents = -1;

    switch(index)
    {
        case 0:
        { contents = SYSTEM->BC; break; }
        case 1:
        { contents = SYSTEM->DE; break; }
        case 2:
        { contents = SYSTEM->HL; break; }
        case 3:
        { contents = SYSTEM->SP; break; }
    }

    return contents;
}

void GBSysWritePair(GBSYS *SYSTEM, int index, uint16_t contents)
{
    switch(index)
    {
        case 0:
        { SYSTEM->BC = contents; break; }
        case 1:
        { SYSTEM->DE = contents; break; }
        case 2:
        { SYSTEM->HL = contents; break; }
        case 3:
        { SYSTEM->SP = contents; break; }
    }
}

void GBSysExecNextInst(GBSYS *SYSTEM)
{
    uint8_t code = GBSysReadByte(SYSTEM, SYSTEM->PC++);

    switch(code & 0xC0)
    {
        case 0x00:
        case 0x40:
        { 
            if(code == 0x76)
            { /* Halt */ }
            else
            {
              uint8_t byte = GBSysReadReg(SYSTEM, code & 0x7);
              GBSysWriteReg(SYSTEM, (code >> 3) & 0x7, byte);
            }
            break;
        }
        case 0x80:
        {
            /* TODO: Move arithmetic functions to separate
                     functions, since they repeat for
                     immediate operands. */

            switch((code >> 3) & 0x7)
            {
                case 0:
                {
                    uint8_t op1 = SYSTEM->A,
                            op2 = GBSysReadReg(SYSTEM, code & 0x7);

                    uint8_t result = op1 + op2;
                    uint8_t newflags = 0;

                    if(result < op1) 
                    { newflags |= CC_C; }
                    if((result & 0xF) < (op1 & 0xF)) 
                    { newflags |= CC_H; }
                    if(!result) 
                    { newflags |= CC_Z; }
                    
                    SYSTEM->F = newflags;                   
                    SYSTEM->A = result;                  
 
                    break;
                }
                case 1:
                { 
		    uint8_t op1 = SYSTEM->A,
			    op2 = GBSysReadReg(SYSTEM, code & 0x7);

		    uint8_t result = op1 + op2;
		    uint8_t newflags = 0;

                    if(SYSTEM->F & CC_C) { result++; }
                    
                    
                    if(result < op1) 
                    { newflags |= CC_C; }
                    if((result & 0xF) < (op1 & 0xF)) 
                    { newflags |= CC_H; }
                    if(!result) 
                    { newflags |= CC_Z; }
                    
                    SYSTEM->F = newflags;                   
                    SYSTEM->A = result;                  

                    break;
                }
                case 2:
                {
		    uint8_t op1 = SYSTEM->A,
			    op2 = GBSysReadReg(SYSTEM, code & 0x7);

		    uint8_t result = op1 - op2;
		    uint8_t newflags = CC_N;
                    
                    if(result > op1) 
                    { newflags |= CC_C; }
                    if((result & 0xF) > (op1 & 0xF)) 
                    { newflags |= CC_H; }
                    if(!result) 
                    { newflags |= CC_Z; }
                    
                    SYSTEM->F = newflags; 
                    SYSTEM->A = result;                  

                    break;
                }
                case 3:
                { 
		    uint8_t op1 = SYSTEM->A,
			    op2 = GBSysReadReg(SYSTEM, code & 0x7);

		    uint8_t result = op1 - op2;
		    uint8_t newflags = CC_N;

                    if(SYSTEM->F & CC_C) { result--; }
                    
                    if(result > op1) 
                    { newflags |= CC_C; }
                    if((result & 0xF) > (op1 & 0xF)) 
                    { newflags |= CC_H; }
                    if(!result) 
                    { newflags |= CC_Z; }

                    SYSTEM->F = newflags;
                    SYSTEM->A = result;                  
                    
                    break;
                }
                case 4:
                { 
		    uint8_t op1 = SYSTEM->A,
			    op2 = GBSysReadReg(SYSTEM, code & 0x7);

		    uint8_t result = op1 & op2;
		    uint8_t newflags = CC_H;

                    if(!result) 
                    { newflags |= CC_Z; }
 
                    SYSTEM->F = newflags;
                    SYSTEM->A = result;

                    break;
                }
                case 5:
                { 
		    uint8_t op1 = SYSTEM->A,
			    op2 = GBSysReadReg(SYSTEM, code & 0x7);

		    uint8_t result = op1 ^ op2;
		    uint8_t newflags = 0;

                    if(!result) 
                    { newflags |= CC_Z; }

                    SYSTEM->F = newflags;
                    SYSTEM->A = result;

                    break;
                }
                case 6:
                { 
		    uint8_t op1 = SYSTEM->A,
			    op2 = GBSysReadReg(SYSTEM, code & 0x7);

		    uint8_t result = op1 | op2;
		    uint8_t newflags = 0;

                    if(!result) 
                    { newflags |= CC_Z; }

                    SYSTEM->F = newflags;
                    SYSTEM->A = result;

                    break;
                }
                case 7:
                { 
		    uint8_t op1 = SYSTEM->A,
			    op2 = GBSysReadReg(SYSTEM, code & 0x7);

		    uint8_t result = op1 - op2;
		    uint8_t newflags = CC_N;
                    
                    if(result > op1) 
                    { newflags |= CC_C; }
                    if((result & 0xF) > (op1 & 0xF)) 
                    { newflags |= CC_H; }
                    if(!result) 
                    { newflags |= CC_Z; }

                    SYSTEM->F = newflags;

                    break;
                }
            }
        }
        case 0xC0:
            break;
    }
}

void GBSysUpdate(GBSYS *SYSTEM)
{
    GBSysExecNextInst(SYSTEM);
}
