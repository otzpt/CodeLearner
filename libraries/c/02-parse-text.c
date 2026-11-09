/*
 * TITLE: sscanf(3), strtol(3), strchr(3): parsing text
 * GROUP: Common tasks
 * USES: #include <stdio.h>, <stdlib.h>, <string.h>
 * SUMMARY: Text from files and other programs usually comes as "Key: value"
 *   lines, "KEY=value" lines and numbers followed by units. These four calls
 *   parse all three.
 * SIGNATURE:
 *   int sscanf(const char *text, const char *format, ...)
 *   long strtol(const char *text, char **end, int base)
 *   char *strchr(const char *text, int c)
 * PARAMETERS:
 *   - text: the string to read.
 *   - format: conversions such as %d and %63s. Each one needs a pointer to
 *     where its value goes.
 *   - end: receives the address of the first character strtol did not use.
 *   - base: 10 for decimal, 16 for hex, 0 to detect a 0x prefix.
 *   - c: the character to look for.
 * RETURNS: sscanf: how many fields it filled, or EOF if the text ended
 *   first. strtol: the number. strchr: a pointer to the first c, or NULL.
 * ERRORS:
 *   - strtol sets errno to ERANGE and returns LONG_MAX or LONG_MIN when the
 *     number does not fit. Set errno = 0 before calling.
 *   - No number at all: strtol leaves end equal to text and returns 0.
 * NOTES:
 *   - sscanf returns how many fields it filled. Check it against what you
 *     asked for, or a line that does not match is silently half-parsed.
 *   - In %s, always give a width: %63s for a 64-byte buffer.
 *   - strtol tells you where it stopped (endptr). If endptr == text, there
 *     was no number. atoi cannot tell you that; do not use it.
 *   - strncmp(line, "MemTotal:", 9) == 0 tests that a line starts with a key.
 *   - strchr(line, '=') finds the split point of KEY=value.
 * SEE: man 3 sscanf, man 3 strtol
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    const char *meminfo = "MemTotal:       16384000 kB";
    unsigned long kib;

    if (sscanf(meminfo, "MemTotal: %lu kB", &kib) == 1) {
        printf("sscanf: %lu kB = %lu MiB\n", kib, kib / 1024);
    }

    char *end;
    long number = strtol("  42 apples", &end, 10);
    printf("strtol: %ld, stopped at \"%s\"\n", number, end);

    const char *bad = "apples";
    number = strtol(bad, &end, 10);
    printf("strtol on \"%s\": %ld, found a number: %s\n", bad, number, end != bad ? "yes" : "no");

    const char *entry = "PRETTY_NAME=\"Ubuntu 26.04 LTS\"";
    const char *equals = strchr(entry, '=');
    if (equals != NULL) {
        printf("key \"%.*s\", value %s\n", (int) (equals - entry), entry, equals + 1);
    }
    printf("starts with PRETTY_NAME=: %s\n", strncmp(entry, "PRETTY_NAME=", 12) == 0 ? "yes" : "no");
    return 0;
}
