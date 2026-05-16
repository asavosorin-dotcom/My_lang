#ifndef _BACK_END_
#define _BACK_END_

#include <cwchar>
#include <stdio.h>

#include "../code_tree.h"
#include "../stack_int.h"

void MakeAsmNode(CompNode_t* node, StackString_t* variables, StackFunc_t* functions, Function_t* func);
void MakeAsmCode(CompNode_t* root, StackString_t* variables, StackFunc_t* functions);

void ParsingParams(CompNode_t* node, StackString_t* variables, Function_t* func);
void PushFuncArgs (CompNode_t* node, StackString_t* variables, Function_t* func);

void MakeAsmOper      (CompNode_t* node, StackString_t* variables, StackFunc_t* functions, Function_t* func);
void MakeAsmCondition (CompNode_t* node, StackString_t* variables, StackFunc_t* functions, Function_t* func);
void MakeAsmIf        (CompNode_t* node, StackString_t* variables, StackFunc_t* functions, Function_t* func);
void MakeAsmWhile     (CompNode_t* node, StackString_t* variables, StackFunc_t* functions, Function_t* func);
int GetCountVariables (CompNode_t* node, int count_now);

void PrintStdLib(void);

#endif
