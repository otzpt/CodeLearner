/*
 * The AVR code lesson 12 shows, compiled for real by check-avr.py:
 *
 *   avr-gcc -mmcu=atmega328p -Os -c snippets.cpp
 *   avr-objdump -d snippets.o
 */
#include <avr/io.h>
#include <stdint.h>

void set_led(void) { PORTB |= (1 << 5); }
void wait_16_cycles(void) {
    asm volatile("nop \n\t nop \n\t nop \n\t nop \n\t nop \n\t nop \n\t nop \n\t nop");
}
uint8_t swap_asm(uint8_t x) {
    asm("swap %0" : "+r"(x));
    return x;
}
uint8_t swap_c(uint8_t x) {
    return (uint8_t)((x << 4) | (x >> 4));
}
uint8_t read_sreg(void) {
    uint8_t s;
    asm volatile("in %0, __SREG__" : "=r"(s));
    return s;
}
uint8_t critical(volatile uint8_t *p) {
    uint8_t sreg, v;
    asm volatile("in %0, __SREG__ \n\t cli" : "=r"(sreg) :: "memory");
    v = *p;
    asm volatile("out __SREG__, %0" :: "r"(sreg) : "memory");
    return v;
}
