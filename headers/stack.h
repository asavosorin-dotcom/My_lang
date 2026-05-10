#ifndef _STACK_H_
#define _STACK_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// #include "compile_struct.h"
#include "Logfile.h"
#include "colors.h"

// #define DEBUG
// #define HASH
// #define CANARY

#ifdef DEBUG
    #define ONDEBUG(func) func

    #define ERRPRINTOK(str) err = STACKVERIFY(stk);     \
                                                        \
    if (!err)                                           \
        fprintf(fileerr, str);                          \
    else                                                \
        return err;                                     \
    

#else 
    #define ONDEBUG(func) 
    #define ERRPRINTOK(str) 
#endif

#ifdef CANARY
    #define ONDEBUGCANARY(func) func
#else
    #define ONDEBUGCANARY(func) 
#endif

#ifdef HASH
    #define ONDEBUGHASH(func) func
#else
    #define ONDEBUGHASH(func)
#endif

typedef char* StackElement_t;
#define TYPEELEM "%s"

enum StackErr_t {STACK_OK           = 0, 
                 ERR_SIZE           = 1, 
                 ERR_ELEM           = 2, 
                 ERR_STACK_ADRESS   = 4, 
                 ERR_DATA_ADRESS    = 8, 
                 ERR_KANAREYKALEFT  = 16, 
                 ERR_KANAREYKARIGHT = 32, 
                 ERR_CAPASITY       = 64, 
                 ERR_CANARY_STRUCT  = 128,
                 ERR_HASH           = 256,
                 STACK_EMPTY        = 512};

#ifdef DEBUG
typedef struct {
    int line;
    const char* filename;
    const char* funcname;
} Passport_t;
#endif

struct StackString_t{
    ONDEBUG(StackElement_t canary_left); // условная компилляция
    ONDEBUG(size_t NumOfStack);
    ONDEBUGHASH(size_t hash);

    StackElement_t* data;
    size_t size;
    int capacity; 

    ONDEBUG(Passport_t passport);
    ONDEBUG(StackElement_t canary_right);
    
};

int StackCtor(StackString_t* stk, int capacity, int line, const char* stackname, const char* funcname);
int StackPush(StackString_t* stk, StackElement_t elem, int line);
int StackPop(StackString_t* stk, StackElement_t* elem, int line);
int StackDtor(StackString_t* stk, int line);
int StackDump(StackString_t* stk, int err);
int StackTop(StackString_t* stk, StackElement_t* elem, int line);
int StackVerify(StackString_t* stk, int line, const char* funcname);
int StackRealloc(StackString_t* stk, int line);
void StackPrint(StackString_t* stk);
void ReallocArrPointData(void);


size_t StackHash(StackString_t* stk);
 
#ifdef DEBUG
#define STACKVERIFY(stk) StackVerify(stk, line, __func__);
#endif

#define INITSTACK(stk, capacity) StackCtor(&stk, capacity, __LINE__, #stk, __func__);

#define STRING_PUSH(stk, elem) StackPush(stk, elem, __LINE__);
#define STRING_POP(stk, elem)  StackPop(stk, &elem, __LINE__);
#define STRING_TOP(stk, elem)  StackTop(stk, &elem, __LINE__);
#define STRING_REALLOC(stk)    StackRealloc(&stk, __LINE__);
#define STRING_DTOR(stk)       StackDtor(stk, __LINE__);
#define STRING_PRINTSTACK(stk) StackPrint(&stk, __LINE__);
#define STRING_PRINTELEM(elem) printf(TYPEELEM"\n", elem);

// const StackElement_t POISON    = 0xDEDDEAD;
// const StackElement_t KANAREYKA = 0xDEADCA;

#endif
