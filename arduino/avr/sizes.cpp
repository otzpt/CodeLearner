/*
 * Every size and wrap claim the course makes about the Uno, as a compile-time
 * check. If one is wrong this fails to compile for the ATmega328P.
 *
 *   avr-gcc -mmcu=atmega328p -std=gnu++11 -c sizes.cpp
 */
#include <stdint.h>
static_assert(sizeof(int)==2, "int");
static_assert(sizeof(long)==4, "long");
static_assert(sizeof(float)==4, "float");
static_assert(sizeof(double)==4, "double");
static_assert(sizeof(int*)==2, "ptr");
static_assert(sizeof(char)==1, "char");
constexpr uint16_t full = 1023;
static_assert(full * 5000 / 1023 == 3, "16-bit wrap");
