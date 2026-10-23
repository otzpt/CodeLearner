#!/usr/bin/env python3
"""Security course - main menu.

Written in Python, the same shape as the Python course's main.py: a list
of module dicts, each with a title, the function that runs it, and the
tier it is filed under. The menu numbers itself; adding a module is one
row here plus the function it points at.

This course teaches security, not Python, so it is implemented in Python
rather than "written in the language it teaches" the way the language
courses are. Everything else follows the same rules -- standard library
only, nothing claimed without being shown, teach the trap not the syntax.

Run:  python3 main.py
"""

import sys

import ui
from lessons_basics import lesson_01_first_request

# "tier" files a module under one of the three menu bands, display grouping
# only -- the lesson functions never see it. Only the BASIC band has a
# module so far; INTERMEDIATE and ADVANCED are printed empty, the way the
# GUI course does, so the arc the course is headed toward stays visible.
MODULES = [
    {"title": "Your first request", "run": lesson_01_first_request, "tier": "BASIC"},
]

TIERS = ["BASIC", "INTERMEDIATE", "ADVANCED"]


def show_menu():
    ui.clear_screen()
    ui.title("SECURITY WITH PYTHON")

    for tier in TIERS:
        print(f"\n  -- {tier} --")
        for i, module in enumerate(MODULES, start=1):
            if module["tier"] == tier:
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
