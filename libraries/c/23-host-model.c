/*
 * TITLE: Machine model (vendor and product name)
 * GROUP: System information
 * USES: read the files in /sys/devices/virtual/dmi/id/ (or /proc/device-tree/model)
 * SUMMARY: On PCs the firmware describes the machine in DMI tables, and the
 *   kernel exposes them as small text files. This is the "Host" line.
 * NOTES:
 *   - Readable by everyone: sys_vendor, product_name, product_version,
 *     board_vendor, board_name, bios_vendor, bios_version.
 *   - Root only: product_serial, product_uuid, board_serial. Do not print
 *     serial numbers in a tool people paste into forums.
 *   - Virtual machines and cheap boards often put placeholders here, such as
 *     "To Be Filled By O.E.M.". Show them or skip them, but expect them.
 *   - On ARM boards (Raspberry Pi) there is no DMI: read
 *     /proc/device-tree/model, a string ended by a zero byte.
 *   - Each file ends with a newline: trim it.
 * SEE: man 5 sysfs
 */

#include <stdio.h>
#include <string.h>

/* Read a one-line text file into `out`, without the trailing newline. */
static int read_small_file(const char *path, char *out, size_t size)
{
    FILE *file = fopen(path, "r");

    if (file == NULL) {
        return 0;
    }
    if (fgets(out, (int) size, file) == NULL) {
        fclose(file);
        return 0;
    }
    fclose(file);
    out[strcspn(out, "\r\n")] = '\0';
    return out[0] != '\0';
}

int main(void)
{
    const char *names[] = { "sys_vendor", "product_name", "product_version",
                            "board_vendor", "board_name", "bios_version" };
    char path[128];
    char value[128];
    int any = 0;

    for (int i = 0; i < 6; i++) {
        snprintf(path, sizeof path, "/sys/devices/virtual/dmi/id/%s", names[i]);
        if (read_small_file(path, value, sizeof value)) {
            printf("%-16s %s\n", names[i], value);
            any = 1;
        }
    }
    if (!any && read_small_file("/proc/device-tree/model", value, sizeof value)) {
        printf("device-tree model %s\n", value);
        any = 1;
    }
    if (!any) {
        puts("no machine model available here");
    }
    return 0;
}
