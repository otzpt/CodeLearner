/*
 * TITLE: dirent.h: listing a directory
 * GROUP: Basics
 * USES: #include <dirent.h>
 * STANDARD: POSIX, not ISO C. scandir() and the d_type field are common
 *   extensions (glibc/BSD) and are in POSIX.1-2008 only in part.
 * SUMMARY: opendir / readdir / closedir walk the entries of a directory.
 *   Counting entries is how a package count or a list of network devices is
 *   made.
 * PROVIDES:
 *   DIR *opendir(const char *path)           NULL on failure
 *   struct dirent *readdir(DIR *d)           next entry, NULL at the end
 *   int closedir(DIR *d)
 *   struct dirent: d_name (the file name), d_type (DT_DIR, DT_REG, DT_LNK,
 *                  DT_UNKNOWN), d_ino (inode number)
 *   int scandir(path, &list, filter, compare)   all entries, sorted, in one call
 *   int alphasort(const struct dirent **, const struct dirent **)
 * NOTES:
 *   - Every directory yields "." and ".." first; skip them.
 *   - Entries come in no particular order. Sort them if order matters
 *     (scandir with alphasort does it for you; free each entry and the list).
 *   - d_type tells file from directory without a second call, but it can be
 *     DT_UNKNOWN on some filesystems. Fall back to lstat() then (sys/stat.h).
 *   - d_type describes the link itself: a symlink to a directory is DT_LNK.
 *     Under /sys most "directories" are symlinks, so test with stat().
 *   - opendir returns NULL for a missing or unreadable directory, with errno
 *     set (ENOENT, EACCES, ENOTDIR).
 *   - readdir returns NULL both at the end AND on error. Set errno = 0
 *     before the loop and test it afterwards to tell them apart.
 *   - The pointer from readdir is only good until the next readdir call.
 * TOOL:
 *   - Count installed packages: entries in /var/lib/pacman/local,
 *     /var/lib/flatpak/app.
 *   - List network interfaces (/sys/class/net) and GPUs (/sys/class/drm).
 *   - Find the battery by walking /sys/class/power_supply.
 * SEE: man 3 readdir, man 3 scandir
 */

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int count_entries(const char *path)
{
    DIR *directory = opendir(path);
    struct dirent *entry;
    int count = 0;

    if (directory == NULL) {
        return -1;
    }
    while ((entry = readdir(directory)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            count++;
        }
    }
    closedir(directory);
    return count;
}

int main(void)
{
    const char *paths[] = { "/sys/class/net", "/proc/self/fd", "/no/such/directory" };

    for (int i = 0; i < 3; i++) {
        int count = count_entries(paths[i]);
        if (count < 0) {
            printf("%-20s cannot open\n", paths[i]);
        } else {
            printf("%-20s %d entries\n", paths[i], count);
        }
    }

    /* scandir: every entry, sorted, in one call. d_type says what each one is. */
    struct dirent **list;
    int found = scandir("/sys/class/net", &list, NULL, alphasort);

    for (int i = 0; i < found; i++) {
        if (list[i]->d_name[0] != '.') {
            const char *type = list[i]->d_type == DT_DIR ? "directory"
                               : list[i]->d_type == DT_LNK ? "symlink (stat it to learn the target's type)"
                               : list[i]->d_type == DT_REG ? "file" : "unknown";
            printf("sorted: %-10s %s\n", list[i]->d_name, type);
        }
        free(list[i]);
    }
    if (found >= 0) {
        free(list);
    }
    return 0;
}
