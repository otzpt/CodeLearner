/* console.h - the screen and serial port of module 15's kernel.c, as a library. */

#ifndef CONSOLE_H
#define CONSOLE_H

#include <stdint.h>

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

void console_init(void);                        /* serial port + a clear screen */
void print(const char *s, uint8_t attr);        /* to the screen and the serial port */
void print_hex(uint64_t value, uint8_t attr);   /* 0x followed by 16 hex digits */
void print_dec(uint64_t value, uint8_t attr);

#endif
