/*
 * TITLE: Printing and formatting text
 * GROUP: Basics
 * USES: #include <stdio.h>
 * SUMMARY: printf writes formatted text to the terminal. snprintf writes it
 *   into a buffer you give it, and never past the end of that buffer.
 * NOTES:
 *   - Use snprintf, not sprintf: sprintf cannot be told how big the buffer is.
 *   - snprintf returns the length the text WOULD have had. If that is not
 *     smaller than the buffer size, the text was cut off.
 *   - %zu is for size_t, %ld for long, %lu for unsigned long, %.1f for one
 *     decimal, %5d pads to width 5, %-8s left-aligns in width 8.
 *   - A buffer for a string needs room for the terminating zero byte.
 * SEE: man 3 printf
 */

#include <stdio.h>

/* Format into a caller-supplied buffer; return what snprintf returns. */
static int format_into(char *out, size_t size, const char *text)
{
    return snprintf(out, size, "%s", text);
}

int main(void)
{
    char line[32];
    int used = snprintf(line, sizeof line, "%-8s|%5d|%6.2f", "cpu", 8, 3.14159);

    printf("%s\n", line);
    printf("length %d, buffer %zu, cut off: %s\n",
           used, sizeof line, used >= (int) sizeof line ? "yes" : "no");

    char tiny[8];
    used = format_into(tiny, sizeof tiny, "longer than the buffer");
    printf("tiny holds \"%s\", wanted %d characters, cut off: %s\n",
           tiny, used, used >= (int) sizeof tiny ? "yes" : "no");

    fflush(stdout);
    fprintf(stderr, "errors and warnings go to stderr\n");
    return 0;
}
