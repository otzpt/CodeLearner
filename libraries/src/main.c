/*
 * Libraries - a reference browser, not a course.
 *
 * A wiki of how to use libraries, one page per thing you might want to do,
 * in the style of a manual page or a good Stack Overflow answer: what it is,
 * which header or module, the code, and the real output of running that code
 * on this machine. There are no lessons and no exercises.
 *
 * Every page is a real source file in the c/ or python/ folder next to this
 * program. The page shows the file and then RUNS it, so what is printed
 * is what this machine said, not what someone remembered. Pages are plain
 * files: add one by dropping a file in the folder.
 *
 * A page starts with a header comment of "KEY: value" lines:
 *
 *   TITLE     the heading
 *   GROUP     the section it is listed under
 *   USES      the header / import it needs
 *   SUMMARY   what it does, in a line or two
 *   NOTES     caveats and gotchas, one per line
 *   SEE       where the real documentation is
 *   LIBS      extra linker flags (C only), e.g. -lm
 *
 * Header reference pages also use:
 *
 *   STANDARD  where it comes from: ISO C, POSIX, or Linux-specific
 *   PROVIDES  the functions, macros and types worth knowing, one per line
 *   TOOL      how a system-information tool (VoidFetch) uses it
 *
 * Pages ending in .txt are documentation only and are not run.
 *
 * Linux only: the pages read /proc and /sys.
 *
 * Build:  make
 * Run:    ./libraries     (from anywhere: it finds its pages by its own path)
 */

#define _POSIX_C_SOURCE 200809L

#include <ctype.h>
#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define WIDTH 54
#define MAX_PAGES 200
#define MAX_TEXT 4096

struct Track {
    const char *name;
    const char *directory;
    const char *extension;
    /* Compiled languages: how to build a page. The three %s are the scratch
     * directory, the page's source path and its LIBS. NULL: not compiled. */
    const char *compile_template;
    /* Interpreted languages: the program that runs a page. */
    const char *interpreter;
};

/* Add a language by adding a row and a folder of pages. */
static const struct Track TRACKS[] = {
    { "C",      "c",      ".c",  "cc -std=gnu11 -Wall -Wextra -o '%s/program' '%s' %s", NULL },
    { "Python", "python", ".py", NULL,                                                  "python3" },
};

#define TRACK_COUNT (int)(sizeof TRACKS / sizeof TRACKS[0])

struct Page {
    char path[PATH_MAX + 320];
    char name[256];
    char title[128];
    char group[64];
    char uses[256];
    char summary[MAX_TEXT];
    char notes[MAX_TEXT];
    char see[256];
    char libs[128];
    char standard[256];
    char provides[MAX_TEXT];
    char tool[MAX_TEXT];
    int documentation_only;
    long body_offset;
};

static char program_directory[PATH_MAX];

/* ---- screen ------------------------------------------------------------- */

static void clear_screen(void)
{
    /* ANSI codes rather than a spawned `clear`: no dependency on TERM. */
    fputs("\033[H\033[2J\033[3J", stdout);
    fflush(stdout);
}

static void rule(void)
{
    printf("  ");
    for (int i = 0; i < WIDTH; i++) {
        putchar('-');
    }
    putchar('\n');
}

static void frame(char fill)
{
    printf("  +");
    for (int i = 0; i < WIDTH - 2; i++) {
        putchar(fill);
    }
    printf("+\n");
}

static void title(const char *text)
{
    printf("\n");
    frame('=');
    printf("  | %-*.*s|\n", WIDTH - 3, WIDTH - 3, text);
    frame('=');
    printf("\n");
}

static void heading(const char *text)
{
    printf("\n  %s\n", text);
    rule();
}

static int read_line(char *destination, int size)
{
    if (fgets(destination, size, stdin) == NULL) {
        destination[0] = '\0';
        return 0;
    }
    destination[strcspn(destination, "\r\n")] = '\0';
    return 1;
}

