"""ui.py - the pieces every lesson uses to draw the screen.

Same purpose and the same visual style as the other courses' ui modules,
kept as a separate implementation rather than shared code. MicroPython has
no str.ljust and no os.name, so the padding is done with a format spec and
the screen is cleared with ANSI codes only.
"""

import sys

from strings_pt import print_summary_pt, tr  # noqa: F401

WIDTH = 54  # inside width of the frame, matching the other courses


def clear_screen():
    sys.stdout.write("\033[H\033[2J\033[3J")


def wait_enter():
    input("\n  " + tr("Press ENTER to continue..."))


def rule():
    print("  " + "-" * WIDTH)


def _padded_line(text, border):
    print("  " + border + " " + "{:<{}}".format(text, WIDTH - 3) + border)


def _frame(fill):
    print("  +" + fill * (WIDTH - 2) + "+")


def title(text):
    print()
    _frame("=")
    _padded_line(text, "|")
    _frame("=")
    print()


def heading(text):
    print("\n  " + tr(text))
    rule()


def ask_yes(question_text):
    answer = input("\n  " + question_text + " (y/N): ")
    return answer[:1].lower() == "y"


def exercise(number):
    print("\n  " + tr(">> EXERCISE - MODULE") + " " + str(number))
    rule()


def question(text, correct, why):
    print("\n  " + text)
    answer = input("  " + tr("Your answer:") + " ")

    right = answer.strip().lower() == correct.strip().lower()
    if right:
        print("\n  " + tr("CORRECT.") + "  " + why)
    else:
        print("\n  " + tr("NOT QUITE. The answer is:") + " " + correct)
        print("             " + why)
    return right


def challenge(task, input_lines, expected, solution):
    """A task to write in a real file.

    `input_lines` is every line the program will read while producing
    `expected` -- empty for a task that reads nothing. Together they are the
    actual specification: run the solution, type `input_lines`, get
    `expected`, however the code that does it is written. `solution` is one
    way of getting there and appears only after a confirmation.
    """
    print("\n  " + tr(">> WRITE THIS YOURSELF, in a real file"))
    rule()

    for line in task:
        print("  " + line)

    if input_lines:
        print("\n  " + tr("Try it with this input:") + "\n")
        for line in input_lines:
            print("      " + line)

    if expected:
        print("\n  " + tr("It must print:") + "\n")
        for line in expected:
            print("      " + line)
        print("\n  " + tr("That output is the whole specification. Any code that"))
        print("  " + tr("produces it is correct."))

    print("\n  " + tr("Try it first. Run with:"))
    print("    micropython test.py")

    if not ask_yes(tr("Want to see example code?")):
        return

    print()
    rule()
    for line in solution:
        print("  " + line)
    rule()
    print("  " + tr("This is EXAMPLE CODE, not the answer. It is one way to get"))
    print("  " + tr("that output; yours may look nothing like it and still be"))
    print("  " + tr("right -- or better. Compare the output, not the code."))
