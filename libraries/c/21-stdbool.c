/*
 * TITLE: stdbool.h: bool, true and false
 * GROUP: Standard C library
 * USES: #include <stdbool.h>
 * STANDARD: ISO C99 and later. In C23 bool, true and false are keywords and
 *   the header is not needed.
 * SUMMARY: stdbool.h gives C a real boolean type. A bool holds 0 or 1;
 *   assigning any non-zero value stores 1.
 * PROVIDES:
 *   bool     the type (a macro for the built-in _Bool before C23)
 *   true     1
 *   false    0
 *   __bool_true_false_are_defined    1
 * NOTES:
 *   - Converting to bool normalises: bool b = 5; makes b equal 1. An int
 *     assigned from a bool is exactly 0 or 1.
 *   - The ctype functions (isdigit, isalpha) return zero or non-zero, NOT 0 or 1.
 *     Write isdigit(c) != 0 before comparing one to true.
 *   - sizeof(bool) is 1 on Linux. A struct of many flags is still one byte
 *     per flag; use bit fields or a uint32_t mask to pack them.
 *   - Do not read a bool from a file or the network byte by byte and trust it:
 *     a value other than 0 or 1 in memory is undefined behaviour. Test
 *     the byte (byte != 0) and store the result.
 *   - Before C99 programs defined their own TRUE and FALSE. Do not mix the two.
 * USEDFOR:
 *   - A function that may find nothing returns bool and fills an out
 *     parameter:  bool read_first_line(path, out, size)  is true when it read
 *     something, so the caller can tell "missing" apart from "empty".
 *   - Command-line options such as --quiet and --json are bools in a struct.
 * SEE: man 0 stdbool.h
 */

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

struct Options {
    bool quiet;
    bool json;
};

/* True when the file exists and has a first line; out holds it. */
static bool read_first_line(const char *path, char *out, size_t size)
{
    FILE *file = fopen(path, "r");

    if (file == NULL) {
        return false;
    }
    bool ok = fgets(out, (int) size, file) != NULL;
    fclose(file);
    return ok;
}

int main(void)
{
    bool from_five = 5;
    int as_int = true;
    struct Options options = { .quiet = false, .json = true };
    char text[256];

    printf("bool from 5     = %d (normalised to 1)\n", from_five);
    printf("true as an int  = %d, false = %d\n", as_int, false);
    printf("sizeof(bool)    = %zu\n", sizeof(bool));
    printf("isdigit('7')    = %d (non-zero, not 1): compare with != 0\n", isdigit('7'));
    printf("options         = quiet %s, json %s\n", options.quiet ? "on" : "off", options.json ? "on" : "off");
    printf("/etc/hostname   = %s\n", read_first_line("/etc/hostname", text, sizeof text) ? "read" : "missing");
    printf("/no/such/file   = %s\n", read_first_line("/no/such/file", text, sizeof text) ? "read" : "missing");
    return 0;
}
