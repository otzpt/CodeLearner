"""
TITLE: exceptions: built-in exception types
GROUP: Built-ins
LIBRARY: Python built-ins (no import needed)
SUMMARY: Errors are raised as exceptions. try/except catches them by type,
  most specific first. OSError and its subclasses cover failures of
  files, processes and the network.
ENTRY: BaseException
   Common base class for all exceptions
ENTRY: Exception
   Common base class for all non-exit exceptions.
ENTRY: OSError
   Base class for I/O related errors.
ENTRY: FileNotFoundError
   File not found.
ENTRY: PermissionError
   Not enough permissions.
ENTRY: IsADirectoryError
   Operation doesn't work on directories.
ENTRY: NotADirectoryError
   Operation only works on directories.
ENTRY: FileExistsError
   File already exists.
ENTRY: TimeoutError
   Timeout expired.
ENTRY: ChildProcessError
   Child process error.
ENTRY: BrokenPipeError
   Broken pipe.
ENTRY: ConnectionRefusedError
   Connection refused.
ENTRY: ValueError
   Inappropriate argument value (of correct type).
ENTRY: TypeError
   Inappropriate argument type.
ENTRY: KeyError
   Mapping key not found.
ENTRY: IndexError
   Sequence index out of range.
ENTRY: AttributeError
   Attribute not found.
ENTRY: ZeroDivisionError
   Second argument to a division or modulo operation was zero.
ENTRY: UnicodeDecodeError
   Unicode decoding error.
ENTRY: RecursionError
   Recursion limit exceeded.
ENTRY: KeyboardInterrupt
   Program interrupted by user.
ENTRY: SystemExit
   Request to exit from the interpreter.
ENTRY: StopIteration
   Signal the end from iterator.__next__().
NOTES:
  - Catch the narrowest type that makes sense. `except Exception`
    hides bugs; a bare `except:` also swallows KeyboardInterrupt.
  - OSError carries errno and strerror; its subclasses
    (FileNotFoundError, ...) are chosen from errno automatically.
  - KeyboardInterrupt and SystemExit derive from BaseException,
    not Exception, so `except Exception` lets Ctrl-C through.
  - Signatures and descriptions are Python's own, read from the
    module's docstrings for the version that runs this page.
    Version notes (added, changed, deprecated) are in the online
    documentation.
SEE: https://docs.python.org/3/library/exceptions.html, pydoc exceptions
"""

import errno

for path in ("/no/such/file", "/root/.hidden", "/proc"):
    try:
        with open(path, encoding="utf-8") as file:
            file.read(1)
    except FileNotFoundError as error:
        print(f"{path:<14} FileNotFoundError, errno {error.errno} ({errno.errorcode[error.errno]}): {error.strerror}")
    except PermissionError as error:
        print(f"{path:<14} PermissionError, errno {error.errno}: {error.strerror}")
    except OSError as error:
        print(f"{path:<14} {type(error).__name__}, errno {error.errno}: {error.strerror}")

for action in (lambda: int("x"), lambda: 1 / 0, lambda: {}["k"], lambda: [][0], lambda: None.name):
    try:
        action()
    except Exception as error:          # catching Exception is for a demo, not for real code
        print(f"{type(error).__name__:<18} {error}")
print("is FileNotFoundError an OSError?", issubclass(FileNotFoundError, OSError))
