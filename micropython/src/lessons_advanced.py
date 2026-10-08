"""Modules 9 to 11 - native code, inline assembly, and a final project.

Modules 9 and 11 run for real here. Module 10 cannot: the inline assembler
produces ARM code for the Pico's chip, and this PC is not an ARM. The module
says exactly what was checked and what was not.
"""

import gc

import micropython
import machine
from machine import Pin, PWM

import time

import sim
import ui


def total_plain(buffer, count):
    total = 0
    for i in range(count):
        total += buffer[i]
    return total


@micropython.native
def total_native(buffer, count):
    total = 0
    for i in range(count):
        total += buffer[i]
    return total


@micropython.viper
def total_viper(buffer: ptr8, count: int) -> int:
    total = 0
    for i in range(count):
        total += buffer[i]
    return total


@micropython.viper
def fill_viper(buffer: ptr8, count: int, value: int):
    for i in range(count):
        buffer[i] = value


def timed(function, buffer, count, repeats):
    start = sim.real_ticks_us()
    result = 0
    for _ in range(repeats):
        result = function(buffer, count)
    return result, sim.real_ticks_us() - start


def lesson_09_speed():
    ui.title("MODULE 9 - SPEED: native AND viper")
    sim.reset()

    ui.heading("PART 1: the same loop, three ways")

    print("  MicroPython normally turns your code into bytecode and an")
    print("  interpreter runs it. Two decorators change that, and both work")
    print("  on this PC, so the numbers below are real:")
    print()
    print("    @micropython.native   compile to machine code, same Python")
    print("    @micropython.viper    machine code with typed values")
    print()
    print("  All three add up the bytes of a 20,480-byte buffer, 20 times:")
    print()
    print("    for i in range(count): total += buffer[i]")
    print()

    data = bytearray(i & 255 for i in range(20480))
    count = len(data)
    rows = (
        ("plain Python", total_plain),
        ("@native", total_native),
        ("@viper", total_viper),
    )
    results = []
    for label, function in rows:
        result, microseconds = timed(function, data, count, 20)
        results.append((label, result, microseconds))
        print("  Running:  {:<13} total {}   {} us".format(label, result, microseconds))
    print()
    agree = results[0][1] == results[1][1] == results[2][1]
    print("  All three agree on the answer: " + str(agree))
    print()
    print("  The times are this PC's, not a Pico's, and they differ a little")
    print("  each run. The ratio is the lesson: the interpreter pays for")
    print("  every bytecode, native removes that, and viper also removes the")
    print("  work of treating every number as a Python object.")

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 2: viper, and writing through a pointer")

    print("  viper needs types. An annotation tells it what each value is:")
    print()
    print("    @micropython.viper")
    print("    def fill_viper(buffer: ptr8, count: int, value: int):")
    print("        for i in range(count):")
    print("            buffer[i] = value")
    print()
    print("  ptr8 means 'the address of a run of bytes': indexing it reads or")
    print("  writes that memory directly. The int type is a machine integer,")
    print("  fixed size, so it wraps instead of growing the way module 2's")
    print("  integers did.")
    print()
    block = bytearray(6)
    fill_viper(block, len(block), 7)
    print("    block = bytearray(6);  fill_viper(block, 6, 7)")
    print("  Running:  " + str(list(block)))
    print()
    print("  A ptr8 does no bounds checking, like a C pointer: asking for")
    print("  buffer[100] of a 6-byte buffer reads or overwrites whatever lies")
    print("  after it. That is not run here. Pass the real length in, as with")
    print("  the Arduino course's arrays.")

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 3: where speed and memory really go")

    print("  Often the time goes to making objects, not to arithmetic.")
    print("  Measured on this build:")
    print()
    gc.collect()
    before = gc.mem_free()
    value = 0.0
    for _ in range(1000):
        value = value + 0.5
    float_cost = before - gc.mem_free()
    gc.collect()
    before = gc.mem_free()
    for i in range(1000):
        text = "v" + str(i)
    text_cost = before - gc.mem_free()
    print("    1000 float additions     allocated " + str(float_cost) + " bytes")
    print("    1000 string concatenations allocated " + str(text_cost) + " bytes")
    print()
    print("  Each of those throws garbage onto the heap, and the garbage")
    print("  collector runs later, at a moment you did not choose. In a loop")
    print("  that must be steady, pre-allocate and reuse a buffer, keep to")
    print("  integers, and call gc.collect() before the loop starts.")
    print()
    print("  One more cheap trick: a name bound with const() is replaced by")
    print("  its value at compile time and takes no RAM.")
    print()
    print("    from micropython import const")
    print("    LED_PIN = const(25)")

    ui.wait_enter()
    ui.clear_screen()
    ui.exercise(9)

    ui.question(
        "Which decorator allows typed values and ptr8 pointers?",
        "viper",
        "@micropython.native keeps Python's types; @viper adds machine ones.",
    )
    ui.question(
        "Does indexing a ptr8 past the end of its buffer raise an error?\n"
        "  (yes or no)",
        "no",
        "It reads or writes the memory next to it, like a C pointer.",
    )

    ui.challenge(
        [
            "Write a @micropython.viper function total(buf: ptr8, n: int) -> int that",
            "adds up n bytes. Call it on bytearray(range(10)) and print the result.",
        ],
        [],
        ["45"],
        [
            "import micropython",
            "",
            "@micropython.viper",
            "def total(buf: ptr8, n: int) -> int:",
            "    result = 0",
            "    for i in range(n):",
            "        result += buf[i]",
            "    return result",
            "",
            "print(total(bytearray(range(10)), 10))",
        ],
    )

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("SUMMARY")

    print("   - @native and @viper compile to machine code; both run on this PC")
    print("   - viper needs types and gives ptr8 access, with no bounds check")
    print("   - allocation in a loop is the usual hidden cost: floats, strings, lists")
    print("   - const() costs no RAM; gc.collect() before a timing-sensitive loop")
    print()
    print("  Module 10: assembly language, for the Pico's own chip.")
    ui.wait_enter()


