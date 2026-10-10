/*
 * TITLE: time.h: now, dates, durations and clocks
 * GROUP: Basics
 * USES: #include <time.h>
 * STANDARD: ISO C for time, difftime, mktime, localtime, gmtime, strftime,
 *   clock. clock_gettime, nanosleep, localtime_r, gmtime_r, strptime and the
 *   CLOCK_ names are POSIX. CLOCK_BOOTTIME is Linux-specific. timegm is a
 *   BSD/glibc extension.
 * SUMMARY: time() is seconds since 1970. localtime_r / gmtime_r split it into
 *   year, month, day. strftime formats it. clock_gettime reads the precise
 *   clocks, including the one that counts since boot.
 * PROVIDES:
 *   time_t time(time_t *out)              seconds since 1970-01-01 00:00 UTC
 *   struct tm: tm_year (since 1900), tm_mon (0-11), tm_mday, tm_hour, tm_min,
 *              tm_sec, tm_wday, tm_yday, tm_isdst
 *   struct tm *localtime_r(&t, &tm), gmtime_r(&t, &tm)
 *   time_t mktime(struct tm *)            local broken-down time to time_t
 *   size_t strftime(buf, size, format, &tm)    0 if it did not fit
 *   double difftime(end, start)           seconds between two time_t
 *   struct timespec: tv_sec, tv_nsec
 *   int clock_gettime(clockid_t, struct timespec *)
 *   CLOCK_REALTIME, CLOCK_MONOTONIC, CLOCK_BOOTTIME (Linux)
 *   int nanosleep(const struct timespec *request, struct timespec *left)
 * NOTES:
 *   - Use the _r versions (localtime_r, gmtime_r). The plain ones return a
 *     pointer to shared static memory.
 *   - tm_year counts from 1900 and tm_mon from 0: add 1900 and 1.
 *   - CLOCK_MONOTONIC never goes backwards: use it to time things.
 *     CLOCK_BOOTTIME also counts time spent suspended: use it for uptime.
 *     CLOCK_REALTIME jumps when the clock is set.
 *   - strftime returns 0 if the result did not fit in the buffer.
 *   - mktime normalises its argument: tm_mday = 32 becomes the 1st of next
 *     month. That is the easy way to add days to a date.
 *   - time_t is 64 bits on 64-bit Linux. On 32-bit systems it overflows in 2038.
 *   - A time zone comes from TZ or /etc/localtime; tm_isdst says whether
 *     daylight saving applies.
 * TOOL:
 *   - clock_gettime(CLOCK_BOOTTIME) for the uptime line.
 *   - time(NULL) minus uptime gives the boot time; format it with strftime.
 *   - CLOCK_MONOTONIC to measure how long each field took, for a --timing option.
 * SEE: man 3 strftime, man 2 clock_gettime, man 3 mktime, man 7 time
 */

#include <stdio.h>
#include <time.h>

int main(void)
{
    time_t epoch = 0;
    struct tm utc;
    char text[64];

    gmtime_r(&epoch, &utc);
    strftime(text, sizeof text, "%Y-%m-%d %H:%M:%S", &utc);
    printf("time 0 is        %s UTC\n", text);

    time_t now = time(NULL);
    struct tm local;
    localtime_r(&now, &local);
    printf("this year is     %d (tm_year %d + 1900)\n", local.tm_year + 1900, local.tm_year);

    struct timespec since_boot;
    if (clock_gettime(CLOCK_BOOTTIME, &since_boot) == 0) {
        long seconds = (long) since_boot.tv_sec;
        printf("up for           %ld days, %ld hours, %ld minutes\n",
               seconds / 86400, seconds % 86400 / 3600, seconds % 3600 / 60);
    }

    struct tm date = { .tm_year = 126, .tm_mon = 0, .tm_mday = 32, .tm_hour = 12, .tm_isdst = -1 };
    mktime(&date);
    strftime(text, sizeof text, "%Y-%m-%d", &date);
    printf("mktime turned January 32nd 2026 into %s\n", text);
    printf("difftime(100, 40) = %.0f seconds\n", difftime(100, 40));

    struct timespec start, stop;
    clock_gettime(CLOCK_MONOTONIC, &start);
    clock_gettime(CLOCK_MONOTONIC, &stop);
    long nanoseconds = (stop.tv_sec - start.tv_sec) * 1000000000L + (stop.tv_nsec - start.tv_nsec);
    printf("two clock reads  %s than a millisecond apart\n", nanoseconds < 1000000L ? "less" : "more");
    return 0;
}
