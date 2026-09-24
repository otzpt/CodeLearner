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
from lessons_advanced import lesson_11_modules
from lessons_io import lesson_12_files, lesson_13_apis

# "tier" files a module under one of the three menu bands. Display
# grouping only -- the lesson functions never see it.
MODULES = [
    {"title": "Running Python and print()", "run": lesson_01_running, "tier": "BASIC"},
    {"title": "Variables and types", "run": lesson_02_variables, "tier": "BASIC"},
    {"title": "Reading input", "run": lesson_03_input, "tier": "BASIC"},
    {"title": "Conditions", "run": lesson_04_conditions, "tier": "BASIC"},
    {"title": "Loops", "run": lesson_05_loops, "tier": "BASIC"},
    {"title": "Lists", "run": lesson_06_lists, "tier": "INTERMEDIATE"},
    {"title": "Functions and default arguments", "run": lesson_07_functions, "tier": "INTERMEDIATE"},
    {"title": "Dictionaries", "run": lesson_08_dicts, "tier": "INTERMEDIATE"},
    {"title": "Classes", "run": lesson_09_classes, "tier": "ADVANCED"},
    {"title": "Exceptions", "run": lesson_10_exceptions, "tier": "ADVANCED"},
    {"title": "Modules and packages", "run": lesson_11_modules, "tier": "ADVANCED"},
    {"title": "Files and context managers", "run": lesson_12_files, "tier": "ADVANCED"},
    {"title": "Working with APIs", "run": lesson_13_apis, "tier": "ADVANCED"},
]


def show_menu():
    ui.clear_screen()
    ui.title("PYTHON COURSE - FROM ZERO TO CLASSES")

    shown_tier = None
    for i, module in enumerate(MODULES, start=1):
        if module["tier"] != shown_tier:
            shown_tier = module["tier"]
            print(f"\n  -- {shown_tier} --")
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
            # A module that lets an exception escape should not end the
            # session with a raw traceback -- every other exit path here
            # prints something and returns to this menu.
            try:
                MODULES[n - 1]["run"]()
            except Exception as e:
                print(f"\n  Something went wrong: {e}")
                ui.wait_enter()
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
