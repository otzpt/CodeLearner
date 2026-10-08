"""Modules 1 to 4 - the language, and how it differs from desktop Python.

Rule used throughout: nothing is claimed without being shown. Where an
example can run here, it runs. Where the PC is not the board -- a different
float size, no real pins -- the lesson says so instead of pretending.
"""

import gc
import sys

import ui


def lesson_01_running():
    ui.title("MODULE 1 - RUNNING MICROPYTHON AND print()")

    ui.heading("PART 1: Python, inside the chip")

    print("  MicroPython is Python 3 trimmed to fit a microcontroller. On a")
    print("  Raspberry Pi Pico it is a program stored in the chip's flash:")
    print("  it starts at power-on, reads your .py files from the same flash")
    print("  and runs them directly. There is no operating system, no")
    print("  compile step on your side, and nothing to install on the board")
    print("  per program.")
    print()
    print("  Two files matter (MicroPython documentation): boot.py runs")
    print("  first, then main.py. Save your program as main.py and it runs")
    print("  every time the board gets power.")
    print()
    print("  This course runs on a PC through the 'unix port' of MicroPython:")
    print()
    print("    micropython main.py")
    print()
    print("  Asking the interpreter who it is:")
    print()
    print("    sys.implementation.name   ->  " + sys.implementation.name)
    print("    sys.platform              ->  " + sys.platform)
    print()
    print("  Here the platform is 'linux'. On a Pico it is 'rp2'. Same")
    print("  language, different machine underneath.")

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 2: print() is print(), but the library is smaller")

    print('    print("a", "b", sep="-")')
    print("  Running:  ", end="")
    print("a", "b", sep="-")
    print()
    print("  print() works as it does on a desktop. The difference is what")
    print("  is missing. MicroPython keeps a subset of the standard library")
    print("  and of the str methods, to fit in a few hundred KB. Two real")
    print("  failures from this very build:")
    print()
    print('    "ab".ljust(5)')
    try:
        "ab".ljust(5)
    except AttributeError as error:
        print("  Running:  AttributeError: " + str(error))
    print()
    print("    import os; os.name")
    try:
        import os

        os.name
    except AttributeError as error:
        print("  Running:  AttributeError: " + str(error))
    print()
    print("  Code that runs on a desktop can fail on the board for a")
    print("  missing method. When it does, there is usually a smaller way")
    print("  to say it -- ljust becomes a format spec:")
    print()
    print('    "{:<5}|".format("ab")')
    print("  Running:  " + "{:<5}|".format("ab"))
    print()
    print("  (That was checked on this build. A different build may include")
    print("  more or fewer methods: try it on your own board.)")

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 3: the REPL")

    print("  Plug a board in and open a serial connection (Thonny, or")
    print("  `mpremote` from the command line) and you get a prompt:")
    print()
    print("    >>> 2 + 3")
    print("    5")
    print()
    print("  That is the REPL: read a line, evaluate it, print the result.")
    print("  It is the fastest way to test a pin or a sensor without")
    print("  writing a file. Ctrl-D restarts the interpreter (a soft reset)")
    print("  and Ctrl-C interrupts a running program (MicroPython")
    print("  documentation). A program that never stops is stopped with")
    print("  Ctrl-C; there is no power button to press on a sketch you can")
    print("  no longer type into.")

    ui.wait_enter()
    ui.clear_screen()
    ui.exercise(1)

    ui.question(
        "On a Pico, which file does MicroPython run after boot.py?",
        "main.py",
        "boot.py sets things up; main.py is your program.",
    )
    ui.question(
        "Does every desktop Python string method exist in MicroPython?\n"
        "  (yes or no)",
        "no",
        "It is a subset: ljust, for one, is missing from this build.",
    )

    ui.challenge(
        ["Print the name of the Python implementation you are running on."],
        [],
        ["micropython"],
        [
            "import sys",
            "print(sys.implementation.name)",
        ],
    )

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("SUMMARY")

    print("   - MicroPython runs your .py directly on the chip: boot.py, then main.py")
    print("   - the same language as Python 3, with a smaller standard library")
    print("   - a missing method fails at runtime, only when that line is reached")
    print("   - the REPL tests one line at a time; Ctrl-C stops a program")
    print()
    print("  Module 2: numbers, and why a float is not the same on the chip.")
    ui.wait_enter()


