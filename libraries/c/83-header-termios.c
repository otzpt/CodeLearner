/*
 * TITLE: termios.h: terminal modes (echo, line editing, raw input)
 * GROUP: Header reference: signals and terminals
 * USES: #include <termios.h>
 * STANDARD: POSIX. posix_openpt, grantpt, unlockpt and ptsname (used below
 *   to get a terminal without a real one) are POSIX and need _XOPEN_SOURCE 600.
 * SUMMARY: A terminal has settings: whether typed characters are echoed,
 *   whether input is collected into lines, whether Ctrl-C raises a signal.
 *   termios reads and changes them. Use it to read one key at a time or to
 *   hide a typed password.
 * PROVIDES:
 *   int tcgetattr(fd, struct termios *)        read the current settings
 *   int tcsetattr(fd, when, const struct termios *)    when: TCSANOW,
 *                                                      TCSADRAIN, TCSAFLUSH
 *   void cfmakeraw(struct termios *)           settings for raw mode
 *   struct termios: c_iflag, c_oflag, c_cflag, c_lflag, c_cc[]
 *   c_lflag bits: ECHO (show typed characters), ICANON (wait for ENTER),
 *                 ISIG (Ctrl-C raises SIGINT)
 *   c_cc[VMIN], c_cc[VTIME]     how many characters / how long a read waits
 *   speed_t cfgetospeed(const struct termios *); int tcflush(fd, queue)
 * NOTES:
 *   - ALWAYS save the original settings with tcgetattr, and restore them with
 *     tcsetattr on every way out: normal exit, error, and signals (SIGINT,
 *     SIGTERM). A program that dies in raw mode leaves the user's terminal
 *     without echo until they type  reset .
 *   - One key without ENTER: clear ICANON and ECHO, set VMIN = 1 and
 *     VTIME = 0, then read(0, &c, 1).
 *   - tcgetattr fails with ENOTTY if the descriptor is not a terminal
 *     (a pipe, a file): check the result, or isatty first.
 *   - Use TCSAFLUSH to discard unread input when you change modes.
 *   - This page opens a pseudo-terminal so it works everywhere, even where the
 *     program has no terminal of its own (ssh, cron, this browser).
 * TOOL:
 *   - A plain fetch tool does not need termios: it prints and exits.
 *   - Needed for a "press any key" pause, for an interactive mode that
 *     redraws on a keypress, or for reading the terminal's reply to an
 *     escape-sequence query (such as the size of a character cell), which
 *     arrives as typed input and needs raw mode.
 * SEE: man 3 termios, man 3 tcgetattr, man 7 pty
 */

#define _XOPEN_SOURCE 600

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

static void show(const char *label, const struct termios *modes)
{
    printf("%-22s ICANON %-3s ECHO %-3s ISIG %-3s VMIN %d VTIME %d\n", label,
           (modes->c_lflag & ICANON) ? "on" : "off", (modes->c_lflag & ECHO) ? "on" : "off",
           (modes->c_lflag & ISIG) ? "on" : "off", modes->c_cc[VMIN], modes->c_cc[VTIME]);
}

int main(void)
{
    int master = posix_openpt(O_RDWR | O_NOCTTY);

    if (master < 0 || grantpt(master) != 0 || unlockpt(master) != 0) {
        perror("pseudo-terminal");
        return 1;
    }
    char *name = ptsname(master);
    int slave = name != NULL ? open(name, O_RDWR | O_NOCTTY) : -1;
    struct termios original, changed;

    if (slave < 0 || tcgetattr(slave, &original) != 0) {
        perror("tcgetattr");
        return 1;
    }
    show("a fresh terminal:", &original);

    changed = original;
    changed.c_lflag &= ~(tcflag_t) (ICANON | ECHO);
    changed.c_cc[VMIN] = 1;
    changed.c_cc[VTIME] = 0;
    tcsetattr(slave, TCSAFLUSH, &changed);

    struct termios reread;
    tcgetattr(slave, &reread);
    show("one key at a time:", &reread);

    tcsetattr(slave, TCSAFLUSH, &original);
    tcgetattr(slave, &reread);
    show("restored:", &reread);

    int not_a_terminal = tcgetattr(STDOUT_FILENO, &reread);
    printf("tcgetattr on a pipe or file: %d (it fails unless stdout is a terminal)\n", not_a_terminal);
    close(slave);
    close(master);
    return 0;
}
