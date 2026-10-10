/*
 * TITLE: A complete mini fastfetch
 * GROUP: Putting it together
 * USES: everything on the System information and Terminal pages
 * SUMMARY: One program that gathers the fields, builds a "Label: value" list,
 *   and prints it next to a logo. Each field is a small function that fills a
 *   string, so adding a field means adding one function and one line.
 * NOTES:
 *   - Each field function returns 0 when the information does not exist on
 *     this machine; the printing loop then leaves the line out. Never print
 *     "(null)" or a half-filled line.
 *   - Every read has a bounded buffer and every parse checks its result.
 *   - Colour comes from ANSI_COLOR in os-release, with a default.
 *   - Compile it on its own to try it:
 *       cc -std=gnu11 -Wall -Wextra minifetch.c -o minifetch
 *   - fastfetch itself does the same job with many more sources, the optional
 *     libraries (libpci, libdrm, wayland, xcb) for things sysfs cannot say,
 *     and a configuration file for which fields to show.
 * SEE: the pages above, and https://github.com/fastfetch-cli/fastfetch
 */

#define _GNU_SOURCE

#include <arpa/inet.h>
#include <dirent.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <netinet/in.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/statvfs.h>
#include <sys/utsname.h>
#include <time.h>
#include <unistd.h>

#define FIELD 160

/* ---- small readers ------------------------------------------------------ */

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
    return ok && out[0] != '\0';
}

/* "key<sep> value" lines, as in /proc/meminfo and /proc/cpuinfo. */
static int file_value(const char *path, const char *key, char *out, size_t size)
{
    FILE *file = fopen(path, "r");
    char line[512];
    size_t length = strlen(key);

    if (file == NULL) {
        return 0;
    }
    while (fgets(line, sizeof line, file) != NULL) {
        if (strncmp(line, key, length) == 0 && (line[length] == ':' || line[length] == ' ' || line[length] == '\t')) {
            char *value = strchr(line, ':');
            if (value != NULL) {
                value++;
                while (*value == ' ' || *value == '\t') {
                    value++;
                }
                value[strcspn(value, "\r\n")] = '\0';
                snprintf(out, size, "%s", value);
                fclose(file);
                return 1;
            }
        }
    }
    fclose(file);
    return 0;
}

static int os_release(const char *key, char *out, size_t size)
{
    const char *paths[] = { "/etc/os-release", "/usr/lib/os-release" };
    size_t length = strlen(key);

    for (int i = 0; i < 2; i++) {
        FILE *file = fopen(paths[i], "r");
        char line[512];

        if (file == NULL) {
            continue;
        }
        while (fgets(line, sizeof line, file) != NULL) {
            if (strncmp(line, key, length) == 0 && line[length] == '=') {
                char *value = line + length + 1;
                size_t n;

                value[strcspn(value, "\r\n")] = '\0';
                n = strlen(value);
                if (n >= 2 && (value[0] == '"' || value[0] == '\'') && value[n - 1] == value[0]) {
                    value[n - 1] = '\0';
                    value++;
                }
                snprintf(out, size, "%s", value);
                fclose(file);
                return 1;
            }
        }
        fclose(file);
    }
    return 0;
}

static long meminfo_kb(const char *key)
{
    char text[64];
    long value;

    if (file_value("/proc/meminfo", key, text, sizeof text) && sscanf(text, "%ld", &value) == 1) {
        return value;
    }
    return -1;
}

/* ---- fields ------------------------------------------------------------- */

static int field_os(char *out, size_t size)
{
    char name[96], arch[96] = "";
    struct utsname u;

    if (!os_release("PRETTY_NAME", name, sizeof name)) {
        return 0;
    }
    if (uname(&u) == 0) {
        snprintf(arch, sizeof arch, " %s", u.machine);
    }
    snprintf(out, size, "%s%s", name, arch);
    return 1;
}

static int field_host(char *out, size_t size)
{
    char vendor[64] = "", product[64] = "";

    first_line("/sys/devices/virtual/dmi/id/sys_vendor", vendor, sizeof vendor);
    if (!first_line("/sys/devices/virtual/dmi/id/product_name", product, sizeof product)) {
        return first_line("/proc/device-tree/model", out, size);
    }
    snprintf(out, size, "%s", product);
    return 1;
}

static int field_kernel(char *out, size_t size)
{
    struct utsname u;

    if (uname(&u) != 0) {
        return 0;
    }
    snprintf(out, size, "%s %s", u.sysname, u.release);
    return 1;
}

static int field_uptime(char *out, size_t size)
{
    struct timespec boot;
    long seconds, days, hours, minutes;
    size_t used = 0;

    if (clock_gettime(CLOCK_BOOTTIME, &boot) != 0) {
        return 0;
    }
    seconds = (long) boot.tv_sec;
    days = seconds / 86400;
    hours = seconds % 86400 / 3600;
    minutes = seconds % 3600 / 60;
    out[0] = '\0';
    if (days > 0) {
        used += (size_t) snprintf(out + used, size - used, "%ld day%s, ", days, days == 1 ? "" : "s");
    }
    if (hours > 0 && used < size) {
        used += (size_t) snprintf(out + used, size - used, "%ld hour%s, ", hours, hours == 1 ? "" : "s");
    }
    if (used < size) {
        snprintf(out + used, size - used, "%ld min%s", minutes, minutes == 1 ? "" : "s");
    }
    return 1;
}

