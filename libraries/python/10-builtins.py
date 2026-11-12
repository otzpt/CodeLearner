"""
TITLE: builtins: functions that need no import
GROUP: Built-ins
LIBRARY: Python built-ins (no import needed)
SUMMARY: Functions that are always available. print() and open() are here,
  with the conversions (int, str), the collection helpers (len,
  sorted, sum) and the inspection ones (type, isinstance). Methods
  of str, list and dict have their own pages.
ENTRY: print(*args, sep=' ', end='\n', file=None, flush=False)
   Prints the values to a stream, or to sys.stdout by default.
ENTRY: input(prompt='', /)
   Read a string from standard input. The trailing newline is
   stripped.
ENTRY: open(file, mode='r', buffering=-1, encoding=None, errors=None,
    newline=None, closefd=True, opener=None)
   Open file and return a stream. Raise OSError upon failure.
ENTRY: len(obj, /)
   Return the number of items in a container.
ENTRY: range(stop) -> range object
   object range(start, stop[, step]) -> range object
ENTRY: enumerate(iterable, start=0)
   Return an enumerate object.
ENTRY: zip(*iterables, strict=False)
   The zip object yields n-length tuples, where n is the number of
   iterables passed as positional arguments to zip(). The i-th
   element in every tuple comes from the i-th iterable argument to
   zip(). This continues until the shortest argument is exhausted.
ENTRY: map(function, iterable, /, *iterables, strict=False)
   Make an iterator that computes the function using arguments from
   each of the iterables. Stops when the shortest iterable is
   exhausted.
ENTRY: filter(function, iterable, /)
   Return an iterator yielding those items of iterable for which
   function(item) is true. If function is None, return the items that
   are true.
ENTRY: sorted(iterable, /, *, key=None, reverse=False)
   Return a new list containing all items from the iterable in
   ascending order.
ENTRY: reversed(sequence, /)
   Return a reverse iterator over the values of the given sequence.
ENTRY: sum(iterable, /, start=0)
   Return the sum of a 'start' value (default: 0) plus an iterable of
   numbers
ENTRY: min(iterable, *[, default=obj, key=func]) -> value
   min(arg1, arg2, *args, *[, key=func]) -> value
ENTRY: max(iterable, *[, default=obj, key=func]) -> value
   max(arg1, arg2, *args, *[, key=func]) -> value
ENTRY: abs(x, /)
   Return the absolute value of the argument.
ENTRY: round(number, ndigits=None)
   Round a number to a given precision in decimal digits.
ENTRY: divmod(x, y, /)
   Return the tuple (x//y, x%y). Invariant: div*y + mod == x.
ENTRY: pow(base, exp, mod=None)
   Equivalent to base**exp with 2 arguments or base**exp % mod with 3
   arguments
ENTRY: int([x]) -> integer
   int(x, base=10) -> integer
ENTRY: float(x=0, /)
   Convert a string or number to a floating-point number, if
   possible.
ENTRY: str(object='') -> str
   str(bytes_or_buffer[, encoding[, errors]]) -> str
ENTRY: bool(object=False, /)
   Returns True when the argument is true, False otherwise. The
   builtins True and False are the only two instances of the class
   bool. The class bool is a subclass of the class int, and cannot be
   subclassed.
ENTRY: list(iterable=(), /)
   Built-in mutable sequence.
ENTRY: dict() -> new empty dictionary
   empty dictionary dict(mapping) -> new dictionary initialized from
   a mapping object's (key, value) pairs dict(iterable) -> new
   dictionary initialized as if via: d = {} for k, v in iterable:
   d[k] = v dict(**kwargs) -> new dictionary initialized with the
   name=value pairs in the keyword argument list. For example:
   dict(one=1, two=2)
ENTRY: tuple(iterable=(), /)
   Built-in immutable sequence.
ENTRY: set(iterable=(), /)
   Build an unordered collection of unique elements.
ENTRY: isinstance(obj, class_or_tuple, /)
   Return whether an object is an instance of a class or of a
   subclass thereof.
ENTRY: type(object) -> the object's type
   object's type type(name, bases, dict, **kwds) -> a new type
ENTRY: repr(obj, /)
   Return the canonical string representation of the object.
ENTRY: format(value, format_spec='', /)
   Return type(value).__format__(value, format_spec)
ENTRY: iter(iterable) -> iterator
   iter(callable, sentinel) -> iterator
ENTRY: next(iterator[, default])
   (no description in the module's docstring)
ENTRY: any(iterable, /)
   Return True if bool(x) is True for any x in the iterable.
ENTRY: all(iterable, /)
   Return True if bool(x) is True for all values x in the iterable.
ENTRY: hasattr(obj, name, /)
   Return whether the object has an attribute with the given name.
ENTRY: getattr(object, name[, default]) -> value
   (no description in the module's docstring)
NOTES:
  - print() writes to sys.stdout and adds a newline;
    file=sys.stderr keeps messages out of piped output.
  - round() rounds halves to the nearest even number: round(2.5)
    is 2 and round(3.5) is 4.
  - A float that cannot be stored exactly rounds as stored:
    round(2.675, 2) gives 2.67, not 2.68.
  - For formatted output see the format page.
  - Signatures and descriptions are Python's own, read from the
    module's docstrings for the version that runs this page.
    Version notes (added, changed, deprecated) are in the online
    documentation.
SEE: https://docs.python.org/3/library/functions.html, pydoc builtins
"""

import sys

print("a", "b", "c", sep="-", end="!\n")
print("to stderr", file=sys.stderr)
print("int('42') + 1 =", int("42") + 1, "  float('2.5') =", float("2.5"))
print("sorted([3, 1, 2], reverse=True) =", sorted([3, 1, 2], reverse=True))
print("sum / min / max:", sum([1, 2, 3]), min([4, 2, 9]), max("pear", "fig", key=len))
print("list(enumerate('ab', start=1)) =", list(enumerate("ab", start=1)))
print("list(zip('ab', [1, 2])) =", list(zip("ab", [1, 2])))
print("round(2.5), round(3.5), round(2.675, 2) =", round(2.5), round(3.5), round(2.675, 2))
print("isinstance(True, int):", isinstance(True, int), "  type(3.0):", type(3.0).__name__)
print("any([]) / all([]):", any([]), all([]))
