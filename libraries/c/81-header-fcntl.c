/*
 * TITLE: fcntl.h: open() and file descriptor flags
 * GROUP: Header reference: signals and terminals
 * USES: #include <fcntl.h>
 * STANDARD: POSIX. O_PATH and O_TMPFILE are Linux-specific. read, write and
 *   close are in unistd.h.
 * SUMMARY: fcntl.h has open(), the system call that stdio's fopen is built
 *   on, the flags that say how a file is opened, and fcntl() to change a
 *   descriptor's flags afterwards.
 * PROVIDES:
 *   int open(path, flags)  or  open(path, flags, mode)     a descriptor, or -1
 *   flags: O_RDONLY, O_WRONLY, O_RDWR, O_CREAT, O_EXCL, O_TRUNC, O_APPEND,
 *          O_NONBLOCK, O_CLOEXEC, O_DIRECTORY, O_NOFOLLOW
 *   int fcntl(fd, cmd, ...)   F_GETFL / F_SETFL (status flags),
 *                             F_GETFD / F_SETFD (FD_CLOEXEC), F_DUPFD_CLOEXEC
 *   int openat(dirfd, path, flags)   relative to an open directory
 *   Linux: O_PATH, O_TMPFILE
 * NOTES:
 *   - open returns a small integer (0, 1 and 2 are stdin, stdout, stderr), or
 *     -1 with errno set. A leaked descriptor is never reclaimed until exit.
 *   - The mode argument is REQUIRED with O_CREAT (and is reduced by umask).
 *   - Add O_CLOEXEC to every open: it closes the descriptor in a program you
 *     start later, so you do not leak files into it.
 *   - read() on a descriptor may return fewer bytes than asked (a short read),
 *     0 at end of file, and -1 on error. Loop until you have what you need.
 *   - With O_NONBLOCK a read on an empty pipe returns -1 and errno EAGAIN
 *     instead of waiting.
 *   - /dev/tty is your terminal even when stdin, stdout and stderr are
 *     redirected. open("/dev/tty") fails when there is no terminal (cron, ssh
 *     without -t).
 *   - Prefer stdio (fopen) for text lines; use open() when you need the
 *     flags or are working with devices, pipes and sockets.
 * TOOL:
 *   - open("/dev/tty", O_RDONLY | O_CLOEXEC) then ioctl(TIOCGWINSZ) gives the
 *     terminal size when output is piped.
 *   - open + one read() for a small /proc file, into a buffer large enough
 *     that the whole file arrives in one call.
 * SEE: man 2 open, man 2 fcntl, man 2 openat
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
    char buffer[256];
    int fd = open("/proc/uptime", O_RDONLY | O_CLOEXEC);

    if (fd < 0) {
        perror("open /proc/uptime");
        return 1;
    }
    ssize_t got = read(fd, buffer, sizeof buffer - 1);
    close(fd);
    if (got > 0) {
        buffer[got] = '\0';
        buffer[strcspn(buffer, "\n")] = '\0';
        printf("read %zd bytes from /proc/uptime: \"%s\"\n", got, buffer);
    }

    fd = open("/no/such/file", O_RDONLY);
    printf("open of a missing file: fd %d, errno %d = %s\n", fd, errno, strerror(errno));

    fd = open("/etc/os-release", O_WRONLY);
    printf("open /etc/os-release for writing: fd %d, errno %d = %s\n", fd, errno, strerror(errno));

    int channel[2];
    if (pipe(channel) == 0) {
        int flags = fcntl(channel[0], F_GETFL);
        fcntl(channel[0], F_SETFL, flags | O_NONBLOCK);
        ssize_t result = read(channel[0], buffer, sizeof buffer);
        printf("non-blocking read of an empty pipe: %zd, errno %d = %s\n", result, errno, strerror(errno));
        close(channel[0]);
        close(channel[1]);
    }

    int tty = open("/dev/tty", O_RDONLY | O_CLOEXEC);
    printf("/dev/tty: %s\n", tty >= 0 ? "a controlling terminal exists" : "no controlling terminal here");
    if (tty >= 0) {
        close(tty);
    }
    return 0;
}