static void wait_enter(void)
{
    char ignored[8];

    printf("\n  Press ENTER to continue...");
    fflush(stdout);
    read_line(ignored, sizeof ignored);
}

/* Print every line of `text`, indented, as it was written. */
static void print_lines(const char *text, const char *indent)
{
    const char *start = text;

    while (*start != '\0') {
        const char *end = strchr(start, '\n');
        int length = end != NULL ? (int) (end - start) : (int) strlen(start);
        printf("%s%.*s\n", indent, length, start);
        if (end == NULL) {
            break;
        }
        start = end + 1;
    }
}

/* "Label:  first line" and then the other lines under it, aligned. */
static void print_labelled(const char *label, const char *text)
{
    const char *newline = strchr(text, '\n');
    int first = newline != NULL ? (int) (newline - text) : (int) strlen(text);
    int width = (int) strlen(label) + 2;

    printf("  %s  %.*s\n", label, first, text);
    if (newline != NULL) {
        char indent[32];
        snprintf(indent, sizeof indent, "  %*s", width, "");
        print_lines(newline + 1, indent);
    }
}

/* ---- pages -------------------------------------------------------------- */

/* Append `line` to a field, one line per call. Fields are fixed buffers, so
 * a page with more text than fits is cut off rather than overrun. */
static void append_line(char *field, size_t size, const char *line)
{
    size_t used = strlen(field);

    if (used > 0 && used + 1 < size) {
        field[used++] = '\n';
        field[used] = '\0';
    }
    strncat(field, line, size - used - 1);
}

static char *field_for(struct Page *page, const char *key, size_t *size)
{
    struct { const char *key; char *field; size_t size; } fields[] = {
        { "TITLE",   page->title,   sizeof page->title },
        { "GROUP",   page->group,   sizeof page->group },
        { "USES",    page->uses,    sizeof page->uses },
        { "SUMMARY", page->summary, sizeof page->summary },
        { "NOTES",   page->notes,   sizeof page->notes },
        { "SEE",     page->see,     sizeof page->see },
        { "LIBS",    page->libs,    sizeof page->libs },
        { "STANDARD", page->standard, sizeof page->standard },
        { "PROVIDES", page->provides, sizeof page->provides },
        { "TOOL",     page->tool,     sizeof page->tool },
    };

    for (size_t i = 0; i < sizeof fields / sizeof fields[0]; i++) {
        if (strcmp(fields[i].key, key) == 0) {
            *size = fields[i].size;
            return fields[i].field;
        }
    }
    return NULL;
}

/* Is `line` the start of a field: capital letters, then a colon? */
static int field_start(const char *line, char *key, size_t key_size)
{
    size_t length = 0;

    while (isupper((unsigned char) line[length])) {
        length++;
    }
    if (length == 0 || line[length] != ':' || length >= key_size) {
        return 0;
    }
    memcpy(key, line, length);
    key[length] = '\0';
    return 1;
}

/* Read the header comment of the page at page->path. Understands a C comment
 * block, a Python docstring, and the --- separated header of a .txt page.
 * Returns 1 if a header was found. */
