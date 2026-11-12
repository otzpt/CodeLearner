"""
TITLE: math: mathematical functions
GROUP: Numbers and time
LIBRARY: Python standard library (math)
USES: import math
SUMMARY: Floating-point mathematics from the C library: roots, powers and
  logarithms, trigonometry, rounding to integers, and number theory.
  Functions take and return floats unless noted; for complex numbers
  see cmath.
ENTRY: math.pi = 3.141592653589793
   The mathematical constant pi, to the precision of a float.
ENTRY: math.e = 2.718281828459045
   The mathematical constant e, the base of natural logarithms.
ENTRY: math.tau = 6.283185307179586
   The constant tau, 2 * pi.
ENTRY: math.inf = inf
   A floating-point positive infinity. -math.inf is negative
   infinity.
ENTRY: math.nan = nan
   A floating-point 'not a number' value. It is never equal to
   anything, itself included: test with math.isnan().
ENTRY: math.sqrt(x, /)
   Return the square root of x.
ENTRY: math.isqrt(n, /)
   Return the integer part of the square root of the input.
ENTRY: math.floor(x, /)
   Return the floor of x as an Integral.
ENTRY: math.ceil(x, /)
   Return the ceiling of x as an Integral.
ENTRY: math.trunc(x, /)
   Truncates the Real x to the nearest Integral toward 0.
ENTRY: math.fabs(x, /)
   Return the absolute value of the float x.
ENTRY: math.pow(x, y, /)
   Return x**y (x to the power of y).
ENTRY: math.exp(x, /)
   Return e raised to the power of x.
ENTRY: math.log(x, [base=math.e])
   Return the logarithm of x to the given base.
ENTRY: math.log2(x, /)
   Return the base 2 logarithm of x.
ENTRY: math.log10(x, /)
   Return the base 10 logarithm of x.
ENTRY: math.sin(x, /)
   Return the sine of x (measured in radians).
ENTRY: math.cos(x, /)
   Return the cosine of x (measured in radians).
ENTRY: math.tan(x, /)
   Return the tangent of x (measured in radians).
ENTRY: math.atan2(y, x, /)
   Return the arc tangent (measured in radians) of y/x.
ENTRY: math.hypot(*coordinates)
   Multidimensional Euclidean distance from the origin to a point.
ENTRY: math.degrees(x, /)
   Convert angle x from radians to degrees.
ENTRY: math.radians(x, /)
   Convert angle x from degrees to radians.
ENTRY: math.gcd(*integers)
   Greatest Common Divisor.
ENTRY: math.lcm(*integers)
   Least Common Multiple.
ENTRY: math.factorial(n, /)
   Find n!.
ENTRY: math.comb(n, k, /)
   Number of ways to choose k items from n items without repetition
   and without order.
ENTRY: math.perm(n, k=None, /)
   Number of ways to choose k items from n items without repetition
   and with order.
ENTRY: math.isclose(a, b, *, rel_tol=1e-09, abs_tol=0.0)
   Determine whether two floating-point numbers are close in value.
ENTRY: math.isnan(x, /)
   Return True if x is a NaN (not a number), and False otherwise.
ENTRY: math.isinf(x, /)
   Return True if x is a positive or negative infinity, and False
   otherwise.
ENTRY: math.fsum(seq, /)
   Return an accurate floating-point sum of values in the iterable
   seq.
ENTRY: math.prod(iterable, /, *, start=1)
   Calculate the product of all the elements in the input iterable.
NOTES:
  - Never compare floats with ==: use math.isclose(a, b). 0.1 +
    0.2 is not 0.3.
  - round() rounds halves to even; floor() rounds toward minus
    infinity, trunc() toward zero.
  - math.sqrt(-1) raises ValueError; use cmath.sqrt for complex
    results.
  - Integers have no overflow in Python: 2 ** 100 is exact.
  - Signatures and descriptions are Python's own, read from the
    module's docstrings for the version that runs this page.
    Version notes (added, changed, deprecated) are in the online
    documentation.
SEE: https://docs.python.org/3/library/math.html, pydoc math
"""

import math

used, total = 5368709120, 16106127360
print("sqrt(144)      =", math.sqrt(144), " isqrt(144) =", math.isqrt(144))
print("floor/ceil/trunc of -2.5:", math.floor(-2.5), math.ceil(-2.5), math.trunc(-2.5))
print("7 / 2, 7 // 2, divmod(7, 2) =", 7 / 2, 7 // 2, divmod(7, 2))
print("round(2.5), round(3.5) =", round(2.5), round(3.5), " (halves go to the even number)")
print("0.1 + 0.2 == 0.3:", 0.1 + 0.2 == 0.3, "  isclose:", math.isclose(0.1 + 0.2, 0.3))
print("fsum([0.1] * 10) =", math.fsum([0.1] * 10), " sum([0.1] * 10) =", sum([0.1] * 10))
print("gcd, lcm, factorial(5), comb(5, 2):", math.gcd(12, 18), math.lcm(4, 6), math.factorial(5), math.comb(5, 2))
print("log2(1024), log10(1000), log(math.e):", math.log2(1024), math.log10(1000), math.log(math.e))
print("hypot(3, 4) =", math.hypot(3, 4), " degrees(pi) =", math.degrees(math.pi))
print("nan == nan:", math.nan == math.nan, " isnan:", math.isnan(math.nan), " inf > 10**100:", math.inf > 10**100)
print(f"memory used = {used / total * 100:.1f}% ({used / 2**30:.2f} GiB of {total / 2**30:.2f} GiB)")
try:
    math.sqrt(-1)
except ValueError as error:
    print("sqrt(-1) raises ValueError:", error)
