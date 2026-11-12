/*
 * Libraries - a reference browser, not a course.
 *
 * One manual per library, each laid out like that library's own official
 * documentation: the C manual like the Linux man-pages (NAME, LIBRARY,
 * SYNOPSIS, DESCRIPTION, RETURN VALUE, ERRORS, ATTRIBUTES, VERSIONS,
 * STANDARDS, HISTORY, NOTES, EXAMPLES, SEE ALSO), the Python manual like
 * docs.python.org's library reference (one page per module, an entry per
 * function). A page covers one function, a family of closely related
 * functions, one header, one module or one system file, never two unrelated
 * libraries. The example is compiled and run on this machine each time the
 * page is opened. There are no lessons and no exercises.
 *
 * Every page is a real source file in the folder of its manual, next to this
 * program. The page shows the file and then RUNS it, so what is printed is
 * what this machine said. Pages are plain files: add one by dropping a file
 * in the folder.
 *
 * A page starts with a header comment of "KEY: value" lines. The section each
 * one is shown under is on the right:
 *
 *   TITLE      "name: what it is for"                     NAME
 *   GROUP      the part of the manual it is listed under  (menu)
 *   LIBRARY    the library that provides it               LIBRARY
 *   USES       the header / import it needs               SYNOPSIS
 *   SIGNATURE  how to call it, one per line               SYNOPSIS
 *   PROVIDES   functions, macros and types, one per line  SYNOPSIS
 *   LIBS       extra linker flags (C only), e.g. -lm      LIBRARY
 *   SUMMARY    what it does                               DESCRIPTION
 *   ENTRY      one function, class or constant: its       REFERENCE
 *              signature, then its description indented
 *              below. Repeat ENTRY once per item.
 *   PARAMETERS what each argument means                   PARAMETERS
 *   RETURNS    what comes back                            RETURN VALUE
 *   ERRORS     how it fails, and what to look for         ERRORS
 *   ATTRIBUTES thread safety and similar                  ATTRIBUTES
 *   VERSIONS   differences between versions               VERSIONS
 *   STANDARD   ISO C, POSIX, or Linux-specific            STANDARDS
 *   HISTORY    when it appeared                           HISTORY
 *   NOTES      caveats, one per line                      NOTES
 *   USEDFOR    common uses (POSIX header pages)           APPLICATION USAGE
 *   SEE        related pages and upstream documentation   SEE ALSO
 *
 * Pages ending in .txt are documentation only and are not run.
 *
 * Linux only: the pages read /proc and /sys.
 *
 * Output is coloured in a terminal (NO_COLOR=1 turns it off) and pauses with
 * "more: press ENTER" when a page would scroll off the screen
 * (CODELEARNER_NO_PAGER=1 turns that off). Both are off when piped.
 *
 * Build:  make
 * Run:    ./libraries     (from anywhere: it finds its pages by its own path)
 */

#define _POSIX_C_SOURCE 200809L

#include <ctype.h>
#include <dirent.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define WIDTH 54
#define MAX_PAGES 160
#define MAX_TEXT 4096
#define MAX_ENTRIES 24000

struct Track {
    const char *name;            /* the manual's name in the menu */
    const char *about;           /* one line under it */
    const char *directory;
    const char *extension;
    /* Compiled languages: how to build a page. The three %s are the scratch
     * directory, the page's source path and its LIBS. NULL: not compiled. */
    const char *compile_template;
    /* What the page shows as the compile command, with %s for the file name
     * and the page's LIBS. */
    const char *shown_command;
    /* Interpreted languages: the program that runs a page. */
    const char *interpreter;
};

/* raylib and raygui are third-party and built by the reader (see the raylib
 * course, module 1). RAYLIB and RAYGUI name their directories; the defaults
 * are $HOME/raylib and $HOME/raygui. A page whose library is missing fails to
 * compile and says so; it does not break the others. */
#define RAYLIB_COMPILE \
    "RL=\"${RAYLIB:-$HOME/raylib}\"; RG=\"${RAYGUI:-$HOME/raygui}\"; " \
    "cc -std=gnu11 -Wall -Wextra -I\"$RL/src\" -isystem \"$RG/src\" " \
    "-o '%s/program' '%s' \"$RL/build/raylib/libraylib.a\" " \
    "-lm -lpthread -ldl -lrt -lGL -lX11 %s"
#define RAYLIB_SHOWN \
    "cc -Wall -Wextra -I$RAYLIB/src -isystem $RAYGUI/src %s -o program " \
    "$RAYLIB/build/raylib/libraylib.a -lm -lpthread -ldl -lrt -lGL -lX11 %s"

