"""Modules 5 to 8 - pins, PWM and ADC, time, and interrupts.

The hardware here is simulated by sim.py: this PC has no GPIO. Every call
that plays the part of the outside world -- sim.drive, sim.set_adc -- is
called out as PC-only where a lesson uses it. The machine and time code the
lessons show is the code a Raspberry Pi Pico would run.
"""

import micropython
import machine
from machine import Pin, PWM, ADC

import time

import sim
import ui


def lesson_05_pins():
    ui.title("MODULE 5 - PINS: machine.Pin")
    sim.reset()

    ui.heading("PART 1: an output pin")

    print("  A GPIO pin is a wire the chip can drive high or low, or read.")
    print("  On a Pico they are called GP0, GP1, ... and the logic level is")
    print("  3.3 V (Raspberry Pi Pico datasheet), where an Arduino Uno uses")
    print("  5 V. Check a sensor's voltage before wiring it.")
    print()
    print("    from machine import Pin")
    print("    led = Pin(25, Pin.OUT)      # GP25: the on-board LED on a Pico")
    print()
    led = Pin(25, Pin.OUT)
    print("    led.value(1)")
    led.value(1)
    print("  Running:  led.value() = " + str(led.value()))
    print("    led.off()")
    led.off()
    print("  Running:  led.value() = " + str(led.value()))
    print("    led.toggle()")
    led.toggle()
    print("  Running:  led.value() = " + str(led.value()))
    print()
    print("  On a Pico W the LED is wired to the wireless chip, not to GP25,")
    print('  and is Pin("LED") instead (MicroPython documentation).')

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 2: an input, and a button that reads backwards")

    print("  A button wired from the pin to ground, with the pin's internal")
    print("  pull-up resistor switched on:")
    print()
    print("    button = Pin(14, Pin.IN, Pin.PULL_UP)")
    button = Pin(14, Pin.IN, Pin.PULL_UP)
    print("    button.value()                   # nobody pressing")
    print("  Running:  " + str(button.value()))
    sim.drive(14, 0)
    print("    (the button is pressed: it connects the pin to ground)")
    print("  Running:  " + str(button.value()))
    sim.drive(14, 1)
    print()
    print("  Released is 1 and PRESSED is 0: inverted, exactly as on the")
    print("  Arduino. It is the usual wiring because it needs no extra")
    print("  resistor, and the usual cause of 'my button does the opposite'.")
    print()
    print("  (sim.drive is not MicroPython. It plays the part of the finger;")
    print("  on a board the world does that itself.)")
    print()
    print("  Without PULL_UP or PULL_DOWN an input with nothing connected")
    print("  floats, and reads whatever noise is nearby. Here it reads 0 so")
    print("  the lesson is repeatable.")

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 3: contact bounce")

    print("  A real button does not go cleanly from 1 to 0. For a few")
    print("  milliseconds the contacts chatter. Reading it in a loop sees")
    print("  several 'presses' for one push. Simulated here: one push whose")
    print("  contacts chatter for 4 ms, read every millisecond.")
    print()

    pin = Pin(13, Pin.IN, Pin.PULL_UP)
    sim.drive(13, 1)
    chatter = [0, 1, 0, 1, 0, 0, 0, 0, 0, 0]

    naive_presses = 0
    clean_presses = 0
    last_level = 1
    last_accepted = time.ticks_ms()
    time.sleep_ms(100)

    for level in chatter:
        sim.drive(13, level)
        time.sleep_ms(1)
        now_level = pin.value()
        if last_level == 1 and now_level == 0:
            naive_presses += 1
            if time.ticks_diff(time.ticks_ms(), last_accepted) >= 20:
                clean_presses += 1
                last_accepted = time.ticks_ms()
        last_level = now_level

    print("    count every 1 -> 0 edge          : " + str(naive_presses) + " presses")
    print("    ignore edges within 20 ms of one : " + str(clean_presses) + " press")
    print()
    print("  The fix is to remember when the last accepted edge was and")
    print("  ignore any new one that follows too soon. 20 ms is a common")
    print("  choice; the right number depends on the button.")

    ui.wait_enter()
    ui.clear_screen()
    ui.exercise(5)

    ui.question(
        "What voltage is a Pico's GPIO logic level? (number, volts)",
        "3.3",
        "Raspberry Pi Pico GPIO is 3.3 V, unlike an Uno's 5 V.",
    )
    ui.question(
        "A button from the pin to ground, with PULL_UP. What does value()\n"
        "  return while it is PRESSED?",
        "0",
        "The pull-up makes released 1; pressing grounds the pin.",
    )

    ui.challenge(
        [
            "Make GP15 an output. Toggle it three times, printing its value after",
            "each toggle. It starts at 0. On this PC, save it next to sim.py; the",
            "first two lines of the example start the simulator.",
        ],
        [],
        ["1", "0", "1"],
        [
            "import sim  # PC only: not needed on a Pico",
            "sim.install()",
            "from machine import Pin",
            "",
            "led = Pin(15, Pin.OUT)",
            "for _ in range(3):",
            "    led.toggle()",
            "    print(led.value())",
        ],
    )

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("SUMMARY")

    print("   - Pin(n, Pin.OUT) drives; Pin(n, Pin.IN, Pin.PULL_UP) reads")
    print("   - Pico logic is 3.3 V; GP25 is the LED on a Pico, Pin('LED') on a Pico W")
    print("   - a pulled-up button reads 0 when pressed")
    print("   - real buttons bounce: ignore edges that follow too soon")
    print()
    print("  Module 6: analog values, and PWM.")
    ui.wait_enter()


