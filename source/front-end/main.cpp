#include "../../headers/main.h"

int main(int argc, char* argv[])
{
    StackString_t variables = {};
    INITSTACK(variables, 8);

    StackString_t functions = {};
    INITSTACK(functions, 8);

    StackTok_t tokens = {};
    TOKINITSTACK(tokens, 16);

    const char* file_name = argv[1];
    char* buffer = CreateBuffer(file_name).buff;

    // const char* s = "if (x - 3) {x = 3;} c = 5;";
    size_t count_lex = GetLex(buffer + 1, &tokens, &variables, &functions);

    // for (int i = 0; i < tokens.size - 1; i++)
    // {
    //     // PRINT_HTM("%d\n", i);
    //     CompDump(tokens.data[i], "token", variables);
    // }

    CompNode_t* root = GetGeneral(&tokens, &variables);

    // CompDump(root, "first dump", variables);

    FILE* file_print = fopen("code_file.txt", "w");
    CompPrintNode(root, file_print, &variables, &functions);

    fclose(file_print);
    STRING_DTOR(&variables);
    STRING_DTOR(&functions);
    TOKDTOR(tokens);
    fclose(file_htm);
    free(buffer);
}
