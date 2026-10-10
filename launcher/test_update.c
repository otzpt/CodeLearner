/*
 * Checks for src/update.c.   make check
 *
 * RELEASE_PREFIX is the start of a real GET /repos/otzpt/CodeLearner/releases/latest
 * response (the author object trimmed): compact JSON, with "tag_name" after
 * the author, as GitHub sends it.
 */

#include <stdio.h>
#include <string.h>

#include "src/update.h"

static int checks;
static int failures;

#define CHECK(expr)                                                       \
    do {                                                                  \
        checks++;                                                         \
        if (!(expr)) {                                                    \
            printf("FAIL line %d: %s\n", __LINE__, #expr);                \
            failures++;                                                   \
        }                                                                 \
    } while (0)

static const char RELEASE_PREFIX[] =
    "{\"url\":\"https://api.github.com/repos/otzpt/CodeLearner/releases/408921034\","
    "\"html_url\":\"https://github.com/otzpt/CodeLearner/releases/tag/v1.5.0\","
    "\"id\":408921034,\"author\":{\"login\":\"github-actions[bot]\",\"id\":41898282},"
    "\"node_id\":\"RE_kwDOTxmN184YX6PK\",\"tag_name\":\"v1.5.0\","
    "\"target_commitish\":\"main\",\"name\":\"v1.5.0\",\"draft\":false}";

static int parses(const char *text, int a, int b, int c)
{
    int v[3];

    return version_parse(text, v) && v[0] == a && v[1] == b && v[2] == c;
}

static int newer(const char *latest, const char *current)
{
    int l[3], c[3];

    return version_parse(latest, l) && version_parse(current, c) && version_newer(l, c);
}

int main(void)
{
    char tag[32];
    char tiny[4];
    int v[3];

    /* version_parse */
    CHECK(parses("v1.5.0", 1, 5, 0));
    CHECK(parses("1.5.0", 1, 5, 0));
    CHECK(parses("1.5.1-ci1", 1, 5, 1));
    CHECK(parses("10.20.30", 10, 20, 30));
    CHECK(!version_parse("dev", v));
    CHECK(!version_parse("", v));
    CHECK(!version_parse("v", v));
    CHECK(!version_parse("1.5", v));
    CHECK(!version_parse("1.5.0.1", v));
    CHECK(!version_parse("1.5.x", v));
    CHECK(!version_parse("1.-5.0", v));

    /* version_newer: numeric, not alphabetical */
    CHECK(newer("v1.5.1", "1.5.0"));
    CHECK(newer("v1.10.0", "1.9.9"));
    CHECK(newer("v2.0.0", "1.99.99"));
    CHECK(!newer("v1.5.0", "1.5.0"));
    CHECK(!newer("v1.5.0", "1.5.1-ci1"));
    CHECK(!newer("v1.4.9", "1.5.0"));

    /* release_tag_from_json */
    CHECK(release_tag_from_json(RELEASE_PREFIX, tag, sizeof tag) && strcmp(tag, "v1.5.0") == 0);
    CHECK(release_tag_from_json("{ \"tag_name\" :\n  \"v2.0.1\" }", tag, sizeof tag)
          && strcmp(tag, "v2.0.1") == 0);
    CHECK(!release_tag_from_json("{\"message\":\"Not Found\"}", tag, sizeof tag));
    CHECK(!release_tag_from_json("curl: (22) The requested URL returned error: 403", tag, sizeof tag));
    CHECK(!release_tag_from_json("{\"tag_name\":null}", tag, sizeof tag));
    CHECK(!release_tag_from_json("{\"tag_name\":\"\"}", tag, sizeof tag));
    CHECK(!release_tag_from_json("{\"tag_name\":\"v1.5.0", tag, sizeof tag));
    CHECK(!release_tag_from_json("{\"tag_name\":\"v1\\u002e5\"}", tag, sizeof tag));
    CHECK(!release_tag_from_json("{\"tag_name\":\"v1.5.0\"}", tiny, sizeof tiny));
    CHECK(!release_tag_from_json("", tag, sizeof tag));

    printf("launcher update helpers: %d checks, %d failed\n", checks, failures);
    return failures != 0;
}