def scale(value, in_low, in_high, out_low, out_high):
    return (value - in_low) * (out_high - out_low) // (in_high - in_low) + out_low


def lesson_06_pwm_adc():
    ui.title("MODULE 6 - ADC AND PWM")
    sim.reset()

    ui.heading("PART 1: reading a voltage")

    print("  The Pico's ADC measures a voltage on GP26, GP27 or GP28 (the")
    print("  chip also has GP29 and an internal channel):")
    print()
    print("    from machine import ADC, Pin")
    print("    sensor = ADC(Pin(26))")
    print("    sensor.read_u16()        # 0 to 65535, for 0 V to 3.3 V")
    print()
    sim.set_adc(0, 32768)
    sensor = ADC(Pin(26))
    raw = sensor.read_u16()
    print("    sim.set_adc(0, 32768)    # PC only: put a voltage on ADC0")
    print("  Running:  read_u16() = " + str(raw))
    print("  Running:  volts      = " + "{:.2f}".format(raw / 65535 * 3.3))
    print()
    print("  The hardware converter has 12 bits (4096 steps); read_u16()")
    print("  scales that up to 16 bits so code is the same across boards.")
    print("  So the lowest 4 bits carry no extra information.")
    print()
    print("  The chip also measures its own temperature, on a channel with no")
    print("  pin. From the RP2040 datasheet, with v the voltage on it:")
    print()
    print("    temperature = 27 - (v - 0.706) / 0.001721")
    print()
    sim.set_adc(ADC.CORE_TEMP, 14020)
    core = ADC(ADC.CORE_TEMP)
    voltage = core.read_u16() / 65535 * 3.3
    celsius = 27 - (voltage - 0.706) / 0.001721
    print("    sim.set_adc(ADC.CORE_TEMP, 14020)")
    print("  Running:  v = " + "{:.3f}".format(voltage) + " V, temperature = " + "{:.1f}".format(celsius) + " C")

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 2: PWM")

    print("  PWM switches a pin on and off fast and varies how long it stays")
    print("  on: the duty cycle. A motor or an LED averages it out.")
    print()
    print("    from machine import PWM, Pin")
    print("    pwm = PWM(Pin(15))")
    print("    pwm.freq(1000)           # 1000 on/off cycles per second")
    print("    pwm.duty_u16(16384)      # 0 = always off, 65535 = always on")
    pwm = PWM(Pin(15))
    pwm.freq(1000)
    pwm.duty_u16(16384)
    print("  Running:  duty = " + "{:.1f}".format(pwm.duty_u16() / 65535 * 100) + " %")
    print()
    print("  Duty is 16 bits here and 8 bits (0-255) on the Arduino. The")
    print("  same idea at a different scale.")
    print()
    print("  A hobby servo wants a pulse of 1 to 2 ms every 20 ms, so 50 Hz:")
    print("  the pulse width as a fraction of the period is the duty.")
    pulse_ms = 1.5
    period_ms = 20
    duty = int(pulse_ms / period_ms * 65535)
    print()
    print("    duty = int(1.5 / 20 * 65535)      # the middle position")
    print("  Running:  " + str(duty))

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 3: rescaling, and the // trap")

    print("  MicroPython has no map() like the Arduino; you write one:")
    print()
    print("    def scale(value, in_low, in_high, out_low, out_high):")
    print("        return (value - in_low) * (out_high - out_low) // (in_high - in_low) + out_low")
    print()
    for value in (0, 32768, 65535):
        print("  Running:  scale(" + str(value) + ", 0, 65535, 0, 100) = " + str(scale(value, 0, 65535, 0, 100)))
    print()
    print("  It uses // so the result is a whole number. With / it would be")
    print("  a float: 50.0 instead of 50, and a PWM duty needs an integer.")
    print("  And like the Arduino's map(), nothing clamps the result: a")
    print("  reading past the input range gives an output past the output")
    print("  range. Clamp it:")
    print()
    print("    max(0, min(100, scale(70000, 0, 65535, 0, 100)))")
    print("  Running:  " + str(max(0, min(100, scale(70000, 0, 65535, 0, 100)))))

    ui.wait_enter()
    ui.clear_screen()
    ui.exercise(6)

    ui.question(
        "read_u16() returns a number from 0 up to what?",
        "65535",
        "16 bits, scaled up from the 12-bit converter.",
    )
    ui.question(
        "A servo needs a pulse every 20 ms. What frequency is that, in Hz?",
        "50",
        "1 / 0.020 s = 50 cycles per second.",
    )

    ui.challenge(
        [
            "Call sim.set_adc(0, 49151) to pretend ADC0 reads 49151 (a PC-only",
            "call). Read it with ADC(Pin(26)) and print the voltage with two",
            "decimals, taking 65535 as 3.3 V. Save it next to sim.py in this folder;",
            "the first two lines of the example start the simulator.",
        ],
        [],
        ["2.47"],
        [
            "import sim  # PC only: not needed on a Pico",
            "sim.install()",
            "from machine import ADC, Pin",
            "",
            "sim.set_adc(0, 49151)",
            "sensor = ADC(Pin(26))",
            'print("{:.2f}".format(sensor.read_u16() / 65535 * 3.3))',
        ],
    )

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("SUMMARY")

    print("   - ADC(Pin(26)).read_u16() is 0-65535 for 0-3.3 V")
    print("   - PWM(Pin(n)): freq() and duty_u16(0-65535)")
    print("   - write your own map: use // and clamp the result")
    print()
    print("  Module 7: waiting without freezing the board.")
    ui.wait_enter()


