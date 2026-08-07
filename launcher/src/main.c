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

/* One entry per language. `binary` is NULL for a language with no course
 * yet -- shown as "coming soon" instead of being launchable. Paths are
 * relative to this program's own working directory, which is why the
 * course itself has to be run the same way: `cd launcher && ./launcher`,
 * not from an arbitrary directory. Simplest thing that works for a repo
 * where every course is already run the same way.
 */
struct Language {
    const char *name;
    const char *binary;
};

static const struct Language LANGUAGES[] = {
    /* Python is a script, not a compiled binary: `binary` here is checked by
     * file_exists() the same as the others, then handed to system() as-is.
     * On Linux this relies on main.py's own shebang and its execute bit
     * (chmod +x); on Windows it relies on .py being associated with the
     * Python launcher, which the standard python.org installer sets up.
     * Neither has a fallback if that association is missing -- untested on
     * Windows, same as the rest of this file. */
#ifdef _WIN32
    { "C",          "..\\c\\c-course.exe" },
    { "C++",        "..\\cpp\\cpp-course.exe" },
    { "Python",     "..\\python\\src\\main.py" },
    { "JavaScript", "..\\javascript\\src\\main.js" },
#else
    { "C",          "../c/c-course" },
    { "C++",        "../cpp/cpp-course" },
    { "Python",     "../python/src/main.py" },
    { "JavaScript", "../javascript/src/main.js" },
#endif
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
    printf("  |            APRENDER A PROGRAMAR                      |\n");
    printf("  +======================================================+\n");
    printf("\n");

    for (int i = 0; i < LANGUAGE_COUNT; i++) {
        if (LANGUAGES[i].binary != NULL) {
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

        if (lang->binary == NULL) {
            printf("\n  %s does not have a course yet.\n", lang->name);
            wait_enter();
            continue;
        }

        if (!file_exists(lang->binary)) {
            printf("\n  %s's course has not been built yet.\n", lang->name);
            printf("  Run `make` inside its folder first.\n");
            wait_enter();
            continue;
        }

        /* Blocks until the course process exits. Its own menu already has
         * [0] Quit; choosing it ends the process, system() returns, and the
         * loop redraws this menu -- the "go back" option, for free. */
        system(lang->binary);
    }

    printf("\n  See you next time.\n\n");
    return 0;
}