/* One row per manual. Add a manual by adding a row and a folder of pages. */
static const struct Track TRACKS[] = {
    { "C library", "ISO C, POSIX and Linux: headers, functions and system files",
      "c", ".c", "cc -std=gnu11 -Wall -Wextra -o '%s/program' '%s' %s",
      "cc -std=gnu11 -Wall -Wextra %s -o program %s", NULL },
    { "Python standard library", "one page per module, like docs.python.org",
      "python", ".py", NULL, NULL, "python3" },
    { "raylib", "windows, drawing, input, sound: every function, from raylib.h",
      "raylib", ".c", RAYLIB_COMPILE, RAYLIB_SHOWN, NULL },
    { "raygui", "immediate-mode controls: every function, from raygui.h",
      "raygui", ".c", RAYLIB_COMPILE, RAYLIB_SHOWN, NULL },
};

#define TRACK_COUNT (int)(sizeof TRACKS / sizeof TRACKS[0])

/* Portuguese (PT-PT) for the menus. CODELEARNER_LANG=pt-pt turns it on; the
 * pages themselves stay in English. */
static const char *const TEXT_PT[][2] = {
    { "LIBRARIES - A REFERENCE MANUAL", "BIBLIOTECAS - MANUAL DE REFERENCIA" },
    { "One manual per library, laid out like its official documentation,", "Um manual por biblioteca, organizado como a sua documentação oficial," },
    { "with each example run on this machine. Pick a manual:", "com cada exemplo corrido nesta máquina. Escolhe um manual:" },
    { "Pick a manual:", "Escolhe um manual:" },
    { "-- more: press ENTER --", "-- mais: prime ENTER --" },
    { "Page or search:", "Página ou pesquisa:" },
    { "Type a number to open a page, a word to search (try: memory),", "Escreve um número para abrir uma página, uma palavra para pesquisar," },
    { "or ENTER to go back.", "ou ENTER para voltar." },
    { "Common tasks", "Tarefas comuns" },
    { "Press ENTER to continue...", "Prime ENTER para continuar..." },
    { "See you next time.", "Até à próxima." },
    { "Quit", "Sair" },
};

static const char *tr(const char *text)
{
    const char *value = getenv("CODELEARNER_LANG");

    if (value != NULL && strcmp(value, "pt-pt") == 0) {
        for (size_t i = 0; i < sizeof TEXT_PT / sizeof TEXT_PT[0]; i++) {
            if (strcmp(TEXT_PT[i][0], text) == 0) {
                return TEXT_PT[i][1];
            }
        }
    }
    return text;
}

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
    char used_for[MAX_TEXT];
    char signature[MAX_TEXT];
    char parameters[MAX_TEXT];
    char returns[MAX_TEXT];
    char errors[MAX_TEXT];
    char library[256];
    char attributes[512];
    char versions[MAX_TEXT];
    char history[MAX_TEXT];
    char entries[MAX_ENTRIES];
    int documentation_only;
    long body_offset;
};

static char program_directory[PATH_MAX];

/* ---- colour and paging ---------------------------------------------------
 *
 * Colour is on only for a terminal with NO_COLOR unset; CODELEARNER_COLOR=1
 * or 0 forces it. Every printf goes through ui_printf(), which counts the
 * lines shown since the last clear and, on a terminal, pauses with
 * "more: press ENTER" before a page would scroll off. Piped output is
 * untouched, and CODELEARNER_NO_PAGER=1 turns paging off. */

static const char *C_RESET = "", *C_BOLD = "", *C_DIM = "", *C_RED = "",
                  *C_GREEN = "", *C_YELLOW = "", *C_CYAN = "";

static void colour_init(void)
{
    const char *force = getenv("CODELEARNER_COLOR");
    const char *no_color = getenv("NO_COLOR");
    int on;

    if (force != NULL && *force != '\0') {
        on = force[0] != '0';
    } else {
        on = (no_color == NULL || *no_color == '\0') && isatty(fileno(stdout));
    }
    if (on) {
        C_RESET = "\033[0m";
        C_BOLD = "\033[1m";
        C_DIM = "\033[2m";
        C_RED = "\033[31m";
        C_GREEN = "\033[32m";
        C_YELLOW = "\033[33m";
        C_CYAN = "\033[36m";
    }
}

static int page_rows = -1;   /* -1 not decided yet, 0 paging off */
static int page_cols = 80;
static int page_lines;       /* lines shown since the last clear */

