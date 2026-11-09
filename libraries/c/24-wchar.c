/*
 * TITLE: wchar.h: display width of text (mbrtowc, wcwidth)
 * GROUP: Standard C library
 * USES: #include <wchar.h>, <locale.h>  (mbrtowc, wcwidth)
 * SUMMARY: printf pads by BYTES. Colour escape codes and UTF-8 letters are
 *   bytes that take no (or a different) space on screen, so "%-12s" misaligns
 *   a column of coloured or accented text. Pad by VISIBLE width instead.
 * SIGNATURE:
 *   size_t mbrtowc(wchar_t *out, const char *s, size_t n, mbstate_t *state)
 *   int wcwidth(wchar_t c)
 * PARAMETERS:
 *   - out: receives the decoded character.
 *   - s, n: the bytes, and how many may be read.
 *   - state: conversion state, zeroed before the first call.
 *   - c: a decoded character.
 * RETURNS: mbrtowc: bytes used, 0 for the zero byte. wcwidth: the columns c
 *   takes, or -1.
 * ERRORS:
 *   - mbrtowc returns (size_t) -1 with EILSEQ for invalid bytes and (size_t)
 *     -2 for a truncated character.
 *   - wcwidth returns -1 for a character that is not printable.
 * NOTES:
 *   - Measure on screen: skip ESC [ ... letter sequences, decode UTF-8 with
 *     mbrtowc, and ask wcwidth how many columns each character takes (1 for
 *     most, 2 for East Asian wide characters and most emoji, 0 for combining
 *     accents).
 *   - Call setlocale(LC_ALL, "") first; without a UTF-8 locale mbrtowc cannot
 *     decode multi-byte text.
 *   - Build the padding with a loop of spaces, or printf("%*s", count, "").
 *   - Never rely on strlen() for layout.
 * SEE: man 3 wcwidth, man 3 mbrtowc
 */

#define _XOPEN_SOURCE 700

#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

/* Columns the text takes on screen. */
static int visible_width(const char *text)
{
    int width = 0;
    mbstate_t state;

    memset(&state, 0, sizeof state);
    while (*text != '\0') {
        if (text[0] == '\033' && text[1] == '[') {
            text += 2;
            while (*text != '\0' && (*text < '@' || *text > '~')) {
                text++;
            }
            if (*text != '\0') {
                text++;
            }
            continue;
        }

        wchar_t character;
        size_t used = mbrtowc(&character, text, MB_CUR_MAX, &state);
        if (used == (size_t) -1 || used == (size_t) -2 || used == 0) {
            width++;
            text++;
            continue;
        }
        int columns = wcwidth(character);
        width += columns > 0 ? columns : 0;
        text += used;
    }
    return width;
}

static void print_padded(const char *text, int width)
{
    printf("%s", text);
    for (int i = visible_width(text); i < width; i++) {
        putchar(' ');
    }
}

int main(void)
{
    setlocale(LC_ALL, "");
    const char *labels[] = { "\033[1;34mOS\033[0m", "\033[1;34mKernel\033[0m", "Memória", "Disk" };

    printf("strlen(\"Memória\") = %zu bytes, visible_width = %d columns\n",
           strlen("Memória"), visible_width("Memória"));
    printf("strlen of a coloured \"OS\" = %zu bytes, visible_width = %d columns\n\n",
           strlen(labels[0]), visible_width(labels[0]));

    puts("printf(\"%-8s|\"), counts bytes:");
    for (int i = 0; i < 4; i++) {
        printf("  %-8s|\n", labels[i]);
    }
    puts("padded by visible width:");
    for (int i = 0; i < 4; i++) {
        printf("  ");
        print_padded(labels[i], 8);
        puts("|");
    }
    return 0;
}
