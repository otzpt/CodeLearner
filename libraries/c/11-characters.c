/*
 * TITLE: ctype.h: classifying and converting characters
 * GROUP: Basics
 * USES: #include <ctype.h>
 * STANDARD: ISO C. Behaviour depends on the locale (setlocale); in the "C"
 *   locale only ASCII letters and digits are classified.
 * SUMMARY: ctype.h classifies and converts single characters: digit, letter,
 *   space, punctuation, upper or lower case.
 * PROVIDES:
 *   isdigit(c), isxdigit(c)         0-9 / 0-9 a-f A-F
 *   isalpha(c), isalnum(c)          letter / letter or digit
 *   isupper(c), islower(c)
 *   isspace(c)                      space, tab, newline, return, form feed
 *   isprint(c), ispunct(c), iscntrl(c)
 *   int toupper(c), tolower(c)      the converted character (other input unchanged)
 * NOTES:
 *   - Pass an unsigned char (or EOF). A plain char with a negative value is
 *     undefined behaviour, so cast: isdigit((unsigned char) c).
 *   - The functions return non-zero for true, not necessarily 1 (glibc returns
 *     2048 for isdigit('7')). Compare with != 0, or wrap in bool.
 *   - isspace is true for space, tab, newline and the other whitespace.
 *   - toupper / tolower change one character; loop to change a string.
 *   - They work on single BYTES. UTF-8 text such as "Memória" has letters
 *     made of several bytes that these functions cannot classify; see
 *     wctype.h (iswalpha) and the alignment page.
 *   - A hexadecimal digit's value is  isdigit(c) ? c - '0' : tolower(c) - 'a' + 10.
 * TOOL:
 *   - isdigit to decide whether a "model name" or version string starts with a
 *     number; isspace to trim a value read from /proc.
 *   - tolower to compare a distribution id case-insensitively.
 *   - isxdigit to read the 0x10de ids in /sys/class/drm.
 * SEE: man 3 isalpha, man 3 toupper
 */

#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    const char *word = "Linux 6.17";

    for (const char *p = word; *p != '\0'; p++) {
        unsigned char c = (unsigned char) *p;
        printf("'%c' digit %d alpha %d space %d upper-case form '%c'\n",
               c, isdigit(c) != 0, isalpha(c) != 0, isspace(c) != 0, toupper(c));
    }

    /* trim: skip leading and trailing whitespace of a value read from /proc */
    char value[] = "  AMD Ryzen 5 \n";
    char *start = value;
    char *end = value + strlen(value);
    while (isspace((unsigned char) *start)) {
        start++;
    }
    while (end > start && isspace((unsigned char) end[-1])) {
        end--;
    }
    printf("trimmed: \"%.*s\"\n", (int) (end - start), start);

    /* the value of a hexadecimal digit, as in the 0x10de ids under /sys */
    unsigned id = 0;
    for (const char *p = "10de"; *p != '\0'; p++) {
        unsigned char c = (unsigned char) *p;
        if (!isxdigit(c)) {
            break;
        }
        id = id * 16 + (isdigit(c) ? (unsigned) (c - '0') : (unsigned) (tolower(c) - 'a' + 10));
    }
    printf("\"10de\" read by hand is %u = 0x%x\n", id, id);
    printf("isdigit('7') = %d, so compare with != 0, never == 1\n", isdigit('7'));
    return 0;
}
