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

void MakeBinPushImm8(char* buffer, char num)
{
    char push_val = 0x6A;

    memcpy(buffer, &push_val, 1);
    memcpy(buffer, &num, 1);
}

void MakeBinPopReg(char* buffer, char reg)
{
    char push_val = 0x58;
    
    push_val += reg;

    memcpy(buffer, &push_val, 1);
    buffer++;
}
#define MEM_CPY  \ 
    memcpy(buffer, &prefix, 1);                                  \        
    buffer++;                                                    \
    memcpy(buffer, &opcode, 1);                                  \
    buffer++;                                                    \
    memcpy(buffer, &modrm, 1);                                   \ 
    buffer++;                                                    \

#define CommandRegReg(name, code)                                \
                                                                 \
void MakeBin ## name ## RegReg(char* buffer, char dst, char src) \
{                                                                \    
    char prefix = 0x48;                                          \            
    char opcode = code;                                          \            
                                                                 \    
    char modrm = 0;                                              \           
    modrm |= dst;                                                \    
    modrm |= src << 3;                                           \    
    modrm |= (1 << 7) + (1 << 6);                                \    
                                                                 \   
    MEM_CPY                                                      \
}       

#define CommandRegImm8(name, code) \
void MakeBin ## name ## RegImm8 (char* buffer, char reg, char num) \
{                                                         \
    char prefix = 0x48;                                   \
    char opcode = 0x83;                                   \
                                                          \
    char modrm = 0;                                       \ 
    modrm |= (1 << 7) + (1 << 6);                         \
    modrm |= code;                                        \
    modrm |= reg;                                         \
                                                          \
    MEM_CPY                                               \
                                                          \
    memcpy(buffer, &num, 1);                              \
    buffer++;                                             \
}

CommandRegReg(Mov,  0x89)
CommandRegReg(Add,  0x01)
CommandRegReg(Sub,  0x29)
CommandRegReg(Cmp,  0x39)
CommandRegReg(Xor,  0x31)
CommandRegReg(iMul, 0xF7) // 101
CommandRegReg(Div,  0xF7) // 110
CommandRegReg(Or,   0x09)

CommandRegImm8(Add, 0)
CommandRegImm8(Sub, 5)
CommandRegImm8(Cmp, 7)

void MakeBinCqo(char* buffer)
{
    int code = 0x4899;

    memcpy(buffer, &code, 2);
    buffer += 2;
}

void MakeBinRet(char* buffer)
{
    char code = 0xC3;

    memcpy(buffer, &code, 1);
    buffer++;
}

void MakeBinMovRegMem(char* buffer, char reg, char reg_mem, char offs, char code)
{
     char prefix = 0x48;   
     char opcode = code /*0x8B*/;

     char modrm  = 0;
     modrm |= 1 << 6;
     modrm |= reg << 3;
     modrm |= reg_mem;

    MEM_CPY
    memcpy(buffer, &offs, 1);
    buffer++;
}

void MakeBinExit(char* buffer)
{
    long int mov_code = 0xB83C000000;
    int xor_rdx_rdx   = 0x4831D2;    
    int syscall_code  = 0x0F05;

    memcpy(buffer, &mov_code, 5);
    buffer += 5;
    memcpy(buffer, &xor_rdx_rdx, 3);
    buffer += 3;
    memcpy(buffer, &syscall_code, 2);
    buffer += 2;
}

void MakeBinMovRegImm32 (char* buffer, char reg, int num)
{   
    char prefix = 0x48;
    memcpy(buffer, &prefix, 1);
    buffer++;

    char opcode = 0xC7; 
    memcpy(buffer, &opcode, 1);
    buffer++;

    char modrm = 0;
    modrm |= 0x3 << 6;
    modrm |= reg;
    
    memcpy(buffer, &modrm, 1);
    buffer++;
    
    memcpy(buffer, &num, 4);
    buffer += 4;
}
// нужен mov в память  
void MakeBinIntSqrt(char* buffer)
{    
    long int instructions_bytes[2] = {0xF3480F2AC0F30F, 0x51C0F3480F2CC0}; 
    memcpy(buffer, instructions_bytes, 14); // вот это надо чекнуть, выглядит сомнительно 
}

//void MakeBinAddRegReg(char* buffer, char )
