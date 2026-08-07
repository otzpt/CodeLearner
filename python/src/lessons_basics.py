"""Modules 1 to 5 - the basics.

Rule used throughout: nothing is claimed without being shown. Where an
example can run here, it runs.
"""

import ui


def lesson_01_running():
    ui.title("MODULE 1 - RUNNING PYTHON AND PRINT()")

    ui.heading("PART 1: no compile step")

    print("  A C program has to be translated into machine code before it")
    print("  can run. Python skips that: the interpreter reads your file")
    print("  and runs it directly, line by line.")
    print()
    print("    python3 program.py")
    print()
    print("  That is the whole command. No -o, no linking, no separate")
    print("  binary left behind. It also means a mistake on line 40 is not")
    print("  caught until the interpreter actually reaches line 40 -- a C")
    print("  compiler would have refused to build at all. Python trades an")
    print("  early, whole-program check for a fast start.")

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 2: print()")

    print('    print("Hello world")')
    print()
    print("  print() takes several arguments and joins them with a space")
    print("  by default:")
    print()
    print('    print("a", "b", "c")')

    print("\n  Running:  ", end="")
    print("a", "b", "c")

    print()
    print("  Two keyword arguments change that behaviour:")
    print()
    print('    print("a", "b", sep="-")     # a-b')
    print('    print("no newline", end="")  # no \\n after this call')
    print()
    print("  Running:  ", end="")
    print("a", "b", sep="-")
    print("  Running:  ", end="")
    print("no newline", end="")
    print("!")

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 3: comments and the file that runs itself")

    print("  # starts a comment, to the end of the line -- there is no")
    print("  block-comment syntax the way C has /* */.")
    print()
    print('    # this line does nothing')
    print('    print("this one does")')
    print()
    print("  A script needs no main() -- the file's top-level code is what")
    print("  runs, top to bottom, the moment the interpreter reads it.")

    ui.wait_enter()
    ui.clear_screen()
    ui.exercise(1)

    ui.question(
        'print("a", "b");  -- what appears between them by default?',
        "a space",
        "print() joins its arguments with a single space unless sep= says otherwise.",
    )
    ui.question(
        "Does Python need a separate compile step before running a script?\n"
        "  (answer: yes or no)",
        "no",
        "The interpreter reads and runs the file directly.",
    )

    ui.challenge(
        ["Print your name and the current year, each on its own line."],
        [],
        ["Jose", "2026"],
        [
            'print("Jose")',
            'print("2026")',
        ],
    )

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("SUMMARY")

    print("   - python3 file.py runs directly, no build step")
    print("   - print() joins arguments with a space; sep= and end= change that")
    print("   - a mistake later in the file is only caught once execution reaches it")
    print()
    print("  Module 2: variables, and a division rule that is not C's.")
    ui.wait_enter()


def lesson_02_variables():
    ui.title("MODULE 2 - VARIABLES AND TYPES")

    ui.heading("PART 1: no declaration, and the type can change")

    print("  C makes you say int age = 25 -- the type first, fixed forever.")
    print("  Python just assigns:")
    print()
    print('    age = 25')
    print('    height = 1.75')
    print('    name = "Ana"')
    print()
    print("  The variable is not typeless -- type(age) really is int -- but")
    print("  nothing stops you rebinding the same name to a different type")
    print("  later:")
    print()

    x = 5
    print(f"    x = 5           -> type(x) is {type(x).__name__}")
    x = "now a string"
    print(f'    x = "now..."    -> type(x) is {type(x).__name__}')
    print()
    print("  That is legal because x was never a box with a fixed type. It")
    print("  is a name, and = just points it at a new object.")

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 2: two kinds of division")

    a, b = 7, 2
    print("    a, b = 7, 2")
    print()
    print(f"    a / b    -> {a / b}     # true division: always a float")
    print(f"    a // b   -> {a // b}       # floor division: rounds down")
    print()
    print("  / never truncates the way C's int/int does -- 7 / 2 is 3.5,")
    print("  full stop. // is the one that drops the fraction, and it")
    print("  rounds DOWN, not toward zero:")
    print()
    print(f"    -7 // 2  -> {-7 // 2}      # C's -7 / 2 is -3; Python's is -4")
    print(f"    -7 % 2   -> {-7 % 2}       # the remainder follows the divisor's sign")
    print()
    print("  If you know C: this is the one arithmetic rule that is")
    print("  genuinely different, not just spelled differently.")

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 3: f-strings")

    name = "Ana"
    pi = 3.14159
    print('    name = "Ana"')
    print("    pi = 3.14159")
    print('    print(f"{name} tem pi={pi:.2f}")')
    print()
    print(f"  Running:  {name} tem pi={pi:.2f}")
    print()
    print("  f before the quote turns {expr} inside the string into its")
    print("  value. :.2f after a colon is a format spec -- two decimal")
    print("  places, rounded for display only; pi itself is unchanged.")

    ui.wait_enter()
    ui.clear_screen()
    ui.exercise(2)

    ui.question("x = 7 / 2   What is x?", "3.5", "/ is true division and never truncates.")
    ui.question(
        "x = 7 // 2   What is x?", "3", "// floors the true result: 3.5 rounds down to 3."
    )
    ui.question(
        "-7 // 2   Does this floor toward zero or toward negative infinity?",
        "negative infinity",
        "Python floors down, always -- -7 // 2 is -4, not -3.",
    )

    ui.challenge(
        ["Declare a = 7 and b = 2, and print their true division", "(should show 3.5)."],
        [],
        ["3.5"],
        ["a = 7", "b = 2", "print(a / b)"],
    )

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("SUMMARY")

    print("   - assignment, not declaration; a name can point at any type")
    print("   - / is always true division; // floors, toward negative infinity")
    print("   - f-strings: f\"{expr:spec}\" -- computed once, formatted for display")
    ui.wait_enter()


