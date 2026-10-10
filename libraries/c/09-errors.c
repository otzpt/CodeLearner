/*
 * TITLE: errno.h: error codes and messages
 * GROUP: Basics
 * USES: #include <errno.h>
 * STANDARD: ISO C defines errno and EDOM, ERANGE, EILSEQ. The rest (ENOENT,
 *   EACCES, EINVAL, EEXIST, EAGAIN, EPIPE ...) are POSIX. The numbers are
 *   Linux's; use the names.
 * SUMMARY: Many library and system calls return -1 or NULL on failure and set
 *   the global errno to say why. strerror turns the number into words.
 * PROVIDES:
 *   errno                        an int (a macro; per thread): the last error
 *   ENOENT    2  no such file or directory      EACCES  13  permission denied
 *   EEXIST   17  file exists                    ENOTDIR 20  not a directory
 *   EISDIR   21  is a directory                 EINVAL  22  invalid argument
 *   ERANGE   34  result out of range            EAGAIN  11  try again
 *   EPIPE    32  broken pipe                    EINTR    4  interrupted
 *   ENOTTY   25  not a terminal (wrong device)  ENOMEM  12  out of memory
 *   char *strerror(int errnum)   (string.h)  the message for a code
 *   void perror(const char *prefix)  (stdio.h)  prints "prefix: message" to stderr
 * NOTES:
 *   - errno is only meaningful right after a call that FAILED. A successful
 *     call may leave any value in it. Read it immediately, or save it.
 *   - To check a function that can legitimately succeed with any value, such
 *     as strtol, set errno = 0 BEFORE the call and test it after.
 *   - perror("what you tried") prints your text, a colon, and the message to
 *     stderr. strerror(errno) gives the message as a string.
 *   - strerror is not thread-safe on every system; strerror_r is the
 *     thread-safe form.
 *   - A /proc or /sys file may exist on one machine and not another (no
 *     battery on a desktop). Treat ENOENT as "not available", not as a crash.
 *   - Never compare errno to a number; compare to the name.
 * TOOL:
 *   - ENOENT after fopen: the field does not exist on this machine, so skip it.
 *   - EACCES: the field needs root. Show "(permission denied)" or skip it.
 *   - ENOTTY after ioctl(TIOCGWINSZ): output is not a terminal; fall back to 80.
 * SEE: man 3 errno, man 3 strerror, man 3 perror
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    FILE *file = fopen("/sys/class/power_supply/BAT0/capacity", "r");

    if (file == NULL) {
        int saved = errno; /* save it before another call can change it */
        printf("fopen failed: errno %d = %s\n", saved, strerror(saved));
        printf("ENOENT means \"not available here\": %s\n", saved == ENOENT ? "yes" : "no");
    } else {
        puts("this machine has a battery");
        fclose(file);
    }

    FILE *root_only = fopen("/etc/shadow", "r");
    if (root_only == NULL) {
        printf("/etc/shadow: %s\n", strerror(errno));
    } else {
        fclose(root_only);
    }

    const int codes[] = { ENOENT, EACCES, EEXIST, ENOTDIR, EISDIR, EINVAL, ERANGE, EAGAIN, EPIPE, ENOTTY, ENOMEM };
    for (size_t i = 0; i < sizeof codes / sizeof codes[0]; i++) {
        printf("%3d  %s\n", codes[i], strerror(codes[i]));
    }

    errno = 0;
    strtol("99999999999999999999", NULL, 10);
    printf("strtol overflow set errno to ERANGE: %s\n", errno == ERANGE ? "yes" : "no");
    return 0;
}
