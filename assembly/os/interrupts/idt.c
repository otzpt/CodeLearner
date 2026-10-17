/* idt.c - the interrupt table, the interrupt controller, a timer and a keyboard. */

#include "console.h"

/* An entry in the interrupt descriptor table: 16 bytes in long mode. */
struct idt_entry {
    uint16_t offset_low;        /* handler address, bits 0-15 */
    uint16_t selector;          /* code segment to run it in */
    uint8_t  ist;               /* alternative stack: 0 for none */
    uint8_t  type_attr;         /* 0x8E: present, ring 0, interrupt gate */
    uint16_t offset_mid;        /* bits 16-31 */
    uint32_t offset_high;       /* bits 32-63 */
    uint32_t reserved;
} __attribute__((packed));

struct idt_pointer {            /* what lidt reads, like lgdt's */
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

_Static_assert(sizeof(struct idt_entry) == 16, "an IDT gate is 16 bytes");
_Static_assert(sizeof(struct idt_pointer) == 10, "lidt wants 10 bytes");

/* What isr.asm builds on the stack, lowest address first. */
struct frame {
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8, rbp, rdi, rsi, rdx, rcx, rbx, rax;
    uint64_t vector, error;
    uint64_t rip, cs, rflags, rsp, ss;
};

#define CODE64 0x18             /* the 64-bit code selector from stage2.asm */
#define VECTORS 48

extern void *isr_table[VECTORS];
static struct idt_entry idt[VECTORS];

volatile uint64_t ticks;        /* the timer interrupt counts here */

static void set_gate(int vector, void *handler)
{
    uint64_t address = (uint64_t)handler;

    idt[vector].offset_low = (uint16_t)address;
    idt[vector].selector = CODE64;
    idt[vector].ist = 0;
    idt[vector].type_attr = 0x8E;
    idt[vector].offset_mid = (uint16_t)(address >> 16);
    idt[vector].offset_high = (uint32_t)(address >> 32);
    idt[vector].reserved = 0;
}

/* The two 8259 interrupt controllers deliver IRQ 0-7 to vectors 8-15 unless
 * told otherwise, which is on top of the CPU's own exceptions (8 is the
 * double fault). Move them to 32 and up. */
static void pic_remap(void)
{
    outb(0x20, 0x11);           /* ICW1: start the sequence, expect ICW4 */
    outb(0xA0, 0x11);
    outb(0x21, 32);             /* ICW2: the master's vectors begin at 32 */
    outb(0xA1, 40);             /* ...the slave's at 40 */
    outb(0x21, 4);              /* ICW3: the slave hangs off IRQ 2 */
    outb(0xA1, 2);
    outb(0x21, 1);              /* ICW4: 8086 mode */
    outb(0xA1, 1);
    outb(0x21, 0xFC);           /* mask: allow IRQ 0 (timer) and IRQ 1 (keyboard) only */
    outb(0xA1, 0xFF);
}

static void pit_init(uint32_t hz)
{
    uint32_t divisor = 1193182 / hz;    /* the timer chip counts at 1.193182 MHz */

    outb(0x43, 0x36);           /* channel 0, low byte then high, square wave */
    outb(0x40, (uint8_t)divisor);
    outb(0x40, (uint8_t)(divisor >> 8));
}

void interrupts_init(void)
{
    struct idt_pointer pointer = { sizeof idt - 1, (uint64_t)idt };

    for (int i = 0; i < VECTORS; i++)
        set_gate(i, isr_table[i]);
    __asm__ volatile ("lidt %0" : : "m"(pointer));

    pic_remap();
    pit_init(100);
}

/* Called by isr.asm for every vector. */
void interrupt_handler(struct frame *f)
{
    if (f->vector < 32) {
        print("exception ", 0x0C);
        print_dec(f->vector, 0x0C);
        print(" at ", 0x0C);
        print_hex(f->rip, 0x0C);
        print("\n", 0x0C);
        if (f->vector == 0) {
            f->rip += 2;        /* skip the two-byte div that faulted */
            return;
        }
        for (;;)
            __asm__ volatile ("cli; hlt");
    }

    if (f->vector == 32) {
        ticks++;
    } else if (f->vector == 33) {
        uint8_t scancode = inb(0x60);   /* reading it also lets the next key through */
        if (scancode < 0x80) {          /* keys going down; 0x80 and up are releases */
            print("key: ", 0x0A);
            print_hex(scancode, 0x0A);
            print("\n", 0x0A);
        }
    }
    outb(0x20, 0x20);           /* end of interrupt: without this, no more arrive */
}
