/*
 * Module 19 - function pointers, callbacks, and macros.
 *
 * A set of things C does with functions and text that look like magic
 * until the mechanism behind them is visible: a function's own address
 * stored in a variable, a function taking another function as an argument
 * so it can call back into caller-supplied behaviour, a function that
 * takes a different number of arguments each time it is called, and the
 * preprocessor's own text substitution running before the compiler ever
 * sees C at all. Bit manipulation closes it out -- unrelated in theory,
 * but it is the same kind of thing: an operator this course has not
 * needed until now, working directly on the bit pattern module 2's
 * sizeof already showed sitting behind every int.
 *
 * Nothing here needs anything past module 9's memory model. Everything is
 * plain, single-file C -- no scratch files, no subprocess, no cc invoked
 * from inside the lesson the way modules 16 through 18 needed one. A
 * function pointer, a variadic function, and a macro all compile straight
 * into this course's own binary, so every number this module prints is
 * this program's own real output, not a second program's.
 */

#include <stdio.h>
#include <stdarg.h>

#include "lessons.h"
#include "ui.h"

/* Module 19's macros, kept at file scope for the same reason module 14 and
 * 15 keep theirs there: #define does not observe C's block scope, so
 * writing one inside a function would misrepresent what it actually is. */
#define BYTE_BITS 8
#define SQUARE_BROKEN(x) x*x
#define SQUARE_FIXED(x) ((x)*(x))

/* PART 1: two functions with the same signature, so a single function
 * pointer variable can point at either one. */
static int add(int a, int b)
{
    return a + b;
}

static int subtract(int a, int b)
{
    return a - b;
}

/* PART 2: a callback that mutates one element, applied to every element
 * of an array by a function that never names double_value itself. */
static void double_value(int *x)
{
    *x *= 2;
}

static void apply_to_each(int arr[], int n, void (*fn)(int *))
{
    for (int i = 0; i < n; i++) {
        fn(&arr[i]);
    }
}

/* PART 2: a comparator callback -- find_best does not know what "better"
 * means, only that better(a, b) answers the question for it. */
static int is_greater(int a, int b)
{
    return a > b;
}

static int is_less(int a, int b)
{
    return a < b;
}

static int find_best(const int arr[], int n, int (*better)(int, int))
{
    int best = arr[0];
    for (int i = 1; i < n; i++) {
        if (better(arr[i], best)) {
            best = arr[i];
        }
    }
    return best;
}

/* PART 3: a variadic function. count tells it how many int arguments
 * follow -- the function has no other way to know. */
static int sum_ints(int count, ...)
{
    va_list args;
    va_start(args, count);

    int total = 0;
    for (int i = 0; i < count; i++) {
        total += va_arg(args, int);
    }

    va_end(args);
    return total;
}

/* PART 5: prints a byte's 8 bits, most significant first, using nothing
 * this course has not already taught -- %c and a loop, not putchar. */
static void print_bits(unsigned char byte)
{
    for (int i = BYTE_BITS - 1; i >= 0; i--) {
        printf("%c", (byte & (1 << i)) ? '1' : '0');
    }
    printf("\n");
}

