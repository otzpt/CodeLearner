/*
 * TITLE: stdlib.h: conversion, memory, sorting and the environment
 * GROUP: Basics
 * USES: #include <stdlib.h>
 * STANDARD: ISO C. realpath, setenv, mkdtemp and mkstemp are POSIX; getloadavg
 *   is a BSD/glibc extension.
 * SUMMARY: stdlib.h converts text to numbers, sorts and searches arrays,
 *   allocates memory, reads environment variables and ends the program.
 * PROVIDES:
 *   long strtol(s, &end, base), unsigned long strtoul, double strtod(s, &end)
 *   int atoi(s), double atof(s)                  convert, but cannot report errors
 *   void *malloc(n), calloc(count, size), realloc(p, n); void free(p)
 *   void qsort(base, count, size, compare)
 *   void *bsearch(key, base, count, size, compare)
 *   char *getenv(name)                           NULL if not set
 *   int abs(int), long labs(long); div_t div(a, b)
 *   int rand(void); void srand(unsigned seed)
 *   void exit(int status), abort(void); int atexit(void (*function)(void))
 *   EXIT_SUCCESS (0), EXIT_FAILURE (1), RAND_MAX, NULL, size_t
 *   POSIX: char *realpath(path, resolved), int setenv(name, value, overwrite)
 * NOTES:
 *   - getenv returns NULL when the variable is not set, and a pointer into
 *     the environment otherwise: read it, never write to it.
 *   - qsort takes a comparison function. Compare with < and >, not by
 *     subtracting: a - b overflows for large values.
 *   - strtol / strtoul / strtod are the safe conversions (see the parsing
 *     page). atoi cannot tell "0" from "no number". For ERANGE, set errno = 0
 *     before the call and test it after.
 *   - malloc can return NULL; check it. Every malloc needs one free. calloc
 *     also zeroes the memory and checks count * size for overflow.
 *   - realloc may move the block and returns NULL on failure while the old
 *     block is still valid: never write p = realloc(p, n), keep the result in
 *     a temporary until it is checked.
 *   - rand() is not random enough for anything that matters, and gives the
 *     same sequence every run unless you call srand.
 *   - system() runs a shell command: never pass it text that came from outside.
 * TOOL:
 *   - strtol for numbers read from /proc; getenv for HOME, SHELL, XDG_*.
 *   - realloc to grow a list of lines or fields as they are found.
 *   - realpath to resolve a symlink fully (for example /etc/os-release).
 *   - exit(EXIT_FAILURE) with a message on stderr when a mandatory source is
 *     missing.
 * SEE: man 3 strtol, man 3 realloc, man 3 qsort, man 3 getenv
 */

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

static int compare(const void *a, const void *b)
{
    int x = *(const int *) a;
    int y = *(const int *) b;
    return (x > y) - (x < y);
}

int main(void)
{
    int values[] = { 5, 2, 9, 1, 7 };
    size_t count = sizeof values / sizeof values[0];

    qsort(values, count, sizeof values[0], compare);
    for (size_t i = 0; i < count; i++) {
        printf("%d ", values[i]);
    }
    printf("\n");

    const char *home = getenv("HOME");
    const char *missing = getenv("NO_SUCH_VARIABLE_HERE");
    printf("HOME is %s\n", home != NULL ? home : "(not set)");
    printf("NO_SUCH_VARIABLE_HERE is %s\n", missing != NULL ? missing : "(not set)");

    int *buffer = malloc(4 * sizeof *buffer);
    if (buffer == NULL) {
        return 1;
    }
    buffer[0] = 42;
    printf("malloc'd int holds %d\n", buffer[0]);
    free(buffer);

    /* A growing list: double the capacity with realloc, keep the old block if it fails. */
    int *list = NULL;
    size_t items = 0, capacity = 0;
    for (int value = 1; value <= 10; value++) {
        if (items == capacity) {
            size_t bigger = capacity == 0 ? 2 : capacity * 2;
            int *moved = realloc(list, bigger * sizeof *list);
            if (moved == NULL) {
                free(list);
                return 1;
            }
            list = moved;
            capacity = bigger;
        }
        list[items++] = value * value;
    }
    printf("grew to %zu items in a block of %zu: last square %d\n", items, capacity, list[items - 1]);

    int wanted = 49;
    int *where = bsearch(&wanted, list, items, sizeof *list, compare);
    printf("bsearch for 49 in the sorted squares: %s\n", where != NULL ? "found" : "not found");
    free(list);

    char *end;
    double ratio = strtod("2.75 GHz", &end);
    printf("strtod read %.2f and stopped at \"%s\"\n", ratio, end);

    errno = 0;
    long huge = strtol("99999999999999999999", &end, 10);
    printf("strtol of a number too big: %ld, errno is ERANGE: %s\n", huge, errno == ERANGE ? "yes" : "no");

    char resolved[PATH_MAX];
    if (realpath("/etc/os-release", resolved) != NULL) {
        printf("realpath(/etc/os-release) = %s\n", resolved);
    }
    return 0;
}
