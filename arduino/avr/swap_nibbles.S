; swap_nibbles.S - the same function as lesson 12's, written as a whole
; assembly file instead of inline. The AVR C calling convention (avr-gcc)
; passes the first 8-bit argument in r24 and returns an 8-bit result in r24.
#include <avr/io.h>

.global swap_nibbles
swap_nibbles:
    swap r24            ; exchange the high and low four bits
    ret
