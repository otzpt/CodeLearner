/*
 * Universal launcher.
 *
 * Picks a language, runs its course as a separate program, and waits.
 * Nothing more. Each course already has its own [0] Quit that ends the
 * process; when that process ends, control returns here on its own -- "go
 * back" is just what a subprocess returning looks like. The launcher does
 * not need to know anything about what happens inside a course.
 *
 * The one other thing it does is [u] Check for updates: when asked, one
 * request to GitHub for the latest release, compared with the version this
 * binary was built as. It never downloads or installs anything and never
 * runs on its own.
 *
 * Build:  make
 * Run:    ./launcher     (from inside this directory, so the relative
 *                          paths to ../c, ../cpp, ... resolve)
 */

/* popen() is POSIX, and -std=c11 hides it unless asked. */
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _WIN32
#include <sys/wait.h>
#endif

#include "update.h"

/* The version this binary was built as. The release workflow passes the tag
 * (make VERSION=1.5.1); a plain `make` is a development build. */
#ifndef CODELEARNER_VERSION
#define CODELEARNER_VERSION "dev"
#endif

#define RELEASES_API  "https://api.github.com/repos/otzpt/CodeLearner/releases/latest"
#define RELEASES_PAGE "https://github.com/otzpt/CodeLearner/releases/latest"

#ifdef _WIN32
#define POPEN  _popen
#define PCLOSE _pclose
#else
#define POPEN  popen
#define PCLOSE pclose
#endif

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
    /* The tab of the menu this entry is listed under ("LANGUAGES",
     * "LIBRARIES"). Left out (NULL), an entry is a language. A heading is
     * printed whenever the tab changes, so a new tab is one field on its row. */
    const char *section;
};

#ifdef _WIN32
#define C_BIN      "..\\c\\c-course.exe"
#define CPP_BIN    "..\\cpp\\cpp-course.exe"
#define PY_PATH    "..\\python\\src\\main.py"
#define JS_PATH    "..\\javascript\\src\\main.js"
#define JAVA_BIN   "..\\java\\run.bat"
#define GUI_BIN    "..\\gui\\gui-course.exe"
#define CSHARP_BIN "..\\csharp\\run.bat"
#define RUST_BIN   "..\\rust\\rust-course.exe"
#else
#define C_BIN      "../c/c-course"
#define CPP_BIN    "../cpp/cpp-course"
#define PY_PATH    "../python/src/main.py"
#define JS_PATH    "../javascript/src/main.js"
#define JAVA_BIN   "../java/run"
#define GUI_BIN    "../gui/gui-course"
#define ASM_BIN    "../assembly/asm-course"
#define CSHARP_BIN "../csharp/run"
#define GIT_PATH   "../git/src/main.sh"
#define ARDUINO_BIN "../arduino/arduino-course"
#define MICROPY_PATH "../micropython/src/main.py"
#define RUST_BIN   "../rust/rust-course"
#define LIBRARIES_BIN "../libraries/libraries"
#endif

