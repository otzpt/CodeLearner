#!/usr/bin/env micropython
"""MicroPython course - main menu.

Written in MicroPython, the same way the other courses are written in the
language they teach: this file uses a list of dicts holding functions as
values, taught in modules 3 and 4.

The unix port that runs this has no GPIO, so sim.py plays the part of
machine.Pin, PWM, ADC and the clock. sim.install() must run BEFORE the lesson
files are imported, because they do `from machine import Pin` at the top.

Run:  micropython main.py
"""

import sys

import sim

sim.install()

import ui
from lessons_basics import (
    lesson_01_running,
    lesson_02_numbers,
    lesson_03_flow,
    lesson_04_memory,
)
from lessons_more import (
    lesson_05_pins,
    lesson_06_pwm_adc,
    lesson_07_time,
    lesson_08_interrupts,
)
from lessons_advanced import (
    lesson_09_speed,
    lesson_10_assembly,
    lesson_11_robot,
)

# "tier" files a module under one of the three menu bands. Display
# grouping only -- the lesson functions never see it.
MODULES = [
    {"title": "Running MicroPython and print()", "run": lesson_01_running, "tier": "BASIC"},
    {"title": "Numbers, text and variables", "run": lesson_02_numbers, "tier": "BASIC"},
    {"title": "Decisions, loops and functions", "run": lesson_03_flow, "tier": "BASIC"},
    {"title": "Lists, bytes and memory", "run": lesson_04_memory, "tier": "BASIC"},
    {"title": "Pins: machine.Pin", "run": lesson_05_pins, "tier": "INTERMEDIATE"},
    {"title": "ADC and PWM", "run": lesson_06_pwm_adc, "tier": "INTERMEDIATE"},
    {"title": "Time without freezing", "run": lesson_07_time, "tier": "INTERMEDIATE"},
    {"title": "Interrupts", "run": lesson_08_interrupts, "tier": "INTERMEDIATE"},
    {"title": "Speed: native and viper", "run": lesson_09_speed, "tier": "ADVANCED"},
    {"title": "Inline assembly on the Pico", "run": lesson_10_assembly, "tier": "ADVANCED"},
    {"title": "Final project: an obstacle robot", "run": lesson_11_robot, "tier": "ADVANCED"},
]


def show_menu():
    ui.clear_screen()
    ui.title("MICROPYTHON COURSE - FROM print() TO THE PICO")

    shown_tier = None
    for number, module in enumerate(MODULES, 1):
        if module["tier"] != shown_tier:
            shown_tier = module["tier"]
            print("\n  -- " + shown_tier + " --")
        print("   [{:2d}]  {}".format(number, module["title"]))
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
            number = int(choice)
        except ValueError:
            number = 0

        if 1 <= number <= len(MODULES):
            ui.clear_screen()
            try:
                MODULES[number - 1]["run"]()
            except EOFError:
                break
        else:
            print("\n  Not a valid option.")
            try:
                ui.wait_enter()
            except EOFError:
                break

    print("\n  See you next time.\n")


main()
