/* console.c - screen and serial output. The code of module 15's kernel.c, moved
 * here unchanged except that print_dec (its challenge) is now part of it. */

#include "console.h"

#define VGA_WIDTH  80
#define VGA_HEIGHT 25
#define COM1       0x3F8

static volatile uint16_t *const vga = (volatile uint16_t *)0xB8000;
static int row, col;

static void serial_putc(char c)
{
    while ((inb(COM1 + 5) & 0x20) == 0)
        ;                       /* wait for the transmitter */
    outb(COM1, (uint8_t)c);
}

void console_init(void)
{
    outb(COM1 + 1, 0x00);       /* no interrupts */
    outb(COM1 + 3, 0x80);       /* DLAB on */
    outb(COM1 + 0, 0x01);       /* divisor 1: 115200 baud */
    outb(COM1 + 1, 0x00);
    outb(COM1 + 3, 0x03);       /* 8 bits, no parity, one stop bit */

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

void print(const char *s, uint8_t attr)
{
    for (; *s; s++) {
        putc_screen(*s, attr);
        serial_putc(*s);
    }
}

void print_hex(uint64_t value, uint8_t attr)
{
    static const char digits[] = "0123456789abcdef";
    char text[19] = "0x0000000000000000";

    for (int i = 0; i < 16; i++)
        text[17 - i] = digits[(value >> (4 * i)) & 0xF];
    print(text, attr);
}

void print_dec(uint64_t value, uint8_t attr)
{
    char reversed[21];
    char text[21];
    int n = 0;

    do {
        reversed[n++] = (char)('0' + value % 10);
        value /= 10;
    } while (value != 0);

    for (int i = 0; i < n; i++)
        text[i] = reversed[n - 1 - i];
    text[n] = '\0';
    print(text, attr);
}
