"""Modules 6 to 10 - the part that sets Python apart.

Nothing here is about memory the way the C course's second half was --
Python collects its own garbage. What replaces it is a different class of
bug: names that share an object without you asking them to, and defaults
that are computed once and then reused forever.
"""

import ui


def lesson_06_lists():
    ui.title("MODULE 6 - LISTS")

    ui.heading("PART 1: b = a does not copy")

    print("  This is the single most expensive surprise in this course.")
    print()
    print("    a = [1, 2, 3]")
    print("    b = a")
    print("    b.append(4)")
    print("    print(a)")
    print()

    a = [1, 2, 3]
    b = a
    b.append(4)
    print(f"  Running: {a}")
    print()
    print("  b = a does not copy the list -- it gives the same list a")
    print("  second name. Both names point at one object, so appending")
    print('  "to b" appends to the thing a also refers to.')
    print()
    print("  A real copy needs to be asked for:")
    print()
    print("    c = a[:]        # or list(a), or a.copy()")
    print("    c.append(5)")
    print("    print(a, c)")
    print()

    c = a[:]
    c.append(5)
    print(f"  Running: {a} {c}")

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 2: slicing")

    numbers = [1, 2, 3, 4, 5]
    print("    a = [1, 2, 3, 4, 5]")
    print()
    print("  Running, with a = [1, 2, 3, 4, 5]:")
    print(f"    a[1:3]   -> {numbers[1:3]}")
    print(f"    a[-1]    -> {numbers[-1]}")
    print(f"    a[::-1]  -> {numbers[::-1]}")
    print()
    print("  The end of a slice is exclusive, the same rule as range(). A")
    print("  negative index counts from the end; [::-1] is the idiomatic")
    print("  way to reverse a copy without mutating the original.")

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 3: sorting and comprehensions")

    nums = [5, 3, 1, 4, 2]
    print("    nums = [5, 3, 1, 4, 2]")
    print("    print(sorted(nums))    # new list, nums unchanged")
    print("    nums.sort()            # sorts in place, returns None")
    print()
    print(f"  Running: sorted(nums) -> {sorted(nums)}")
    nums.sort()
    print(f"           nums.sort(); nums -> {nums}")
    print()
    print("  `x = nums.sort()` is a real and common bug: sort() returns")
    print("  None, and x ends up bound to that, not to a sorted list.")
    print()
    print("    squares = [x * x for x in range(5)]")
    squares = [x * x for x in range(5)]
    print(f"  Running: {squares}")
    print()
    print("  A list comprehension is a for loop and an append compressed")
    print("  into one expression -- the same result as building an empty")
    print("  list and appending inside a loop, written more directly.")

    ui.wait_enter()
    ui.clear_screen()
    ui.exercise(6)

    ui.question(
        "a = [1, 2]; b = a; b.append(3)  -- does a change too?\n"
        "  (answer: yes or no)",
        "yes",
        "b = a is a second name for the same list, not a copy.",
    )
    ui.question("What copies a list, instead of aliasing it?", "a[:]", "a[:], list(a) and a.copy() all make a real copy.")
    ui.question(
        "x = nums.sort()   What is x?",
        "None",
        "sort() sorts in place and returns None -- use sorted(nums) for a copy you can assign.",
    )

    ui.challenge(
        [
            "Given nums = [3, 1, 2], print a SORTED COPY (nums itself",
            "must stay [3, 1, 2] afterwards), then print nums itself",
            "to prove it was not touched.",
        ],
        [],
        ["[1, 2, 3]", "[3, 1, 2]"],
        [
            "nums = [3, 1, 2]",
            "print(sorted(nums))",
            "print(nums)",
        ],
    )

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("SUMMARY")

    print("   - b = a aliases; a[:] / list(a) / a.copy() actually copy")
    print("   - slice ends are exclusive, same rule as range()")
    print("   - sort() returns None; sorted() returns a new list")
    ui.wait_enter()