static int load_page(struct Page *page)
{
    FILE *file = fopen(page->path, "r");
    if (file == NULL) {
        return 0;
    }

    int c_comment = 0;
    int docstring = 0;
    int text_page = strstr(page->name, ".txt") != NULL;
    int in_header = text_page;
    int found = 0;
    char line[1024];
    char key[16];
    char *current = NULL;
    size_t current_size = 0;

    page->documentation_only = text_page;
    page->body_offset = 0;

    while (fgets(line, sizeof line, file) != NULL) {
        line[strcspn(line, "\r\n")] = '\0';
        char *text = line;

        if (!in_header) {
            while (*text == ' ' || *text == '\t') {
                text++;
            }
            if (strncmp(text, "/*", 2) == 0) {
                c_comment = in_header = 1;
                text += 2;
            } else if (strncmp(text, "\"\"\"", 3) == 0) {
                docstring = in_header = 1;
                text += 3;
            } else if (*text == '\0') {
                continue;
            } else {
                break; /* code before any header: no header */
            }
        } else if (text_page && strcmp(text, "---") == 0) {
            found = 1;
            page->body_offset = ftell(file);
            break;
        } else if ((c_comment && strstr(text, "*/") != NULL)
                   || (docstring && strstr(text, "\"\"\"") != NULL)) {
            found = 1;
            page->body_offset = ftell(file);
            break;
        }

        /* Strip the " * " that decorates C comment lines. */
        if (c_comment) {
            while (*text == ' ' || *text == '\t') {
                text++;
            }
            if (*text == '*') {
                text++;
            }
        }
        if (*text == ' ') {
            text++;
        }
        if (*text == '\0') {
            continue;
        }

        if (field_start(text, key, sizeof key)) {
            current = field_for(page, key, &current_size);
            text += strlen(key) + 1;
            while (*text == ' ') {
                text++;
            }
            if (current != NULL && *text != '\0') {
                append_line(current, current_size, text);
            }
        } else if (current != NULL) {
            /* Summaries wrap onto indented lines: join them flush left.
             * Lists (notes, provides, tool) keep their own indentation. */
            if (current != page->notes && current != page->provides && current != page->tool) {
                while (*text == ' ') {
                    text++;
                }
            }
            append_line(current, current_size, text);
        }
    }

    fclose(file);
    return found && page->title[0] != '\0';
}

static int by_name(const struct dirent **a, const struct dirent **b)
{
    return strcmp((*a)->d_name, (*b)->d_name);
}

static int is_page_name(const char *name, const char *extension)
{
    size_t length = strlen(name);
    size_t extension_length = strlen(extension);

    if (name[0] == '.') {
        return 0;
    }
    return (length > extension_length
            && strcmp(name + length - extension_length, extension) == 0)
           || (length > 4 && strcmp(name + length - 4, ".txt") == 0);
}

/* Load every page of a track, in file-name order. Returns how many. */
static int load_track(const struct Track *track, struct Page *pages)
{
    char directory[PATH_MAX + 64];
    struct dirent **entries;
    const char *extension = track->extension;
    int count = 0;

    snprintf(directory, sizeof directory, "%s/%s", program_directory, track->directory);
    int found = scandir(directory, &entries, NULL, by_name);
    if (found < 0) {
        return 0;
    }

    for (int i = 0; i < found && count < MAX_PAGES; i++) {
        if (is_page_name(entries[i]->d_name, extension)) {
            struct Page *page = &pages[count];
            memset(page, 0, sizeof *page);
            snprintf(page->name, sizeof page->name, "%s", entries[i]->d_name);
            if (snprintf(page->path, sizeof page->path, "%s/%s", directory, page->name)
                    < (int) sizeof page->path && load_page(page)) {
                count++;
            }
        }
        free(entries[i]);
    }
    free(entries);
    return count;
}

/* ---- showing a page ----------------------------------------------------- */

static void show_body(const struct Page *page)
{
    FILE *file = fopen(page->path, "r");
    char line[1024];
    int seen_text = 0;

    if (file == NULL) {
        return;
    }
    fseek(file, page->body_offset, SEEK_SET);
    while (fgets(line, sizeof line, file) != NULL) {
        line[strcspn(line, "\r\n")] = '\0';
        if (!seen_text && line[0] == '\0') {
            continue; /* the blank lines between the header and the code */
        }
        seen_text = 1;
        printf("    %s\n", line);
    }
    fclose(file);
}

static int safe_path(const char *path)
{
    return strchr(path, '\'') == NULL;
}

static int run_shell(const char *command)
{
    int status = system(command);

    fflush(stdout);
    return status;
}

