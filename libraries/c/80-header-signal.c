/*
 * TITLE: signal.h: handling signals (Ctrl-C, resize, broken pipe)
 * GROUP: Header reference: signals and terminals
 * USES: #include <signal.h>
 * STANDARD: ISO C has signal(), raise() and a few signals (SIGINT, SIGTERM,
 *   SIGSEGV). sigaction(), kill(), sigprocmask() and SIGPIPE, SIGUSR1,
 *   SIGCHLD are POSIX. SIGWINCH (terminal resized) is a BSD/Linux signal.
 * SUMMARY: A signal is a small message the kernel delivers to a process:
 *   Ctrl-C sends SIGINT, a closed pipe sends SIGPIPE, a resized terminal
 *   sends SIGWINCH. The program can handle, ignore or leave each one.
 * PROVIDES:
 *   int sigaction(sig, const struct sigaction *new, struct sigaction *old)
 *   struct sigaction: sa_handler, sa_mask, sa_flags (SA_RESTART, SA_NOCLDSTOP)
 *   int raise(int sig); int kill(pid_t pid, int sig)
 *   sig_atomic_t                    the only type a handler may safely write
 *   SIGINT, SIGTERM, SIGHUP, SIGQUIT, SIGPIPE, SIGWINCH, SIGUSR1, SIGUSR2, SIGCHLD
 *   SIG_IGN (ignore), SIG_DFL (default action)
 *   const char *strsignal(int sig)  a description (string.h)
 *   int sigprocmask / pthread_sigmask(...)   block signals for a while
 * NOTES:
 *   - Use sigaction, not signal(): signal() has different meanings on different
 *     systems (whether the handler stays installed, whether calls restart).
 *   - A handler runs in the middle of anything. Inside it call only
 *     async-signal-safe functions: write(), _exit(), and setting a
 *     volatile sig_atomic_t flag. NOT printf, malloc, or free. The usual pattern:
 *     the handler sets a flag; the main loop checks it.
 *   - SIGKILL and SIGSTOP cannot be caught or ignored.
 *   - SIGPIPE kills a program that writes to a closed pipe, which is what
 *     happens to  prog | head -1  once head exits. Ignore it (SIG_IGN) and
 *     check that write returns -1 with errno EPIPE.
 *   - A signal can interrupt a slow call: read() may return -1 with errno
 *     EINTR. SA_RESTART restarts most calls for you.
 *   - SIGKILL numbers and SIGUSR1's number differ between architectures: use
 *     the names.
 * TOOL:
 *   - SIGINT: reset colours and show the cursor before exiting.
 *   - SIGWINCH: re-read the terminal size and redraw, in a live mode.
 *   - SIGPIPE: ignore it so  voidfetch | head  ends quietly.
 * SEE: man 7 signal, man 2 sigaction, man 7 signal-safety
 */

#define _POSIX_C_SOURCE 200809L

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static volatile sig_atomic_t last_signal;

/* Async-signal-safe: only set a flag. */
static void remember(int signal_number)
{
    last_signal = signal_number;
}

static int install(int signal_number, void (*handler)(int))
{
    struct sigaction action;

    memset(&action, 0, sizeof action);
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = SA_RESTART;
    return sigaction(signal_number, &action, NULL);
}

int main(void)
{
    int channel[2];

    if (install(SIGUSR1, remember) != 0 || install(SIGWINCH, remember) != 0) {
        perror("sigaction");
        return 1;
    }

    raise(SIGUSR1);
    printf("after raise(SIGUSR1): flag holds %d = \"%s\"\n", (int) last_signal, strsignal(last_signal));
    raise(SIGWINCH);
    printf("after raise(SIGWINCH): flag holds \"%s\"\n", strsignal(last_signal));

    /* SIGPIPE: ignore it, then write to a pipe whose reader has gone. */
    signal(SIGPIPE, SIG_IGN);
    if (pipe(channel) == 0) {
        close(channel[0]);
        ssize_t written = write(channel[1], "x", 1);
        printf("write to a closed pipe: returned %zd, errno %d = %s\n", written, errno, strerror(errno));
        close(channel[1]);
    }

    printf("SIGKILL can be caught: %s\n", install(SIGKILL, remember) == 0 ? "yes" : "no (sigaction refuses)");
    return 0;
}