ADD_ONE_LISTING = [
    "@micropython.asm_thumb",
    "def add_one(r0):",
    "    add(r0, r0, 1)",
]

ASM_LISTING = [
    "@micropython.asm_thumb",
    "def sum_array(r0, r1):",
    "    mov(r2, 0)            # r2 = running total",
    "    label(LOOP)",
    "    cmp(r1, 0)            # any bytes left?",
    "    beq(DONE)             # no: finish",
    "    ldrb(r3, [r0, 0])     # r3 = the byte r0 points at",
    "    add(r2, r2, r3)       # total += byte",
    "    add(r0, r0, 1)        # point at the next byte",
    "    sub(r1, r1, 1)        # one fewer left",
    "    b(LOOP)",
    "    label(DONE)",
    "    mov(r0, r2)           # the result goes back in r0",
]


def model_sum_array(data):
    """The listing above, one Python line per instruction. A model of what
    the registers do, not the assembly itself."""
    r0 = 0  # an address; here, an index into data
    r1 = len(data)
    r2 = 0  # mov(r2, 0)
    rows = []
    while True:  # label(LOOP)
        if r1 == 0:  # cmp(r1, 0) ; beq(DONE)
            break
        r3 = data[r0]  # ldrb(r3, [r0, 0])
        r2 = r2 + r3  # add(r2, r2, r3)
        r0 = r0 + 1  # add(r0, r0, 1)
        r1 = r1 - 1  # sub(r1, r1, 1)
        rows.append((r0, r1, r2, r3))
    return r2, rows  # mov(r0, r2)


