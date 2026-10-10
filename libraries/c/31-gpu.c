/*
 * TITLE: Graphics card (vendor, model, driver)
 * GROUP: System information
 * USES: read the card folders in /sys/class/drm, then look the ids up in pci.ids
 * SUMMARY: Each GPU the kernel drives appears as /sys/class/drm/cardN. Its
 *   device/ folder has the PCI vendor and device ids as hex numbers, and a
 *   symlink to the kernel driver. The ids become names through the pci.ids
 *   database.
 * NOTES:
 *   - Only list cardN, not cardN-HDMI-A-1 (those are the connectors, see the
 *     display page): skip names containing a '-'.
 *   - vendor 0x10de is NVIDIA, 0x1002 AMD, 0x8086 Intel. The file holds "0x" and
 *     hex digits with a newline.
 *   - The driver is the last part of the device/driver symlink: nvidia,
 *     amdgpu, i915, xe, nouveau, radeon.
 *   - pci.ids is a text file of "vendor  name" lines and, below each,
 *     tab-indented "device  name" lines. It is installed by pciutils-ids or
 *     hwdata (/usr/share/misc/pci.ids or /usr/share/hwdata/pci.ids). Fall
 *     back to the raw ids if neither exists.
 *   - A laptop shows two cards (integrated and discrete). device/boot_vga
 *     reads 1 on the one the firmware used at boot.
 *   - The libraries that do this for you are libpci (pciutils) and libdrm.
 * SEE: man 8 lspci, Documentation/gpu/drm-uapi.rst in the kernel
 */

#include <dirent.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int read_hex(const char *path, unsigned *value)
{
    FILE *file = fopen(path, "r");
    int ok = file != NULL && fscanf(file, "%x", value) == 1;

    if (file != NULL) {
        fclose(file);
    }
    return ok;
}

/* Fill vendor_name and device_name from pci.ids; leave them empty if unknown. */
static void lookup_names(unsigned vendor, unsigned device, char *vendor_name, char *device_name, size_t size)
{
    const char *paths[] = { "/usr/share/misc/pci.ids", "/usr/share/hwdata/pci.ids" };
    char line[512];
    int in_vendor = 0;

    vendor_name[0] = device_name[0] = '\0';
    for (int i = 0; i < 2; i++) {
        FILE *file = fopen(paths[i], "r");
        if (file == NULL) {
            continue;
        }
        while (fgets(line, sizeof line, file) != NULL) {
            unsigned id;
            int offset;

            line[strcspn(line, "\n")] = '\0';
            if (line[0] == '#' || line[0] == '\0') {
                continue;
            }
            if (line[0] != '\t') {                       /* a vendor line */
                if (in_vendor) {
                    break;                               /* its block ended */
                }
                if (sscanf(line, "%4x %n", &id, &offset) == 1 && id == vendor) {
                    snprintf(vendor_name, size, "%s", line + offset);
                    in_vendor = 1;
                }
            } else if (in_vendor && line[1] != '\t'
                       && sscanf(line + 1, "%4x %n", &id, &offset) == 1 && id == device) {
                snprintf(device_name, size, "%s", line + 1 + offset);
                break;
            }
        }
        fclose(file);
        if (vendor_name[0] != '\0') {
            return;
        }
    }
}

int main(void)
{
    DIR *directory = opendir("/sys/class/drm");
    struct dirent *entry;
    int found = 0;

    if (directory == NULL) {
        puts("no /sys/class/drm here");
        return 0;
    }
    while ((entry = readdir(directory)) != NULL) {
        char path[512], target[512], vendor_name[128], device_name[160];
        unsigned vendor, device;
        ssize_t length;

        if (strncmp(entry->d_name, "card", 4) != 0 || strchr(entry->d_name, '-') != NULL) {
            continue;
        }
        snprintf(path, sizeof path, "/sys/class/drm/%s/device/vendor", entry->d_name);
        if (!read_hex(path, &vendor)) {
            continue;
        }
        snprintf(path, sizeof path, "/sys/class/drm/%s/device/device", entry->d_name);
        if (!read_hex(path, &device)) {
            continue;
        }
        lookup_names(vendor, device, vendor_name, device_name, sizeof vendor_name);

        snprintf(path, sizeof path, "/sys/class/drm/%s/device/driver", entry->d_name);
        length = readlink(path, target, sizeof target - 1);
        target[length > 0 ? length : 0] = '\0';

        printf("%s: %s %s\n", entry->d_name, vendor_name[0] ? vendor_name : "unknown vendor",
               device_name[0] ? device_name : "unknown device");
        printf("       ids 0x%04x:0x%04x, driver %s\n", vendor, device,
               length > 0 ? basename(target) : "(none)");
        found = 1;
    }
    closedir(directory);
    if (!found) {
        puts("no graphics devices found");
    }
    return 0;
}