/* Compile (if the language needs it) and run the page, showing the commands
 * the way a terminal would and the real output of each. */
static void run_page(const struct Track *track, const struct Page *page)
{
    char command[PATH_MAX * 3];
    char scratch[PATH_MAX];
    char program[PATH_MAX + 16];

    if (!safe_path(page->path) || !safe_path(program_directory)) {
        puts("    (not run: the path contains a quote)");
        return;
    }

    snprintf(scratch, sizeof scratch, "%s/codelearner-libraries-XXXXXX",
             getenv("TMPDIR") != NULL ? getenv("TMPDIR") : "/tmp");
    if (mkdtemp(scratch) == NULL) {
        puts("    (not run: could not make a scratch directory)");
        return;
    }
    snprintf(program, sizeof program, "%s/program", scratch);

    if (track->compile_template != NULL) {
        printf("    $ cc -std=gnu11 -Wall -Wextra %s -o program %s\n", page->name, page->libs);
        fflush(stdout);
        snprintf(command, sizeof command, "( ");
        snprintf(command + 2, sizeof command - 2, track->compile_template,
                 scratch, page->path, page->libs);
        strncat(command, " ) 2>&1 | sed 's/^/      /'", sizeof command - strlen(command) - 1);
        run_shell(command);

        if (access(program, X_OK) != 0) {
            puts("    (it did not compile)");
        } else {
            printf("    $ ./program\n");
            fflush(stdout);
            snprintf(command, sizeof command, "timeout 20 '%s' 2>&1 | sed 's/^/      /'", program);
            run_shell(command);
        }
    } else {
        printf("    $ %s %s\n", track->interpreter, page->name);
        fflush(stdout);
        snprintf(command, sizeof command, "timeout 20 %s '%s' 2>&1 | sed 's/^/      /'",
                 track->interpreter, page->path);
        run_shell(command);
    }

    /* Only ever remove what mkdtemp just made. */
    if (strstr(scratch, "/codelearner-libraries-") != NULL) {
        snprintf(command, sizeof command, "rm -rf '%s'", scratch);
        if (run_shell(command) != 0) {
            fprintf(stderr, "  (could not remove %s)\n", scratch);
        }
    }
}

static void show_page(const struct Track *track, const struct Page *page)
{
    clear_screen();
    title(page->title);

    if (page->uses[0] != '\0') {
        print_labelled("Uses:", page->uses);
        printf("\n");
    }
    if (page->standard[0] != '\0') {
        print_labelled("Standard:", page->standard);
        printf("\n");
    }
    if (page->summary[0] != '\0') {
        print_lines(page->summary, "  ");
        printf("\n");
    }
    if (page->provides[0] != '\0') {
        printf("  Provides:\n");
        print_lines(page->provides, "  ");
        printf("\n");
    }
    if (page->notes[0] != '\0') {
        printf("  Good to know:\n");
        print_lines(page->notes, "  ");
        printf("\n");
    }
    if (page->tool[0] != '\0') {
        printf("  In VoidFetch:\n");
        print_lines(page->tool, "  ");
        printf("\n");
    }
    if (page->see[0] != '\0') {
        printf("  Documentation: %s\n", page->see);
    }

    if (page->documentation_only) {
        printf("\n");
        show_body(page);
    } else {
        heading("CODE");
        show_body(page);
        heading("RUN, on this machine");
        run_page(track, page);
    }
    wait_enter();
}

/* ---- menus -------------------------------------------------------------- */

static int contains_ignoring_case(const char *haystack, const char *needle)
{
    size_t length = strlen(needle);

    if (length == 0) {
        return 1;
    }
    for (; *haystack != '\0'; haystack++) {
        size_t i = 0;
        while (i < length && haystack[i] != '\0'
               && tolower((unsigned char) haystack[i]) == tolower((unsigned char) needle[i])) {
            i++;
        }
        if (i == length) {
            return 1;
        }
    }
    return 0;
}

