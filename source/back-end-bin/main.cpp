#include "../../headers/back-end/main.h"
// пусть variables не заполняется для стекового фреймах
// полный ахтунг с обращением к параметру
// !!!!!!!!!!! Добавляю в nasm section and global 

int main()
{
    StackString_t variables = {};
    INITSTACK(variables, 8);
    
    StackFunc_t functions = {};
    INITStackFunc(functions, 8);

    StackInt_t init_var = {};
    INIT_INTSTACK(init_var, 8);

    char* buffer = CreateBuffer("code_file.txt").buff;
    // printf("buffer: %s\n", buffer + 1);

    CompNode_t* root = ReadNodeBack(buffer + 1, &variables, &functions);
    CompDump(root, "dump_back", variables);
    // StackFuncPrint(&functions);

    // StackPrint(&variables);

    MakeAsmCode(root, &variables, &functions);

    STRING_DTOR(&variables); 
    FUNC_DTOR(&functions);
    INTDTOR(init_var);
    CompDtor(root);
    free(buffer);
    fclose(file_asm);
}
