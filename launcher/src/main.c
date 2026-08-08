/*
 * Universal launcher.
 *
 * Picks a language, runs its course as a separate program, and waits.
 * Nothing more. Each course already has its own [0] Quit that ends the
 * process; when that process ends, control returns here on its own -- "go
 * back" is just what a subprocess returning looks like. The launcher does
 * not need to know anything about what happens inside a course.
 *
 * Build:  make
 * Run:    ./launcher     (from inside this directory, so the relative
 *                          paths to ../c, ../cpp, ... resolve)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* One entry per language. `run_command` is NULL for a language with no
 * course yet -- shown as "coming soon" instead of being launchable. `path`
 * is what file_exists() checks before offering the option; `run_command` is
 * what system() actually runs, which is not always the same string.
 *
 * They diverge on Windows for the interpreted languages. Verified (not
 * assumed): the standard python.org installer sets .py as the default
 * handler for a bare path, so Python could stay a plain path -- but
 * Node.js's installer does not do the equivalent for .js. Windows' own
 * default handler for .js is Windows Script Host, an entirely different,
 * incompatible engine (WSH's JScript, not V8), and Node only registers
 * itself as an "Open With" candidate under
 * HKEY_CLASSES_ROOT\Applications\node.exe, never as the default. A bare
 * path to main.js would silently run under the wrong engine and fail. Both
 * are made explicit (`python ...`, `node ...`) rather than leaving one
 * correct by installer behavior and the other wrong by it -- relying on
 * PATH is one guarantee to reason about instead of two different
 * file-association stories.
 *
 * All paths are relative to this program's own working directory, which is
 * why the course itself has to be run the same way: `cd launcher &&
 * ./launcher`, not from an arbitrary directory.
 */
struct Language {
    const char *name;
    const char *path;
    const char *run_command;
};

#ifdef _WIN32
#define C_BIN    "..\\c\\c-course.exe"
#define CPP_BIN  "..\\cpp\\cpp-course.exe"
#define PY_PATH  "..\\python\\src\\main.py"
#define JS_PATH  "..\\javascript\\src\\main.js"
#define JAVA_BIN "..\\java\\run.bat"
#else
#define C_BIN    "../c/c-course"
#define CPP_BIN  "../cpp/cpp-course"
#define PY_PATH  "../python/src/main.py"
#define JS_PATH  "../javascript/src/main.js"
#define JAVA_BIN "../java/run"
#endif

static const struct Language LANGUAGES[] = {
    { "C",          C_BIN,    C_BIN },
    { "C++",        CPP_BIN,  CPP_BIN },
#ifdef _WIN32
    /* On Linux these two are unchanged: a shebang line plus the execute bit
     * (chmod +x) already picks the right interpreter, the same guarantee
     * PATH gives here, so there is nothing to make explicit. */
    { "Python",     PY_PATH,  "python " PY_PATH },
    { "JavaScript", JS_PATH,  "node " JS_PATH },
#else
    { "Python",     PY_PATH,  PY_PATH },
    { "JavaScript", JS_PATH,  JS_PATH },
#endif
    { "Java",       JAVA_BIN, JAVA_BIN },
};

#define LANGUAGE_COUNT (int)(sizeof LANGUAGES / sizeof LANGUAGES[0])

static void clear_screen(void)
{
#ifdef _WIN32
    system("cls");
#else
    /* Same fix as every course: ANSI codes, not system("clear"), so this
     * does not depend on TERM being set. See c/src/ui.c for the full
     * explanation -- this is the same three lines, kept here rather than
     * shared, because the courses and the launcher are meant to stay
     * independent programs. */
    fputs("\033[H\033[2J\033[3J", stdout);
    fflush(stdout);
#endif
}

static void wait_enter(void)
{
    printf("\n  Press ENTER to continue...");
    fflush(stdout);
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* discard */
    }
}

static int read_line(char *dest, int size)
{
    if (fgets(dest, size, stdin) == NULL) {
        dest[0] = '\0';
        return 0;
    }
    dest[strcspn(dest, "\n")] = '\0';
    return 1;
}

/* True if the file at `path` can be opened for reading. Used instead of a
 * POSIX-only check (access(), stat()) so the same source compiles on
 * Windows too -- fopen is standard C either way. */
static int file_exists(const char *path)
{
    FILE *f = fopen(path, "rb");
    if (f == NULL) {
        return 0;
    }
    fclose(f);
    return 1;
}

static void show_menu(void)
{
    clear_screen();
    printf("\n");
    printf("  +======================================================+\n");
    printf("  |                  CODELEARNER                         |\n");
    printf("  +======================================================+\n");
    printf("\n");

    for (int i = 0; i < LANGUAGE_COUNT; i++) {
        if (LANGUAGES[i].run_command != NULL) {
            printf("   [%d]  %s\n", i + 1, LANGUAGES[i].name);
        } else {
            printf("   [%d]  %s (coming soon)\n", i + 1, LANGUAGES[i].name);
        }
    }
    printf("\n   [0]  Exit\n");
    printf("  ------------------------------------------------------\n");
}

int main(void)
{
    char choice[16];

    for (;;) {
        show_menu();
        printf("\n  Pick a language: ");
        fflush(stdout);

        if (!read_line(choice, sizeof choice)) {
            break;
        }
        if (strcmp(choice, "0") == 0) {
            break;
        }

        int n = atoi(choice);
        if (n < 1 || n > LANGUAGE_COUNT) {
            printf("\n  Not a valid option.\n");
            wait_enter();
            continue;
        }

        const struct Language *lang = &LANGUAGES[n - 1];

        if (lang->run_command == NULL) {
            printf("\n  %s does not have a course yet.\n", lang->name);
            wait_enter();
            continue;
        }

        if (!file_exists(lang->path)) {
            printf("\n  %s's course has not been built yet.\n", lang->name);
            printf("  Run `make` inside its folder first.\n");
            wait_enter();
            continue;
        }

        /* Blocks until the course process exits. Its own menu already has
         * [0] Quit; choosing it ends the process, system() returns, and the
         * loop redraws this menu -- the "go back" option, for free. */
        system(lang->run_command);
    }

    printf("\n  See you next time.\n\n");
    return 0;
}
