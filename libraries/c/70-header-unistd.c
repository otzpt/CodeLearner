/*
 * TITLE: unistd.h: process ids, system limits and low-level file calls
 * GROUP: Header reference: POSIX and Linux
 * USES: #include <unistd.h>
 * STANDARD: POSIX, not ISO C. Compiled with -std=c11 you must define
 *   _POSIX_C_SOURCE 200809L before any include, or the names are hidden (see
 *   the notes). _SC_NPROCESSORS_ONLN and _SC_PHYS_PAGES are glibc extensions,
 *   not POSIX.
 * SUMMARY: unistd.h is the doorway to the operating system: who the process
 *   is, how the machine is configured, and the file-descriptor calls that
 *   stdio is built on.
 * PROVIDES:
 *   pid_t getpid(void), getppid(void)      this process and its parent
 *   uid_t getuid(void), geteuid(void); gid_t getgid(void)
 *   int gethostname(char *name, size_t size)
 *   long sysconf(int name)       _SC_PAGE_SIZE, _SC_CLK_TCK, _SC_OPEN_MAX,
 *                                _SC_NPROCESSORS_ONLN, _SC_PHYS_PAGES (glibc)
 *   char *getcwd(char *buf, size_t size); int chdir(path)
 *   ssize_t readlink(path, buf, size)      where a symlink points; no zero byte
 *   int access(path, mode)                 R_OK, W_OK, X_OK, F_OK
 *   ssize_t read(fd, buf, count), write(fd, buf, count); int close(fd)
 *   int isatty(fd)                         1 if fd is a terminal
 *   STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO   0, 1, 2
 *   pid_t fork(void); int execvp(...), pipe(fd[2]), dup2(old, new); _exit(status)
 * NOTES:
 *   - Under -std=c11 this fails: "implicit declaration of function
 *     'readlink'". Fix with  #define _POSIX_C_SOURCE 200809L  as the very first
 *     line, or compile with -std=gnu11.
 *   - sysconf returns -1 for "no limit" or "not supported"; errno is set
 *     only for an error. Check for -1 before using the number.
 *   - readlink does not add a zero byte and does not tell you when the buffer
 *     was too small: pass size - 1 and terminate it yourself.
 *   - read and write may transfer fewer bytes than asked; loop until done.
 *   - access() then open() is a race (the file can change between them). To
 *     test "can I read this optional file", just try fopen and check NULL.
 *   - getlogin() needs a controlling terminal and fails in many places: use
 *     getpwuid(getuid()) (see pwd.h).
 * TOOL:
 *   - gethostname for the user@host title; getuid with getpwuid for the name.
 *   - sysconf(_SC_NPROCESSORS_ONLN) for the thread count, _SC_PAGE_SIZE for
 *     converting page counts, _SC_CLK_TCK for CPU times in /proc/<pid>/stat.
 *   - readlink("/proc/self/exe") for the program's own path;
 *     readlink("/proc/<pid>/exe") names another process.
 *   - isatty(STDOUT_FILENO) decides whether to print colour.
 * SEE: man 3 sysconf, man 2 readlink, man 3 isatty, man 7 feature_test_macros
 */

#include <limits.h>
#include <stdio.h>
#include <unistd.h>

int main(void)
{
    char host[HOST_NAME_MAX + 1];
    char path[PATH_MAX];
    char cwd[PATH_MAX];

    printf("pid %d, parent %d, uid %d, gid %d\n", (int) getpid(), (int) getppid(), (int) getuid(), (int) getgid());

    if (gethostname(host, sizeof host) == 0) {
        printf("hostname      %s\n", host);
    }
    printf("page size     %ld bytes\n", sysconf(_SC_PAGE_SIZE));
    printf("clock ticks   %ld per second\n", sysconf(_SC_CLK_TCK));
    printf("online CPUs   %ld\n", sysconf(_SC_NPROCESSORS_ONLN));
    printf("open files    %ld per process (soft limit)\n", sysconf(_SC_OPEN_MAX));

    ssize_t length = readlink("/proc/self/exe", path, sizeof path - 1);
    if (length > 0) {
        path[length] = '\0';
        printf("this program  %s\n", path);
    }
    if (getcwd(cwd, sizeof cwd) != NULL) {
        printf("working dir   %s\n", cwd);
    }

    printf("access /etc/os-release readable: %s\n", access("/etc/os-release", R_OK) == 0 ? "yes" : "no");
    printf("access /etc/shadow readable:     %s\n", access("/etc/shadow", R_OK) == 0 ? "yes" : "no");
    printf("isatty: stdin %d, stdout %d, stderr %d\n", isatty(STDIN_FILENO), isatty(STDOUT_FILENO),
           isatty(STDERR_FILENO));
    return 0;
}
