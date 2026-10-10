"""
TITLE: Command-line options (argparse)
GROUP: Basics
USES: import argparse
SUMMARY: argparse reads --options and arguments, checks them, and writes the
  --help text for you.
NOTES:
  - add_argument("--no-logo", action="store_true") is a flag;
    add_argument("--width", type=int, default=80) takes a value.
  - parse_args() reads sys.argv[1:]. Pass a list to test: parse_args(["--width", "60"]).
  - Wrong input prints a usage message and exits with status 2 by itself.
  - Give every option a help= text: it becomes --help.
  - A system-information tool usually offers --json, --no-color and a list of
    fields to show.
SEE: pydoc argparse
"""

import argparse

parser = argparse.ArgumentParser(prog="minifetch", description="Show system information.")
parser.add_argument("--no-logo", action="store_true", help="leave out the logo")
parser.add_argument("--width", type=int, default=80, help="layout width in columns")
parser.add_argument("fields", nargs="*", help="fields to show (default: all)")

options = parser.parse_args(["--width", "60", "os", "kernel"])
print(options)
print("logo shown:", not options.no_logo)
parser.print_usage()