static void page_init(void)
{
    struct winsize size;

    page_rows = 0;
    if (isatty(fileno(stdout)) && getenv("CODELEARNER_NO_PAGER") == NULL
        && ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) == 0 && size.ws_row > 8) {
        page_rows = size.ws_row;
        page_cols = size.ws_col > 20 ? size.ws_col : 80;
    }
}

/* Visible columns of s[0..length): colour codes take none. */
static int visible_width(const char *s, size_t length)
{
    int width = 0;

    for (size_t i = 0; i < length; i++) {
        if (s[i] == '\033' && i + 1 < length && s[i + 1] == '[') {
            i += 2;
            while (i < length && !isalpha((unsigned char) s[i])) {
                i++;
            }
        } else if (((unsigned char) s[i] & 0xC0) != 0x80) {
            width++;
        }
    }
    return width;
}

static const char *tr(const char *text);

static void page_pause(void)
{
    int c;

    fprintf(stdout, "  %s%s%s", C_DIM, tr("-- more: press ENTER --"), C_RESET);
    fflush(stdout);
    while ((c = getchar()) != '\n' && c != EOF) {
        /* discard */
    }
    fputs("\033[A\033[2K\r", stdout);
    page_lines = 0;
}

static int ui_printf(const char *format, ...)
{
    char small[1024];
    char *buffer = small;
    va_list args, again;

    va_start(args, format);
    va_copy(again, args);
    int length = vsnprintf(small, sizeof small, format, args);
    va_end(args);
    if (length >= (int) sizeof small) {
        buffer = malloc((size_t) length + 1);
        if (buffer != NULL) {
            vsnprintf(buffer, (size_t) length + 1, format, again);
        } else {
            buffer = small;
        }
    }
    va_end(again);

    if (page_rows < 0) {
        page_init();
    }
    if (page_rows == 0) {
        fputs(buffer, stdout);
    } else {
        const char *p = buffer;
        while (*p != '\0') {
            const char *newline = strchr(p, '\n');
            size_t part = newline != NULL ? (size_t) (newline - p) + 1 : strlen(p);
            fwrite(p, 1, part, stdout);
            if (newline != NULL) {
                int width = visible_width(p, part - 1);
                page_lines += 1 + (width > page_cols ? (width - 1) / page_cols : 0);
                if (page_lines >= page_rows - 3) {
                    page_pause();
                }
            }
            p += part;
        }
    }
    if (buffer != small) {
        free(buffer);
    }
    return length;
}

#define printf ui_printf

/* ---- screen ------------------------------------------------------------- */

static void clear_screen(void)
{
    /* ANSI codes rather than a spawned `clear`: no dependency on TERM. */
    fputs("\033[H\033[2J\033[3J", stdout);
    fflush(stdout);
    page_lines = 0;
}

static void rule(void)
{
    printf("  %s", C_DIM);
    for (int i = 0; i < WIDTH; i++) {
        putchar('-');
    }
    printf("%s\n", C_RESET);
}

static void frame(char fill)
{
    printf("  %s+", C_CYAN);
    for (int i = 0; i < WIDTH - 2; i++) {
        putchar(fill);
    }
    printf("+%s\n", C_RESET);
}

static void title(const char *text)
{
    printf("\n");
    frame('=');
    printf("  %s|%s %s%-*.*s%s%s|%s\n", C_CYAN, C_RESET, C_BOLD, WIDTH - 3, WIDTH - 3, text,
           C_RESET, C_CYAN, C_RESET);
    frame('=');
    printf("\n");
}

