/*
 * TITLE: Colours and styles (ANSI escape codes)
 * GROUP: Terminal
 * USES: print escape sequences; no library
 * SUMMARY: A terminal changes colour when it reads ESC [ code m. ESC is the
 *   byte 27, written "\033" in C. "\033[0m" resets everything.
 * NOTES:
 *   - Styles: 1 bold, 2 dim, 3 italic, 4 underline, 7 reverse.
 *   - Colours: 30-37 text (black red green yellow blue magenta cyan white),
 *     40-47 background, 90-97 and 100-107 the bright versions.
 *   - 256 colours: ESC[38;5;Nm for text, ESC[48;5;Nm for background.
 *   - True colour: ESC[38;2;R;G;Bm. Check $COLORTERM for "truecolor" or "24bit".
 *   - ALWAYS reset (ESC[0m) before the end of a line, or the colour leaks into
 *     the shell prompt.
 *   - Print colours only when stdout is a terminal, and never when $NO_COLOR is
 *     set (https://no-color.org). A tool whose output is piped to a file should
 *     contain no escape bytes.
 *   - The distribution's own colour, for a logo, is ANSI_COLOR in os-release
 *     (for example "38;2;23;147;209").
 * SEE: man 4 console_codes
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* Colour only for a person at a terminal who has not opted out. */
static int use_colour(int force)
{
    if (getenv("NO_COLOR") != NULL) {
        return 0;
    }
    return force || isatty(STDOUT_FILENO);
}

int main(void)
{
    int colour = use_colour(1); /* forced on: this page's output is piped */

    if (!colour) {
        puts("colour is off ($NO_COLOR is set)");
        return 0;
    }

    printf("text colours:   ");
    for (int code = 30; code <= 37; code++) {
        printf("\033[%dm%d\033[0m ", code, code);
    }
    printf("\nbright:         ");
    for (int code = 90; code <= 97; code++) {
        printf("\033[%dm%d\033[0m ", code, code);
    }
    printf("\nstyles:         \033[1mbold\033[0m \033[2mdim\033[0m \033[3mitalic\033[0m "
           "\033[4munderline\033[0m \033[7mreverse\033[0m\n");
    printf("blocks:         ");
    for (int code = 40; code <= 47; code++) {
        printf("\033[%dm  \033[0m", code);
    }
    printf("\n256 colours:    ");
    for (int n = 16; n < 52; n++) {
        printf("\033[48;5;%dm \033[0m", n);
    }
    printf("\ntrue colour:    ");
    for (int step = 0; step < 36; step++) {
        printf("\033[48;2;%d;%d;255m \033[0m", step * 7, 255 - step * 7);
    }
    printf("\n");
    return 0;
}