def lesson_10_assembly():
    ui.title("MODULE 10 - INLINE ASSEMBLY ON THE PICO")
    sim.reset()

    ui.heading("PART 1: what runs where")

    print("  The Pico's RP2040 has two Arm Cortex-M0+ cores. MicroPython can")
    print("  assemble instructions for that chip inside a Python file:")
    print()
    for line in ADD_ONE_LISTING:
        print("    " + line)
    print()
    print("  The rules (MicroPython documentation): up to four arguments,")
    print("  named r0 to r3; the value left in r0 when the function ends is")
    print("  returned, as an integer; an array argument arrives as the")
    print("  ADDRESS of its data. Most instructions use only r0-r7, and")
    print("  r8-r12 must be put back as they were before returning.")
    print()
    print("  This PC is an x86-64, not an Arm, so it cannot run that. The")
    print("  unix port even refuses to read it. Compiling the lines above:")
    print()
    try:
        exec("import micropython\n@micropython.asm_thumb\ndef f(r0):\n    add(r0, r0, 1)\n")
    except SyntaxError as error:
        print("  Running:  SyntaxError: " + str(error))
    print()
    print("  So nothing in this module is run on the chip. What was done")
    print("  instead: every listing here was assembled for the Cortex-M0+")
    print("  by mpy-cross (the MicroPython compiler, run with -march=armv6m)")
    print("  by check-asm.py in this course's folder, which fails if one")
    print("  stops assembling. Assembled, not executed.")

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 2: a loop over a buffer")

    print("  Adding up the bytes of a bytearray:")
    print()
    for line in ASM_LISTING:
        print("    " + line)
    print()
    print("  label() names a spot; b() jumps to it; beq() jumps if the last")
    print("  cmp found equal. ldrb loads one byte from the address in r0.")
    print()
    print("  To see what the registers do, here is the same listing as a")
    print("  small model in Python, one line per instruction. It is a model")
    print("  of the registers, not the assembly, so it shows the logic, not")
    print("  the chip. For the bytes 3, 5, 9:")
    print()
    total, rows = model_sum_array([3, 5, 9])
    print("      after each pass:   r0(next)  r1(left)  r2(total)  r3(byte)")
    for row in rows:
        print("                         {:>5}  {:>8}  {:>9}  {:>8}".format(row[0], row[1], row[2], row[3]))
    print("      returned in r0: " + str(total))
    print()
    print("  Call it from Python as sum_array(bytearray(b'...'), 3): the")
    print("  bytearray becomes r0, the 3 becomes r1.")

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 3: PIO, the other assembly on the Pico")

    print("  The RP2040 has a second, stranger assembly: PIO. Two blocks of")
    print("  four tiny state machines, each running its own program in")
    print("  hardware, no CPU involved, at one instruction per clock. They")
    print("  generate timing-exact signals (RP2040 datasheet). The example")
    print("  from the MicroPython documentation, blinking the LED at 1 Hz:")
    print()
    pio_listing = [
        "@rp2.asm_pio(set_init=rp2.PIO.OUT_LOW)",
        "def blink_1hz():",
        "    # Cycles: 1 + 7 + 32 * (30 + 1) = 1000",
        "    set(pins, 1)",
        "    set(x, 31)                  [6]",
        '    label("delay_high")',
        "    nop()                       [29]",
        '    jmp(x_dec, "delay_high")',
        "",
        "    # Cycles: 1 + 7 + 32 * (30 + 1) = 1000",
        "    set(pins, 0)",
        "    set(x, 31)                  [6]",
        '    label("delay_low")',
        "    nop()                       [29]",
        '    jmp(x_dec, "delay_low")',
        "",
        "sm = rp2.StateMachine(0, blink_1hz, freq=2000, set_base=Pin(25))",
        "sm.active(1)",
    ]
    for line in pio_listing:
        print("    " + line)
    print()
    print("  The [6] and [29] make an instruction wait that many extra")
    print("  cycles. The cycle count in the comment, checked:")
    print("  Running:  1 + 7 + 32 * (30 + 1) = " + str(1 + 7 + 32 * (30 + 1)))
    print("  Two of those make 2000 cycles; at freq=2000 that is one second:")
    print("  half on, half off.")
    print()
    print("  Not run here, because the rp2 module only exists on the board:")
    try:
        import rp2
    except ImportError as error:
        print("  Running:  ImportError: " + str(error))

    ui.wait_enter()
    ui.clear_screen()
    ui.exercise(10)

    ui.question(
        "In an asm_thumb function, which register holds the return value?",
        "r0",
        "Whatever is in r0 when the function ends is returned, as an integer.",
    )
    ui.question(
        "At most how many arguments can an inline assembler function take?\n"
        "  (a number)",
        "4",
        "They arrive in r0, r1, r2 and r3.",
    )

    ui.challenge(
        [
            "NEEDS A PICO: the assembler cannot run on a PC. Write an asm_thumb",
            "function double(r0) that returns twice its argument, and print",
            "double(21) on the board. It must print 42 there.",
        ],
        [],
        ["42"],
        [
            "import micropython",
            "",
            "@micropython.asm_thumb",
            "def double(r0):",
            "    add(r0, r0, r0)",
            "",
            "print(double(21))",
        ],
    )

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("SUMMARY")

    print("   - @micropython.asm_thumb assembles Cortex-M0+ code; up to 4 args, r0 returns")
    print("   - r0-r7 are free; r8-r12 must be restored")
    print("   - this PC cannot run it; the listings were assembled with mpy-cross")
    print("   - PIO is a separate, hardware assembly for timing-exact signals")
    print()
    print("  Module 11: everything together, in a robot.")
    ui.wait_enter()


