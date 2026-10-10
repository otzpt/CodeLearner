/*
 * TITLE: Number of installed packages
 * GROUP: System information
 * USES: read the package manager's database; #include <dirent.h>
 * SUMMARY: There is no single API. Each package manager keeps its own
 *   database, and "how many packages" means reading the right one. Count only
 *   the managers that exist on the machine.
 * NOTES:
 *   - Debian, Ubuntu, Mint: /var/lib/dpkg/status is a text file of blocks.
 *     Count the blocks whose line is "Status: install ok installed".
 *   - Arch: one directory per package in /var/lib/pacman/local. Count the
 *     sub-directories (a file called ALPM_DB_VERSION sits there too).
 *   - Flatpak: one directory per app in /var/lib/flatpak/app and in
 *     ~/.local/share/flatpak/app.
 *   - Snap: one .snap file per installed snap in /var/lib/snapd/snaps. That
 *     count includes runtimes such as core22 and snapd itself.
 *   - Fedora, openSUSE (rpm): the database is SQLite or Berkeley DB. Reading
 *     it needs librpm or sqlite3; the simple way is to run `rpm -qa` and
 *     count its lines.
 *   - The result is approximate: managers disagree on what counts as one.
 * SEE: man 5 deb-status, man 8 pacman (Arch), man 8 dpkg
 */

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Entries of `path` that satisfy `want_directories`. -1 if it cannot be opened. */
static int count_entries(const char *path, int want_directories)
{
    DIR *directory = opendir(path);
    struct dirent *entry;
    int count = 0;

    if (directory == NULL) {
        return -1;
    }
    while ((entry = readdir(directory)) != NULL) {
        if (entry->d_name[0] == '.') {
            continue;
        }
        if (want_directories ? entry->d_type == DT_DIR : entry->d_type != DT_DIR) {
            count++;
        }
    }
    closedir(directory);
    return count;
}

static int count_dpkg(void)
{
    FILE *file = fopen("/var/lib/dpkg/status", "r");
    char line[1024];
    int count = 0;

    if (file == NULL) {
        return -1;
    }
    while (fgets(line, sizeof line, file) != NULL) {
        if (strcmp(line, "Status: install ok installed\n") == 0) {
            count++;
        }
    }
    fclose(file);
    return count;
}

int main(void)
{
    int dpkg = count_dpkg();
    int pacman = count_entries("/var/lib/pacman/local", 1);
    int flatpak = count_entries("/var/lib/flatpak/app", 1);
    int snap = count_entries("/var/lib/snapd/snaps", 0);
    int shown = 0;

    if (dpkg >= 0) {
        printf("%d (dpkg)\n", dpkg);
        shown = 1;
    }
    if (pacman >= 0) {
        printf("%d (pacman)\n", pacman);
        shown = 1;
    }
    if (flatpak >= 0) {
        printf("%d (flatpak)\n", flatpak);
        shown = 1;
    }
    if (snap >= 0) {
        printf("%d (snap)\n", snap);
        shown = 1;
    }
    if (!shown) {
        puts("no known package manager database found");
    }
    return 0;
}
