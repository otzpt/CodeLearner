/*
 * TITLE: Uptime, boot time and load average
 * GROUP: System information
 * USES: #include <time.h> (clock_gettime), <stdlib.h> (getloadavg); /proc/uptime
 * SUMMARY: Seconds since boot come from clock_gettime(CLOCK_BOOTTIME),
 *   sysinfo(), or the first number in /proc/uptime. getloadavg() gives the
 *   1, 5 and 15 minute load.
 * NOTES:
 *   - CLOCK_BOOTTIME keeps counting while the machine is suspended;
 *     CLOCK_MONOTONIC does not. Use BOOTTIME for "up for".
 *   - Format the number yourself: days, hours, minutes. Print "1 day" and
 *     "2 days" correctly, and leave out zero parts.
 *   - boot time = now - uptime. /proc/stat also has it as "btime", in
 *     seconds since 1970.
 *   - Load average is the number of processes wanting a CPU, averaged. A
 *     value above the number of logical CPUs means the machine is busy.
 * SEE: man 2 clock_gettime, man 3 getloadavg, man 5 proc (/proc/uptime)
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* "1 day, 2 hours, 3 mins": omit zero days and hours, always show minutes. */
static void format_uptime(long seconds, char *out, size_t size)
{
    long days = seconds / 86400;
    long hours = seconds % 86400 / 3600;
    long minutes = seconds % 3600 / 60;
    size_t used = 0;

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
}

int main(void)
{
    struct timespec boot;
    char text[96];
    double load[3];

    if (clock_gettime(CLOCK_BOOTTIME, &boot) != 0) {
        perror("clock_gettime");
        return 1;
    }
    format_uptime((long) boot.tv_sec, text, sizeof text);
    printf("Uptime: %s\n", text);

    FILE *file = fopen("/proc/uptime", "r");
    double from_proc;
    if (file != NULL && fscanf(file, "%lf", &from_proc) == 1) {
        printf("/proc/uptime says %.2f seconds\n", from_proc);
    }
    if (file != NULL) {
        fclose(file);
    }

    time_t booted = time(NULL) - (time_t) boot.tv_sec;
    char when[64];
    struct tm local;
    localtime_r(&booted, &local);
    strftime(when, sizeof when, "%Y-%m-%d %H:%M", &local);
    printf("Booted at %s\n", when);

    if (getloadavg(load, 3) == 3) {
        printf("Load: %.2f %.2f %.2f\n", load[0], load[1], load[2]);
    }

    printf("format_uptime(93780) = ");
    format_uptime(93780, text, sizeof text);
    printf("%s\n", text);
    return 0;
}