def lesson_02_numbers():
    ui.title("MODULE 2 - NUMBERS, TEXT AND VARIABLES")

    ui.heading("PART 1: integers, and the two kinds of division")

    print("  Python integers do not overflow. They grow to whatever size is")
    print("  needed, as long as the memory lasts:")
    print()
    print("    2 ** 100")
    print("  Running:  " + str(2**100))
    print()
    print("  That is unlike C, and unlike the Arduino course's uint8_t. It")
    print("  costs memory and time, which is why fast code on a board uses")
    print("  bytes and arrays (module 4) or the viper compiler (module 9).")
    print()
    print("    7 / 2     ->  " + str(7 / 2) + "    # true division, always a float")
    print("    7 // 2    ->  " + str(7 // 2) + "      # floor division")
    print("    -7 // 2   ->  " + str(-7 // 2) + "     # rounds DOWN, not toward zero")
    print("    -7 % 2    ->  " + str(-7 % 2) + "      # the remainder takes the divisor's sign")

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 2: floats, and a difference between PC and board")

    print("    1 / 3")
    print("  Running:  " + str(1 / 3))
    print()
    print("  That is 16 digits: a 64-bit double, which is what the unix port")
    print("  uses. On most MicroPython boards a float is single precision,")
    print("  32 bits, about 7 digits (MicroPython project wiki). The same")
    print("  line on such a board prints 0.3333333.")
    print()
    print("  A test that tells the two apart, which you can type on your own")
    print("  board:")
    print()
    print("    1.0 + 1e-8 - 1.0")
    print("  Running (here):  " + str(1.0 + 1e-8 - 1.0))
    print()
    print("  Here the tiny 1e-8 survives. In single precision it is lost and")
    print("  the answer is 0.0. So a calculation that behaves on the PC can")
    print("  lose its small terms on the board. Keep sums of very different")
    print("  sizes out of sensor code, or work in integers.")

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 3: text is not bytes")

    word = "é"
    print('    word = "é"')
    print("    len(word)            ->  " + str(len(word)))
    print("    len(word.encode())   ->  " + str(len(word.encode())))
    print()
    print("  A str is characters; bytes is what travels down a wire. The one")
    print("  letter takes 2 bytes in UTF-8. A serial port, an I2C sensor and")
    print("  a file on flash all deal in bytes, so converting is your job:")
    print()
    print("    b'Hi'.decode()  ->  " + b"Hi".decode())
    print("    'Hi'.encode()   ->  " + repr("Hi".encode()))
    print()
    name = "Pico"
    pi = 3.14159
    print('    f"{name} sees {pi:.2f}"')
    print("  Running:  " + f"{name} sees {pi:.2f}")
    print()
    print("  f-strings work, with the same format specs as desktop Python.")

    ui.wait_enter()
    ui.clear_screen()
    ui.exercise(2)

    ui.question("-7 // 2   What is it?", "-4", "// rounds down, toward minus infinity.")
    ui.question(
        'How many bytes is the single letter "é" in UTF-8? (a number)',
        "2",
        "Characters outside plain ASCII take two or more bytes.",
    )

    ui.challenge(
        [
            "Print 7 // 2, then 7 % 2, then 7 / 2, each on its own line.",
        ],
        [],
        ["3", "1", "3.5"],
        [
            "print(7 // 2)",
            "print(7 % 2)",
            "print(7 / 2)",
        ],
    )

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("SUMMARY")

    print("   - integers never overflow; / always gives a float, // floors")
    print("   - a float is 64-bit on this PC and usually 32-bit on a board")
    print("   - 1.0 + 1e-8 - 1.0 tells the two apart")
    print("   - str is characters, bytes is wire data; encode() and decode()")
    print()
    print("  Module 3: decisions, loops and functions.")
    ui.wait_enter()


def largest_of(values):
    best = values[0]
    for value in values:
        if value > best:
            best = value
    return best


counter = 5


def broken_counter():
    # Reading a global is fine; assigning to it makes the name local for the
    # whole function, so the read on the first line has nothing to read.
    print(counter)
    counter = counter + 1


def lesson_03_flow():
    ui.title("MODULE 3 - DECISIONS, LOOPS AND FUNCTIONS")

    ui.heading("PART 1: if / elif / else, and what counts as false")

    reading = 620
    print("    reading = 620")
    print('    if reading > 800: print("very bright")')
    print('    elif reading > 500: print("bright")')
    print('    else: print("dark")')
    print("  Running:  ", end="")
    if reading > 800:
        print("very bright")
    elif reading > 500:
        print("bright")
    else:
        print("dark")
    print()
    print("  Blocks are marked by indentation, not braces. Besides False,")
    print("  these count as false: 0, 0.0, an empty string, an empty list,")
    print("  None. So `if buffer:` means 'if the buffer has anything in it':")
    print()
    print("    bool(0), bool(''), bool([]), bool([0])")
    print("  Running:  " + str((bool(0), bool(""), bool([]), bool([0]))))
    print()
    print("  [0] is true: a list with one element, whatever the element is.")

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 2: for, range, while")

    print("    for i in range(1, 4): print(i, end=' ')")
    print("  Running:  ", end="")
    for i in range(1, 4):
        print(i, end=" ")
    print()
    print()
    print("  range(1, 4) is 1, 2, 3: it stops BEFORE the end value, the same")
    print("  off-by-one as the Arduino course's i < 4.")
    print()
    print("    for position, name in enumerate(['left', 'right']): ...")
    print("  Running:  ", end="")
    for position, name in enumerate(["left", "right"]):
        print(position, name, end="  ")
    print()
    print()
    print("    n = 3")
    print("    while n > 0: print(n, end=' '); n -= 1")
    print("  Running:  ", end="")
    n = 3
    while n > 0:
        print(n, end=" ")
        n -= 1
    print()
    print()
    print("  On a board the main program is very often `while True:` -- a")
    print("  loop that never ends, because a robot has no 'finished'.")

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 3: functions, and the global trap")

    print("    def largest_of(values):")
    print("        best = values[0]")
    print("        for value in values:")
    print("            if value > best: best = value")
    print("        return best")
    print()
    print("    largest_of([4, 9, 2, 7])")
    print("  Running:  " + str(largest_of([4, 9, 2, 7])))
    print()
    print("  A function may return several values at once; they arrive as a")
    print("  tuple:  def low_high(v): return min(v), max(v)")
    print()
    print("  The trap. Reading a global works. ASSIGNING to it makes the name")
    print("  local to the whole function:")
    print()
    print("    counter = 5")
    print("    def broken_counter():")
    print("        print(counter)")
    print("        counter = counter + 1")
    print()
    print("  Running broken_counter():")
    try:
        broken_counter()
    except NameError as error:
        print("    NameError: " + str(error))
    print()
    print("  The fix is `global counter` on the first line of the function")
    print("  -- or, better, pass the value in and return the new one.")

    ui.wait_enter()
    ui.clear_screen()
    ui.exercise(3)

    ui.question(
        "for i in range(1, 4): print(i)   How many lines does it print?",
        "3",
        "range(1, 4) gives 1, 2, 3 and stops before 4.",
    )
    ui.question(
        "bool([])   What is it? (True or False)",
        "False",
        "An empty list is false; so is 0, '' and None.",
    )

    ui.challenge(
        [
            "Write a function lowest(values) that returns the smallest value",
            "WITHOUT using min(). Print lowest([4, 9, 2, 7]).",
        ],
        [],
        ["2"],
        [
            "def lowest(values):",
            "    best = values[0]",
            "    for value in values:",
            "        if value < best:",
            "            best = value",
            "    return best",
            "",
            "print(lowest([4, 9, 2, 7]))",
        ],
    )

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("SUMMARY")

    print("   - indentation is the block; 0, '', [] and None are false")
    print("   - range(a, b) stops before b; while True is the robot's main loop")
    print("   - assigning to a global inside a function makes it local")
    print()
    print("  Module 4: lists, bytes, and the memory they use.")
    ui.wait_enter()


def lesson_04_memory():
    ui.title("MODULE 4 - LISTS, BYTES AND MEMORY")

    ui.heading("PART 1: a list is a reference, and a slice is a copy")

    first = [1, 2]
    second = first
    second.append(3)
    print("    first = [1, 2]")
    print("    second = first")
    print("    second.append(3)")
    print("  Running:  first is now " + str(first))
    print()
    print("  `second = first` copied nothing: both names point at the same")
    print("  list. To get an independent one, copy it:")
    print()
    third = first[:]
    third.append(4)
    print("    third = first[:]      # a slice is a new list")
    print("    third.append(4)")
    print("  Running:  first " + str(first) + ", third " + str(third))
    print()
    print("  On a board every copy uses scarce RAM, so know when you make one.")

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 2: bytearray, and what a list costs")

    buffer = bytearray(4)
    buffer[1] = 200
    print("    buffer = bytearray(4)")
    print("    buffer[1] = 200")
    print("  Running:  " + str(list(buffer)))
    print()
    print("  A bytearray holds raw bytes 0-255, one byte each, and can be")
    print("  changed in place. A list of numbers holds one machine word per")
    print("  element. Measured here, 1000 elements each:")
    print()
    gc.collect()
    before = gc.mem_free()
    numbers = list(range(1000))
    after_list = gc.mem_free()
    del numbers
    gc.collect()
    before_bytes = gc.mem_free()
    raw = bytearray(1000)
    after_bytes = gc.mem_free()
    print("    list(range(1000))   used " + str(before - after_list) + " bytes")
    print("    bytearray(1000)     used " + str(before_bytes - after_bytes) + " bytes")
    print()
    print("  (A word is 8 bytes on this PC and 4 on a Pico, so the list is")
    print("  smaller there, but the bytearray still wins by a wide margin.)")
    print()
    print("  The trap. Storing a number that does not fit a byte does not")
    print("  fail, on this build; it keeps only the low 8 bits:")
    print()
    raw[0] = 256
    raw[1] = -1
    print("    raw[0] = 256;  raw[1] = -1")
    print("  Running:  raw[0] = " + str(raw[0]) + ", raw[1] = " + str(raw[1]))

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 3: memoryview, and changing memory in place")

    data = bytearray(b"abcdef")
    view = memoryview(data)
    print('    data = bytearray(b"abcdef")')
    print("    view = memoryview(data)     # a window onto data, no copy")
    print("    view[0:2] = b'ZZ'")
    view[0:2] = b"ZZ"
    print("  Running:  data is now " + repr(data))
    print()
    print("  Slicing the bytearray itself (data[0:2]) would build a new")
    print("  object; slicing the memoryview does not. Parsing a packet in a")
    print("  loop is where that saves memory.")
    print()
    print("  Below all of this the chip's registers are memory too. On a")
    print("  board, machine.mem32[address] reads or writes a 32-bit register")
    print("  directly. It is not run here: this PC has no such address map,")
    print("  and reading a wrong one stops the program.")
    print()
    print("  gc.collect() frees what is no longer used; gc.mem_free() says")
    print("  how much RAM is left. Calling gc.collect() on purpose, before a")
    print("  timing-sensitive stretch, avoids a collection starting in the")
    print("  middle of it.")

    ui.wait_enter()
    ui.clear_screen()
    ui.exercise(4)

    ui.question(
        "b = a  where a is a list. Does b get its own copy? (yes or no)",
        "no",
        "Both names point at the same list; use a[:] for a copy.",
    )
    ui.question(
        "Which holds 1000 small numbers in less RAM: a list or a bytearray?",
        "bytearray",
        "One byte per element, against one machine word.",
    )

    ui.challenge(
        [
            "Make a bytearray of 4 zeros, set byte 1 to 200 and byte 3 to 7, and",
            "print it as a list.",
        ],
        [],
        ["[0, 200, 0, 7]"],
        [
            "data = bytearray(4)",
            "data[1] = 200",
            "data[3] = 7",
            "print(list(data))",
        ],
    )

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("SUMMARY")

    print("   - b = a shares a list; a[:] copies it")
    print("   - bytearray: one byte per element, changeable in place")
    print("   - an out-of-range store into a bytearray is silently masked")
    print("   - memoryview slices without copying; gc.collect() before timing")
    print()
    print("  Module 5: pins, the first real hardware.")
    ui.wait_enter()