/* Does the page mention `query` in its title, summary, notes, header or code? */
static int page_matches(const struct Page *page, const char *query)
{
    if (contains_ignoring_case(page->title, query) || contains_ignoring_case(page->summary, query)
        || contains_ignoring_case(page->notes, query) || contains_ignoring_case(page->uses, query)) {
        return 1;
    }

    FILE *file = fopen(page->path, "r");
    char line[1024];
    int found = 0;

    if (file == NULL) {
        return 0;
    }
    while (!found && fgets(line, sizeof line, file) != NULL) {
        found = contains_ignoring_case(line, query);
    }
    fclose(file);
    return found;
}

static void show_menu(const struct Track *track, const struct Page *pages, int count,
                      const char *query)
{
    char heading_text[128];
    const char *shown_group = "";

    clear_screen();
    snprintf(heading_text, sizeof heading_text, "LIBRARIES - %s", track->name);
    title(heading_text);

    if (query[0] != '\0') {
        printf("  Pages mentioning \"%s\":\n", query);
    }
    for (int i = 0; i < count; i++) {
        if (query[0] != '\0' && !page_matches(&pages[i], query)) {
            continue;
        }
        if (strcmp(pages[i].group, shown_group) != 0) {
            shown_group = pages[i].group;
            printf("\n  -- %s --\n", shown_group[0] != '\0' ? shown_group : "OTHER");
        }
        printf("   [%2d]  %s\n", i + 1, pages[i].title);
    }

    printf("\n   Type a number to open a page, a word to search (try: memory),\n");
    printf("   or ENTER to go back.\n");
    rule();
}

static void run_track(const struct Track *track)
{
    static struct Page pages[MAX_PAGES];
    char query[64] = "";
    char choice[64];
    int count = load_track(track, pages);

    if (count == 0) {
        printf("\n  No %s pages found next to this program.\n", track->name);
        wait_enter();
        return;
    }

    for (;;) {
        show_menu(track, pages, count, query);
        printf("\n  Page or search: ");
        fflush(stdout);

        if (!read_line(choice, sizeof choice) || choice[0] == '\0') {
            if (query[0] != '\0') {
                query[0] = '\0'; /* leave the search before leaving the track */
                if (choice[0] == '\0' && feof(stdin)) {
                    return;
                }
                continue;
            }
            return;
        }

        char *end;
        long number = strtol(choice, &end, 10);
        if (*end == '\0') {
            if (number >= 1 && number <= count) {
                show_page(track, &pages[number - 1]);
            } else if (number == 0) {
                return;
            }
        } else {
            snprintf(query, sizeof query, "%s", choice);
        }
    }
}

static void show_tracks(void)
{
    clear_screen();
    title("LIBRARIES - A REFERENCE, NOT A COURSE");
    puts("  How to use libraries, one page per job, with the code and its");
    puts("  real output on this machine. Pick a language:\n");
    for (int i = 0; i < TRACK_COUNT; i++) {
        printf("   [%d]  %s\n", i + 1, TRACKS[i].name);
    }
    printf("\n   [0]  Quit\n");
    rule();
}

int main(void)
{
    char choice[16];
    ssize_t length = readlink("/proc/self/exe", program_directory, sizeof program_directory - 1);

    if (length <= 0) {
        fputs("libraries: cannot find its own path (Linux only)\n", stderr);
        return 1;
    }
    program_directory[length] = '\0';
    *strrchr(program_directory, '/') = '\0';

    for (;;) {
        show_tracks();
        printf("\n  Pick a language: ");
        fflush(stdout);

        if (!read_line(choice, sizeof choice) || strcmp(choice, "0") == 0) {
            break;
        }

        int n = atoi(choice);
        if (n >= 1 && n <= TRACK_COUNT) {
            run_track(&TRACKS[n - 1]);
        }
    }

    printf("\n  See you next time.\n\n");
    return 0;
}