def lesson_07_functions():
    ui.title("MODULE 7 - FUNCTIONS AND DEFAULT ARGUMENTS")

    ui.heading("PART 1: the shape, and returning nothing")

    print("    def greet(name):")
    print('        return f"hello, {name}"')
    print()
    print("  No type on the parameter, no type on the return -- Python")
    print("  finds out at call time whether it works. A function with no")
    print("  return statement returns None, same as a C function you")
    print("  forgot to put a return in, except Python never warns you.")

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 2: the mutable default argument trap")

    print("  This is Python's single most infamous gotcha:")
    print()
    print("    def add_bad(item, items=[]):")
    print("        items.append(item)")
    print("        return items")
    print()
    print('    print(add_bad("a"))')
    print('    print(add_bad("b"))')
    print()

    def add_bad(item, items=[]):
        items.append(item)
        return items

    print(f'  Running: add_bad("a") -> {add_bad("a")}')
    print(f'           add_bad("b") -> {add_bad("b")}')
    print()
    print('  "b" should have started fresh. It did not -- the default []')
    print("  is created exactly ONCE, when the function is defined, not")
    print("  once per call. Every call that relies on the default shares")
    print("  the very same list.")
    print()
    print("  The fix is a sentinel and a check inside the function:")
    print()
    print("    def add_good(item, items=None):")
    print("        if items is None:")
    print("            items = []")
    print("        items.append(item)")
    print("        return items")
    print()

    def add_good(item, items=None):
        if items is None:
            items = []
        items.append(item)
        return items

    print(f'  Running: add_good("a") -> {add_good("a")}')
    print(f'           add_good("b") -> {add_good("b")}')
    print()
    print("  Rule: never use a mutable value (list, dict, set) as a")
    print("  default argument. None plus a check inside is the idiom.")

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 3: *args, **kwargs, and defaults that are not mutable")

    print("    def power(base, exp=2):")
    print("        return base ** exp")
    print()

    def power(base, exp=2):
        return base ** exp

    print(f"  Running: power(3) -> {power(3)}, power(3, 3) -> {power(3, 3)}")
    print()
    print("  A default that is a plain int, string or tuple has no trap --")
    print("  those are immutable, so there is nothing to share by accident.")
    print("  The rule only bites for lists, dicts and sets.")
    print()
    print("  *args collects extra positional arguments into a tuple,")
    print("  **kwargs collects extra keyword arguments into a dict:")
    print()
    print("    def total(*args):")
    print("        return sum(args)")
    print()

    def total(*args):
        return sum(args)

    print(f"  Running: total(1, 2, 3) -> {total(1, 2, 3)}")

    ui.wait_enter()
    ui.clear_screen()
    ui.exercise(7)

    ui.question(
        "def f(items=[]):   When is the empty list created?\n"
        "  (answer: once, when defined, or once per call)",
        "once, when defined",
        "Default argument values are evaluated once, at function-definition time, "
        "not on every call.",
    )
    ui.question(
        "What is the standard fix for a mutable default argument?",
        "items=None",
        "Default to None, then create the real mutable value inside the function "
        "if it was not passed.",
    )

    ui.challenge(
        [
            "Write power(base, exp=2) with a default exponent of 2.",
            "Print power(3) and power(3, 3) on the same line.",
        ],
        [],
        ["9 27"],
        [
            "def power(base, exp=2):",
            "    return base ** exp",
            "",
            "print(power(3), power(3, 3))",
        ],
    )

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("SUMMARY")

    print("   - a default value is computed once, at definition time")
    print("   - never default to a mutable value; default to None and build it inside")
    print("   - *args -> tuple of extras, **kwargs -> dict of extras")
    ui.wait_enter()


