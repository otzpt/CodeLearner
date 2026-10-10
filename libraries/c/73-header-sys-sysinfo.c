/*
 * TITLE: sys/sysinfo.h: uptime, load, memory and process count in one call
 * GROUP: Header reference: POSIX and Linux
 * USES: #include <sys/sysinfo.h>
 * STANDARD: Linux-specific. Not POSIX, not available on macOS or the BSDs.
 * SUMMARY: sysinfo() fills a struct with the uptime, the load averages, the
 *   memory and swap totals and the number of processes, from one system call
 *   and no file parsing.
 * PROVIDES:
 *   int sysinfo(struct sysinfo *info)         0, or -1 with errno set
 *   struct sysinfo:
 *     long uptime                  seconds since boot
 *     unsigned long loads[3]       1, 5, 15 minute load, FIXED POINT (see notes)
 *     unsigned long totalram, freeram, sharedram, bufferram
 *     unsigned long totalswap, freeswap
 *     unsigned short procs         number of processes
 *     unsigned int mem_unit        size in bytes of the units above
 *   int get_nprocs(void), get_nprocs_conf(void)   online and configured CPUs
 *   long get_phys_pages(void), get_avphys_pages(void)
 * NOTES:
 *   - The memory fields are counted in units of mem_unit BYTES, not in bytes
 *     or kilobytes: bytes = totalram * mem_unit. On current kernels mem_unit
 *     is 1, but do the multiplication anyway.
 *   - loads[] are fixed point numbers scaled by 65536 (the kernel's
 *     SI_LOAD_SHIFT is 16). Divide by 65536.0 to get 0.62; printing the raw
 *     value gives 40632.
 *   - freeram is MemFree: memory nothing uses, NOT memory a program could get.
 *     The disk cache is not counted. For "used" use MemAvailable from
 *     /proc/meminfo (see the memory page).
 *   - uptime is a whole number of seconds.
 *   - struct sysinfo has no "available" field and no per-process data.
 * TOOL:
 *   - The cheapest way to get total RAM, swap size, uptime and process count
 *     without opening a file.
 *   - get_nprocs() for the logical CPU count (same as sysconf's
 *     _SC_NPROCESSORS_ONLN).
 * SEE: man 2 sysinfo, man 3 get_nprocs
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>

/* MemAvailable from /proc/meminfo, in kB, or -1. */
static long mem_available_kb(void)
{
    FILE *file = fopen("/proc/meminfo", "r");
    char line[256];
    long value = -1;

    if (file == NULL) {
        return -1;
    }
    while (fgets(line, sizeof line, file) != NULL) {
        if (sscanf(line, "MemAvailable: %ld kB", &value) == 1) {
            break;
        }
    }
    fclose(file);
    return value;
}

int main(void)
{
    struct sysinfo info;
    double loads[3];
    double gib = 1024.0 * 1024.0 * 1024.0;

    if (sysinfo(&info) != 0) {
        perror("sysinfo");
        return 1;
    }
    printf("uptime        %ld seconds (%ld hours)\n", info.uptime, info.uptime / 3600);
    printf("raw loads[0]  %lu   scaled: %.2f\n", info.loads[0], (double) info.loads[0] / 65536.0);
    if (getloadavg(loads, 3) == 3) {
        printf("getloadavg    %.2f  (agrees to about two decimals)\n", loads[0]);
    }
    printf("mem_unit      %u\n", info.mem_unit);
    printf("total RAM     %.2f GiB\n", (double) info.totalram * info.mem_unit / gib);
    printf("freeram       %.2f GiB   <- MemFree, not what a program can use\n", (double) info.freeram * info.mem_unit / gib);
    printf("buffer RAM    %.2f GiB\n", (double) info.bufferram * info.mem_unit / gib);
    printf("total swap    %.2f GiB, free %.2f GiB\n", (double) info.totalswap * info.mem_unit / gib,
           (double) info.freeswap * info.mem_unit / gib);
    printf("processes     %u\n", info.procs);
    printf("get_nprocs    %d online, %d configured\n", get_nprocs(), get_nprocs_conf());

    long available = mem_available_kb();
    if (available >= 0) {
        printf("MemAvailable  %.2f GiB   <- the figure to subtract from the total\n", (double) available * 1024.0 / gib);
    }
    return 0;
}
