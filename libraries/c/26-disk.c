/*
 * TITLE: sys/statvfs.h: disk usage and mounted filesystems
 * GROUP: System information
 * USES: #include <sys/statvfs.h>; read /proc/self/mounts
 * STANDARD: POSIX. /proc/self/mounts is Linux-specific (getmntent in
 *   mntent.h reads it for you; it is a glibc function).
 * SUMMARY: statvfs() reports the size and free space of the filesystem that
 *   holds a path. /proc/self/mounts lists what is mounted where.
 * PROVIDES:
 *   int statvfs(const char *path, struct statvfs *out)    0, or -1 with errno
 *   struct statvfs:
 *     unsigned long f_bsize, f_frsize    block size, fragment size (the unit)
 *     fsblkcnt_t f_blocks                total size in f_frsize units
 *     fsblkcnt_t f_bfree, f_bavail       free blocks: all / for ordinary users
 *     fsfilcnt_t f_files, f_ffree        inodes: total / free
 *     unsigned long f_namemax, f_flag    longest file name, mount flags (ST_RDONLY)
 *   int fstatvfs(fd, ...)                for an open file
 *   struct mntent *getmntent(FILE *)     (mntent.h) parse the mounts file
 * NOTES:
 *   - Multiply block counts by f_frsize (the fragment size), not f_bsize.
 *   - f_bfree is free space including the part reserved for root. f_bavail is
 *     free space a normal user can really use. df uses f_bavail.
 *   - used = (f_blocks - f_bfree) * f_frsize; df shows used / (used + avail).
 *   - Block counts can pass 4 billion: use unsigned long long, not int.
 *   - /proc/self/mounts has virtual filesystems too (proc, tmpfs, cgroup).
 *     Filter on the type (ext4, btrfs, xfs, vfat) to list real disks.
 *   - Snap and loop mounts show up as squashfs: usually skipped.
 *   - A network or removable drive can block statvfs for a long time.
 *   - Linux also has statfs() in sys/vfs.h, with f_type to identify the
 *     filesystem; statvfs is the portable one.
 * TOOL:
 *   - The "Disk" line: used and total of "/" (and any other real mount).
 *   - f_flag & ST_RDONLY to mark a read-only root.
 * SEE: man 3 statvfs, man 5 proc (search /proc/[pid]/mounts), man 3 getmntent
 */

#include <stdio.h>
#include <string.h>
#include <sys/statvfs.h>

int main(void)
{
    struct statvfs stats;

    if (statvfs("/", &stats) != 0) {
        perror("statvfs /");
        return 1;
    }
    double total = (double) stats.f_blocks * stats.f_frsize;
    double used = (double) (stats.f_blocks - stats.f_bfree) * stats.f_frsize;
    double available = (double) stats.f_bavail * stats.f_frsize;
    double shown_total = used + available;

    printf("/   %.1f GiB / %.1f GiB (%.0f%%)\n", used / 1073741824.0,
           shown_total / 1073741824.0, used / shown_total * 100.0);
    printf("raw filesystem size %.1f GiB; %.1f GiB is reserved for root\n",
           total / 1073741824.0, (double) (stats.f_bfree - stats.f_bavail) * stats.f_frsize / 1073741824.0);

    printf("longest file name %lu, inodes %llu of %llu free, read-only mount: %s\n", stats.f_namemax,
           (unsigned long long) stats.f_ffree, (unsigned long long) stats.f_files,
           (stats.f_flag & ST_RDONLY) ? "yes" : "no");

    FILE *mounts = fopen("/proc/self/mounts", "r");
    char line[1024];
    char device[256], where[256], type[64];

    puts("real filesystems:");
    if (mounts != NULL) {
        while (fgets(line, sizeof line, mounts) != NULL) {
            if (sscanf(line, "%255s %255s %63s", device, where, type) == 3
                && (strcmp(type, "ext4") == 0 || strcmp(type, "btrfs") == 0
                    || strcmp(type, "xfs") == 0 || strcmp(type, "vfat") == 0
                    || strcmp(type, "ntfs3") == 0 || strcmp(type, "f2fs") == 0)) {
                printf("  %-22s %-18s %s\n", device, where, type);
            }
        }
        fclose(mounts);
    }
    return 0;
}
