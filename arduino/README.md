# Arduino course

Thirteen modules, written as an Arduino sketch (`course/*.ino`), run on a PC
in a terminal. No board, no screen, no Arduino IDE needed.

## Running

```bash
cd arduino
make
./arduino-course
```

Needs `g++` and Linux (the stand-in for Serial input uses `poll()`).

Your own sketches, built against the same stand-in:

```bash
make try FILE=test.ino            # setup() once, then loop() 10 times
make try FILE=test.ino LOOPS=21   # a different number of passes
```

## What is real and what is simulated

The sketch is built the way the Arduino IDE builds one: the `.ino` tabs are
joined into one C++ file. The Arduino core is replaced by `shim/Arduino.h`,
which reimplements the part of the API the course uses with the behaviour of
an Uno (ATmega328P): the pin to port mapping, `PORTx`/`DDRx`/`PINx`,
`analogWrite` falling back to digital on a non-PWM pin, `Serial.print`'s
float formatting, and so on.

It is not the real core. Where the PC differs from the chip (an `int` is 4
bytes here and 2 on the Uno) the lessons say so. Everything named `sim_...`
plays the outside world (a button press, a voltage, time passing) and is not
Arduino. Time is virtual: `delay()` moves a clock instead of waiting.

Module 12 (inline assembly) cannot run AVR code on the PC itself. Its
listings are real `avr-gcc` output, and `avr/run-avr.py` executes them on a
simulated ATmega328P (simavr); see `avr/`.

## How it was verified

```bash
python3 check-course.py       # every module runs to its SUMMARY, feeding one line at a time
python3 check-solutions.py    # every challenge solution builds warning-free and prints what it promises
python3 avr/check-avr.py      # AVR sizes and the module 12 listings, recompiled with avr-gcc
python3 avr/run-avr.py        # the same code executed on a simulated ATmega328P (simavr)
python3 ../tools/check-teaching-order.py
```

`check-avr.py` needs `avr-gcc` (Ubuntu: `sudo apt install gcc-avr avr-libc`).
It proves the Uno sizes (`int` 2, `long` 4, `float` 4, `double` 4, pointer 2
bytes), the 16-bit wrap in module 7, and that `PORTB |= (1 << 5)` is one
`sbi`. `run-avr.py` (needs `simavr`) then runs that code on a simulated
ATmega328P and reads the results back: `swap_nibbles(0xA5)` gives `0x5A`,
the critical section clears the interrupt flag and puts it back, and the
16-bit multiply wraps to 3 where the 32-bit one gives 5000. A simulator is
not a board: timing against real hardware is not checked.
