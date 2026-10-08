# MicroPython course

Eleven modules, written in MicroPython, aimed at the Raspberry Pi Pico
(RP2040). It runs on a PC in a terminal through MicroPython's unix port.

## Running

```bash
sudo apt install micropython      # Ubuntu; provides the `micropython` command
cd micropython/src
micropython main.py               # or ./main.py
```

Linux only: the unix port is not shipped for Windows.

## What is real and what is simulated

The unix port has no GPIO, so `src/sim.py` supplies `machine.Pin`, `PWM`,
`ADC`, `disable_irq`/`enable_irq`, `time_pulse_us` and a virtual `time`
module, with the API of the Pico port. `main.py` installs them before the
lessons import `machine` and `time`, so the code in a lesson is the code a
Pico would run. Calls that play the outside world (`sim.drive`,
`sim.set_adc`, `sim.set_pulse`) are called out as PC-only. Time is virtual.

The unix port runs `@micropython.native` and `@micropython.viper` for real,
so module 9's timings are real (this PC's). It cannot run
`@micropython.asm_thumb` or PIO: module 10 assembles its listings with
`mpy-cross` for the Cortex-M0+ and says plainly that they were assembled, not
executed.

## How it was verified

```bash
python3 check-course.py       # every module runs to its SUMMARY
python3 check-solutions.py    # every challenge solution runs and prints what it promises
python3 check-asm.py          # every asm_thumb listing assembles for ARMv6-M
python3 ../tools/check-teaching-order.py
```

`check-asm.py` needs `mpy-cross` (`pip install mpy-cross`), a dev-time tool,
not a dependency of the course. Floats here are 64-bit and on most boards
32-bit; module 2 shows the difference rather than hiding it.
