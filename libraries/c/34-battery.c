/*
 * TITLE: Battery level and charging state
 * GROUP: System information
 * USES: read the folders in /sys/class/power_supply
 * SUMMARY: Each power source is a folder in /sys/class/power_supply. A
 *   battery has type "Battery", a capacity from 0 to 100, and a status such as
 *   Charging, Discharging, Full or Not charging.
 * NOTES:
 *   - Desktops have no battery: the folder is empty, or holds only mains
 *     adapters (type "Mains"). That is the normal case, not an error.
 *   - Names vary: BAT0, BAT1, macsmc-battery. Look at the type file instead of
 *     guessing the name.
 *   - Some batteries report energy_now / energy_full (or charge_*) instead of
 *     capacity. Compute 100 * now / full.
 *   - A laptop with two batteries has two entries. Show both or add them up.
 * SEE: Documentation/ABI/testing/sysfs-class-power in the kernel
 */

#include <dirent.h>
#include <stdio.h>
#include <string.h>

static int read_line(const char *directory, const char *file_name, char *out, size_t size)
{
    char path[512];
    FILE *file;

    snprintf(path, sizeof path, "/sys/class/power_supply/%s/%s", directory, file_name);
    file = fopen(path, "r");
    if (file == NULL) {
        return 0;
    }
    if (fgets(out, (int) size, file) == NULL) {
        fclose(file);
        return 0;
    }
    fclose(file);
    out[strcspn(out, "\r\n")] = '\0';
    return 1;
}

int main(void)
{
    DIR *directory = opendir("/sys/class/power_supply");
    struct dirent *entry;
    int found = 0;

    if (directory == NULL) {
        puts("no power supply information here");
        return 0;
    }
    while ((entry = readdir(directory)) != NULL) {
        char type[32], capacity[16], status[32];

        if (entry->d_name[0] == '.' || !read_line(entry->d_name, "type", type, sizeof type)
            || strcmp(type, "Battery") != 0) {
            continue;
        }
        if (!read_line(entry->d_name, "capacity", capacity, sizeof capacity)) {
            strcpy(capacity, "?");
        }
        if (!read_line(entry->d_name, "status", status, sizeof status)) {
            strcpy(status, "unknown");
        }
        printf("%s: %s%% (%s)\n", entry->d_name, capacity, status);
        found = 1;
    }
    closedir(directory);
    if (!found) {
        puts("no battery on this machine");
    }
    return 0;
}
