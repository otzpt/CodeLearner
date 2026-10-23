"""
TITLE: argparse: command-line options
GROUP: Standard library
USES: import argparse
SUMMARY: argparse reads --options and arguments, checks them, and writes the
  --help text for you.
NOTES:
  - add_argument("--quiet", action="store_true") is a flag;
    add_argument("--width", type=int, default=80) takes a value.
  - parse_args() reads sys.argv[1:]. Pass a list to test: parse_args(["--width", "60"]).
  - Wrong input prints a usage message and exits with status 2 by itself.
  - Give every option a help= text: it becomes --help.
  - Common options: --json for machine-readable output, --no-color, --quiet.
  - parse_known_args() returns the arguments it did not recognise instead
    of exiting, for programs that pass them on to another tool.
SEE: pydoc argparse
"""

import argparse

parser = argparse.ArgumentParser(prog="count", description="Count lines in files.")
parser.add_argument("--quiet", action="store_true", help="print totals only")
parser.add_argument("--width", type=int, default=80, help="output width in columns")
parser.add_argument("files", nargs="*", help="files to read (default: stdin)")

options = parser.parse_args(["--width", "60", "a.txt", "b.txt"])
print(options)
print("quiet:", options.quiet)
parser.print_usage()