TRIG = 3
ECHO = 2
SLOW = 31000
FAST = 52000

MODE_FORWARD = 0
MODE_TURN = 1
MODE_STOP = 2

robot = {"mode": MODE_FORWARD, "since": 0, "last_check": 0}


def read_distance_cm(trig, echo):
    # An HC-SR04: a 10 microsecond pulse on TRIG starts a measurement and
    # ECHO stays high for as long as the sound took to go and come back.
    # The datasheet's rule of thumb: microseconds divided by 58 is cm.
    trig.value(0)
    time.sleep_us(2)
    trig.value(1)
    time.sleep_us(10)
    trig.value(0)
    pulse_us = machine.time_pulse_us(echo, 1, 30000)
    if pulse_us < 0:
        return 400  # no echo: nothing in range, not 'touching'
    return pulse_us // 58


def choose_speed(distance_cm):
    if distance_cm < 10:
        return 0
    if distance_cm < 30:
        return SLOW
    return FAST


def robot_update(trig, echo, left, right):
    if time.ticks_diff(time.ticks_ms(), robot["last_check"]) < 100:
        return
    robot["last_check"] = time.ticks_add(robot["last_check"], 100)

    distance = read_distance_cm(trig, echo)

    if robot["mode"] == MODE_FORWARD:
        if distance < 10:
            robot["mode"] = MODE_STOP
        elif distance < 30:
            robot["mode"] = MODE_TURN
            robot["since"] = time.ticks_ms()
    elif robot["mode"] == MODE_TURN:
        if time.ticks_diff(time.ticks_ms(), robot["since"]) >= 400:
            robot["mode"] = MODE_FORWARD

    if robot["mode"] == MODE_FORWARD:
        left.duty_u16(choose_speed(distance))
        right.duty_u16(choose_speed(distance))
    elif robot["mode"] == MODE_TURN:
        left.duty_u16(38000)
        right.duty_u16(0)
    else:
        left.duty_u16(0)
        right.duty_u16(0)


