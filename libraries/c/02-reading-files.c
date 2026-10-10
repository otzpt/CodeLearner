/*
 * TITLE: Reading a text file line by line
 * GROUP: Basics
 * USES: #include <stdio.h>
 * SUMMARY: fopen opens a file, fgets reads one line at a time into a buffer,
 *   fclose closes it. This is the loop behind every /proc and /etc page.
 * NOTES:
 *   - fopen returns NULL on failure. Always check, and say why with
 *     strerror(errno) (see the errors page).
 *   - fgets keeps the newline. Cut it with line[strcspn(line, "\n")] = '\0'.
 *   - A line longer than the buffer comes back in pieces, with no newline on
 *     the first pieces. Use a buffer bigger than any line you expect.
 *   - Files under /proc report a size of 0. Read until fgets returns NULL;
 *     never size the read from the file size.
 *   - getline() (POSIX) grows its own buffer when a line is long.
 * SEE: man 3 fgets, man 3 getline
 */

#include <stdio.h>
#include <string.h>

int main(void)
{
    FILE *file = fopen("/proc/loadavg", "r");
    char line[256];

    if (file == NULL) {
        perror("fopen /proc/loadavg");
        return 1;
    }
    while (fgets(line, sizeof line, file) != NULL) {
        line[strcspn(line, "\n")] = '\0';
        printf("read: \"%s\"\n", line);
    }
    fclose(file);

    file = fopen("/no/such/file", "r");
    if (file == NULL) {
        fflush(stdout);
        perror("fopen /no/such/file");
    }
    return 0;
}
