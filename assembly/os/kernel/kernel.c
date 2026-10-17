/* kernel.c - a kernel written in C: screen, serial port, and nothing else. */

#include <stdint.h>

#define VGA_WIDTH  80
#define VGA_HEIGHT 25
#define COM1       0x3F8

/* Video memory is written through a volatile pointer: every store must reach
 * the hardware, in order, even if the program never reads it back. */
static volatile uint16_t *const vga = (volatile uint16_t *)0xB8000;

static inline void outb(uint16_t port, uint8_t value)
{
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

static inline uint8_t inb(uint16_t port)
{
    uint8_t value;
    __asm__ volatile ("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

static void serial_init(void)
{
    outb(COM1 + 1, 0x00);       /* no interrupts */
    outb(COM1 + 3, 0x80);       /* DLAB on */
    outb(COM1 + 0, 0x01);       /* divisor 1: 115200 baud */
    outb(COM1 + 1, 0x00);
    outb(COM1 + 3, 0x03);       /* 8 bits, no parity, one stop bit */
}

static void serial_putc(char c)
{
    while ((inb(COM1 + 5) & 0x20) == 0)
        ;                       /* wait for the transmitter */
    outb(COM1, (uint8_t)c);
}

static int row, col;

static void clear_screen(void)
{
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
        vga[i] = 0x0720;
    row = col = 0;
}

static void putc_screen(char c, uint8_t attr)
{
    if (c == '\n') {
        col = 0;
        row++;
        return;
    }
    vga[row * VGA_WIDTH + col] = (uint16_t)((uint16_t)attr << 8 | (uint8_t)c);
    if (++col == VGA_WIDTH) {
        col = 0;
        row++;
    }
}

static void print(const char *s, uint8_t attr)
{
    for (; *s; s++) {
        putc_screen(*s, attr);
        serial_putc(*s);
    }
}

static void print_hex(uint64_t value, uint8_t attr)
{
    static const char digits[] = "0123456789abcdef";
    char text[19] = "0x0000000000000000";

    for (int i = 0; i < 16; i++)
        text[17 - i] = digits[(value >> (4 * i)) & 0xF];
    print(text, attr);
}

/* volatile, or the compiler sees that nothing ever writes them and prints
 * the constants it knows instead of reading memory. */
static volatile int zeroed_counter;        /* .bss: zero only if entry.asm cleared it */
static volatile int initialised = 0x1234;  /* .data: its value is in the file */

void kmain(void)
{
    serial_init();
    clear_screen();

    print("Hello from a kernel written in C\n", 0x0F);
    print("kmain is at ", 0x07);
    print_hex((uint64_t)(uintptr_t)kmain, 0x0A);
    print("\n.bss counter = ", 0x07);
    print_hex((uint64_t)zeroed_counter, 0x0A);
    print("\n.data value  = ", 0x07);
    print_hex((uint64_t)initialised, 0x0A);
    print("\n", 0x07);

    for (;;)
        __asm__ volatile ("hlt");
}