def lesson_08_dicts():
    ui.title("MODULE 8 - DICTIONARIES")

    ui.heading("PART 1: [] raises, .get() does not")

    grades = {"ana": 15, "bea": 18}
    print('    grades = {"ana": 15, "bea": 18}')
    print()
    print(f'    grades["ana"]          -> {grades["ana"]}')
    print(f'    grades.get("cal")      -> {grades.get("cal")}')
    print(f'    grades.get("cal", 0)   -> {grades.get("cal", 0)}')
    print()
    print("  [] raises KeyError for a missing key; .get() returns None, or")
    print("  a default you choose. Running the raise for real:")
    print()

    try:
        grades["cal"]
    except KeyError as e:
        print(f"    KeyError: {e}")

    print()
    print("  Prefer [] by default. A KeyError that stops the program beats")
    print("  a silent None that becomes a wrong number three lines later.")

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 2: order, looping, and building one up")

    grades["cal"] = 9
    print('    grades["cal"] = 9')
    print(f'    "cal" in grades    -> {"cal" in grades}')
    print()
    print("    for name, grade in grades.items():")
    print("        print(name, grade)")
    print()
    for name, grade in grades.items():
        print(f"    {name} {grade}")
    print()
    print("  Insertion order is kept -- a language guarantee since Python")
    print("  3.7, not an implementation accident. It is still not sorted;")
    print("  for that:")
    print()
    print("    sorted(grades.items(), key=lambda kv: kv[1], reverse=True)")
    print()
    print(f"  Running: {sorted(grades.items(), key=lambda kv: kv[1], reverse=True)}")

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 3: grouping with setdefault, and dict comprehensions")

    print("    d = {}")
    print('    d.setdefault("x", []).append(1)')
    print('    d.setdefault("x", []).append(2)')
    print()

    d = {}
    d.setdefault("x", []).append(1)
    d.setdefault("x", []).append(2)
    print(f"  Running: {d}")
    print()
    print("  setdefault inserts the default only if the key is missing,")
    print("  then returns whatever is now there -- so append always lands")
    print("  on a real list, first call or not.")
    print()
    print("    {k: v * 2 for k, v in grades.items()}")
    print(f"  Running: {({k: v * 2 for k, v in grades.items()})}")

    ui.wait_enter()
    ui.clear_screen()
    ui.exercise(8)

    ui.question(
        'd = {"a": 1}; d["b"]   What happens?', "KeyError", "[] on a missing key raises."
    )
    ui.question(
        'd.get("missing")   What does this return, with no default given?',
        "None",
        ".get() never raises; it returns None unless you give it a default.",
    )
    ui.question(
        "Since which Python version does a dict keep insertion order,\n"
        "  as a language guarantee?",
        "3.7",
        "Before 3.7 it was an implementation detail; 3.7 made it official.",
    )

    ui.challenge(
        [
            'Given grades = {"ana": 15, "bea": 18}, print the items',
            "sorted by value, highest first.",
        ],
        [],
        ["[('bea', 18), ('ana', 15)]"],
        [
            'grades = {"ana": 15, "bea": 18}',
            "print(sorted(grades.items(), key=lambda kv: kv[1], reverse=True))",
        ],
    )

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("SUMMARY")

    print("   - [] raises KeyError; .get(key, default) never does")
    print("   - insertion order is kept, since 3.7 -- still not sorted")
    print("   - setdefault(key, []).append(x) is the standard grouping idiom")
    ui.wait_enter()


