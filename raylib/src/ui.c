/* fileno() and isatty() are POSIX, not ISO C; -std=c11 hides them without this. */
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#ifndef _WIN32
#include <sys/ioctl.h>
#endif

#ifdef _WIN32
#include <io.h>
#define isatty _isatty
#define fileno _fileno
#else
#include <unistd.h>
#endif

#include "ui.h"

const char *C_RESET = "", *C_BOLD = "", *C_DIM = "", *C_RED = "",
           *C_GREEN = "", *C_YELLOW = "", *C_BLUE = "", *C_MAGENTA = "",
           *C_CYAN = "";

void ui_init(void)
{
    const char *force = getenv("CODELEARNER_COLOR");
    int on;

    if (force != NULL && *force != '\0') {
        on = force[0] != '0';
    } else {
        const char *no_color = getenv("NO_COLOR");
        on = (no_color == NULL || *no_color == '\0') && isatty(fileno(stdout));
#ifdef _WIN32
        /* The old console host ignores ANSI codes; Windows Terminal does not. */
        on = on && getenv("WT_SESSION") != NULL;
#endif
    }

    if (on) {
        C_RESET = "\033[0m";
        C_BOLD = "\033[1m";
        C_DIM = "\033[2m";
        C_RED = "\033[31m";
        C_GREEN = "\033[32m";
        C_YELLOW = "\033[33m";
        C_BLUE = "\033[34m";
        C_MAGENTA = "\033[35m";
        C_CYAN = "\033[36m";
    }
}

/* Inside width of the frame. Changing this changes every box in the course. */
#define WIDTH 54

/* ---- paging (see ui.h) -------------------------------------------------- */

static int page_rows = -1;   /* -1 not decided yet, 0 paging off */
static int page_cols = 80;
static int page_lines;       /* lines shown since the last clear */

static void page_init(void)
{
    page_rows = 0;
#ifndef _WIN32
    if (isatty(fileno(stdout)) && getenv("CODELEARNER_NO_PAGER") == NULL) {
        struct winsize w;
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0 && w.ws_row > 8) {
            page_rows = w.ws_row;
            page_cols = w.ws_col > 20 ? w.ws_col : 80;
        }
    }
#endif
}

static void ui_page_reset(void)
{
    page_lines = 0;
}

/* Visible columns of s[0..len): ANSI colour codes take none. */
static int visible_width(const char *s, size_t len)
{
    int width = 0;
    for (size_t i = 0; i < len; i++) {
        if (s[i] == '\033' && i + 1 < len && s[i + 1] == '[') {
            i += 2;
            while (i < len && !((s[i] >= 'A' && s[i] <= 'Z') || (s[i] >= 'a' && s[i] <= 'z'))) {
                i++;
            }
        } else if (((unsigned char) s[i] & 0xC0) != 0x80) {
            width++;   /* count UTF-8 characters, not bytes */
        }
    }
    return width;
}

static void page_pause(void)
{
    fputs("  \033[2m-- more: press ENTER --\033[0m", stdout);
    fflush(stdout);
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* discard */
    }
    fputs("\033[A\033[2K\r", stdout);   /* erase the prompt line */
    page_lines = 0;
}

int ui_printf(const char *format, ...)
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

void clear_screen(void)
{
#ifdef _WIN32
    /* The old cmd.exe does not understand ANSI codes; cls handles it. */
    system("cls");
#else
    /* ANSI codes rather than system("clear"): no second process per screen,
     * and no dependency on the TERM variable, which is missing when the
     * program runs outside a normal terminal. */
    fputs("\033[H\033[2J\033[3J", stdout);
    fflush(stdout);
    ui_page_reset();
#endif
}

void wait_enter(void)
{
    printf("\n  %s%s%s", C_DIM, tr("Press ENTER to continue..."), C_RESET);
    fflush(stdout);

    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* discard */
    }
    ui_page_reset();
}

void rule(void)
{
    printf("  %s", C_DIM);
    for (int i = 0; i < WIDTH; i++) {
        putchar('-');
    }
    printf("%s\n", C_RESET);
}

static void padded_line(const char *text, char border)
{
    int written = (int) strlen(text);
    printf("  %s%c%s %s%s%s", C_CYAN, border, C_RESET, C_BOLD, text, C_RESET);

    for (int i = written; i < WIDTH - 3; i++) {
        putchar(' ');
    }
    printf("%s%c%s\n", C_CYAN, border, C_RESET);
}

static void frame(char fill)
{
    printf("  %s+", C_CYAN);
    for (int i = 0; i < WIDTH - 2; i++) {
        putchar(fill);
    }
    printf("+%s\n", C_RESET);
}

void title(const char *text)
{
    printf("\n");
    frame('=');
    padded_line(text, '|');
    frame('=');
    printf("\n");
}

void heading(const char *text)
{
    printf("\n  %s%s%s%s\n", C_BOLD, C_CYAN, tr(text), C_RESET);
    rule();
}

int read_line(char *dest, int size)
{
    if (fgets(dest, size, stdin) == NULL) {
        dest[0] = '\0';
        return 0;
    }

    dest[strcspn(dest, "\n")] = '\0';
    return 1;
}

int ask_yes(const char *question_text)
{
    char answer[8];

    printf("\n  %s (y/N): ", question_text);
    fflush(stdout);

    if (!read_line(answer, sizeof answer)) {
        return 0;
    }
    return answer[0] == 'y' || answer[0] == 'Y';
}

void exercise(int number)
{
    printf("\n  %s%s %d%s\n", C_BOLD, tr(">> EXERCISE - MODULE"), number, C_RESET);
    rule();
}

