#ifndef _TRANSL_NASM_
#define _TRANSL_NASM_

#include <stdio.h>
#include <cstring>
#include <sys/types.h>

void MakeBinPushReg(char* buffer, char reg);
void MakeBinMovRegReg(char* buffer, char dst, char src);
void MakeBinPopReg(char* buffer, char reg);

void MakeBinAddRegReg(char* buffer, char dst, char src);
void MakeBinSubRegReg(char* buffer, char dst, char src);
void MakeBiniMulRegReg(char* buffer, char dst, char src);
void MakeBinDivRegReg(char* buffer, char dst, char src);
void MakeBinXorRegReg(char* buffer, char dst, char src);

extern char rax;
extern char rcx;
extern char rdx;
extern char rbx;
extern char rsp;
extern char rbp;
extern char rsi;
extern char rdi;

#define push_reg(reg)          MakeBinPushReg(buffer, reg)
#define pop_reg(reg)           MakeBinPopReg(buffer, reg)
#define mov_reg_reg(dst, src)  MakeBinMovRegReg(buffer, dst, src)
#define add_reg_reg(dst, src)  MakeBinMovRegReg(buffer, dst, src)
#define sub_reg_reg(dst, src)  MakeBinMovRegReg(buffer, dst, src)
#define imul_reg(dst)          MakeBinMovRegReg(buffer, dst, 0x101)
#define div_reg(dst)           MakeBinMovRegReg(buffer, dst, 0x110)
#define xor_reg_reg(dst, src)  MakeBinMovRegReg(buffer, dst, src)

#endif
