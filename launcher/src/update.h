/*
 * The pure parts of the launcher's "Check for updates": comparing versions
 * and pulling the release tag out of GitHub's answer. No I/O, so
 * test_update.c can exercise them directly; main.c does the request.
 */

#ifndef UPDATE_H
#define UPDATE_H

#include <stddef.h>

/* "v1.5.0", "1.5.0" or "1.5.1-ci1" -> {1, 5, 0}. Returns 1 on success and 0
 * for anything else (a development build's "dev", an empty string, "1.5",
 * "1.5.0.1"). */
int version_parse(const char *text, int out[3]);

/* 1 if `latest` is a higher version than `current`, else 0. */
int version_newer(const int latest[3], const int current[3]);

/* Copies the value of the "tag_name" field of a GitHub release object into
 * `out`. Returns 1 on success, and 0 if the field is missing, not a string,
 * empty, unterminated, contains an escape, or does not fit in `size`. */
int release_tag_from_json(const char *json, char *out, size_t size);

#endif
