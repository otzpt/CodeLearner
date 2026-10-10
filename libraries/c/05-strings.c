/*
 * TITLE: string.h: lengths, comparing, copying and searching
 * GROUP: Basics
 * USES: #include <string.h>
 * STANDARD: ISO C. strdup, strndup, strnlen and strtok_r are POSIX (strdup and
 *   strndup joined ISO C in C23).
 * SUMMARY: A C string is an array of char ended by a zero byte. string.h has
 *   the functions that work on strings, and on raw blocks of memory.
 * PROVIDES:
 *   size_t strlen(s)                   characters before the zero byte
 *   int strcmp(a, b), strncmp(a, b, n)       0 when equal
 *   char *strchr(s, c), strrchr(s, c)        first / last occurrence of c
 *   char *strstr(haystack, needle)           first occurrence of a string
 *   size_t strspn(s, accept), strcspn(s, reject)    length of a leading run
 *   char *strncpy, strcpy, strcat, strncat    copying (avoid; see notes)
 *   void *memcpy(dst, src, n), memmove(dst, src, n)  copy bytes
 *   void *memset(p, byte, n); int memcmp(a, b, n)    fill and compare bytes
 *   char *strerror(int errnum)          message for an errno value
 *   POSIX: char *strdup(s), strtok_r(s, delim, &save), size_t strnlen(s, max)
 * NOTES:
 *   - == on two strings compares addresses, not text. Use strcmp(a, b) == 0.
 *   - strcpy and strcat cannot be told a size. Copy with
 *     snprintf(dest, sizeof dest, "%s", source) instead.
 *   - strncpy does NOT add the terminating zero when the source is long.
 *   - strlen counts to the zero byte, so it is slow on long strings; call it
 *     once, not in a loop condition.
 *   - memcpy with overlapping blocks is undefined: use memmove.
 *   - strtok changes the string and keeps hidden state; use strtok_r (POSIX).
 *   - A buffer for N characters needs N + 1 bytes.
 *   - strdup allocates: free() the result.
 *   - strrchr(path, '/') + 1 is the file name; strcspn(line, "\n") is the
 *     index of the newline (the length without it).
 * TOOL:
 *   - strncmp(line, "MemTotal:", 9) == 0 to pick a key out of /proc/meminfo.
 *   - strchr(line, '=') to split KEY=value in os-release.
 *   - strcspn to trim the newline fgets leaves; strrchr to get a process name.
 * SEE: man 3 string, man 3 strtok_r, man 3 strdup
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void copy_into(char *out, size_t size, const char *text)
{
    snprintf(out, size, "%s", text);
}

int main(void)
{
    const char *path = "/usr/bin/bash";

    printf("strlen: %zu\n", strlen(path));
    printf("strcmp equal: %s\n", strcmp(path, "/usr/bin/bash") == 0 ? "yes" : "no");

    const char *slash = strrchr(path, '/');
    printf("after the last slash: %s\n", slash != NULL ? slash + 1 : path);
    printf("contains \"bin\": %s\n", strstr(path, "bin") != NULL ? "yes" : "no");

    char copy[8];
    copy_into(copy, sizeof copy, path);
    printf("copy into 8 bytes (7 characters and the zero): \"%s\"\n", copy);

    char joined[32] = "";
    snprintf(joined, sizeof joined, "%s%s", "up ", "3 hours");
    printf("built: %s\n", joined);

    /* strspn / strcspn: the length of a run, without copying. */
    const char *entry = "  model name : AMD Ryzen\n";
    size_t leading = strspn(entry, " \t");
    size_t body = strcspn(entry + leading, "\n");
    printf("skip %zu blanks, then %zu characters before the newline\n", leading, body);

    /* memmove handles overlap, memcpy does not. */
    char buffer[16] = "abcdef";
    memmove(buffer + 2, buffer, 4);
    buffer[6] = '\0';
    printf("memmove(buffer + 2, buffer, 4) turned abcdef into %s\n", buffer);
    memset(buffer, '-', 3);
    printf("memset 3 bytes to '-': %s, memcmp with \"---\": %d\n", buffer, memcmp(buffer, "---", 3));

    /* strtok_r splits without hidden state; it writes into the string, so use a copy. */
    char list[] = "Wayland:X11:tty";
    char *save = NULL;
    printf("strtok_r on a colon list:");
    for (char *part = strtok_r(list, ":", &save); part != NULL; part = strtok_r(NULL, ":", &save)) {
        printf(" [%s]", part);
    }
    printf("\n");

    char *owned = strdup(path);
    if (owned != NULL) {
        owned[0] = '\\';
        printf("strdup made a private copy: %s, original still %s\n", owned, path);
        free(owned);
    }
    return 0;
}
