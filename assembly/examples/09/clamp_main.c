/* clamp_main.c - the C half of the challenge.
 *
 *   gcc clamp_main.c prog.o -o prog
 */
#include <stdio.h>

int asm_clamp(int x, int lo, int hi);

int main(void)
{
    printf("%d\n", asm_clamp(5, 0, 10));
    printf("%d\n", asm_clamp(-3, 0, 10));
    printf("%d\n", asm_clamp(99, 0, 10));
    return 0;
}