void lesson_19_advanced(void)
{
    title("MODULE 19 - FUNCTION POINTERS, CALLBACKS, AND MACROS");

    heading("PART 1: a function pointer -- a variable holding a function's address");

    printf("  This course's own main.c already does this, quietly, since\n");
    printf("  module 1:\n\n");

    printf("    struct Module {\n");
    printf("        const char *title;\n");
    printf("        void (*run)(void);\n");
    printf("        const char *tier;\n");
    printf("    };\n\n");

    printf("  run is not an int, a char *, or a struct -- it is a pointer\n");
    printf("  to a function taking nothing and returning nothing. Every\n");
    printf("  row in MODULES fills it with a different lesson function,\n");
    printf("  and MODULES[n - 1].run() calls whichever one that row holds.\n");
    printf("  That is what lets the whole menu be a table instead of an\n");
    printf("  18-case switch.\n\n");

    printf("  The same idea, smaller:\n\n");
    printf("    int (*op)(int, int) = add;\n\n");

    int (*op)(int, int) = add;

    printf("  add here is not a call -- there are no parentheses after\n");
    printf("  it -- so it is the function's own address, the same way an\n");
    printf("  array's name with no [] is its own address (module 6). No &\n");
    printf("  is needed, though &add means the same thing.\n\n");

    printf("  Running: op(3, 5) and (*op)(3, 5) are both legal, and call\n");
    printf("  the exact same function through the exact same pointer:\n\n");

    printf("    op(3, 5)     = %d\n", op(3, 5));
    printf("    (*op)(3, 5)  = %d\n\n", (*op)(3, 5));

    op = subtract;
    printf("  op = subtract;   op(10, 4) = %d\n\n", op(10, 4));

    printf("  Nothing about add or subtract changed. Reassigning op is\n");
    printf("  what changed which function 10 and 4 are actually handed to.\n");

    wait_enter();
    clear_screen();
    heading("PART 2: callbacks -- passing behaviour, not just data");

    printf("  A callback is a function pointer received as a parameter,\n");
    printf("  called from inside the function that received it. The\n");
    printf("  function doing the calling never needs to know what its\n");
    printf("  callback actually does:\n\n");

    printf("    void apply_to_each(int arr[], int n, void (*fn)(int *))\n");
    printf("    {\n");
    printf("        for (int i = 0; i < n; i++) {\n");
    printf("            fn(&arr[i]);\n");
    printf("        }\n");
    printf("    }\n\n");

    int values[] = { 1, 2, 3, 4, 5 };
    int value_count = (int) (sizeof values / sizeof values[0]);

    printf("  Running, with fn = double_value:\n\n");
    printf("    before: ");
    for (int i = 0; i < value_count; i++) {
        printf("%d ", values[i]);
    }
    printf("\n");

    apply_to_each(values, value_count, double_value);

    printf("    after:  ");
    for (int i = 0; i < value_count; i++) {
        printf("%d ", values[i]);
    }
    printf("\n\n");

    printf("  apply_to_each never mentions double_value by name -- any\n");
    printf("  function shaped void (*)(int *) could be passed instead,\n");
    printf("  and apply_to_each would not need a single line changed.\n\n");

    printf("  A second shape: a callback that COMPARES two values instead\n");
    printf("  of changing one, so the caller decides what \"best\" means:\n\n");

    printf("    int find_best(const int arr[], int n, int (*better)(int, int))\n\n");

    int scores[] = { 42, 17, 89, 3, 56 };
    int score_count = (int) (sizeof scores / sizeof scores[0]);

    printf("  Running, on the same array {42, 17, 89, 3, 56}:\n\n");
    printf("    find_best(scores, %d, is_greater) = %d\n",
           score_count, find_best(scores, score_count, is_greater));
    printf("    find_best(scores, %d, is_less)    = %d\n\n",
           score_count, find_best(scores, score_count, is_less));

    printf("  find_best's own code did not change between those two\n");
    printf("  calls -- only which comparator it was handed did.\n");

    wait_enter();
    clear_screen();
    heading("PART 3: variadic functions -- a different argument count each call");

    printf("  printf itself has taken a different number of arguments\n");
    printf("  every time this course has called it, since module 1. That\n");
    printf("  works because printf reads its OWN format string first and\n");
    printf("  counts the %%d/%%s markers in it -- the format string is a\n");
    printf("  sentinel telling it how many more arguments to expect. A\n");
    printf("  function written by hand needs the exact same kind of\n");
    printf("  telling; there is no way in C to ask \"how many arguments\n");
    printf("  was I actually called with\" without one:\n\n");

    printf("    #include <stdarg.h>\n\n");
    printf("    int sum_ints(int count, ...)\n");
    printf("    {\n");
    printf("        va_list args;\n");
    printf("        va_start(args, count);\n\n");
    printf("        int total = 0;\n");
    printf("        for (int i = 0; i < count; i++) {\n");
    printf("            total += va_arg(args, int);\n");
    printf("        }\n\n");
    printf("        va_end(args);\n");
    printf("        return total;\n");
    printf("    }\n\n");

    printf("  count is the sentinel here -- an ordinary parameter the\n");
    printf("  caller fills in honestly. va_start points args at whatever\n");
    printf("  comes after count; each va_arg(args, int) reads one more\n");
    printf("  argument as an int and moves past it; va_end closes it out.\n");
    printf("  Nothing checks that count matches what was really passed --\n");
    printf("  sum_ints(1, 10, 20) would silently read only the 10 and\n");
    printf("  never know 20 was there. Getting the count right is entirely\n");
    printf("  on the caller.\n\n");

    printf("  Running:\n\n");
    printf("    sum_ints(3, 10, 20, 30)    = %d\n", sum_ints(3, 10, 20, 30));
    printf("    sum_ints(5, 1, 2, 3, 4, 5) = %d\n", sum_ints(5, 1, 2, 3, 4, 5));

    wait_enter();
    clear_screen();
    heading("PART 4: macros -- text substitution before the compiler ever runs");

    printf("  Module 14 and 15 already used #define for a plain constant\n");
    printf("  (STACK_CAP, MAX_RETRIES) -- an object-like macro, gone\n");
    printf("  before the compiler sees it, replaced by a number with no\n");
    printf("  type. A function-like macro is the same substitution with\n");
    printf("  parameters, and it is where the substitution stops being\n");
    printf("  harmless:\n\n");

    printf("    #define SQUARE(x) x*x\n\n");

    printf("  Called as SQUARE(2+3), the preprocessor does not evaluate\n");
    printf("  2+3 first -- it has no idea what + or an int even is, only\n");
    printf("  text. It substitutes x with the literal tokens 2+3, giving:\n\n");

    printf("    2+3*2+3\n\n");

    printf("  which is 2 + (3*2) + 3, not (2+3) * (2+3), because C's own\n");
    printf("  operator precedence puts * before + and the macro added no\n");
    printf("  parentheses to override it. Running the actual macro above,\n");
    printf("  called exactly as SQUARE(2+3):\n\n");

    printf("    SQUARE(2+3) = %d   (25 was the intent)\n\n", SQUARE_BROKEN(2 + 3));

    printf("  This is wrong arithmetic, not undefined behaviour -- every\n");
    printf("  run of this exact program produces the exact same %d,\n",
           SQUARE_BROKEN(2 + 3));
    printf("  which is exactly why it is safe to run and show. The fix\n");
    printf("  wraps every use of the parameter in its own parentheses:\n\n");

    printf("    #define SQUARE(x) ((x)*(x))\n\n");

    printf("  which turns SQUARE(2+3) into ((2+3)*(2+3)) -- 2+3 forced to\n");
    printf("  finish evaluating before the multiplication touches it\n");
    printf("  either side. Running the fixed macro, same call:\n\n");

    printf("    SQUARE(2+3) = %d\n\n", SQUARE_FIXED(2 + 3));

    printf("  Same source line, same call, two different macro bodies,\n");
    printf("  two different real numbers -- neither one a guess.\n");

    wait_enter();
    clear_screen();
    heading("PART 5: bit manipulation -- &, |, ^, ~, <<, >>");

    printf("  Module 2's sizeof already established that an int is a\n");
    printf("  fixed number of bits. These operators work on that bit\n");
    printf("  pattern directly, instead of on the number it represents:\n\n");

    printf("    &   AND    both bits 1 -> 1, otherwise 0   (checks a bit)\n");
    printf("    |   OR     either bit 1 -> 1               (sets a bit)\n");
    printf("    ^   XOR    bits differ -> 1                (toggles a bit)\n");
    printf("    ~   NOT    flips every bit\n");
    printf("    <<  shifts every bit left,  filling with 0\n");
    printf("    >>  shifts every bit right, filling with 0 here (unsigned)\n\n");

    printf("  1 << n is n zero bits followed by a single 1 -- a mask\n");
    printf("  naming exactly one bit. Running, on an unsigned char\n");
    printf("  starting at all zero bits:\n\n");

    unsigned char flags = 0;
    printf("    flags = 0;                          ");
    print_bits(flags);

    flags = (unsigned char) (flags | (1 << 0) | (1 << 2) | (1 << 5));
    printf("    flags |= bit 0, bit 2, bit 5;       ");
    print_bits(flags);
    printf("      -- OR turned three separate bits on in one flags byte\n\n");

    int bit2_is_set = (flags & (1 << 2)) != 0;
    printf("    (flags & (1 << 2)) != 0             -> %s\n\n",
           bit2_is_set ? "1 (bit 2 is set)" : "0 (bit 2 is clear)");

    flags = (unsigned char) (flags & ~(1 << 2));
    printf("    flags &= ~(1 << 2);                 ");
    print_bits(flags);
    printf("      -- bit 2 is off; bits 0 and 5, never named in that\n");
    printf("      line, are still on -- AND with a NOT'd mask reaches\n");
    printf("      exactly one bit, not the whole byte\n\n");

    flags = (unsigned char) (flags ^ (1 << 0));
    printf("    flags ^= (1 << 0);                  ");
    print_bits(flags);
    printf("      -- XOR toggled bit 0 off without being told whether\n");
    printf("      it was on or off first; bit 5 is untouched\n");

    wait_enter();
    clear_screen();
    exercise(19);

    question("int (*op)(int, int) = add;   -- with no & and no (), what\n"
             "  does the plain name `add` give op here? (one word: a call,\n"
             "  or the function's own address)",
             "address",
             "A function used with no () decays to its own address, the\n"
             "             same way an array's bare name decays to the address of\n"
             "             its first element (module 6).");

    question("sum_ints(int count, ...) reads exactly `count` arguments\n"
             "  with va_arg. Without that count, or some other sentinel the\n"
             "  caller provides, can C tell on its own how many arguments a\n"
             "  variadic call actually passed? (yes or no)",
             "no",
             "There is no hidden argument count anywhere. printf works this\n"
             "             out from its own format string; sum_ints here uses an\n"
             "             explicit int parameter instead -- either way, something\n"
             "             the caller controls has to say how many.");

    question("#define SQUARE(x) x*x  makes SQUARE(2+3) evaluate to 11, not\n"
             "  25. What does the fixed body need around x that the broken\n"
             "  one does not have? (one word)",
             "parentheses",
             "((x)*(x)) forces 2+3 to finish evaluating before the\n"
             "             multiplication reaches it. The preprocessor never\n"
             "             evaluates + or * itself -- it only ever substitutes\n"
             "             tokens, so nothing about the substitution fixes this\n"
             "             except adding parentheses to the macro's own text.");

    question("Which one of &, |, ^, ~ checks whether a specific bit is\n"
             "  already set, without changing the value at all? (symbol)",
             "&",
             "flags & (1 << n) leaves every bit of flags exactly as it was\n"
             "             and produces a nonzero result only if bit n was\n"
             "             already 1. | would set that bit; ^ would toggle it;\n"
             "             ~ flips every bit and is not even a two-operand test.");

    {
        const char *task[] = {
            "Write print_filtered(const int arr[], int n, int (*keep)(int)),",
            "printing only the elements keep() accepts, one per line.",
            "Call it on the array {3, 8, 15, 16, 23, 42} with a filter",
            "that keeps only even numbers.",
        };
        const char *expected[] = { "8", "16", "42" };
        const char *solution[] = {
            "#include <stdio.h>",
            "",
            "int is_even(int n)",
            "{",
            "    return n % 2 == 0;",
            "}",
            "",
            "void print_filtered(const int arr[], int n, int (*keep)(int))",
            "{",
            "    for (int i = 0; i < n; i++) {",
            "        if (keep(arr[i])) {",
            "            printf(\"%d\\n\", arr[i]);",
            "        }",
            "    }",
            "}",
            "",
            "int main(void)",
            "{",
            "    int numbers[] = { 3, 8, 15, 16, 23, 42 };",
            "    int count = (int) (sizeof numbers / sizeof numbers[0]);",
            "",
            "    print_filtered(numbers, count, is_even);",
            "    return 0;",
            "}",
            "",
            "print_filtered never mentions is_even by name -- keep is a",
            "plain int (*)(int) parameter, so any other function of that",
            "same shape could be passed in its place untouched.",
        };
        challenge(task, (int) (sizeof task / sizeof task[0]),
                  0, 0,
                  expected, (int) (sizeof expected / sizeof expected[0]),
                  solution, (int) (sizeof solution / sizeof solution[0]));
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    printf("   - a function's bare name decays to its own address; a\n");
    printf("     function pointer variable can hold it and call through\n");
    printf("     it, exactly the way this course's own MODULES table\n");
    printf("     dispatches to every lesson\n");
    printf("   - a callback is a function pointer parameter -- the code\n");
    printf("     calling it never needs to know what it actually does\n");
    printf("   - a variadic function has no way to know its own argument\n");
    printf("     count on its own; something the caller controls (an\n");
    printf("     explicit count, or printf's own format string) has to\n");
    printf("     say so\n");
    printf("   - a function-like macro substitutes raw tokens, not\n");
    printf("     evaluated values -- ((x)*(x)) is what keeps the caller's\n");
    printf("     own operators from reaching in unparenthesized\n");
    printf("   - &, |, ^, ~, <<, >> work on an int's actual bits: & tests\n");
    printf("     one, | sets it, ^ toggles it, and & with a NOT'd mask\n");
    printf("     clears it without touching any other bit\n");
    wait_enter();
}
