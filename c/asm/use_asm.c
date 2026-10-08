/*
 * use_asm.c - calls the two NASM functions in add.asm.
 *
 *   nasm -f elf64 asm/add.asm -o asm/add.o
 *   cc -std=c11 -Wall -Wextra asm/use_asm.c asm/add.o -o asm/use_asm
 */

#include <stdint.h>
#include <stdio.h>

/* The compiler cannot see into the assembly, so these two declarations are
 * the whole contract: they must match what the assembly really does. */
uint64_t add_numbers(uint64_t a, uint64_t b);
uint64_t sum_bytes(const uint8_t *data, uint64_t count);

int main(void)
{
    const uint8_t bytes[] = {1, 2, 3, 4};

    printf("add_numbers(40, 2)    = %llu\n", (unsigned long long) add_numbers(40, 2));
    printf("sum_bytes([1,2,3,4]) = %llu\n",
           (unsigned long long) sum_bytes(bytes, sizeof bytes));
    return 0;
}
