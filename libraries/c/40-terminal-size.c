/*
 * TITLE: Terminal width and height
 * GROUP: Terminal
 * USES: #include <sys/ioctl.h>, <unistd.h>, <fcntl.h>
 * SUMMARY: ioctl(fd, TIOCGWINSZ, &size) asks the terminal how many columns and
 *   rows it has. isatty(fd) says whether a file descriptor is a terminal at all.
 * NOTES:
 *   - When output is piped or redirected (`prog | less`, `prog > file`),
 *     stdout is not a terminal and the ioctl fails. Try stdin and stderr, then
 *     open /dev/tty, which is your terminal even when everything is redirected.
 *   - Fall back to $COLUMNS / $LINES, then to 80x24. Over ssh or in a script
 *     there may be no terminal at all.
 *   - ws_col and ws_row are in characters. ws_xpixel and ws_ypixel are pixels
 *     and are often 0.
 *   - The size changes when the window is resized: catch SIGWINCH and ask again.
 *   - Do not print more than the width, or lines wrap and a logo falls apart.
 * SEE: man 4 tty_ioctl, man 3 isatty
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

/* Fill columns and rows; return where the answer came from. */
static const char *terminal_size(int *columns, int *rows)
{
    struct winsize size;
    int descriptors[] = { STDOUT_FILENO, STDERR_FILENO, STDIN_FILENO };
    const char *names[] = { "stdout", "stderr", "stdin" };

    for (int i = 0; i < 3; i++) {
        if (isatty(descriptors[i]) && ioctl(descriptors[i], TIOCGWINSZ, &size) == 0 && size.ws_col > 0) {
            *columns = size.ws_col;
            *rows = size.ws_row;
            return names[i];
        }
    }

    int tty = open("/dev/tty", O_RDONLY);
    if (tty >= 0) {
        int ok = ioctl(tty, TIOCGWINSZ, &size) == 0 && size.ws_col > 0;
        close(tty);
        if (ok) {
            *columns = size.ws_col;
            *rows = size.ws_row;
            return "/dev/tty";
        }
    }

    const char *env_columns = getenv("COLUMNS");
    const char *env_rows = getenv("LINES");
    *columns = env_columns != NULL && atoi(env_columns) > 0 ? atoi(env_columns) : 80;
    *rows = env_rows != NULL && atoi(env_rows) > 0 ? atoi(env_rows) : 24;
    return env_columns != NULL ? "$COLUMNS" : "default 80x24";
}

int main(void)
{
    int columns, rows;
    const char *source = terminal_size(&columns, &rows);

    printf("stdout is a terminal: %s\n", isatty(STDOUT_FILENO) ? "yes" : "no");
    printf("size: %d columns x %d rows (from %s)\n", columns, rows, source);
    return 0;
}