static void normalise(char *dest, int size, const char *src)
{
    int end = (int) strlen(src);
    int start = 0;

    while (src[start] == ' ' || src[start] == '\t') {
        start++;
    }
    while (end > start && (src[end - 1] == ' ' || src[end - 1] == '\t')) {
        end--;
    }

    int n = 0;
    for (int i = start; i < end && n < size - 1; i++) {
        char c = src[i];
        dest[n++] = (c >= 'A' && c <= 'Z') ? (char) (c + 32) : c;
    }
    dest[n] = '\0';
}

int question(const char *text, const char *correct, const char *why)
{
    char answer[64];
    char cleaned[64];
    char expected[64];

    printf("\n  %s\n", text);
    printf("  %s ", tr("Your answer:"));
    fflush(stdout);

    if (!read_line(answer, sizeof answer)) {
        answer[0] = '\0';
    }

    normalise(cleaned, sizeof cleaned, answer);
    normalise(expected, sizeof expected, correct);

    int right = strcmp(cleaned, expected) == 0;

    if (right) {
        printf("\n  %s%s%s%s  %s\n", C_BOLD, C_GREEN, tr("CORRECT."), C_RESET, why);
    } else {
        printf("\n  %s%s%s%s %s%s%s\n", C_BOLD, C_YELLOW, tr("NOT QUITE. The answer is:"),
               C_RESET, C_BOLD, correct, C_RESET);
        printf("             %s\n", why);
    }
    return right;
}

/* ---- example code ----------------------------------------------------- */

static const char *const KEYWORDS[] = {
    "static", "void", "int", "char", "const", "return", "struct", "if",
    "else", "for", "while", "NULL", "TRUE", "FALSE", NULL,
};

static int is_keyword(const char *word, size_t length)
{
    for (int i = 0; KEYWORDS[i] != NULL; i++) {
        if (strlen(KEYWORDS[i]) == length && strncmp(KEYWORDS[i], word, length) == 0) {
            return 1;
        }
    }
    return 0;
}

/* Colours one line of C. Deliberately small: // comments, "strings",
 * #preprocessor lines and a few keywords. It keeps no state between lines,
 * so a block comment spanning several lines would not be coloured; the
 * examples only use // comments. */
static void highlight(const char *line)
{
    const char *p = line;

    while (*p == ' ') {
        putchar(*p++);
    }
    if (*p == '#') {
        /* The directive is magenta; a trailing // comment is coloured as a
         * comment by the loop below. */
        const char *comment = strstr(p, "//");
        int length = comment != NULL ? (int) (comment - p) : (int) strlen(p);
        printf("%s%.*s%s", C_MAGENTA, length, p, C_RESET);
        p += length;
    }

    while (*p != '\0') {
        if (p[0] == '/' && p[1] == '/') {
            printf("%s%s", C_GREEN, C_DIM);
            const char *marker = strstr(p, "NEW:");
            if (marker != NULL) {
                printf("%.*s%s%s%sNEW:%s%s%s%s", (int) (marker - p), p, C_RESET,
                       C_BOLD, C_YELLOW, C_RESET, C_GREEN, C_DIM, marker + 4);
            } else {
                printf("%s", p);
            }
            printf("%s", C_RESET);
            return;
        }
        if (*p == '"') {
            const char *end = p + 1;
            while (*end != '\0' && *end != '"') {
                end += (*end == '\\' && end[1] != '\0') ? 2 : 1;
            }
            if (*end == '"') {
                end++;
            }
            printf("%s%.*s%s", C_YELLOW, (int) (end - p), p, C_RESET);
            p = end;
        } else if ((*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z') || *p == '_') {
            const char *end = p;
            while ((*end >= 'a' && *end <= 'z') || (*end >= 'A' && *end <= 'Z')
                   || (*end >= '0' && *end <= '9') || *end == '_') {
                end++;
            }
            if (is_keyword(p, (size_t) (end - p))) {
                printf("%s%.*s%s", C_BLUE, (int) (end - p), p, C_RESET);
            } else {
                printf("%.*s", (int) (end - p), p);
            }
            p = end;
        } else {
            putchar(*p++);
        }
    }
}

static void print_code(const char *const *lines, int n, const char *margin,
                       const char *margin_colour)
{
    for (int i = 0; i < n; i++) {
        printf("  %s%s%s", margin_colour, margin, C_RESET);
        highlight(lines[i]);
        printf("\n");
    }
}

void code(const char *const *lines, int n)
{
    print_code(lines, n, "  ", "");
}

void code_good(const char *const *lines, int n)
{
    print_code(lines, n, "+ ", C_GREEN);
}

void code_bad(const char *const *lines, int n)
{
    print_code(lines, n, "x ", C_RED);
}

static void note(const char *label, const char *colour, const char *text)
{
    printf("  %s%s%-5s%s %s\n", C_BOLD, colour, label, C_RESET, text);
}

void good(const char *text)
{
    note("GOOD", C_GREEN, text);
}

void bad(const char *text)
{
    note("BAD", C_RED, text);
}

void tip(const char *text)
{
    note("TIP", C_CYAN, text);
}

void more(const char *text)
{
    printf("        %s\n", text);
}

void ref_head(const char *signature)
{
    printf("\n  %s%s%s%s\n", C_BOLD, C_CYAN, signature, C_RESET);
}

void ref_line(const char *label, const char *text)
{
    if (*label == '\0') {
        printf("              %s\n", text);
    } else {
        printf("    %s%s%-9s%s %s\n", C_BOLD, C_YELLOW, label, C_RESET, text);
    }
}

void api_line(const char *signature, const char *text)
{
    printf("  %s%s%s\n", C_CYAN, signature, C_RESET);
    printf("      %s%s%s\n", C_DIM, text, C_RESET);
}
