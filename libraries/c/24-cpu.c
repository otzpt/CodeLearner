/*
 * TITLE: CPU model, core count and speed
 * GROUP: System information
 * USES: #include <unistd.h> (sysconf); read /proc/cpuinfo and the cpu files in /sys
 * SUMMARY: The model name is a line in /proc/cpuinfo. The number of logical
 *   processors is one sysconf() call. The top speed is a file under
 *   /sys/devices/system/cpu.
 * NOTES:
 *   - /proc/cpuinfo repeats one block per logical processor. "model name"
 *     appears in every block; the first is enough.
 *   - "Logical" counts hyper-threads. For physical cores read "cpu cores"
 *     (per package) or count unique "core id" values.
 *   - On ARM the key is not "model name": cpuinfo lists "CPU part" numbers
 *     and "Hardware"/"Model" instead. Look for several keys and fall back.
 *   - cpuinfo_max_freq is in kHz and is missing in many virtual machines.
 *   - "cpu MHz" in cpuinfo is the CURRENT speed and changes second to second.
 *   - _SC_NPROCESSORS_ONLN counts online processors; _SC_NPROCESSORS_CONF
 *     counts configured ones. They differ when cores are switched off.
 * SEE: man 3 sysconf, man 5 proc
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Return the text after "key" and its colon in the first line that has it. */
static int cpuinfo_first(const char *key, char *out, size_t size)
{
    FILE *file = fopen("/proc/cpuinfo", "r");
    char line[512];
    size_t key_length = strlen(key);

    if (file == NULL) {
        return 0;
    }
    while (fgets(line, sizeof line, file) != NULL) {
        if (strncmp(line, key, key_length) == 0) {
            char *colon = strchr(line, ':');
            if (colon != NULL) {
                colon++;
                while (*colon == ' ' || *colon == '\t') {
                    colon++;
                }
                colon[strcspn(colon, "\r\n")] = '\0';
                snprintf(out, size, "%s", colon);
                fclose(file);
                return 1;
            }
        }
    }
    fclose(file);
    return 0;
}

int main(void)
{
    char model[256];
    char cores[32];
    char max_khz[32];
    FILE *file;

    if (cpuinfo_first("model name", model, sizeof model)
        || cpuinfo_first("Model", model, sizeof model)
        || cpuinfo_first("Hardware", model, sizeof model)) {
        printf("model            %s\n", model);
    } else {
        puts("model            (not in /proc/cpuinfo)");
    }
    if (cpuinfo_first("cpu cores", cores, sizeof cores)) {
        printf("cpu cores        %s (per package)\n", cores);
    }
    printf("logical online   %ld\n", sysconf(_SC_NPROCESSORS_ONLN));
    printf("logical total    %ld\n", sysconf(_SC_NPROCESSORS_CONF));

    file = fopen("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq", "r");
    if (file != NULL && fgets(max_khz, sizeof max_khz, file) != NULL) {
        printf("max frequency    %.2f GHz\n", atof(max_khz) / 1000000.0);
    } else {
        puts("max frequency    (not reported here)");
    }
    if (file != NULL) {
        fclose(file);
    }
    return 0;
}
