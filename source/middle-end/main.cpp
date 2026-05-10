#include "../../headers/middle-end/main.h"

int main()
{
    char* buffer = CreateBuffer("code_file.txt").buff;
    buffer += 1;

    CompNode_t* root = ReadNode(buffer);

    root = CompOptimiz(root);
    CompDumpString(root, "middle_dump");

    FILE* file_print = fopen("code_file.txt", "w");
    CompPrintNodeMiddle(root, file_print);

    fclose(file_print);
    CompDtor(root);
    free(buffer - 1);
}
