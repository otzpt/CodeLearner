/*
 * TITLE: stdint.h and inttypes.h: fixed-width integers
 * GROUP: Basics
 * USES: #include <stdint.h>, <inttypes.h>
 * STANDARD: ISO C99 and later. The ranges of int, long and char are in
 *   limits.h (see that page); PATH_MAX and friends there are POSIX.
 * SUMMARY: int, long and size_t change size between machines. The types in
 *   stdint.h (uint64_t, int32_t) do not. inttypes.h adds the macros to print
 *   and read them.
 * PROVIDES:
 *   int8_t, int16_t, int32_t, int64_t       exactly that many bits, signed
 *   uint8_t, uint16_t, uint32_t, uint64_t   exactly that many bits, unsigned
 *   intptr_t, uintptr_t                     wide enough to hold a pointer
 *   intmax_t, uintmax_t                     the widest integer there is
 *   int_least32_t, int_fast32_t             at least / fastest with 32 bits
 *   INT32_MAX, INT32_MIN, UINT64_MAX, SIZE_MAX     limits as macros
 *   inttypes.h: PRId32, PRIu64, PRIx64 (printf) and SCNu64, SCNd32 (scanf)
 * NOTES:
 *   - Memory sizes from /proc are in kB and can pass 4 billion in bytes:
 *     keep byte counts in uint64_t, not int or unsigned.
 *   - Print a uint64_t with PRIu64:  printf("%" PRIu64 "\n", value). A plain
 *     %lu works only where uint64_t is unsigned long, which is not portable.
 *   - Subtracting unsigned numbers that would go below zero wraps around to
 *     a huge number instead of becoming negative.
 *   - exact-width types are optional: a machine without 8-bit bytes would not
 *     have uint8_t. In practice every Linux system has them.
 *   - To look at the bytes of a number, copy them with memcpy into a
 *     uint8_t array; do not cast the pointer.
 * TOOL:
 *   - uint64_t for byte counts built from kB values (kib * 1024).
 *   - uint64_t clock ticks and jiffies read from /proc/<pid>/stat.
 *   - SCNu64 with sscanf to read counters from /proc/net/dev without a cast.
 * SEE: man 0 stdint.h, man 0 inttypes.h
 */

#include <inttypes.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    uint64_t total = 16384000ULL * 1024;
    unsigned small = 3;

    printf("sizeof(int) %zu, long %zu, size_t %zu, uint64_t %zu\n",
           sizeof(int), sizeof(long), sizeof(size_t), sizeof(uint64_t));
    printf("INT_MAX %d, UINT32_MAX %" PRIu32 "\n", INT_MAX, UINT32_MAX);
    printf("16384000 kB in bytes: %" PRIu64 "\n", total);
    printf("3u - 5u            : %u (wrapped around, not -2)\n", small - 5u);

    uint64_t counter = 0;
    const char *line = "rx_bytes 1234567890123";
    if (sscanf(line, "rx_bytes %" SCNu64, &counter) == 1) {
        printf("SCNu64 read %" PRIu64 " (does not fit in 32 bits: %s)\n", counter, counter > UINT32_MAX ? "true" : "false");
    }
    printf("SIZE_MAX           : %zu\n", SIZE_MAX);
    printf("least / fast 32    : %zu / %zu bytes here\n", sizeof(int_least32_t), sizeof(int_fast32_t));

    uint32_t word = 0x01020304;
    uint8_t bytes[4];
    memcpy(bytes, &word, sizeof bytes);
    printf("0x01020304 in memory: %u %u %u %u -> %s-endian\n", bytes[0], bytes[1], bytes[2], bytes[3],
           bytes[0] == 4 ? "little" : "big");
    return 0;
}