def lesson_09_classes():
    ui.title("MODULE 9 - CLASSES")

    ui.heading("PART 1: __init__, self, and no real privacy")

    print("    class Student:")
    print("        def __init__(self, name, grade):")
    print("            self.name = name")
    print("            self.grade = grade")
    print()
    print("        def describe(self):")
    print('            return f"{self.name}: {self.grade}"')
    print()

    class Student:
        def __init__(self, name, grade):
            self.name = name
            self.grade = grade

        def describe(self):
            return f"{self.name}: {self.grade}"

    a = Student("Ana", 15)
    print(f"  Running: {a.describe()}")
    print()
    print("  self is the instance, and it is explicit everywhere -- every")
    print("  method takes it as its first parameter, spelled out, unlike")
    print("  C++'s implicit this. __init__ is the constructor; there is no")
    print("  matching destructor you can rely on the timing of (more on")
    print("  that in Part 3).")
    print()
    print("  There is no private keyword. This works, and nothing stops it:")
    print()

    a.grade = 20
    print(f"    a.grade = 20   # just works")
    print(f"    a.describe()   -> {a.describe()}")
    print()
    print('  A single leading underscore (self._grade) is a convention --')
    print('  "leave this alone" -- enforced by nobody. Python calls this')
    print('  "we are all consenting adults here."')

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 2: inheritance")

    print("    class Bolseiro(Student):")
    print("        def describe(self):")
    print('            return f"{super().describe()} (bolseiro)"')
    print()

    class Bolseiro(Student):
        def describe(self):
            return f"{super().describe()} (bolseiro)"

    b = Bolseiro("Bea", 18)
    print(f"  Running: {b.describe()}")
    print(f"  isinstance(b, Student) -> {isinstance(b, Student)}")
    print()
    print("  super().describe() calls the parent's version -- same idea as")
    print("  C++, different spelling: no :: needed, because Python looks")
    print("  the method up through the class hierarchy at call time.")

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 3: garbage collection, and why there is no RAII here")

    print("  C++ ties cleanup to scope: a destructor fires at the closing")
    print("  brace, deterministically, every time. Python does not make")
    print("  that promise. CPython happens to free an object the moment")
    print("  its reference count hits zero, which is often prompt -- but")
    print("  it is an implementation detail, not a language guarantee, and")
    print("  a __del__ method's exact timing should never be relied on.")
    print()
    print("  For anything that must close at a known point -- a file, a")
    print("  network connection -- Python's answer is not a destructor,")
    print("  it is a context manager:")
    print()
    print('    with open("data.txt") as f:')
    print("        contents = f.read()")
    print("    # the file is guaranteed closed here, even if read() raised")
    print()
    print("  with/as is the deterministic cleanup C++'s RAII gives you for")
    print("  free on every object -- in Python you opt in, per resource.")

    ui.wait_enter()
    ui.clear_screen()
    ui.exercise(9)

    ui.question(
        "In a method, is self written explicitly as the first\n"
        "  parameter, or implicit like C++'s this?  (answer: explicit or implicit)",
        "explicit",
        "Every method takes self as a real, visible first parameter.",
    )
    ui.question(
        "Does a leading underscore (self._grade) actually stop code\n"
        "  outside the class from reading it?  (answer: yes or no)",
        "no",
        "It is a naming convention only. Python has no enforced private access.",
    )
    ui.question(
        "Is a Python object guaranteed to be destroyed at a specific,\n"
        "  predictable moment, the way a C++ destructor is?  (answer: yes or no)",
        "no",
        "Garbage collection timing is an implementation detail. Use "
        "`with`/context managers for anything that must close on time.",
    )

    ui.challenge(
        [
            "Write a class Counter with count starting at 0, a method",
            "increment() that adds 1, and a method value() that returns",
            "the count. Create one, call increment() three times, and",
            "print value().",
        ],
        [],
        ["3"],
        [
            "class Counter:",
            "    def __init__(self):",
            "        self.count = 0",
            "",
            "    def increment(self):",
            "        self.count += 1",
            "",
            "    def value(self):",
            "        return self.count",
            "",
            "c = Counter()",
            "c.increment()",
            "c.increment()",
            "c.increment()",
            "print(c.value())",
        ],
    )

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("SUMMARY")

    print("   - self is explicit; __init__ is the constructor")
    print("   - no enforced privacy -- a leading _ is a convention, not a lock")
    print("   - no guaranteed destructor timing -- use `with` for deterministic cleanup")
    ui.wait_enter()