def lesson_03_input():
    ui.title("MODULE 3 - READING INPUT")

    ui.heading("PART 1: input() always returns a string")

    print('    name = input("What is your name? ")')
    print()
    print("  Whatever the user types comes back as str, whether it looks")
    print('  like a number or not. Unlike C\'s scanf, there is no size to')
    print("  get wrong -- input() reads the whole line, however long.")
    print()
    print("  For a number, convert explicitly:")
    print()
    print('    age = int(input("Age: "))')
    print('    price = float(input("Price: "))')

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 2: forgetting to convert")

    print("  This is Python's version of a very common beginner bug:")
    print()
    print('    age = input("Age: ")')
    print('    print("Next year: " + age + 1)')
    print()
    print("  Running it, with age typed as 25:")
    print()
    print('    TypeError: can only concatenate str (not "int") to str')
    print()
    print("  + on strings concatenates; + between a str and an int is not")
    print("  defined at all, so Python refuses rather than guess. Verified")
    print("  on 3.14 -- the exact wording has changed across versions, but")
    print("  the refusal has not.")

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 3: bad input raises, and can be caught")

    print("    try:")
    print('        n = int(input("Number: "))')
    print('        print(f"You typed {n}")')
    print("    except ValueError:")
    print('        print("That is not a number.")')
    print()
    print("  Typing \"abc\" where a number was expected raises ValueError.")
    print("  Uncaught, that crashes the script with a traceback; caught,")
    print("  you get to say something useful instead. Module 10 covers")
    print("  try/except properly -- this is the shape you will use most.")

    ui.wait_enter()
    ui.clear_screen()
    ui.exercise(3)

    ui.question(
        'age = input("Age: ")   What is the type of age, whatever is typed?',
        "str",
        "input() always returns a string. int()/float() convert it explicitly.",
    )
    ui.question(
        '"Total: " + age + 1  where age is a str  -- what happens?',
        "TypeError",
        "+ between str and int is not defined; Python raises rather than guesses.",
    )

    ui.challenge(
        ["Ask for the user's name and greet them."],
        ["Jose"],
        ["What is your name? Jose", "Hello, Jose"],
        [
            'name = input("What is your name? ")',
            'print(f"Hello, {name}")',
        ],
    )

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("SUMMARY")

    print("   - input() always returns a str; convert with int()/float()")
    print("   - str + int raises TypeError rather than silently doing the wrong thing")
    print("   - int(bad_text) raises ValueError -- catchable with try/except")
    ui.wait_enter()


