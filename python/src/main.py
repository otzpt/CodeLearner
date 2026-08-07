#!/usr/bin/env python3
"""Python course - main menu.

Written in Python, the same way the C and C++ courses are written in the
language they teach: the source is itself worked example code. This file
uses a list of dicts and closures held as values, both taught properly in
module 7.

Run:  python3 main.py
"""

import sys

import ui
from lessons_basics import (
    lesson_01_running,
    lesson_02_variables,
    lesson_03_input,
    lesson_04_conditions,
    lesson_05_loops,
)
from lessons_more import (
    lesson_06_lists,
    lesson_07_functions,
    lesson_08_dicts,
    lesson_09_classes,
    lesson_10_exceptions,
)

MODULES = [
    {"title": "Running Python and print()", "run": lesson_01_running},
    {"title": "Variables and types", "run": lesson_02_variables},
    {"title": "Reading input", "run": lesson_03_input},
    {"title": "Conditions", "run": lesson_04_conditions},
    {"title": "Loops", "run": lesson_05_loops},
    {"title": "Lists", "run": lesson_06_lists},
    {"title": "Functions and default arguments", "run": lesson_07_functions},
    {"title": "Dictionaries", "run": lesson_08_dicts},
    {"title": "Classes", "run": lesson_09_classes},
    {"title": "Exceptions", "run": lesson_10_exceptions},
]


def show_menu():
    ui.clear_screen()
    ui.title("PYTHON COURSE - FROM ZERO TO CLASSES")

    for i, module in enumerate(MODULES, start=1):
        print(f"   [{i:2d}]  {module['title']}")
    print("\n   [ 0]  Quit")
    ui.rule()


def main():
    while True:
        show_menu()
        try:
            choice = input("\n  Pick a module: ").strip()
        except EOFError:
            break

        if choice == "0":
            break

        try:
            n = int(choice)
        except ValueError:
            n = 0

        if 1 <= n <= len(MODULES):
            ui.clear_screen()
            MODULES[n - 1]["run"]()
        else:
            print("\n  Not a valid option.")
            ui.wait_enter()

    print("\n  See you next time.\n")


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("\n  See you next time.\n")
        sys.exit(0)
