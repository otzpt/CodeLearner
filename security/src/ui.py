"""ui.py - the pieces every lesson uses to draw the screen.

Same purpose and the same visual style as the C and C++ courses' ui
modules, kept as a separate implementation rather than shared code -- see
docs/writing-a-course.md for why. No compile step here, so this file is
also the simplest of the three: nothing to link, no headers.
"""

import os
import sys

WIDTH = 54  # inside width of the frame, matching the other two courses


def clear_screen():
    # Python has no compiled-in system() split by platform the way C does,
    # but the same TERM problem applies to os.system("clear") on Linux, so
    # the fix is the same: ANSI codes, not a spawned process.
    if os.name == "nt":
        os.system("cls")
    else:
        sys.stdout.write("\033[H\033[2J\033[3J")
        sys.stdout.flush()


def wait_enter():
    input("\n  Press ENTER to continue...")


def rule():
    print("  " + "-" * WIDTH)


def _padded_line(text, border):
    print(f"  {border} " + text.ljust(WIDTH - 3) + border)


def _frame(fill):
    print("  +" + fill * (WIDTH - 2) + "+")


def title(text):
    print()
    _frame("=")
    _padded_line(text, "|")
    _frame("=")
    print()


def heading(text):
    print(f"\n  {text}")
    rule()


def ask_yes(question_text):
    answer = input(f"\n  {question_text} (y/N): ")
    return answer[:1].lower() == "y"


def exercise(number):
    print(f"\n  >> EXERCISE - MODULE {number}")
    rule()


def question(text, correct, why):
    print(f"\n  {text}")
    answer = input("  Your answer: ")

    right = answer.strip().lower() == correct.strip().lower()
    if right:
        print(f"\n  CORRECT.  {why}")
    else:
        print(f"\n  NOT QUITE. The answer is: {correct}")
        print(f"             {why}")
    return right


def challenge(task, input_lines, expected, solution):
    """A task to write in a real file.

    `input_lines` is every line the program will read while producing
    `expected` -- empty for a task that reads nothing. Together they are the
    actual specification: run the solution, type `input_lines`, get
    `expected`, however the code that does it is written. `solution` is one
    way of getting there and appears only after a confirmation.
    """
    print("\n  >> WRITE THIS YOURSELF, in a real file")
    rule()

    for line in task:
        print(f"  {line}")

    if input_lines:
        print("\n  Try it with this input:\n")
        for line in input_lines:
            print(f"      {line}")

    if expected:
        print("\n  It must print:\n")
        for line in expected:
            print(f"      {line}")
        print("\n  That output is the whole specification. Any code that")
        print("  produces it is correct.")

    print("\n  Try it first. Run with:")
    print("    python3 test.py")

    if not ask_yes("Want to see example code?"):
        return

    print()
    rule()
    for line in solution:
        print(f"  {line}")
    rule()
    print("  This is EXAMPLE CODE, not the answer. It is one way to get")
    print("  that output; yours may look nothing like it and still be")
    print("  right -- or better. Compare the output, not the code.")
