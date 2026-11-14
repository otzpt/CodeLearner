"""sim.py - a stand-in for the parts of `machine` and `time` the course uses.

This is NOT the real MicroPython board support. The unix port that runs this
course has no machine.Pin, machine.PWM or machine.ADC, so this file supplies
them, with the API and the behaviour of the Raspberry Pi Pico port (rp2):
Pin modes and pulls, PWM with duty_u16, ADC with read_u16 and CORE_TEMP,
Pin.irq. install() puts them in sys.modules, so a lesson's own
`from machine import Pin` and `import time` get these.

Everything that does not exist on a real board -- pressing a button, putting
a voltage on a pin, letting time pass -- is a function here (drive, set_adc,
advance) and is called out as PC-only wherever a lesson uses it.

Time is virtual: sleep() moves a clock forward instead of waiting, so
"wait one second" costs nothing. The lessons say so where it matters.
"""

import sys
import time as _real_time  # captured before install() swaps in the virtual one

# time.ticks_*: the wrap-around period is a power of two that differs by
# port (MicroPython's documentation). 2**30 is the one the documentation uses
# to describe the behaviour.
TICKS_PERIOD = 1 << 30
_HALF = TICKS_PERIOD >> 1

_now_ms = 0
_levels = {}  # what the outside world drives onto a pin: pin id -> 0 or 1
_adc_values = {}  # channel (0-4) -> value 0..65535
_pins = {}  # pin id -> the Pin object created for it
_pulses = {}  # pin id -> pulse width in microseconds for time_pulse_us
_irq_enabled = True
_pending = []  # pins whose irq fired while interrupts were disabled


class Pin:
    IN = 0
    OUT = 1
    OPEN_DRAIN = 2
    PULL_UP = 3
    PULL_DOWN = 4
    IRQ_FALLING = 8
    IRQ_RISING = 4

    def __init__(self, pin_id, mode=-1, pull=-1, value=None):
        self.id = pin_id
        self._mode = Pin.IN
        self._pull = None
        self._out = 0
        self._handler = None
        self._trigger = 0
        _pins[pin_id] = self
        self.init(mode, pull, value)

    def init(self, mode=-1, pull=-1, value=None):
        if mode != -1:
            self._mode = mode
        if pull != -1:
            self._pull = pull
        if value is not None:
            self._out = 1 if value else 0

    def value(self, x=None):
        if x is not None:
            self._out = 1 if x else 0
            return None
        return self._level()

    def on(self):
        self.value(1)

    def off(self):
        self.value(0)

    def toggle(self):
        self._out = 0 if self._out else 1

    def irq(self, handler=None, trigger=IRQ_FALLING | IRQ_RISING):
        self._handler = handler
        self._trigger = trigger

    def _level(self):
        if self._mode == Pin.OUT:
            return self._out
        if self.id in _levels:
            return _levels[self.id]
        # Nothing connected: a pull-up reads high, anything else low. A real
        # floating input would read either; low keeps this repeatable.
        return 1 if self._pull == Pin.PULL_UP else 0


def drive(pin_id, level):
    """PC only. Put a level on a pin from outside, as a button would. Fires
    the pin's irq handler if this is an edge it asked for."""
    pin = _pins.get(pin_id)
    before = pin._level() if pin else 0
    _levels[pin_id] = 1 if level else 0
    after = pin._level() if pin else _levels[pin_id]
    if pin is None or pin._handler is None or before == after:
        return
    rising = after == 1
    wanted = Pin.IRQ_RISING if rising else Pin.IRQ_FALLING
    if not pin._trigger & wanted:
        return
    if _irq_enabled:
        pin._handler(pin)
    elif pin not in _pending:
        # One waiting event per pin: a second edge finds it already raised.
        _pending.append(pin)


class PWM:
    def __init__(self, pin, freq=None, duty_u16=None):
        self._pin = pin
        self._freq = 0
        self._duty = 0
        pin._mode = Pin.OUT
        if freq is not None:
            self.freq(freq)
        if duty_u16 is not None:
            self.duty_u16(duty_u16)

    def freq(self, hz=None):
        if hz is None:
            return self._freq
        self._freq = hz

    def duty_u16(self, value=None):
        if value is None:
            return self._duty
        self._duty = max(0, min(65535, value))

    def deinit(self):
        self._duty = 0


class ADC:
    CORE_TEMP = 4

    def __init__(self, source):
        # A Pin object (GP26-GP28 are ADC0-ADC2) or a channel number.
        if isinstance(source, Pin):
            self._channel = source.id - 26
        else:
            self._channel = source

    def read_u16(self):
        return _adc_values.get(self._channel, 0)


def set_adc(channel, value):
    """PC only. Put a reading on an ADC channel (0-65535)."""
    _adc_values[channel] = max(0, min(65535, value))


# ---- time -------------------------------------------------------------------


class _Time:
    """Stands in for the `time` module: same names, virtual clock."""

    def sleep(self, seconds):
        global _now_ms
        _now_ms += int(seconds * 1000)

    def sleep_ms(self, ms):
        global _now_ms
        _now_ms += ms

    def sleep_us(self, us):
        global _now_ms
        _now_ms += us // 1000

    def ticks_ms(self):
        return _now_ms % TICKS_PERIOD

    def ticks_us(self):
        return (_now_ms * 1000) % TICKS_PERIOD

    def ticks_add(self, ticks, delta):
        return (ticks + delta) % TICKS_PERIOD

    def ticks_diff(self, ticks1, ticks2):
        return ((ticks1 - ticks2 + _HALF) % TICKS_PERIOD) - _HALF


def set_time(ms):
    """PC only. Jump the clock, e.g. to just before the ticks wrap."""
    global _now_ms
    _now_ms = ms


def reset():
    """PC only. Forget every pin, reading and the clock: a fresh board."""
    global _now_ms, _irq_enabled
    _now_ms = 0
    _irq_enabled = True
    _levels.clear()
    _adc_values.clear()
    _pins.clear()
    _pulses.clear()
    del _pending[:]


def set_pulse(pin_id, microseconds):
    """PC only. The width of the pulse time_pulse_us will measure on a pin;
    None means no pulse at all."""
    if microseconds is None:
        _pulses.pop(pin_id, None)
    else:
        _pulses[pin_id] = microseconds


def real_ticks_us():
    """PC only. The real clock, for timing code, not the virtual one."""
    return _real_time.ticks_us()


def _disable_irq():
    global _irq_enabled
    previous = _irq_enabled
    _irq_enabled = False
    return previous


def _enable_irq(state=True):
    global _irq_enabled
    _irq_enabled = bool(state)
    if _irq_enabled:
        while _pending:
            pin = _pending.pop(0)
            if pin._handler is not None:
                pin._handler(pin)


def _time_pulse_us(pin, level, timeout_us=1000000):
    # The real one returns -2 if the pulse never starts and -1 if it never
    # ends; a missing echo is the -2 case.
    return _pulses.get(pin.id, -2)


class _Machine:
    Pin = Pin
    PWM = PWM
    ADC = ADC
    disable_irq = staticmethod(_disable_irq)
    enable_irq = staticmethod(_enable_irq)
    time_pulse_us = staticmethod(_time_pulse_us)


def install():
    sys.modules["machine"] = _Machine()
    sys.modules["time"] = _Time()