static int field_packages(char *out, size_t size)
{
    FILE *file = fopen("/var/lib/dpkg/status", "r");
    char line[1024];
    int count = 0;

    if (file == NULL) {
        return 0;
    }
    while (fgets(line, sizeof line, file) != NULL) {
        if (strcmp(line, "Status: install ok installed\n") == 0) {
            count++;
        }
    }
    fclose(file);
    snprintf(out, size, "%d (dpkg)", count);
    return 1;
}

static int field_shell(char *out, size_t size)
{
    const char *shell = getenv("SHELL");
    const char *slash;

    if (shell == NULL || shell[0] == '\0') {
        return 0;
    }
    slash = strrchr(shell, '/');
    snprintf(out, size, "%s", slash != NULL ? slash + 1 : shell);
    return 1;
}

static int field_desktop(char *out, size_t size)
{
    const char *desktop = getenv("XDG_CURRENT_DESKTOP");
    const char *session = getenv("XDG_SESSION_TYPE");

    if (desktop == NULL || desktop[0] == '\0') {
        return 0;
    }
    snprintf(out, size, "%s (%s)", desktop, session != NULL ? session : "unknown session");
    return 1;
}

static int field_resolution(char *out, size_t size)
{
    DIR *directory = opendir("/sys/class/drm");
    struct dirent *entry;
    size_t used = 0;

    if (directory == NULL) {
        return 0;
    }
    out[0] = '\0';
    while ((entry = readdir(directory)) != NULL) {
        char path[512], status[32], mode[64];

        if (strncmp(entry->d_name, "card", 4) != 0 || strchr(entry->d_name, '-') == NULL) {
            continue;
        }
        snprintf(path, sizeof path, "/sys/class/drm/%s/status", entry->d_name);
        if (!first_line(path, status, sizeof status) || strcmp(status, "connected") != 0) {
            continue;
        }
        snprintf(path, sizeof path, "/sys/class/drm/%s/modes", entry->d_name);
        if (first_line(path, mode, sizeof mode) && used + strlen(mode) + 3 < size) {
            used += (size_t) snprintf(out + used, size - used, "%s%s", used > 0 ? ", " : "", mode);
        }
    }
    closedir(directory);
    return used > 0;
}

static int field_cpu(char *out, size_t size)
{
    char model[128], khz[32];
    long threads = sysconf(_SC_NPROCESSORS_ONLN);
    size_t used;

    if (!file_value("/proc/cpuinfo", "model name", model, sizeof model)
        && !file_value("/proc/cpuinfo", "Model", model, sizeof model)) {
        return 0;
    }
    used = (size_t) snprintf(out, size, "%s (%ld)", model, threads);
    if (first_line("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq", khz, sizeof khz) && used < size) {
        snprintf(out + used, size - used, " @ %.2f GHz", atof(khz) / 1000000.0);
    }
    return 1;
}

static int read_hex(const char *path, unsigned *value)
{
    char text[32];

    return first_line(path, text, sizeof text) && sscanf(text, "%x", value) == 1;
}

/* First graphics card, named from pci.ids when that file exists. */
static int field_gpu(char *out, size_t size)
{
    unsigned vendor, device;
    const char *ids[] = { "/usr/share/misc/pci.ids", "/usr/share/hwdata/pci.ids" };
    char line[512], vendor_name[96] = "", device_name[128] = "";

    if (!read_hex("/sys/class/drm/card0/device/vendor", &vendor)
        && !read_hex("/sys/class/drm/card1/device/vendor", &vendor)) {
        return 0;
    }
    if (!read_hex("/sys/class/drm/card0/device/device", &device)
        && !read_hex("/sys/class/drm/card1/device/device", &device)) {
        return 0;
    }
    for (int i = 0; i < 2 && vendor_name[0] == '\0'; i++) {
        FILE *file = fopen(ids[i], "r");
        int in_vendor = 0;

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
            if (line[0] != '\t') {
                if (in_vendor) {
                    break;
                }
                if (sscanf(line, "%4x %n", &id, &offset) == 1 && id == vendor) {
                    snprintf(vendor_name, sizeof vendor_name, "%s", line + offset);
                    in_vendor = 1;
                }
            } else if (in_vendor && line[1] != '\t' && sscanf(line + 1, "%4x %n", &id, &offset) == 1
                       && id == device) {
                snprintf(device_name, sizeof device_name, "%s", line + 1 + offset);
                break;
            }
        }
        fclose(file);
    }
    if (device_name[0] != '\0') {
        snprintf(out, size, "%s", device_name);
    } else {
        snprintf(out, size, "PCI %04x:%04x", vendor, device);
    }
    return 1;
}

