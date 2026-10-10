/*
 * TITLE: stdio.h: streams, files and formatted output
 * GROUP: Header reference: standard C
 * USES: #include <stdio.h>
 * STANDARD: ISO C. popen, pclose, getline and fileno are POSIX, and need
 *   _POSIX_C_SOURCE 200809L when you compile with -std=c11 (see unistd.h).
 * SUMMARY: stdio.h is C's buffered input and output: the FILE stream type,
 *   the three standard streams, and the printf and scanf families.
 * PROVIDES:
 *   FILE *fopen(path, mode)            open a file; NULL on failure
 *   int fclose(FILE *f)                close; EOF if the last write failed
 *   char *fgets(buf, size, FILE *f)    one line; NULL at end of file or error
 *   size_t fread / fwrite(ptr, size, count, f)    binary blocks
 *   int fseek(f, offset, whence); long ftell(f); void rewind(f)
 *   int fflush(FILE *f)                push buffered output out now
 *   int printf, fprintf, snprintf(buf, size, fmt, ...)    formatted output
 *   int sscanf, fscanf(..., fmt, ...)  formatted input; returns fields filled
 *   void perror(prefix)                message for errno, on stderr
 *   FILE *tmpfile(void); int remove(path); int rename(old, new)
 *   stdin, stdout, stderr; EOF; BUFSIZ; FILENAME_MAX
 *   POSIX: popen / pclose, getline, fileno
 * NOTES:
 *   - Modes: "r" read, "w" create or empty the file, "a" append; add "+" to
 *     read and write. "b" changes nothing on Linux.
 *   - fclose of a file you wrote can fail (disk full): check its result.
 *   - stdout is line-buffered on a terminal and fully buffered in a pipe, so
 *     piped output appears late, and after stderr. fflush(stdout) before
 *     writing to stderr, forking, or running another program.
 *   - fgets stores at most size - 1 characters plus the zero byte, and keeps
 *     the newline when there is room.
 *   - scanf("%s") with no width overflows its buffer: write %63s.
 *   - Files under /proc report size 0: loop on fgets until NULL, never use
 *     ftell to size a read.
 *   - Do not use gets() (removed from C11) or sprintf (no size limit).
 * TOOL:
 *   - Read /proc and /sys files with fopen and fgets, one line at a time.
 *   - Build every field with snprintf into a fixed buffer, and check the
 *     returned length against the buffer size.
 *   - popen("lspci -nn", "r") reads another program's output when no file has
 *     the answer; pclose returns its exit status.
 * SEE: man 3 stdio, man 3 fopen, man 3 fgets, man 3 popen
 */

#define _POSIX_C_SOURCE 200809L

#include <stdint.h>
#include <stdio.h>

/* The text of `source` as far as `out` can hold it; returns what snprintf returns. */
static int copy_text(char *out, size_t size, const char *source)
{
    return snprintf(out, size, "%s", source);
}

int main(void)
{
    FILE *file = tmpfile();
    char line[64];

    if (file == NULL) {
        perror("tmpfile");
        return 1;
    }
    fprintf(file, "cpu: %d\nmemory: %.1f GiB\n", 12, 29.7);
    rewind(file);
    while (fgets(line, sizeof line, file) != NULL) {
        printf("text line: %s", line);
    }

    uint32_t values[3] = { 1, 258, 65536 };
    fseek(file, 0, SEEK_SET);
    fwrite(values, sizeof values[0], 3, file);
    long size = ftell(file);
    rewind(file);

    uint32_t back[3];
    size_t got = fread(back, sizeof back[0], 3, file);
    printf("wrote %ld bytes, read back %zu values: %u %u %u\n", size, got, back[0], back[1], back[2]);
    fclose(file);

    char text[8];
    int wanted = copy_text(text, sizeof text, "Ubuntu 26.04");
    printf("snprintf wanted %d characters, buffer holds %zu, kept \"%s\"\n", wanted, sizeof text - 1, text);

    FILE *pipe = popen("echo from-another-program", "r");
    if (pipe != NULL) {
        if (fgets(line, sizeof line, pipe) != NULL) {
            printf("popen read: %s", line);
        }
        printf("pclose status: %d\n", pclose(pipe));
    }
    return 0;
}