static void heading(const char *text)
{
    printf("\n  %s%s%s%s\n", C_BOLD, C_CYAN, text, C_RESET);
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

    printf("\n  %s%s%s", C_DIM, tr("Press ENTER to continue..."), C_RESET);
    fflush(stdout);
    read_line(ignored, sizeof ignored);
    page_lines = 0;
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

/* A man page section: its name, then `text` indented under it. */
static void section(const char *name, const char *text, const char *indent)
{
    if (text[0] != '\0') {
        printf("\n  %s%s%s\n", C_BOLD, name, C_RESET);
        print_lines(text, indent);
    }
}

/* The REFERENCE section: one entry per function, class or constant. A line
 * with no indentation is a signature; the lines under it are its description. */
static void entries_section(const char *text)
{
    const char *start = text;

    if (text[0] == '\0') {
        return;
    }
    printf("\n  %sREFERENCE%s\n", C_BOLD, C_RESET);
    while (*start != '\0') {
        const char *end = strchr(start, '\n');
        int length = end != NULL ? (int) (end - start) : (int) strlen(start);

        if (length == 0) {
            printf("\n");
        } else if (start[0] != ' ') {
            printf("    %s%s%.*s%s\n", C_BOLD, C_CYAN, length, start, C_RESET);
        } else {
            printf("    %.*s\n", length, start);
        }
        if (end == NULL) {
            break;
        }
        start = end + 1;
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
        { "USEDFOR",  page->used_for, sizeof page->used_for },
        { "SIGNATURE",  page->signature,  sizeof page->signature },
        { "PARAMETERS", page->parameters, sizeof page->parameters },
        { "RETURNS",    page->returns,    sizeof page->returns },
        { "ERRORS",     page->errors,     sizeof page->errors },
        { "LIBRARY",    page->library,    sizeof page->library },
        { "ATTRIBUTES", page->attributes, sizeof page->attributes },
        { "VERSIONS",   page->versions,   sizeof page->versions },
        { "HISTORY",    page->history,    sizeof page->history },
        { "ENTRY",      page->entries,    sizeof page->entries },
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
            if (current == page->entries && page->entries[0] != '\0') {
                append_line(current, current_size, "");   /* one blank line between entries */
            }
            if (current != NULL && *text != '\0') {
                append_line(current, current_size, text);
            }
        } else if (current != NULL) {
            /* Summaries wrap onto indented lines: join them flush left.
             * Lists (notes, provides, used for) keep their own indentation. */
            if (current != page->notes && current != page->provides && current != page->used_for
                && current != page->signature && current != page->parameters
                && current != page->errors && current != page->entries
                && current != page->history && current != page->versions) {
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

/* Print one line of example code, colouring a trailing comment. `marker` is
 * "//" for C and "#" for Python. Quote-aware, so a marker inside a string is
 * left alone. */
static void print_code_line(const char *line, const char *marker)
{
    size_t marker_length = strlen(marker);
    char quote = '\0';

    for (const char *p = line; *p != '\0'; p++) {
        if (quote != '\0') {
            if (*p == '\\' && p[1] != '\0') {
                p++;
            } else if (*p == quote) {
                quote = '\0';
            }
        } else if (*p == '"' || *p == '\'') {
            quote = *p;
        } else if (strncmp(p, marker, marker_length) == 0) {
            printf("    %.*s%s%s%s\n", (int) (p - line), line, C_GREEN, p, C_RESET);
            return;
        }
    }
    printf("    %s\n", line);
}

static void show_body(const struct Page *page)
{
    FILE *file = fopen(page->path, "r");
    char line[1024];
    int seen_text = 0;
    const char *marker = strstr(page->name, ".py") != NULL ? "#" : "//";

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
        print_code_line(line, marker);
    }
    fclose(file);
}

static int safe_path(const char *path)
{
    return strchr(path, '\'') == NULL;
}

/* Run a command and show its output indented, line by line through the pager. */
static int run_shell(const char *command)
{
    char line[1024];
    char full[PATH_MAX * 3 + 64];

    fflush(stdout);
    snprintf(full, sizeof full, "( %s ) 2>&1", command);
    FILE *pipe = popen(full, "r");
    if (pipe == NULL) {
        return -1;
    }
    while (fgets(line, sizeof line, pipe) != NULL) {
        line[strcspn(line, "\r\n")] = '\0';
        printf("      %s%s%s\n", C_DIM, line, C_RESET);
    }
    return pclose(pipe);
}

/* Run a command whose output is not shown (removing the scratch directory). */
static int run_quiet(const char *command)
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
    if (!safe_path(scratch)) {
        puts("    (not run: TMPDIR contains a quote)");
        return;
    }
    if (mkdtemp(scratch) == NULL) {
        puts("    (not run: could not make a scratch directory)");
        return;
    }
    snprintf(program, sizeof program, "%s/program", scratch);

    if (track->compile_template != NULL) {
        char shown[PATH_MAX];
        snprintf(shown, sizeof shown, track->shown_command, page->name, page->libs);
        printf("    %s$ %s%s\n", C_BOLD, shown, C_RESET);
        fflush(stdout);
        snprintf(command, sizeof command, track->compile_template,
                 scratch, page->path, page->libs);
        run_shell(command);

        if (access(program, X_OK) != 0) {
            puts("    (it did not compile)");
        } else {
            printf("    %s$ ./program%s\n", C_BOLD, C_RESET);
            fflush(stdout);
            snprintf(command, sizeof command, "timeout 20 '%s'", program);
            run_shell(command);
        }
    } else {
        printf("    %s$ %s %s%s\n", C_BOLD, track->interpreter, page->name, C_RESET);
        fflush(stdout);
        snprintf(command, sizeof command, "timeout 20 %s '%s'",
                 track->interpreter, page->path);
        run_shell(command);
    }

    /* Only ever remove what mkdtemp just made. */
    if (strstr(scratch, "/codelearner-libraries-") != NULL) {
        snprintf(command, sizeof command, "rm -rf '%s'", scratch);
        if (run_quiet(command) != 0) {
            fprintf(stderr, "  (could not remove %s)\n", scratch);
        }
    }
}

static void show_page(const struct Track *track, const struct Page *page)
{
    /* The frame shows the name alone ("stdio.h", "printf(3)"), as a man
     * page header does; the full title is the NAME section. */
    char name[sizeof page->title];
    snprintf(name, sizeof name, "%s", page->title);
    char *colon = strstr(name, ": ");
    if (colon != NULL) {
        *colon = '\0';
    }

    clear_screen();
    title(name);

    section("NAME", page->title, "      ");
    if (page->library[0] != '\0' || page->libs[0] != '\0') {
        printf("\n  %sLIBRARY%s\n", C_BOLD, C_RESET);
        if (page->library[0] != '\0') {
            print_lines(page->library, "      ");
        }
        if (page->libs[0] != '\0') {
            printf("      Link with %s.\n", page->libs);
        }
    }
    if (page->uses[0] != '\0' || page->provides[0] != '\0' || page->signature[0] != '\0') {
        printf("\n  %sSYNOPSIS%s\n", C_BOLD, C_RESET);
        if (page->uses[0] != '\0') {
            print_lines(page->uses, "      ");
        }
        if (page->signature[0] != '\0') {
            printf("\n");
            print_lines(page->signature, "    ");
        }
        if (page->provides[0] != '\0') {
            printf("\n");
            print_lines(page->provides, "    ");
        }
    }
    section("DESCRIPTION", page->summary, "      ");
    entries_section(page->entries);
    section("PARAMETERS", page->parameters, "    ");
    section("RETURN VALUE", page->returns, "      ");
    section("ERRORS", page->errors, "    ");
    section("ATTRIBUTES", page->attributes, "      ");
    section("VERSIONS", page->versions, "    ");
    section("STANDARDS", page->standard, "      ");
    section("HISTORY", page->history, "    ");
    section("NOTES", page->notes, "    ");
    section("APPLICATION USAGE", page->used_for, "    ");

    if (page->documentation_only) {
        printf("\n");
        show_body(page);
    } else {
        heading("EXAMPLES");
        show_body(page);
        heading("EXAMPLES: OUTPUT, on this machine");
        run_page(track, page);
    }
    section("SEE ALSO", page->see, "      ");
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
        || contains_ignoring_case(page->signature, query)
        || contains_ignoring_case(page->notes, query) || contains_ignoring_case(page->uses, query)
        || contains_ignoring_case(page->entries, query)) {
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
            printf("\n  -- %s --\n", shown_group[0] != '\0' ? tr(shown_group) : "OTHER");
        }
        printf("   [%2d]  %s\n", i + 1, pages[i].title);
    }

    printf("\n   %s\n", tr("Type a number to open a page, a word to search (try: memory),"));
    printf("   %s\n", tr("or ENTER to go back."));
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
        printf("\n  %s ", tr("Page or search:"));
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
    title(tr("LIBRARIES - A REFERENCE MANUAL"));
    printf("  %s\n", tr("One manual per library, laid out like its official documentation,"));
    printf("  %s\n\n", tr("with each example run on this machine. Pick a manual:"));
    for (int i = 0; i < TRACK_COUNT; i++) {
        printf("   [%d]  %s%s%s\n", i + 1, C_BOLD, TRACKS[i].name, C_RESET);
        printf("        %s%s%s\n", C_DIM, TRACKS[i].about, C_RESET);
    }
    printf("\n   [0]  %s\n", tr("Quit"));
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

    colour_init();
    for (;;) {
        show_tracks();
        printf("\n  %s ", tr("Pick a manual:"));
        fflush(stdout);

        if (!read_line(choice, sizeof choice) || strcmp(choice, "0") == 0) {
            break;
        }

        char *end;
        long n = strtol(choice, &end, 10);
        if (*end == '\0' && n >= 1 && n <= TRACK_COUNT) {
            run_track(&TRACKS[n - 1]);
        }
    }

    printf("\n  %s\n\n", tr("See you next time."));
    return 0;
}