static int field_memory(char *out, size_t size)
{
    long total = meminfo_kb("MemTotal"), available = meminfo_kb("MemAvailable");

    if (total <= 0 || available < 0) {
        return 0;
    }
    snprintf(out, size, "%.2f GiB / %.2f GiB (%ld%%)", (double) (total - available) / 1048576.0,
             (double) total / 1048576.0, (total - available) * 100 / total);
    return 1;
}

static int field_swap(char *out, size_t size)
{
    long total = meminfo_kb("SwapTotal"), free_kb = meminfo_kb("SwapFree");

    if (total <= 0 || free_kb < 0) {
        return 0;
    }
    snprintf(out, size, "%.2f GiB / %.2f GiB (%ld%%)", (double) (total - free_kb) / 1048576.0,
             (double) total / 1048576.0, (total - free_kb) * 100 / total);
    return 1;
}

static int field_disk(char *out, size_t size)
{
    struct statvfs stats;
    double used, shown_total;

    if (statvfs("/", &stats) != 0 || stats.f_blocks == 0) {
        return 0;
    }
    used = (double) (stats.f_blocks - stats.f_bfree) * stats.f_frsize;
    shown_total = used + (double) stats.f_bavail * stats.f_frsize;
    snprintf(out, size, "%.1f GiB / %.1f GiB (%.0f%%) /", used / 1073741824.0, shown_total / 1073741824.0,
             used / shown_total * 100.0);
    return 1;
}

static int field_ip(char *out, size_t size)
{
    struct ifaddrs *list, *item;
    int found = 0;

    if (getifaddrs(&list) != 0) {
        return 0;
    }
    for (item = list; item != NULL && !found; item = item->ifa_next) {
        if (item->ifa_addr != NULL && item->ifa_addr->sa_family == AF_INET
            && !(item->ifa_flags & IFF_LOOPBACK) && (item->ifa_flags & IFF_UP)) {
            char text[INET_ADDRSTRLEN];
            struct sockaddr_in *address = (struct sockaddr_in *) item->ifa_addr;
            struct sockaddr_in *mask = (struct sockaddr_in *) item->ifa_netmask;

            inet_ntop(AF_INET, &address->sin_addr, text, sizeof text);
            snprintf(out, size, "%s/%d (%s)", text, mask != NULL ? __builtin_popcount(mask->sin_addr.s_addr) : 0,
                     item->ifa_name);
            found = 1;
        }
    }
    freeifaddrs(list);
    return found;
}

/* ---- layout ------------------------------------------------------------- */

struct Line {
    char text[FIELD + 96];
};

int main(void)
{
    static const char *logo[] = {
        "     .-------.     ",
        "    /  o   o  \\    ",
        "   |     >     |   ",
        "   |   \\___/   |   ",
        "    \\         /    ",
        "     '-------'     ",
    };
    const int logo_rows = (int) (sizeof logo / sizeof logo[0]);
    const int logo_width = 21;
    struct { const char *label; int (*get)(char *, size_t); } fields[] = {
        { "OS", field_os },         { "Host", field_host },     { "Kernel", field_kernel },
        { "Uptime", field_uptime }, { "Packages", field_packages }, { "Shell", field_shell },
        { "Desktop", field_desktop }, { "Display", field_resolution }, { "CPU", field_cpu },
        { "GPU", field_gpu },       { "Memory", field_memory }, { "Swap", field_swap },
        { "Disk", field_disk },     { "Local IP", field_ip },
    };
    struct Line lines[32];
    int count = 0;
    char value[FIELD], accent[32] = "1;36", title[256], user_text[96] = "user";
    struct passwd account, *found = NULL;
    char buffer[1024], host[96] = "host";

    if (getpwuid_r(getuid(), &account, buffer, sizeof buffer, &found) == 0 && found != NULL) {
        snprintf(user_text, sizeof user_text, "%s", found->pw_name);
    }
    gethostname(host, sizeof host);
    host[sizeof host - 1] = '\0';
    os_release("ANSI_COLOR", accent, sizeof accent);

    snprintf(title, sizeof title, "\033[%sm%s@%s\033[0m", accent, user_text, host);
    snprintf(lines[count++].text, sizeof lines[0].text, "%s", title);
    snprintf(lines[count++].text, sizeof lines[0].text, "%.*s",
             (int) (strlen(user_text) + strlen(host) + 1), "------------------------------------------------");

    for (size_t i = 0; i < sizeof fields / sizeof fields[0]; i++) {
        if (fields[i].get(value, sizeof value)) {
            snprintf(lines[count++].text, sizeof lines[0].text, "\033[%sm%s\033[0m: %s", accent, fields[i].label, value);
        }
    }

    for (int row = 0; row < (count > logo_rows ? count : logo_rows); row++) {
        printf("\033[%sm%-*s\033[0m %s\n", accent, logo_width, row < logo_rows ? logo[row] : "",
               row < count ? lines[row].text : "");
    }
    return 0;
}
