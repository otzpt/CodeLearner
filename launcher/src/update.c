#include "update.h"

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int version_parse(const char *text, int out[3])
{
    int parts[3];

    if (text[0] == 'v' || text[0] == 'V') {
        text++;
    }
    for (int i = 0; i < 3; i++) {
        char *end;
        long value;

        /* strtol would skip leading blanks and accept a sign. */
        if (text[0] < '0' || text[0] > '9') {
            return 0;
        }
        errno = 0;
        value = strtol(text, &end, 10);
        if (errno == ERANGE || value > INT_MAX) {
            return 0;
        }
        parts[i] = (int)value;
        text = end;
        if (i < 2) {
            if (*text != '.') {
                return 0;
            }
            text++;
        }
    }
    /* Only an end of string or a "-ci1" / "+build" style suffix may follow. */
    if (*text != '\0' && *text != '-' && *text != '+') {
        return 0;
    }
    memcpy(out, parts, sizeof parts);
    return 1;
}

int version_newer(const int latest[3], const int current[3])
{
    for (int i = 0; i < 3; i++) {
        if (latest[i] != current[i]) {
            return latest[i] > current[i];
        }
    }
    return 0;
}

static const char *skip_blanks(const char *p)
{
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r') {
        p++;
    }
    return p;
}

int release_tag_from_json(const char *json, char *out, size_t size)
{
    static const char KEY[] = "\"tag_name\"";
    const char *p = strstr(json, KEY);
    size_t length = 0;

    if (p == NULL) {
        return 0;
    }
    p = skip_blanks(p + sizeof KEY - 1);
    if (*p != ':') {
        return 0;
    }
    p = skip_blanks(p + 1);
    if (*p != '"') {
        return 0;
    }
    p++;
    while (*p != '"' && *p != '\0') {
        /* A tag with a backslash escape is not one of ours, and an
         * unbounded copy would overflow `out`. */
        if (*p == '\\' || length + 1 >= size) {
            return 0;
        }
        out[length++] = *p++;
    }
    if (*p != '"' || length == 0) {
        return 0;
    }
    out[length] = '\0';
    return 1;
}
