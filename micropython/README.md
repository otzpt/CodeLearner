# MicroPython course

For someone who has never programmed a board, aimed at the Raspberry Pi
Pico. It runs on a PC in a terminal: no board needed.

## Lessons

1. A board and its pins
2. Blinking: waiting and the clock
3. Buttons: reading a pin
4. Sensors and dimming
5. Doing two things at once
6. Reacting at once: interrupts
7. Project: a traffic light

## What is real and what is simulated

The examples run in ordinary Python with `sim.py` installed as the board's
`machine` module and a simulated `time` (`sim.install()`), with the API of the
Pico port: `Pin`, `PWM`, `ADC`, `Pin.irq`, `ticks_ms` and `ticks_diff`.
Calls that play the outside world (`sim.drive`, `sim.set_adc`) are marked
PC-only. Time is simulated: `sleep` moves a clock instead of waiting. No
`micropython` binary is needed; the parts of the language the lessons use
behave the same there.

On a board, copy your file to it as `main.py` (Thonny does this).

## Running

From the launcher, or directly:

```bash
node micropython/learn.js
```

Your practice files and progress go in `~/codelearner/micropython/`.

## Checking

```bash
node learn/check.js micropython            # every example, question, solution and starter
python3 learn/check-course.py micropython  # every lesson played to the end
```