def checks_with_sleep(run_ms):
    start = time.ticks_ms()
    checks = 0
    while time.ticks_diff(time.ticks_ms(), start) < run_ms:
        checks += 1
        time.sleep_ms(1)  # the work of checking a button
        time.sleep_ms(500)  # the blocking wait
    return checks


def checks_with_ticks(run_ms):
    start = time.ticks_ms()
    last_blink = start
    checks = 0
    while time.ticks_diff(time.ticks_ms(), start) < run_ms:
        checks += 1
        time.sleep_ms(1)  # the same work of checking a button
        if time.ticks_diff(time.ticks_ms(), last_blink) >= 500:
            last_blink = time.ticks_add(last_blink, 500)
    return checks


def lesson_07_time():
    ui.title("MODULE 7 - TIME WITHOUT FREEZING")
    sim.reset()

    ui.heading("PART 1: what time.sleep costs")

    print("  A robot has to blink a light every 500 ms AND notice a button.")
    print("  time.sleep_ms(500) leaves the chip doing nothing for half a")
    print("  second. Counting how many times each version gets to check the")
    print("  button in 3 seconds (simulated time; each check costs 1 ms):")
    print()
    sim.reset()
    with_sleep = checks_with_sleep(3000)
    sim.reset()
    with_ticks = checks_with_ticks(3000)
    print("  Running:  with sleep_ms(500)   " + str(with_sleep) + " checks")
    print("            with ticks_ms()      " + str(with_ticks) + " checks")
    print()
    print("  Both blink at the same rate. A button press shorter than 500 ms")
    print("  can fall completely inside a sleep and never be seen.")
    print()
    print("  Honest note: on this PC the clock is simulated, so the sleeps")
    print("  cost nothing. On a board they take real time.")

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 2: the ticks_ms pattern")

    print("  time.ticks_ms() is a millisecond counter. Instead of waiting,")
    print("  ask whether enough time has passed. Always compare with")
    print("  ticks_diff, never with < or >:")
    print()
    print("    if time.ticks_diff(time.ticks_ms(), last) >= 500:")
    print("        last = time.ticks_add(last, 500)")
    print("        ...do the periodic thing...")
    print()
    sim.reset()
    last = time.ticks_ms()
    print("  Running, 1 ms per pass, toggles at:", end="")
    while time.ticks_ms() <= 2000:
        if time.ticks_diff(time.ticks_ms(), last) >= 500:
            last = time.ticks_add(last, 500)
            print(" " + str(time.ticks_ms()), end="")
        time.sleep_ms(1)
    print()
    print()
    print("  ticks_add(last, 500) keeps the rhythm exact, for the same")
    print("  reason as on the Arduino: `last = now` would add the time each")
    print("  pass takes to every period.")

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 3: when the counter wraps")

    print("  ticks_ms() does not count forever. It wraps back to near 0 after")
    print("  a period that is a power of two and differs from port to port")
    print("  (MicroPython documentation). The simulation here uses 2**30:")
    print()
    print("    2 ** 30 ms in days  =  " + "{:.2f}".format(2**30 / 1000 / 3600 / 24))
    print()
    print("  Never hard-code the period; use ticks_diff and ticks_add, which")
    print("  know it. Two tests for 'have 10 ms passed?', right at the wrap:")
    print()
    sim.reset()
    sim.set_time(sim.TICKS_PERIOD - 3)
    start = time.ticks_ms()
    deadline = time.ticks_add(start, 10)
    time.sleep_ms(1)
    now = time.ticks_ms()
    print("    start = ticks_ms()         # 3 ms before the wrap")
    print("    deadline = ticks_add(start, 10)")
    print("  Running:  start = " + str(start) + ", deadline = " + str(deadline) + ", now = " + str(now))
    print("            now >= deadline                      is " + str(now >= deadline) + "   (WRONG: 1 ms passed)")
    print("            ticks_diff(now, deadline) >= 0       is " + str(time.ticks_diff(now, deadline) >= 0) + "  (right)")
    print()
    print("  The deadline wrapped to a small number, so a plain >= fires at")
    print("  once. ticks_diff works on the ring and gets it right as long as")
    print("  the two times are less than half a period apart.")
    sim.reset()

    ui.wait_enter()
    ui.clear_screen()
    ui.exercise(7)

    ui.question(
        "Which is safe across the counter wrap:  1) now >= deadline\n"
        "  2) ticks_diff(now, deadline) >= 0    (type 1 or 2)",
        "2",
        "ticks_diff knows the counter is a ring; a plain comparison does not.",
    )
    ui.question(
        "While time.sleep_ms(500) runs, can the board read a button?\n"
        "  (yes or no)",
        "no",
        "sleep blocks everything else until it ends.",
    )

    ui.challenge(
        [
            "Print the word tick every 250 ms of simulated time, WITHOUT a single",
            "sleep(250). Use ticks_ms and ticks_diff, and end each pass with",
            "time.sleep_ms(50). Run for 1100 ms; it must print 4 ticks. Save it next",
            "to sim.py in this folder; the example's first two lines start the simulator.",
        ],
        [],
        ["tick", "tick", "tick", "tick"],
        [
            "import sim  # PC only: not needed on a Pico",
            "sim.install()",
            "import time",
            "",
            "start = time.ticks_ms()",
            "last = start",
            "while time.ticks_diff(time.ticks_ms(), start) < 1100:",
            "    if time.ticks_diff(time.ticks_ms(), last) >= 250:",
            "        last = time.ticks_add(last, 250)",
            '        print("tick")',
            "    time.sleep_ms(50)",
        ],
    )

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("SUMMARY")

    print("   - sleep makes the chip deaf; ticks_ms lets it keep working")
    print("   - if ticks_diff(ticks_ms(), last) >= period: last = ticks_add(last, period)")
    print("   - the counter wraps; the period is port-specific, so never compare with <")
    print()
    print("  Module 8: code that runs the instant something happens.")
    ui.wait_enter()


