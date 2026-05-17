#include "../../headers/back-end/traslate_nasm.h"

// ==================================================================================================
// писать в буфер байты команды
// надо соответсвие регистра и его кода
//
// ==================================================================================================

char rax = 0x0;
char rcx = 0x1; 
char rdx = 0x2;
char rbx = 0x3;
char rsp = 0x4;
char rbp = 0x5;
char rsi = 0x6;
char rdi = 0x7;

void MakeBinPushReg(char* buffer, char reg)
{
    char push_val = 0x50;
    
    push_val += reg;

    memcpy(buffer, &push_val, 1);
    buffer++;
}

void MakeBinPopReg(char* buffer, char reg)
{
    char push_val = 0x58;
    
    push_val += reg;

    memcpy(buffer, &push_val, 1);
    buffer++;
}

#define CommandRegReg(name, code)                                \
                                                                 \
void MakeBin ## name ## RegReg(char* buffer, char dst, char src) \
{                                                                \    
    char prefix = 0x48;                                          \            
    char opcode = code;                                          \            
                                                                 \    
    char modrm = 0;                                              \           
    modrm |= (1 << 7) + (1 << 6);                                \    
    modrm |= src << 3;                                           \    
    modrm |= dst;                                                \    
                                                                 \    
    memcpy(buffer, &prefix, 1);                                  \        
    buffer++;                                                    \
    memcpy(buffer, &opcode, 1);                                  \
    buffer++;                                                    \
    memcpy(buffer, &modrm, 1);                                   \ 
    buffer++;                                                    \
}       

CommandRegReg(Mov,  0x89)
CommandRegReg(Add,  0x01)
CommandRegReg(Sub,  0x29)
CommandRegReg(Cmp,  0x39)
CommandRegReg(Xor,  0x31)
CommandRegReg(iMul, 0xF7) // 101
CommandRegReg(Div,  0xF7) // 110

// нужен mov в память  

//void MakeBinAddRegReg(char* buffer, char )
