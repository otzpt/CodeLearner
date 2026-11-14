# Arduino course

For someone who has never programmed a board. The sketches run on a PC in a
terminal: no board, no Arduino IDE.

## Lessons

1. setup and loop
2. Variables and their sizes
3. Switching things on: digital output
4. Buttons: digital input
5. Sensors and dimming: analog pins
6. Doing two things at once: millis
7. Commands from the computer: Serial input
8. Project: a traffic light

## What is real and what is simulated

Each example is compiled with `g++` against `shim/Arduino.h`, a stand-in for
the Arduino core with the behaviour of an Uno (ATmega328P): the pin to port
mapping, `PORTx`/`DDRx`/`PINx`, `analogWrite` falling back to digital on a
non-PWM pin, `Serial.print`'s float formatting. `shim/main.cpp` calls `setup()`
once and `loop()` `SIM_LOOPS` times (3 in the lessons), since on a board it
never stops.

It is not the real core. Where the PC differs from the chip (an `int` is 4
bytes here and 2 on the Uno) the lessons say so. Everything named `sim_...`
plays the outside world (a button press, a voltage) and is not Arduino. Time
is simulated: `delay()` moves a clock instead of waiting.

Needs `g++`, and Linux: the stand-in reads Serial input with `poll()`.

## Running

From the launcher, or directly:

```bash
node arduino/learn.js
```

Your practice files and progress go in `~/codelearner/arduino/`.

## Checking

```bash
node learn/check.js arduino            # every example, question, solution and starter
python3 learn/check-course.py arduino  # every lesson played to the end
```

`avr/` holds real AVR code, separate from the lessons: `check-avr.py`
recompiles it with `avr-gcc` (Uno sizes, a 16-bit wrap, `PORTB |= (1 << 5)`
as one `sbi`) and `run-avr.py` executes it on a simulated ATmega328P
(`simavr`). A simulator is not a board: timing on real hardware is not
checked.
