/*
 * TITLE: limits.h: type limits and system limits
 * GROUP: Header reference: standard C
 * USES: #include <limits.h>
 * STANDARD: ISO C for CHAR_BIT and the INT, LONG and CHAR limits. PATH_MAX,
 *   NAME_MAX, HOST_NAME_MAX and LOGIN_NAME_MAX are POSIX.
 * SUMMARY: limits.h holds the largest and smallest values of the integer
 *   types, and the system's own size limits for things like file paths.
 * PROVIDES:
 *   CHAR_BIT                 bits in a char (8)
 *   INT_MIN, INT_MAX, UINT_MAX                    int limits
 *   LONG_MIN, LONG_MAX, ULONG_MAX, LLONG_MAX      long and long long limits
 *   SHRT_MAX, SCHAR_MAX, UCHAR_MAX                small types
 *   POSIX: PATH_MAX (4096 on Linux), NAME_MAX (255), HOST_NAME_MAX (64),
 *          LOGIN_NAME_MAX (256), SSIZE_MAX, IOV_MAX
 * NOTES:
 *   - For fixed-width types use stdint.h (INT32_MAX, UINT64_MAX): INT_MAX
 *     changes between machines, INT32_MAX does not.
 *   - PATH_MAX is the length of the longest path you can pass to a system
 *     call, including the zero byte. A buffer of PATH_MAX is the right size
 *     for readlink and realpath results. Some filesystems allow longer
 *     names, so do not assume it for data you only store.
 *   - A POSIX limit may be absent from limits.h when the system does not
 *     fix it; then ask at run time with sysconf or pathconf.
 *   - Check before you add or multiply: INT_MAX + 1 is undefined behaviour
 *     for a signed int. Compare against the limit first.
 *   - Plain char can be signed or unsigned depending on the machine
 *     (CHAR_MIN tells you). Cast to unsigned char before ctype functions.
 * TOOL:
 *   - char path[PATH_MAX] for readlink("/proc/self/exe") and realpath.
 *   - char host[HOST_NAME_MAX + 1] for gethostname.
 *   - char user[LOGIN_NAME_MAX] when you need a login name buffer.
 * SEE: man 0 limits.h, man 3 sysconf
 */

#include <limits.h>
#include <stdio.h>
#include <unistd.h>

/* True when a + b would overflow an int. Test BEFORE adding: the overflow itself is undefined. */
static int add_overflows(int a, int b)
{
    return (b > 0 && a > INT_MAX - b) || (b < 0 && a < INT_MIN - b);
}

int main(void)
{
    char path[PATH_MAX];
    char host[HOST_NAME_MAX + 1];
    ssize_t length = readlink("/proc/self/exe", path, sizeof path - 1);

    printf("CHAR_BIT        %d\n", CHAR_BIT);
    printf("INT_MAX         %d\n", INT_MAX);
    printf("LONG_MAX        %ld\n", LONG_MAX);
    printf("UCHAR_MAX       %d\n", UCHAR_MAX);
    printf("PATH_MAX        %d\n", PATH_MAX);
    printf("NAME_MAX        %d\n", NAME_MAX);
    printf("HOST_NAME_MAX   %d\n", HOST_NAME_MAX);

    if (length > 0) {
        path[length] = '\0';
        printf("the path of this program uses %zd of the %d characters allowed\n", length, PATH_MAX - 1);
    }
    if (gethostname(host, sizeof host) == 0) {
        host[HOST_NAME_MAX] = '\0';
        printf("hostname %s fits in %d\n", host, HOST_NAME_MAX + 1);
    }
    printf("INT_MAX + 1 overflows: %s; 1 + 2 overflows: %s\n",
           add_overflows(INT_MAX, 1) ? "yes" : "no", add_overflows(1, 2) ? "yes" : "no");
    return 0;
}
