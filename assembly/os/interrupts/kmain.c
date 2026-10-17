/* kmain.c - turn interrupts on, break something on purpose, and wait. */

#include "console.h"

void interrupts_init(void);
extern volatile uint64_t ticks;

static void divide_by_zero(void)
{
    __asm__ volatile ("xor %%ecx, %%ecx\n\t"
                      "xor %%edx, %%edx\n\t"
                      "mov $1, %%eax\n\t"
                      "div %%ecx"           /* 1 / 0: the CPU raises exception 0 */
                      : : : "eax", "ecx", "edx");
}

void kmain(void)
{
    console_init();
    print("Interrupts: IDT, PIC and PIT set up\n", 0x0F);
    interrupts_init();
    __asm__ volatile ("sti");               /* only now can an interrupt arrive */

    divide_by_zero();
    print("returned from the handler\n", 0x0F);

    while (ticks < 100)
        __asm__ volatile ("hlt");           /* sleep until the next interrupt */
    print("timer: 100 ticks\n", 0x0F);

    for (;;)
        __asm__ volatile ("hlt");           /* keys are handled in the background */
}