presses = 0


def count_press(pin):
    global presses
    presses += 1


scheduled_log = []


def log_press(argument):
    scheduled_log.append("handled outside the interrupt: " + str(argument))


def hand_off(pin):
    micropython.schedule(log_press, pin.id)


def lesson_08_interrupts():
    global presses
    ui.title("MODULE 8 - INTERRUPTS")
    sim.reset()

    ui.heading("PART 1: Pin.irq")

    print("  Polling checks a pin each time round the loop; a short pulse")
    print("  between two checks is lost. An interrupt makes the chip stop,")
    print("  run a small function right now, and carry on.")
    print()
    print("    button = Pin(14, Pin.IN, Pin.PULL_UP)")
    print("    button.irq(handler=count_press, trigger=Pin.IRQ_FALLING)")
    print()
    button = Pin(14, Pin.IN, Pin.PULL_UP)
    sim.drive(14, 1)
    presses = 0
    button.irq(handler=count_press, trigger=Pin.IRQ_FALLING)
    print("    def count_press(pin):")
    print("        global presses")
    print("        presses += 1")
    print()
    print("  Then three presses (the pin goes 0, then back to 1):")
    for _ in range(3):
        sim.drive(14, 0)
        sim.drive(14, 1)
    print("  Running:  presses = " + str(presses))
    print()
    print("  IRQ_FALLING is a change from 1 to 0; IRQ_RISING the reverse;")
    print("  the two can be combined with |.")

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 2: what a handler may not do")

    print("  The MicroPython documentation on interrupt handlers gives the")
    print("  rules. A handler must be short, and it must not allocate memory:")
    print("  no building lists or strings, and no floats (a float is an")
    print("  object on the heap). If the memory manager is already busy when")
    print("  the interrupt arrives, the program stops with an error that is")
    print("  hard to trace.")
    print()
    print("  The way out is micropython.schedule, which asks the main program")
    print("  to run a function as soon as it safely can:")
    print()
    print("    def hand_off(pin):")
    print("        micropython.schedule(log_press, pin.id)")
    print()
    button.irq(handler=hand_off, trigger=Pin.IRQ_FALLING)
    sim.drive(14, 0)
    sim.drive(14, 1)
    for _ in range(3):
        pass  # give the scheduler a few bytecodes to run it
    for line in scheduled_log:
        print("  Running:  " + line)
    del scheduled_log[:]
    print()
    print("  The handler only schedules; the print happens outside the")
    print("  interrupt, where allocating and printing are fine. (Also")
    print("  useful: micropython.alloc_emergency_exception_buf(100) at the")
    print("  top of a program, so an error inside a handler can be shown.)")

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 3: shared data, and events while interrupts are off")

    print("  If a handler and the main loop share a value that takes several")
    print("  steps to read or write, an interrupt can arrive between the steps.")
    print("  Turn interrupts off around the access and restore what was there:")
    print()
    print("    state = machine.disable_irq()")
    print("    copy = presses")
    print("    machine.enable_irq(state)")
    print()
    print("  disable_irq returns the previous state so enable_irq can put it")
    print("  back, even if interrupts were already off.")
    print()
    button.irq(handler=count_press, trigger=Pin.IRQ_FALLING)
    presses = 0
    state = machine.disable_irq()
    for _ in range(3):
        sim.drive(14, 0)
        sim.drive(14, 1)
    off_count = presses
    machine.enable_irq(state)
    print("  Running:  3 presses while off -> presses = " + str(off_count))
    print("            after enable_irq     -> presses = " + str(presses))
    print()
    print("  In this simulation one event waits per pin, so three presses")
    print("  while off count as one. Treat that as a reason to keep the")
    print("  interrupts-off stretch short, not as a promise of what any")
    print("  particular chip does with a second event.")

    ui.wait_enter()
    ui.clear_screen()
    ui.exercise(8)

    ui.question(
        "May an interrupt handler build a new list or string? (yes or no)",
        "no",
        "Handlers must not allocate memory; hand the work to micropython.schedule.",
    )
    ui.question(
        "Which micropython function hands work from a handler to the\n"
        "  main program?",
        "schedule",
        "micropython.schedule(function, argument) runs it outside the interrupt.",
    )

    ui.challenge(
        [
            "Count falling edges on GP14 with an interrupt. Simulate three presses",
            "with sim.drive(14, 0) then sim.drive(14, 1), three times, then print",
            "the count. Save it next to sim.py in this folder.",
        ],
        [],
        ["3"],
        [
            "import sim  # PC only: not needed on a Pico",
            "sim.install()",
            "from machine import Pin",
            "",
            "presses = 0",
            "",
            "def count(pin):",
            "    global presses",
            "    presses += 1",
            "",
            "button = Pin(14, Pin.IN, Pin.PULL_UP)",
            "sim.drive(14, 1)",
            "button.irq(handler=count, trigger=Pin.IRQ_FALLING)",
            "for _ in range(3):",
            "    sim.drive(14, 0)",
            "    sim.drive(14, 1)",
            "print(presses)",
        ],
    )

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("SUMMARY")

    print("   - pin.irq(handler=f, trigger=Pin.IRQ_FALLING) runs f on the event")
    print("   - a handler is short and allocates nothing; use micropython.schedule")
    print("   - guard multi-step shared data with disable_irq / enable_irq")
    print()
    print("  Module 9: making code fast, with real native compilation.")
    ui.wait_enter()
