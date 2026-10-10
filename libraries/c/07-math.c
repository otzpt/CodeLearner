/*
 * TITLE: Maths: sqrt, pow, floor, rounding
 * GROUP: Basics
 * USES: #include <math.h>
 * LIBS: -lm
 * SUMMARY: math.h has the floating-point functions. On Linux they live in
 *   a separate library, libm, so the link needs -lm.
 * NOTES:
 *   - Forget -lm and the link fails with "undefined reference to `sqrt'"
 *     (the compiler sees the header, the linker cannot find the code).
 *     -lm goes AFTER the source file on the command line.
 *   - Integer / integer is an integer: 7 / 2 is 3. Make one side a double
 *     first: 7 / 2.0 is 3.5.
 *   - round() rounds half away from zero; floor() always goes down.
 *   - Compare doubles with a tolerance (fabs(a - b) < 1e-9), never with ==.
 *   - Percentages for a display (memory used / total): do the division in
 *     double, then round once at the end.
 * SEE: man 3 sqrt, man 7 math_error
 */

#include <math.h>
#include <stdio.h>

int main(void)
{
    double used = 5368709120.0;   /* bytes */
    double total = 16106127360.0;

    printf("sqrt(144)        = %.1f\n", sqrt(144.0));
    printf("pow(2, 10)       = %.0f\n", pow(2.0, 10.0));
    printf("floor(2.7)       = %.0f, round(2.5) = %.0f, ceil(2.1) = %.0f\n",
           floor(2.7), round(2.5), ceil(2.1));
    printf("7 / 2            = %d, 7 / 2.0 = %.1f\n", 7 / 2, 7 / 2.0);
    printf("memory used      = %.1f%% (%.2f GiB of %.2f GiB)\n",
           used / total * 100.0, used / 1073741824.0, total / 1073741824.0);
    return 0;
}