def lesson_10_exceptions():
    ui.title("MODULE 10 - EXCEPTIONS")

    ui.heading("PART 1: try / except / finally")

    print("    def dividir(a, b):")
    print("        if b == 0:")
    print('            raise ValueError("divisao por zero")')
    print("        return a / b")
    print()
    print("    try:")
    print("        print(dividir(10, 2))")
    print("        print(dividir(1, 0))")
    print("    except ValueError as e:")
    print('        print(f"apanhado: {e}")')
    print("    finally:")
    print('        print("sempre corre")')
    print()

    def dividir(a, b):
        if b == 0:
            raise ValueError("divisao por zero")
        return a / b

    print("  Running:")
    try:
        print(f"    {dividir(10, 2)}")
        print(f"    {dividir(1, 0)}")
    except ValueError as e:
        print(f"    apanhado: {e}")
    finally:
        print("    sempre corre")

    print()
    print("  The second dividir() never returns -- the raise jumps straight")
    print("  to except, so nothing after it in try runs. finally runs")
    print("  either way, including if you return from inside try.")

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 2: catch what you expect, not everything")

    print("    try:")
    print("        risky()")
    print("    except:          # catches EVERYTHING, including your own typos")
    print("        pass")
    print()
    print("  A bare except swallows KeyboardInterrupt, SystemExit, and any")
    print("  bug you did not anticipate, silently. If you must catch")
    print("  broadly, `except Exception:` at least leaves those two alone.")
    print()
    print("  Catching a specific type and re-raising what you did not")
    print("  expect is the right shape:")
    print()
    print("    try:")
    print('        raise TypeError("not mine")')
    print("    except ValueError:")
    print('        print("never reaches here")')
    print()

    try:
        try:
            raise TypeError("not mine")
        except ValueError:
            print("    never reaches here")
    except TypeError as e:
        print(f"  Running: uncaught by the inner except, caught outside instead: {e}")

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 3: the exceptions you will actually meet")

    print("    exception          raised by")
    ui.rule()
    print("    ValueError         int(\"abc\") -- right type, wrong value")
    print("    TypeError          \"1\" + 1 -- wrong type entirely")
    print("    KeyError           d[\"missing\"] on a dict")
    print("    IndexError         lst[99] past the end")
    print("    ZeroDivisionError  1 / 0")
    print("    AttributeError     None.foo -- often means something returned None")
    print()

    try:
        [1, 2, 3][10]
    except IndexError as e:
        print(f"  Running: {e}")

    print()
    print("  Raise your own with a real message -- the value that broke")
    print('  things, not just "invalid input":')
    print()
    print("    if age < 0:")
    print('        raise ValueError(f"age cannot be negative, got {age}")')

    ui.wait_enter()
    ui.clear_screen()
    ui.exercise(10)

    ui.question(
        "try: risky() finally: cleanup()  -- if risky() raises and is\n"
        "  never caught, does cleanup() still run?  (answer: yes or no)",
        "yes",
        "finally always runs, whether the try block succeeded, raised, or returned.",
    )
    ui.question("int(\"abc\")   Which exception does this raise?", "ValueError", "Right type of call, wrong value to convert.")
    ui.question(
        "except: (bare, no type named)  -- does this also catch\n"
        "  KeyboardInterrupt?  (answer: yes or no)",
        "yes",
        "A bare except catches everything. except Exception: at least leaves "
        "KeyboardInterrupt and SystemExit alone.",
    )

    ui.challenge(
        [
            "Write dividir(a, b) that raises ValueError on b == 0.",
            "Call it with (10, 2) then (1, 0) inside one try, catching",
            "the ValueError and printing its message, with a finally",
            "that always prints.",
        ],
        [],
        ["5.0", "apanhado: divisao por zero", "sempre corre"],
        [
            "def dividir(a, b):",
            "    if b == 0:",
            '        raise ValueError("divisao por zero")',
            "    return a / b",
            "",
            "try:",
            "    print(dividir(10, 2))",
            "    print(dividir(1, 0))",
            "except ValueError as e:",
            '    print(f"apanhado: {e}")',
            "finally:",
            '    print("sempre corre")',
        ],
    )

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("SUMMARY")

    print("   - raise to signal a problem; try/except to handle it; finally always runs")
    print("   - catch a specific type; a bare except hides bugs, including your own")
    print("   - ValueError, TypeError, KeyError, IndexError -- know what each one means")
    print()
    print("  That is the core course. What C needed manual memory for,")
    print("  and what C++ ties to scope, Python leaves to the garbage")
    print("  collector -- and hands you exceptions and `with` instead.")
    ui.wait_enter()
