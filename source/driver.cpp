#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* GetName(const char* buff);

int main(int argc, char* argv[])
{
    const char* file_name = argv[1];

    char* buffer = (char*) calloc(500, sizeof(char));
    char* name   = GetName(file_name);
    int len = 0;
    int count = 0;

    sprintf(buffer, "./compile %s\n%n", file_name, &count);
    len += count;

    sprintf(buffer + len, "./middle\n%n", &count);
    len += count;

    sprintf(buffer + len, "./back\n%n", &count);
    len += count;

    sprintf(buffer + len, "nasm -f elf64 ./compile_files/file_asm.s\n%n", &count);
    len += count;

    sprintf(buffer + len, "ld ./compile_files/print.o ./compile_files/file_asm.o -o %s\n", name);

    system(buffer);
    free(name);
    free(buffer);
}

char* GetName(const char* buff)
{
    int index = 0;
    while (buff[index] != '.') index++;
    return strndup(buff, index);
}