def lesson_11_robot():
    ui.title("MODULE 11 - FINAL PROJECT: AN OBSTACLE ROBOT")
    sim.reset()

    ui.heading("PART 1: the sensor")

    print("  Two motors on PWM pins GP16 and GP17, and an HC-SR04 ultrasonic")
    print("  sensor: TRIG on GP3, ECHO on GP2. This module uses almost")
    print("  everything so far: Pin, PWM, ticks_ms, integer division, dicts")
    print("  and functions.")
    print()
    print("    trig.value(1); time.sleep_us(10); trig.value(0)    # start pulse")
    print("    pulse_us = machine.time_pulse_us(echo, 1, 30000)")
    print("    return pulse_us // 58                              # to cm")
    print()
    print("  time_pulse_us measures how long a pin stays at a level, in")
    print("  microseconds. It returns a NEGATIVE number when there was no")
    print("  pulse (-2 if it never started, -1 if it never ended). That must")
    print("  become 'nothing there', not 'touching': a robot that treats")
    print("  no echo as 0 cm stops for no reason. Here it returns 400.")
    print()

    trig = Pin(TRIG, Pin.OUT)
    echo = Pin(ECHO, Pin.IN)
    for wall_cm in (20, 55, None):
        if wall_cm is None:
            sim.set_pulse(ECHO, None)  # PC only: no echo comes back
            print("  Running:  no wall at all reads " + str(read_distance_cm(trig, echo)) + " cm")
        else:
            sim.set_pulse(ECHO, wall_cm * 58)  # PC only
            print("  Running:  a wall " + str(wall_cm) + " cm away (echo " + str(wall_cm * 58) + " us) reads " + str(read_distance_cm(trig, echo)) + " cm")

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 2: the decisions")

    print("  Three modes, a dict holding which one is active, and the clock")
    print("  deciding when to leave the timed one:")
    print()
    print("    FORWARD  drive; closer than 30 cm -> TURN; closer than 10 -> STOP")
    print("    TURN     pivot for 400 ms, then back to FORWARD")
    print("    STOP     both motors off")
    print()
    print("  robot_update() looks at the sensor every 100 ms and returns at")
    print("  once otherwise, so the main loop stays free for buttons,")
    print("  commands, anything else -- module 7's pattern:")
    print()
    print("    if time.ticks_diff(time.ticks_ms(), robot['last_check']) < 100:")
    print("        return")
    print()
    print("  Speeds come from a small function that can be tested alone:")
    print()
    print("    def choose_speed(distance_cm):")
    print("        if distance_cm < 10: return 0")
    print("        if distance_cm < 30: return SLOW")
    print("        return FAST")

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 3: running it against a simulated room")

    print("  The world is a distance that shrinks while the robot drives and")
    print("  grows while it turns. At 2000 ms a hand appears 6 cm in front.")
    print()
    print("    time    distance  mode     left   right")

    sim.reset()
    robot["mode"] = MODE_FORWARD
    robot["since"] = 0
    robot["last_check"] = 0
    trig = Pin(TRIG, Pin.OUT)
    echo = Pin(ECHO, Pin.IN)
    left = PWM(Pin(16))
    right = PWM(Pin(17))
    names = ("FORWARD", "TURN   ", "STOP   ")
    world_cm = 80

    for tick in range(1, 27):
        if tick >= 20:
            world_cm = 6
        sim.set_pulse(ECHO, world_cm * 58)
        time.sleep_ms(100)
        robot_update(trig, echo, left, right)

        if tick % 2 == 0:
            print("    {:>5} ms  {:>3} cm   {}  {:>5}  {:>5}".format(
                time.ticks_ms(), world_cm, names[robot["mode"]],
                left.duty_u16(), right.duty_u16()))

        if robot["mode"] == MODE_FORWARD and tick < 20:
            world_cm = world_cm - left.duty_u16() // 8000
        elif robot["mode"] == MODE_TURN:
            world_cm = world_cm + 12
    sim.reset()

    print()
    print("  Read it top to bottom: drive, the wall gets close, it pivots,")
    print("  the way clears, it drives again, and when the hand appears it")
    print("  stops and stays stopped.")

    ui.wait_enter()
    ui.clear_screen()
    ui.exercise(11)

    ui.question(
        "An echo pulse of 1160 microseconds. How many centimetres?\n"
        "  (divide by 58)",
        "20",
        "1160 // 58 = 20.",
    )
    ui.question(
        "time_pulse_us returns -2. Is something touching the sensor?\n"
        "  (yes or no)",
        "no",
        "A negative value means no pulse arrived: nothing in range.",
    )

    ui.challenge(
        [
            "Write choose_speed(distance_cm): 0 below 10 cm, 31000 below 30,",
            "otherwise 52000. Print its result for 5, 20 and 50, one per line.",
        ],
        [],
        ["0", "31000", "52000"],
        [
            "def choose_speed(distance_cm):",
            "    if distance_cm < 10:",
            "        return 0",
            "    if distance_cm < 30:",
            "        return 31000",
            "    return 52000",
            "",
            "for distance in (5, 20, 50):",
            "    print(choose_speed(distance))",
        ],
    )

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("SUMMARY")

    print("   - split the problem: read the sensor, decide, drive")
    print("   - a mode plus ticks_ms is a state machine that never blocks")
    print("   - a negative time_pulse_us means no echo: far away, not zero")
    print("   - small pure functions like choose_speed are easy to test")
    print()
    print("  That is the course. The Arduino course has the same ideas in")
    print("  C++, down to the registers.")
    ui.wait_enter()