static const struct Language LANGUAGES[] = {
    { "C",          C_BIN,    C_BIN, NULL },
    { "C++",        CPP_BIN,  CPP_BIN, NULL },
#ifdef _WIN32
    /* On Linux these two are unchanged: a shebang line plus the execute bit
     * (chmod +x) already picks the right interpreter, the same guarantee
     * PATH gives here, so there is nothing to make explicit. */
    { "Python",     PY_PATH,  "python " PY_PATH, NULL },
    { "JavaScript", JS_PATH,  "node " JS_PATH, NULL },
#else
    { "Python",     PY_PATH,  PY_PATH, NULL },
    { "JavaScript", JS_PATH,  JS_PATH, NULL },
#endif
    { "Java",       JAVA_BIN, JAVA_BIN, NULL },
    /* Same wrapper story as Java: `dotnet run` has to run from inside the
     * project directory, so this cannot point straight at a .cs file the
     * way Python's/JavaScript's entries do. Cross-platform via `dotnet`
     * itself, so unlike Python/JavaScript there's no Windows-vs-Linux
     * split to make explicit here -- the run/run.bat wrapper covers it. */
    { "C#",         CSHARP_BIN, CSHARP_BIN, NULL },
    /* Not a language -- GTK, a C library -- but treated as a peer entry
     * here on purpose, the same way it gets its own top-level course
     * directory instead of living inside c/'s module list. */
    { "GUI (GTK)",  GUI_BIN,  GUI_BIN, NULL },
#ifdef _WIN32
    /* Genuinely not "coming soon later" the way an unbuilt language is --
     * this course makes raw Linux syscalls directly (syscall + Linux
     * syscall numbers), which have no Windows equivalent to translate to.
     * A Windows build would need its own real implementation against a
     * completely different ABI, not a path change. */
    { "Assembly",   NULL, NULL, NULL },
#else
    { "Assembly",   ASM_BIN, ASM_BIN, NULL },
#endif
#ifdef _WIN32
    /* Same reasoning as Python/JavaScript's own installer story, the
     * other way around: python.org's and Node's installers are what
     * make a bare .py/.js path reliable on Windows, verified in the
     * comment above this struct. Bash has no equivalent guaranteed
     * install on a stock Windows machine -- Git for Windows and WSL both
     * ship one, but neither is assumed here the way a JDK is for Java. */
    { "Git",        NULL, NULL, NULL },
#else
    { "Git",        GIT_PATH, GIT_PATH, NULL },
#endif
#ifdef _WIN32
    /* arduino/shim/Arduino.h reads Serial input with poll() and read() from
     * <poll.h>/<unistd.h>, which Windows does not have. Not a path change:
     * the stand-in for the Arduino core would need a Windows port. */
    { "Arduino",    NULL, NULL, NULL },
#else
    { "Arduino",    ARDUINO_BIN, ARDUINO_BIN, NULL },
#endif
    { "Rust",       RUST_BIN, RUST_BIN, NULL },
#ifdef _WIN32
    /* The course runs on the unix port of MicroPython, which is not shipped
     * for Windows (and has no shebang story there either). */
    { "MicroPython", NULL, NULL, NULL },
#else
    /* A script with a `#!/usr/bin/env micropython` shebang, like Python's
     * and JavaScript's entries: no build step, needs `micropython` on PATH. */
    { "MicroPython", MICROPY_PATH, MICROPY_PATH, NULL },
#endif
#ifdef _WIN32
    /* Not a course: a reference to the libraries behind a task (and, for C and
     * Python, everything a system-information tool such as fastfetch needs).
     * Its pages read /proc and /sys, which Windows does not have. */
    { "Libraries",  NULL, NULL, "LIBRARIES" },
#else
    { "Libraries",  LIBRARIES_BIN, LIBRARIES_BIN, "LIBRARIES" },
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

/* Runs `command` through the shell and keeps the first size - 1 bytes of what
 * it prints. The rest is read and dropped so the child never writes into a
 * closed pipe. Returns pclose()'s status, or -1 if the shell did not start. */
static int run_capture(const char *command, char *out, size_t size)
{
    FILE *pipe = POPEN(command, "r");
    char chunk[512];
    size_t used = 0;
    size_t got;

    out[0] = '\0';
    if (pipe == NULL) {
        return -1;
    }
    while ((got = fread(chunk, 1, sizeof chunk, pipe)) > 0) {
        size_t room = size - 1 - used;
        size_t take = got < room ? got : room;

        memcpy(out + used, chunk, take);
        used += take;
    }
    out[used] = '\0';
    return PCLOSE(pipe);
}

static int exit_code(int status)
{
#ifdef _WIN32
    return status;
#else
    return WIFEXITED(status) ? WEXITSTATUS(status) : -1;
#endif
}

/* The first line of `text`, indented, cut to 100 characters. */
static void print_first_line(const char *text)
{
    int length = (int)strcspn(text, "\r\n");

    if (length > 100) {
        length = 100;
    }
    if (length > 0) {
        printf("  %.*s\n", length, text);
    }
}

static void check_for_updates(void)
{
    static const char *const COMMANDS[] = {
        "curl -fsS --max-time 10 -A \"CodeLearner/" CODELEARNER_VERSION "\" " RELEASES_API " 2>&1",
#ifndef _WIN32
        /* Some machines ship wget and not curl, which is why install.sh
         * tries both. Only tried when the shell says curl is not there. */
        "wget -qO- -T 10 -U \"CodeLearner/" CODELEARNER_VERSION "\" " RELEASES_API " 2>&1",
#endif
    };
    /* SIMPLIFICATION: only the first 4 KB of GitHub's answer is kept. "tag_name"
     * is about 1.5 KB in today; if GitHub ever moved it past 4 KB the check
     * would report "no release tag" and this buffer is what to enlarge. */
    char response[4096];
    char tag[64];
    int latest[3];
    int current[3];
    int status = -1;

    printf("\n  Asking GitHub for the latest release...\n\n");
    fflush(stdout);
    for (size_t i = 0; i < sizeof COMMANDS / sizeof COMMANDS[0]; i++) {
        status = exit_code(run_capture(COMMANDS[i], response, sizeof response));
        if (status != 127) {   /* 127: the shell did not find the tool */
            break;
        }
    }

    if (status == 127) {
        printf("  Could not check for updates: neither curl nor wget was found.\n");
    } else if (status != 0) {
        printf("  Could not check for updates (the request failed, status %d):\n", status);
        print_first_line(response);
    } else if (!release_tag_from_json(response, tag, sizeof tag)) {
        printf("  Could not check for updates: GitHub's answer had no release tag.\n");
    } else if (!version_parse(tag, latest)) {
        printf("  Could not check for updates: the latest release is tagged \"%s\".\n", tag);
    } else if (!version_parse(CODELEARNER_VERSION, current)) {
        printf("  The latest release is %s.\n", tag);
        printf("  This is a development build (version \"%s\"), so there is nothing to compare.\n",
               CODELEARNER_VERSION);
        return;
    } else if (version_newer(latest, current)) {
        printf("  A newer release is available: %s (you have v%s).\n\n", tag, CODELEARNER_VERSION);
        printf("  Download it: %s\n", RELEASES_PAGE);
        printf("  CodeLearner does not update itself. Reinstall the way you installed it\n");
        printf("  (install.sh, the package, or the AppImage).\n");
        return;
    } else if (version_newer(current, latest)) {
        printf("  You are running v%s, which is newer than the latest release (%s).\n",
               CODELEARNER_VERSION, tag);
        return;
    } else {
        printf("  You are up to date: v%s is the latest release.\n", CODELEARNER_VERSION);
        return;
    }
    printf("\n  You can check by hand: %s\n", RELEASES_PAGE);
}

static void show_menu(void)
{
    clear_screen();
    printf("\n");
    printf("  +======================================================+\n");
    printf("  |                  CODELEARNER                         |\n");
    printf("  +======================================================+\n");
    printf("  version %s\n", CODELEARNER_VERSION);
    printf("\n");

    const char *shown_section = "";
    for (int i = 0; i < LANGUAGE_COUNT; i++) {
        const char *section = LANGUAGES[i].section != NULL ? LANGUAGES[i].section : "LANGUAGES";

        if (strcmp(section, shown_section) != 0) {
            printf("%s   -- %s --\n", shown_section[0] != '\0' ? "\n" : "", section);
            shown_section = section;
        }
        if (LANGUAGES[i].run_command != NULL) {
            printf("   [%d]  %s\n", i + 1, LANGUAGES[i].name);
        } else {
            printf("   [%d]  %s (coming soon)\n", i + 1, LANGUAGES[i].name);
        }
    }
    printf("\n   [u]  Check for updates\n");
    printf("   [0]  Exit\n");
    printf("  ------------------------------------------------------\n");
}

int main(void)
{
    char choice[16];

    for (;;) {
        show_menu();
        printf("\n  Pick an entry: ");
        fflush(stdout);

        if (!read_line(choice, sizeof choice)) {
            break;
        }
        if (strcmp(choice, "0") == 0) {
            break;
        }
        if (strcmp(choice, "u") == 0 || strcmp(choice, "U") == 0) {
            check_for_updates();
            wait_enter();
            continue;
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
