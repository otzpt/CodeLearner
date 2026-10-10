/*
 * TITLE: A logo on the left, information on the right
 * GROUP: Terminal
 * USES: print line by line; pad each logo line to the logo's width
 * SUMMARY: A fastfetch layout is two lists printed side by side: line N of the
 *   logo, a gap, then line N of the information. Pad the logo lines to equal
 *   width so the information column lines up.
 * NOTES:
 *   - The logo is an array of strings, one per row. Its width is the widest
 *     row, measured in VISIBLE columns (see the alignment page when a row has
 *     colours).
 *   - Loop to the longer of the two lists; print an empty string, padded, for
 *     whichever list has run out.
 *   - Colour the logo with one colour for the whole logo (the distribution's
 *     ANSI_COLOR), reset at the end of EACH row, then print the information.
 *   - Check the terminal width first and drop the logo on a narrow terminal.
 *   - Real fastfetch logos are ASCII art with colour placeholders; to use a
 *     distribution's logo, store each as a text file and read it into the
 *     array.
 * SEE: the alignment page, the terminal size page
 */

#include <stdio.h>
#include <string.h>

int main(void)
{
    const char *logo[] = {
        "    .-----.    ",
        "   /  o o  \\   ",
        "  |    >    |  ",
        "  |  \\___/  |  ",
        "   \\       /   ",
        "    '-----'    ",
    };
    const char *info[] = {
        "user@host",
        "---------",
        "OS: Example Linux",
        "Kernel: 6.17.0",
        "Uptime: 3 hours, 12 mins",
        "Memory: 4.2 GiB / 15.6 GiB",
        "Shell: bash",
    };
    int logo_rows = (int) (sizeof logo / sizeof logo[0]);
    int info_rows = (int) (sizeof info / sizeof info[0]);
    int rows = logo_rows > info_rows ? logo_rows : info_rows;
    int logo_width = 0;

    for (int i = 0; i < logo_rows; i++) {
        int length = (int) strlen(logo[i]);
        if (length > logo_width) {
            logo_width = length;
        }
    }

    for (int row = 0; row < rows; row++) {
        const char *left = row < logo_rows ? logo[row] : "";
        const char *right = row < info_rows ? info[row] : "";

        printf("\033[1;36m%-*s\033[0m  %s\n", logo_width, left, right);
    }
    return 0;
}
