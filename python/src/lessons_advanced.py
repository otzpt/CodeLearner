"""Module 11 onward - past the OOP/exceptions core.

Modules 1-10 cover one running program. Real Python is rarely one file:
this module is about the seam between files -- what a module actually is,
and the one line (`if __name__ == "__main__":`) that lets a file be both
a runnable script and something another file imports without side effects.
"""
import ui


def lesson_11_modules():
    ui.title("MODULE 11 - MODULES AND PACKAGES")

    ui.heading("PART 1: import")

    print("    import math")
    print("    print(math.sqrt(16))")
    print()

    import math
    print(f"  Running: {math.sqrt(16)}")

    print()
    print("  `math` is a module -- code grouped under one name. Every name")
    print("  inside it is reached through that name: math.sqrt, math.pi.")
    print()
    print("  Two other forms pull a name in directly, or rename the module:")
    print()
    print("    from math import sqrt")
    print("    print(sqrt(16))")
    print()
    print("    import math as m")
    print("    print(m.sqrt(16))")
    print()

    from math import sqrt
    import math as m
    print(f"  Running both: {sqrt(16)}, {m.sqrt(16)}")

    print()
    print("  `from math import sqrt` drops the `math.` prefix at every call")
    print("  site -- shorter, but a reader can no longer tell where sqrt")
    print("  came from just by looking at the call. `import math as m` is")
    print("  for a long name you'll type often, not for hiding where")
    print("  something is from.")

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 2: a module is a .py file, and this course proves it")

    print("  This course's own main.py starts with:")
    print()
    print("    import ui")
    print("    from lessons_basics import lesson_01_running, ...")
    print()
    print("  `ui` is not special syntax -- it is ui.py, imported the same")
    print("  way math was above. Every module object carries its own name")
    print("  as a real attribute:")
    print()
    print("    print(ui.__name__)")
    print()

    import lessons_basics
    print(f"  Running: ui.__name__ is {ui.__name__!r}, and")
    print(f"  lessons_basics.__name__ is {lessons_basics.__name__!r} --")
    print("  each module's own file name, without the .py, exactly as")
    print("  Python assigned it on import. That is about to matter.")

    ui.wait_enter()
    ui.clear_screen()
    ui.heading('PART 3: if __name__ == "__main__":')

    print("  __name__ is not always the file name. The file Python actually")
    print("  runs -- `python3 main.py` -- gets one special value no matter")
    print("  what it's called:")
    print()
    print(f"    Running right now, inside this program: __name__ is {__name__!r}")
    print()
    print("  This course's own main.py ends with exactly this guard (its")
    print('  real last lines, module 1 onward has been running under it):')
    print()
    print('    if __name__ == "__main__":')
    print("        main()")
    print()
    print("  Without the guard, importing main.py from anywhere else would")
    print("  immediately run the whole menu loop as a side effect of the")
    print("  import -- the guard is what lets a file be both a script you")
    print("  run and a module something else can safely import.")

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 4: packages")

    print("  A package is a directory of modules with an __init__.py in")
    print("  it (empty is fine) so Python treats the directory itself as")
    print("  importable:")
    print()
    print("    mypackage/")
    print("        __init__.py")
    print("        helpers.py")
    print()
    print("    from mypackage import helpers")
    print()
    print("  A real project also reaches for code nobody wrote in-house --")
    print("  `pip install requests`, inside a venv so one project's")
    print("  installed packages don't collide with another's. This course")
    print("  never does that itself (every module here runs with nothing")
    print("  but the Python you already have), but a real program almost")
    print("  always will.")

    ui.wait_enter()
    ui.clear_screen()
    ui.exercise(11)

    ui.question(
        "You write `from math import sqrt`. How do you call it\n"
        "  afterward -- math.sqrt(16) or sqrt(16)?",
        "sqrt(16)",
        "from-import pulls the name in directly; the math. prefix is gone.",
    )
    ui.question(
        'A file run directly (`python3 file.py`) has __name__ equal\n'
        "  to exactly what string?",
        "__main__",
        "Always that literal string for the file Python was actually told to run.",
    )
    ui.question(
        "That same file, instead imported from somewhere else\n"
        "  (`import file`), has __name__ equal to what?",
        "file",
        "Its own module name -- the file name without .py -- once it is on\n"
        "             the importing end instead of the running end.",
    )

    ui.challenge(
        [
            "Write two files. greetings.py defines greet(name), returning",
            'f"Hello, {name}!", guarded so running greetings.py directly',
            'prints greet("World") but importing it does not. use.py',
            'imports greet from greetings and prints greet("Ada").',
        ],
        [],
        [
            "$ python3 greetings.py",
            "Hello, World!",
            "",
            "$ python3 use.py",
            "Hello, Ada!",
        ],
        [
            "# greetings.py",
            "def greet(name):",
            '    return f"Hello, {name}!"',
            "",
            'if __name__ == "__main__":',
            '    print(greet("World"))',
            "",
            "# use.py",
            "from greetings import greet",
            "",
            'print(greet("Ada"))',
        ],
    )

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("SUMMARY")

    print("   - import brings a module in; from X import Y skips the X. prefix")
    print("   - every module carries its own name as __name__")
    print('   - __name__ == "__main__" is true only for the file actually run')
    print("   - a package is a directory with __init__.py; pip/venv is how")
    print("     a real project reaches for code it didn't write")
    print()
    print("  Module 12 is the other half of working across files: reading")
    print("  and writing them, and the `with` statement modules 6-10 have")
    print("  already been quietly pointing toward.")
    ui.wait_enter()
