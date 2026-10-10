/*
 * TITLE: Operating system name and version (os-release)
 * GROUP: System information
 * USES: read /etc/os-release (fallback /usr/lib/os-release)
 * SUMMARY: Every modern Linux distribution ships a file of KEY=value lines
 *   that names itself. This is where "Ubuntu 26.04 LTS" comes from.
 * NOTES:
 *   - Keys you will want: PRETTY_NAME (display name), NAME, ID (short,
 *     lower-case, like "ubuntu"), ID_LIKE (what it derives from),
 *     VERSION_ID, VERSION_CODENAME, and ANSI_COLOR (the colour its logo uses).
 *   - Values may be wrapped in double or single quotes: strip them.
 *   - /etc/os-release may be a symlink to /usr/lib/os-release. Try the first
 *     path and fall back to the second.
 *   - Not every key is present on every distribution. Print only what exists.
 *   - There is no library call: it is a file format, specified by systemd.
 * SEE: man 5 os-release
 */

#include <stdio.h>
#include <string.h>

/* Look up `key` in os-release. Returns 1 and fills `out` if found. */
static int os_release(const char *key, char *out, size_t size)
{
    const char *paths[] = { "/etc/os-release", "/usr/lib/os-release" };
    size_t key_length = strlen(key);

    for (int i = 0; i < 2; i++) {
        FILE *file = fopen(paths[i], "r");
        char line[512];

        if (file == NULL) {
            continue;
        }
        while (fgets(line, sizeof line, file) != NULL) {
            if (strncmp(line, key, key_length) != 0 || line[key_length] != '=') {
                continue;
            }
            char *value = line + key_length + 1;
            value[strcspn(value, "\r\n")] = '\0';

            size_t length = strlen(value);
            if (length >= 2 && (value[0] == '"' || value[0] == '\'')
                && value[length - 1] == value[0]) {
                value[length - 1] = '\0';
                value++;
            }
            snprintf(out, size, "%s", value);
            fclose(file);
            return 1;
        }
        fclose(file);
    }
    return 0;
}

int main(void)
{
    const char *keys[] = { "PRETTY_NAME", "NAME", "ID", "ID_LIKE", "VERSION_ID",
                           "VERSION_CODENAME", "ANSI_COLOR" };
    char value[128];

    for (int i = 0; i < 7; i++) {
        if (os_release(keys[i], value, sizeof value)) {
            printf("%-17s %s\n", keys[i], value);
        } else {
            printf("%-17s (not set)\n", keys[i]);
        }
    }
    return 0;
}
