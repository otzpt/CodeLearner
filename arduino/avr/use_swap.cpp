#include <avr/io.h>
#include <stdint.h>

extern "C" uint8_t swap_nibbles(uint8_t value);

int main(void)
{
    PORTB = swap_nibbles(0xA5);
    for (;;) {
    }
}
