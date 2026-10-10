/*
 * TITLE: sys/stat.h: file type, size, permissions and times
 * GROUP: Header reference: POSIX and Linux
 * USES: #include <sys/stat.h>
 * STANDARD: POSIX. statx() and the birth time are Linux-specific.
 * SUMMARY: stat() fills a struct with everything the filesystem knows about
 *   a file: its type, size, owner, permission bits and timestamps. It does
 *   not open the file.
 * PROVIDES:
 *   int stat(path, struct stat *)    follows symbolic links
 *   int lstat(path, struct stat *)   describes the link itself
 *   int fstat(fd, struct stat *)     for an open file descriptor
 *   struct stat: st_mode, st_size, st_uid, st_gid, st_nlink, st_ino, st_dev,
 *                st_mtime (modified), st_atime (accessed), st_ctime (changed)
 *   S_ISREG(m), S_ISDIR(m), S_ISLNK(m), S_ISCHR(m), S_ISBLK(m), S_ISSOCK(m)
 *   S_IRUSR, S_IWUSR, S_IXUSR, S_IRGRP, S_IROTH ...  permission bits
 *   int mkdir(path, mode); int chmod(path, mode); mode_t umask(mode_t)
 * NOTES:
 *   - Every function returns 0 on success and -1 on failure with errno set:
 *     ENOENT (missing), EACCES (a directory on the way is not searchable).
 *   - Test the type with the S_ISxxx macros, never with ==: st_mode also holds
 *     the permission bits.
 *   - stat follows a symlink, lstat does not. /etc/os-release is a symlink on
 *     Ubuntu: stat reports the file it points to, lstat reports the link.
 *   - Files under /proc and /sys report st_size 0 although they have content.
 *   - st_size of a file larger than 2 GiB needs a 64-bit off_t (default on
 *     64-bit Linux).
 *   - Checking with stat() and then opening is a race. To find out whether an
 *     optional file exists, fopen it and test for NULL.
 *   - Linux adds statx(), which also returns the file's creation time where
 *     the filesystem records it (needs _GNU_SOURCE).
 * TOOL:
 *   - S_ISDIR to count only directories (pacman local database, flatpak apps)
 *     when dirent's d_type is DT_UNKNOWN.
 *   - lstat to see that /bin/sh is a link and readlink to find what it is.
 *   - st_mtime of /var/lib/dpkg/status approximates "last package change".
 * SEE: man 2 stat, man 7 inode, man 2 statx
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

static const char *kind(mode_t mode)
{
    if (S_ISREG(mode)) {
        return "regular file";
    }
    if (S_ISDIR(mode)) {
        return "directory";
    }
    if (S_ISLNK(mode)) {
        return "symbolic link";
    }
    if (S_ISCHR(mode)) {
        return "character device";
    }
    if (S_ISBLK(mode)) {
        return "block device";
    }
    return "other";
}

/* rwxr-xr-x for the lowest nine permission bits. */
static void permissions(mode_t mode, char out[10])
{
    const char letters[] = "rwxrwxrwx";

    for (int i = 0; i < 9; i++) {
        out[i] = (mode & (1u << (8 - i))) ? letters[i] : '-';
    }
    out[9] = '\0';
}

static void describe(const char *path, int follow)
{
    struct stat info;
    char bits[10], when[32];

    if ((follow ? stat(path, &info) : lstat(path, &info)) != 0) {
        printf("%-22s %s: %s\n", path, follow ? "stat" : "lstat", strerror(errno));
        return;
    }
    permissions(info.st_mode, bits);
    {
        struct tm local;

        localtime_r(&info.st_mtime, &local);
        strftime(when, sizeof when, "%Y-%m-%d", &local);
    }
    printf("%-22s %-5s %-16s %s %4o size %-8lld modified %s\n", path, follow ? "stat" : "lstat", kind(info.st_mode),
           bits, (unsigned) (info.st_mode & 07777), (long long) info.st_size, when);
}

int main(void)
{
    describe("/etc/os-release", 1);
    describe("/etc/os-release", 0);
    describe("/proc/meminfo", 1);
    describe("/dev/null", 1);
    describe("/sys/class/net", 1);
    describe("/etc/shadow", 1);
    describe("/no/such/file", 1);
    return 0;
}
