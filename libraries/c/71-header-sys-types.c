/*
 * TITLE: sys/types.h: pid_t, uid_t, off_t, ssize_t and friends
 * GROUP: Header reference: POSIX and Linux
 * USES: #include <sys/types.h>
 * STANDARD: POSIX. size_t and time_t are ISO C (stddef.h, time.h); the rest
 *   are POSIX. Most POSIX headers include this one for you.
 * SUMMARY: sys/types.h names the integer types that system calls use, so a
 *   program says what a number MEANS (a process id, a file size) and the
 *   system picks the width.
 * PROVIDES:
 *   pid_t     process id (signed)           uid_t, gid_t   user and group ids
 *   off_t     file offset or size, signed   ssize_t        size or -1 for an error
 *   mode_t    file permission bits          dev_t, ino_t   device, inode ids
 *   size_t    object size, unsigned         time_t         seconds since 1970
 *   key_t, id_t, blksize_t, blkcnt_t, nlink_t, useconds_t
 * NOTES:
 *   - Do not assume pid_t is int. To print one, cast to a type that is certain
 *     to be wide enough: printf("%d", (int) pid) is fine for pids on Linux;
 *     (long) with %ld, or (intmax_t) with %jd, is always correct.
 *   - size_t is UNSIGNED and ssize_t is signed. Storing the -1 that read()
 *     returns in a size_t gives a huge positive number. Keep the result of
 *     read, readlink and recv in an ssize_t and test for < 0.
 *   - off_t is 64 bits on 64-bit Linux. On 32-bit systems it is 32 bits unless
 *     you compile with -D_FILE_OFFSET_BITS=64, and then files over 2 GiB
 *     break. Always use off_t for sizes, never int.
 *   - mode_t is shown in octal: printf("%o", mode) gives 644, not 420.
 *   - uid 0 is root. A tool run as root sees more (some /sys and /proc files)
 *     and should say so.
 * TOOL:
 *   - pid_t for walking parent processes in /proc.
 *   - uid_t from getuid() into getpwuid().
 *   - ssize_t from readlink and read; off_t from stat's st_size.
 * SEE: man 0 sys_types.h, man 7 feature_test_macros
 */

#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
    printf("sizeof pid_t   %zu, uid_t %zu, gid_t %zu\n", sizeof(pid_t), sizeof(uid_t), sizeof(gid_t));
    printf("sizeof off_t   %zu, ssize_t %zu, size_t %zu\n", sizeof(off_t), sizeof(ssize_t), sizeof(size_t));
    printf("sizeof mode_t  %zu, dev_t %zu, ino_t %zu\n", sizeof(mode_t), sizeof(dev_t), sizeof(ino_t));

    pid_t pid = getpid();
    printf("pid printed three safe ways: %d, %ld, %jd\n", (int) pid, (long) pid, (intmax_t) pid);

    ssize_t result = readlink("/no/such/link", (char[1]){ 0 }, 1);
    size_t wrong = (size_t) result;
    printf("readlink failed: ssize_t = %zd, but stored in a size_t it is %zu\n", result, wrong);

    mode_t mode = 0644;
    printf("mode_t 0644 printed with %%o: %o, with %%d: %d\n", (unsigned) mode, (int) mode);
    printf("running as root: %s\n", getuid() == 0 ? "yes" : "no");
    return 0;
}
