/*
 * TITLE: Memory and swap (used, available, total)
 * GROUP: System information
 * USES: read /proc/meminfo; or #include <sys/sysinfo.h> (sysinfo)
 * SUMMARY: /proc/meminfo lists memory in kB. "Used" is MemTotal minus
 *   MemAvailable, which is how free, htop and fastfetch count it.
 * NOTES:
 *   - Values are in kB but mean KiB (1024 bytes). 16384000 kB is 15.6 GiB.
 *   - MemFree is memory nothing uses at all. Linux fills spare memory with
 *     cache, so MemFree is small on a healthy machine. Use MemAvailable: the
 *     kernel's estimate of what a new program could get without swapping.
 *   - Swap: SwapTotal minus SwapFree is the swap in use. Both are 0 when
 *     there is no swap.
 *   - sysinfo() gives totalram and freeram in units of mem_unit bytes, but
 *     has no "available" figure: its freeram is the MemFree trap again.
 *   - Read the whole file and pick keys; do not assume line order.
 * SEE: man 5 proc (search /proc/meminfo), man 2 sysinfo
 */

#include <stdio.h>
#include <string.h>
#include <sys/sysinfo.h>

/* Value of a "Key:   123 kB" line in /proc/meminfo, in kB. -1 if absent. */
static long meminfo_kb(const char *key)
{
    FILE *file = fopen("/proc/meminfo", "r");
    char line[256];
    size_t key_length = strlen(key);
    long value = -1;

    if (file == NULL) {
        return -1;
    }
    while (fgets(line, sizeof line, file) != NULL) {
        if (strncmp(line, key, key_length) == 0 && line[key_length] == ':') {
            if (sscanf(line + key_length + 1, " %ld", &value) != 1) {
                value = -1;
            }
            break;
        }
    }
    fclose(file);
    return value;
}

static double gib(long kb)
{
    return (double) kb / 1024.0 / 1024.0;
}

int main(void)
{
    long total = meminfo_kb("MemTotal");
    long available = meminfo_kb("MemAvailable");
    long free_kb = meminfo_kb("MemFree");
    long swap_total = meminfo_kb("SwapTotal");
    long swap_free = meminfo_kb("SwapFree");

    if (total < 0 || available < 0) {
        puts("memory figures not available");
        return 1;
    }
    printf("Memory  %.2f GiB / %.2f GiB (%ld%%)\n",
           gib(total - available), gib(total), (total - available) * 100 / total);
    printf("MemFree alone would say %.2f GiB used: do not use it\n", gib(total - free_kb));
    if (swap_total > 0) {
        printf("Swap    %.2f GiB / %.2f GiB\n", gib(swap_total - swap_free), gib(swap_total));
    } else {
        puts("Swap    none");
    }

    struct sysinfo info;
    if (sysinfo(&info) == 0) {
        printf("sysinfo totalram: %.2f GiB (mem_unit %u)\n",
               (double) info.totalram * info.mem_unit / 1073741824.0, info.mem_unit);
    }
    return 0;
}
