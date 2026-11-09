"""
TITLE: math, statistics: numbers and rounding
GROUP: Standard library
USES: import math, statistics
SUMMARY: math has the floating-point functions; statistics has mean and
  median; divmod and // do integer division.
SIGNATURE:
  round(number, ndigits=None)
  divmod(a, b)
  math.floor(x), math.ceil(x), math.isclose(a, b)
  statistics.mean(data), statistics.median(data)
PARAMETERS:
  - number, x, a, b: the values.
  - ndigits: how many decimals to keep.
  - data: a sequence of numbers.
RETURNS: round: an int when ndigits is None. divmod: a tuple of quotient and
  remainder. mean, median: a number.
ERRORS:
  - ZeroDivisionError from division by zero.
  - ValueError from math.sqrt of a negative number.
  - statistics.StatisticsError for empty data.
NOTES:
  - 7 / 2 is 3.5 (always a float); 7 // 2 is 3; divmod(7, 2) is (3, 1).
  - round(2.5) is 2 and round(3.5) is 4: Python rounds halves to even.
    For "round half up" use math.floor(x + 0.5).
  - Python integers never overflow, so byte counts need no special type.
  - Compare floats with math.isclose(a, b), not ==.
  - Percentages for a display (memory used / total): divide as floats, round
    once at the end.
SEE: pydoc math, pydoc statistics
"""

import math
import statistics

used, total = 5368709120, 16106127360

print("sqrt(144)      =", math.sqrt(144), " isqrt(144) =", math.isqrt(144))
print("7 / 2, 7 // 2  =", 7 / 2, 7 // 2, " divmod(7, 2) =", divmod(7, 2))
print("round(2.5), round(3.5) =", round(2.5), round(3.5))
print("0.1 + 0.2 == 0.3:", 0.1 + 0.2 == 0.3, "  isclose:", math.isclose(0.1 + 0.2, 0.3))
print(f"memory used = {used / total * 100:.1f}% ({used / 2**30:.2f} GiB of {total / 2**30:.2f} GiB)")
print("median of [3, 1, 4, 1, 5]:", statistics.median([3, 1, 4, 1, 5]))
print("2 ** 100 =", 2 ** 100)
