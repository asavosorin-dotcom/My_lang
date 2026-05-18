#ifndef _TRANSL_NASM_
#define _TRANSL_NASM_

#include <stdio.h>
#include <cstring>
#include <sys/types.h>

void MakeBinPushReg(char* buffer, char reg);
void MakeBinPushImm8(char* buffer, char num);

void MakeBinMovRegReg(char* buffer, char dst, char src);
void MakeBinPopReg(char* buffer, char reg);

void MakeBinAddRegReg(char* buffer, char dst, char src);
void MakeBinSubRegReg(char* buffer, char dst, char src);
void MakeBiniMulRegReg(char* buffer, char dst, char src);
void MakeBinDivRegReg(char* buffer, char dst, char src);
void MakeBinXorRegReg(char* buffer, char dst, char src);

void MakeBinAddRegImm8  (char* buffer, char reg, char num);
void MakeBinSubRegImm8  (char* buffer, char reg, char num);
void MakeBinMovRegImm32 (char* buffer, char reg, int num);

void MakeBinCqo(char* buffer);
void MakeBinRet(char* buffer);

void MakeBinMovRegMem(char* buffer, char reg, char reg_mem, char offs, char code);
void MakeBinExit(char* buffer);
void MakeBinIntSqrt(char* buffer);

extern char rax;
extern char rcx;
extern char rdx;
extern char rbx;
extern char rsp;
extern char rbp;
extern char rsi;
extern char rdi;

#define push_reg(reg)           MakeBinPushReg(buffer, reg)
#define push_imm8(num)          MakeBinPushImm8(buffer, num)

#define pop_reg(reg)            MakeBinPopReg(buffer, reg) 
#define mov_reg_reg(dst, src)   MakeBinMovRegReg(buffer, dst, src)
#define add_reg_reg(dst, src)   MakeBinAddRegReg(buffer, dst, src)
#define sub_reg_reg(dst, src)   MakeBinSubRegReg(buffer, dst, src)
#define imul_reg(dst)           MakeBiniMulRegReg(buffer, dst, 0x101)
#define idiv_reg(dst)           MakeBiniDivRegReg(buffer, dst, 0x111)
#define xor_reg_reg(dst, src)   MakeBinXorRegReg(buffer, dst, src)
#define or_reg_reg(dst, src)    MakeBinOrRegReg(buffer,  dst, src)
#define cmp_reg_reg(reg1, reg2) MakeBinCmpRegReg(buffer, reg1, reg2)

#define add_reg_imm8(reg, num)  MakeBinAddRegImm8(buffer, reg, num)
#define sub_reg_imm8(reg, num)  MakeBinSubRegImm8(buffer, reg, num)
#define cmp_reg_imm8(reg, num)  MakeBinCmpRegImm8(buffer, reg, num)

#define mov_reg_mem(reg, reg_mem, offset) MakeBinMovRegMem(buffer, reg, reg_mem, offset, 0x8B)
#define mov_mem_reg(mem_reg, offset, reg) MakeBinMovRegMem(buffer, reg, reg_mem, offset, 0x89)

#define mov_reg_imm32(reg, num) MakeBinMovRegImm32 (buffer, reg, num);
#define _exit MakeBinExit(buffer)

#define sqrt_rax MakeBinIntSqrt(buffer)

#define cqo                     MakeBinCqo(buffer)
#define ret                     MakeBinRet(buffer)

#endif
