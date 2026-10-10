#include "update.h"

#include <stdio.h>
#include <string.h>

int version_parse(const char *text, int out[3])
{
    int major, minor, patch;
    int used = 0;

    if (text[0] == 'v' || text[0] == 'V') {
        text++;
    }
    /* %n records how much was read, so a fourth component can be refused
     * below; the return value alone would accept "1.5.0.1". */
    if (sscanf(text, "%d.%d.%d%n", &major, &minor, &patch, &used) != 3) {
        return 0;
    }
    if (major < 0 || minor < 0 || patch < 0) {
        return 0;
    }
    /* Only an end of string or a "-ci1" / "+build" style suffix may follow. */
    if (text[used] != '\0' && text[used] != '-' && text[used] != '+') {
        return 0;
    }
    out[0] = major;
    out[1] = minor;
    out[2] = patch;
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
