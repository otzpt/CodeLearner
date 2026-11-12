"""
TITLE: argparse: command-line options
GROUP: Text and data
LIBRARY: Python standard library (argparse)
USES: import argparse
SUMMARY: Declares a program's options and positional arguments, parses
  sys.argv, and writes --help and error messages for you. Exits with
  status 2 on a bad command line.
ENTRY: argparse.ArgumentParser(prog=None, usage=None, description=None,
    epilog=None, parents=[], formatter_class=<class
    'argparse.HelpFormatter'>, prefix_chars='-',
    fromfile_prefix_chars=None, argument_default=None,
    conflict_handler='error', add_help=True, allow_abbrev=True,
    exit_on_error=True, *, suggest_on_error=False, color=True)
   Object for parsing command line strings into Python objects.
ENTRY: argparse.ArgumentParser.add_argument(self, *args, **kwargs)
   add_argument(option_string, option_string, ..., name=value, ...)
ENTRY: argparse.ArgumentParser.parse_args(self, args=None, namespace=None)
   (no description in the module's docstring)
ENTRY: argparse.ArgumentParser.print_usage(self, file=None)
   (no description in the module's docstring)
ENTRY: argparse.ArgumentParser.print_help(self, file=None)
   (no description in the module's docstring)
ENTRY: argparse.ArgumentParser.add_subparsers(self, **kwargs)
   (no description in the module's docstring)
ENTRY: argparse.Namespace(**kwargs)
   Simple object for storing attributes.
ENTRY: argparse.ArgumentError(argument, message)
   An error from creating or using an argument (optional or
   positional).
NOTES:
  - parse_args() with no argument reads sys.argv[1:]; pass a list
    to test your parser.
  - type=int converts and rejects bad values; choices=[...]
    restricts them; default= is used when the option is absent.
  - action='store_true' makes a flag: False unless given.
  - On a bad command line parse_args() prints usage to stderr and
    raises SystemExit(2).
  - Signatures and descriptions are Python's own, read from the
    module's docstrings for the version that runs this page.
    Version notes (added, changed, deprecated) are in the online
    documentation.
SEE: https://docs.python.org/3/library/argparse.html, pydoc argparse
"""

import argparse

parser = argparse.ArgumentParser(prog="count", description="Count lines in files.")
parser.add_argument("--quiet", "-q", action="store_true", help="print totals only")
parser.add_argument("--width", type=int, default=80, help="output width in columns")
parser.add_argument("files", nargs="*", help="files to read (default: stdin)")

options = parser.parse_args(["--width", "60", "-q", "a.txt", "b.txt"])
print(options)
print("quiet:", options.quiet, " width + 1 =", options.width + 1)
parser.print_usage()

try:
    parser.parse_args(["--width", "wide"])
except SystemExit as error:
    print("a bad value exits with status", error.code, "(the message went to stderr)")
