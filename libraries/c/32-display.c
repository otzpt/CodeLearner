/*
 * TITLE: Monitors and resolution
 * GROUP: System information
 * USES: read the connector folders in /sys/class/drm (files status, modes, enabled)
 * SUMMARY: Every video output of a GPU is a "connector" folder named like
 *   card1-HDMI-A-1. status says connected or not, and modes lists the
 *   resolutions the monitor offers, best first.
 * NOTES:
 *   - modes is a list of "WIDTHxHEIGHT" lines, the monitor's preferred mode
 *     first. It is what the monitor SUPPORTS, not necessarily what is in use.
 *   - The mode actually in use, and the refresh rate, belong to the display
 *     server: ask the Wayland compositor (wl_output events, via
 *     libwayland-client), X11 (XRandR, via libXrandr or xcb-randr), or the
 *     kernel directly (libdrm). Shelling out to `xrandr` or `wlr-randr` is the
 *     quick way.
 *   - enabled says whether the connector is currently driving a monitor.
 *   - Connector names tell the cable: HDMI-A, DP (DisplayPort), eDP (a
 *     laptop's own panel), DVI-D, VGA.
 *   - A machine over SSH or in a container has no /sys/class/drm connectors.
 * SEE: Documentation/gpu/drm-kms.rst in the kernel
 */

#include <dirent.h>
#include <stdio.h>
#include <string.h>

static int first_line(const char *path, char *out, size_t size)
{
    FILE *file = fopen(path, "r");
    int ok = file != NULL && fgets(out, (int) size, file) != NULL;

    if (file != NULL) {
        fclose(file);
    }
    if (ok) {
        out[strcspn(out, "\r\n")] = '\0';
    }
    return ok;
}

int main(void)
{
    DIR *directory = opendir("/sys/class/drm");
    struct dirent *entry;
    int connected = 0;

    if (directory == NULL) {
        puts("no /sys/class/drm here");
        return 0;
    }
    while ((entry = readdir(directory)) != NULL) {
        char path[512], status[32], mode[64], enabled[32];
        const char *dash = strchr(entry->d_name, '-');

        if (strncmp(entry->d_name, "card", 4) != 0 || dash == NULL) {
            continue;
        }
        snprintf(path, sizeof path, "/sys/class/drm/%s/status", entry->d_name);
        if (!first_line(path, status, sizeof status) || strcmp(status, "connected") != 0) {
            continue;
        }
        snprintf(path, sizeof path, "/sys/class/drm/%s/modes", entry->d_name);
        if (!first_line(path, mode, sizeof mode)) {
            strcpy(mode, "unknown mode");
        }
        snprintf(path, sizeof path, "/sys/class/drm/%s/enabled", entry->d_name);
        if (!first_line(path, enabled, sizeof enabled)) {
            strcpy(enabled, "?");
        }
        printf("%-18s preferred mode %-10s (%s)\n", dash + 1, mode, enabled);
        connected++;
    }
    closedir(directory);
    if (connected == 0) {
        puts("no connected monitors found");
    }
    return 0;
}
