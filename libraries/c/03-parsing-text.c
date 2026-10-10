/*
 * TITLE: Parsing text: sscanf, strtol, strchr, strncmp
 * GROUP: Basics
 * USES: #include <stdio.h>, <stdlib.h>, <string.h>
 * SUMMARY: Most system information arrives as text: "Key: value" lines,
 *   "KEY=value" lines, numbers followed by units. These four calls cover it.
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