def lesson_04_conditions():
    ui.title("MODULE 4 - CONDITIONS")

    ui.heading("PART 1: indentation is the block")

    print("  C uses { }. Python has no braces at all -- the indentation")
    print("  itself marks what is inside the if:")
    print()
    print("    score = 85")
    print("    if score >= 90:")
    print('        print("A")')
    print("    elif score >= 80:")
    print('        print("B")')
    print("    else:")
    print('        print("F")')
    print()

    score = 85
    print(f"  Running with score = {score}:  ", end="")
    if score >= 90:
        print("A")
    elif score >= 80:
        print("B")
    else:
        print("F")

    print()
    print("  Get the indentation wrong -- mix tabs and spaces, or indent by")
    print("  a different amount within the same block -- and Python refuses")
    print("  to run at all:")
    print()
    print("    IndentationError: expected an indented block after 'if'")
    print("                       statement on line 1")
    print()
    print("  Verified on 3.14. In C, bad indentation is a style problem the")
    print("  compiler never sees; in Python it is a syntax error, every time.")

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 2: truthiness")

    print("  if converts whatever you give it to a bool. These are falsy:")
    print()
    print('    False   0   0.0   ""   []   {}   None')
    print()
    print("  Everything else is truthy, including a string that just says")
    print('  "False" or a list holding a single 0:')
    print()

    print(
        "   ",
        bool(0),
        bool(""),
        bool([]),
        bool(None),
        bool({}),
        "  <- all falsy",
    )
    print("   ", bool(1), bool("x"), bool([0]), " <- all truthy")

    print()
    name = ""
    print('    name = ""')
    print("    if name:")
    print('        print("has name")')
    print("    else:")
    print('        print("no name")')
    print()
    print("  Running:  ", end="")
    if name:
        print("has name")
    else:
        print("no name")

    print()
    print("  An empty string is falsy, so testing `if name:` directly is")
    print('  idiomatic -- there is no need for `if name != "":`.')

    ui.wait_enter()
    ui.clear_screen()
    ui.exercise(4)

    ui.question(
        "Which of these are falsy: 0, 1, [], [0], \"\", \"False\"?\n"
        "  (answer: list just the falsy ones, comma separated)",
        "0, [], \"\"",
        '1, [0] and the string "False" are all truthy -- a non-empty '
        "container or string is truthy regardless of what it contains.",
    )
    ui.question(
        "A block is indented with 2 spaces on one line and 4 on the\n"
        "  next, inside the same if. What happens?",
        "IndentationError",
        "Python does not silently accept inconsistent indentation within one block.",
    )

    ui.challenge(
        ["Read a number and say whether it is positive, negative or zero."],
        ["-5"],
        ["Number: -5", "negative"],
        [
            'n = int(input("Number: "))',
            "if n > 0:",
            '    print("positive")',
            "elif n < 0:",
            '    print("negative")',
            "else:",
            '    print("zero")',
        ],
    )

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("SUMMARY")

    print("   - indentation IS the block; no braces, and inconsistency is a syntax error")
    print("   - 0, 0.0, \"\", [], {}, None are falsy; everything else is truthy")
    print("   - `if name:` is the idiomatic emptiness check")
    ui.wait_enter()


def lesson_05_loops():
    ui.title("MODULE 5 - LOOPS")

    ui.heading("PART 1: for walks a sequence, range() makes one")

    print("  Python's for does not count -- it walks whatever you give it.")
    print("  range() is how you get something to count with:")
    print()
    print("    for i in range(1, 6):")
    print('        print(i, end=" ")')
    print()
    print("  Running:  ", end="")
    for i in range(1, 6):
        print(i, end=" ")
    print()
    print()
    print("  range(1, 6) gives 1, 2, 3, 4, 5 -- the end is EXCLUSIVE, the")
    print("  same rule as C's `i < 5`, just spelled as a boundary instead")
    print("  of a comparison. range(5) alone starts at 0:")
    print()
    print("    for i in range(5):")
    print('        print(i, end=" ")')
    print()
    print("  Running:  ", end="")
    for i in range(5):
        print(i, end=" ")
    print()

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 2: walking a list directly")

    fruits = ["maca", "banana", "cereja"]
    print('    fruits = ["maca", "banana", "cereja"]')
    print("    for f in fruits:")
    print('        print(f"- {f}")')
    print()
    for f in fruits:
        print(f"    - {f}")

    print()
    print("  No index at all, unless you need one -- and when you do,")
    print("  enumerate() gives both without a separate counter variable:")
    print()
    print("    for i, f in enumerate(fruits):")
    print('        print(i, f)')
    print()
    for i, f in enumerate(fruits):
        print(f"    {i} {f}")

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 3: while, break, continue")

    print("    n = 3")
    print("    while n > 0:")
    print('        print(n, end=" ")')
    print("        n -= 1")
    print()
    n = 3
    print("  Running:  ", end="")
    while n > 0:
        print(n, end=" ")
        n -= 1
    print()
    print()
    print("  There is no n-- in Python -- n -= 1 is the whole vocabulary")
    print("  for it. break and continue work exactly as they do in C.")

    ui.wait_enter()
    ui.clear_screen()
    ui.exercise(5)

    ui.question("range(1, 6)  -- is 6 included?", "no", "The end of range() is always exclusive.")
    ui.question(
        "for f in fruits: ...  -- do you need an index to reach each element?\n"
        "  (answer: yes or no)",
        "no",
        "for walks the values directly; enumerate() adds the index only when you ask for it.",
    )

    ui.challenge(
        [
            "Print 1 through 5, one per line. Then print each of",
            '"apple", "banana", "cherry" prefixed with "- ".',
        ],
        [],
        ["1", "2", "3", "4", "5", "- apple", "- banana", "- cherry"],
        [
            "for i in range(1, 6):",
            "    print(i)",
            "",
            'fruits = ["apple", "banana", "cherry"]',
            "for f in fruits:",
            '    print(f"- {f}")',
        ],
    )

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("SUMMARY")

    print("   - for walks a sequence; range(a, b) is a to b-1")
    print("   - looping over a list needs no index; enumerate() adds one when wanted")
    print("   - n -= 1, not n--; break/continue are unchanged from C")
    print()
    print("  Module 6: lists, and the aliasing bug that costs people an afternoon.")
    ui.wait_enter()
