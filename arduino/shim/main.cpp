/*
 * main.cpp - the main() a sketch never shows.
 *
 * On a real board this lives in the Arduino core, not in your sketch: it
 * sets the hardware up, calls setup() once, then calls loop() until the power
 * is cut. This is the same shape, with one addition for a PC: if SIM_LOOPS
 * is set, loop() runs that many times and the program ends, so a sketch that
 * would run forever on a board can be tried and stopped. Not part of Arduino.
 */

#include <Arduino.h>

int main(void)
{
    const char *limit = getenv("SIM_LOOPS");
    long remaining = limit ? atol(limit) : -1;

    setup();
    while (remaining != 0) {
        loop();
        if (remaining > 0) {
            remaining--;
        }
    }
    sim_quit();
    return 0;
}
