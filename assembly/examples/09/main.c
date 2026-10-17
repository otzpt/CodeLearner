/* main.c - calls the NASM function. gcc main.c max.o -o demo */
#include <stdio.h>

int asm_max(int a, int b);

int main(void)
{
    printf("asm_max(3, 9)   = %d\n", asm_max(3, 9));
    printf("asm_max(-5, -2) = %d\n", asm_max(-5, -2));
    return 0;
}
