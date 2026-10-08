// execute.cpp - runs the functions of snippets.cpp on a simulated ATmega328P.
//
// run-avr.py builds this for the Uno's chip, runs it in simavr, stops it at
// finished(), and reads these variables out of the chip's memory.

#include <avr/interrupt.h>

#include "snippets.cpp"

volatile uint8_t result_swap;
volatile uint8_t result_read_sreg_interrupts_on;
volatile uint8_t result_inside_critical;
volatile uint8_t result_after_critical;
volatile uint8_t result_inside_critical_when_off;
volatile uint8_t result_after_critical_when_off;
volatile uint16_t result_wrapped_16bit;
volatile uint16_t result_widened;
volatile uint8_t result_portb;

extern "C" __attribute__((noinline)) void finished(void)
{
    for (;;) {
    }
}

int main(void)
{
    result_swap = swap_asm(0xA5);

    // The 16-bit multiply of lesson 7, with the value hidden from the
    // optimiser so the chip really does the arithmetic.
    volatile uint16_t full = 1023;
    uint16_t product = full * 5000;
    result_wrapped_16bit = product / 1023;
    result_widened = (uint16_t) ((uint32_t) full * 5000 / 1023);

    // Lesson 12's critical section, reading SREG itself while inside it.
    sei();
    result_read_sreg_interrupts_on = read_sreg();
    result_inside_critical = critical(&SREG);
    result_after_critical = read_sreg();

    // The reason it saves and restores SREG instead of calling sei():
    // when interrupts were already off, they must still be off afterwards.
    cli();
    result_inside_critical_when_off = critical(&SREG);
    result_after_critical_when_off = read_sreg();

    set_led();  // lesson 12's one-instruction PORTB |= (1 << 5)
    DDRB |= (1 << 5);
    result_portb = PORTB;

    finished();
}
