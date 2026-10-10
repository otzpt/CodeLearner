/*
 * TITLE: Shell and terminal emulator (walking up the process tree)
 * GROUP: System information
 * USES: read /proc/<pid>/stat and /proc/<pid>/comm; #include <unistd.h>
 * SUMMARY: The shell and the terminal emulator are your program's ancestors.
 *   Every process records its parent's pid; /proc/<pid>/comm names a process.
 *   Walk upward from getppid() and read the names.
 * NOTES:
 *   - $SHELL is your LOGIN shell, set at login. The shell you are using is the
 *     parent process, which may be different (you typed `zsh` inside bash).
 *   - Typical chain: your program -> bash -> gnome-terminal-server (or kitty,
 *     alacritty, konsole, foot) -> systemd. The first non-shell ancestor is the
 *     terminal emulator.
 *   - Field 4 of /proc/<pid>/stat is the parent pid, but field 2 is the
 *     command name in parentheses and may itself contain spaces and ")".
 *     Parse from the LAST ")" in the line, as below.
 *   - comm holds at most 15 characters, so long names are cut.
 *   - $TERM names the terminal TYPE (xterm-256color), not the program.
 *     $TERM_PROGRAM, when set, names the program.
 *   - This page is run by the reference browser, so the chain it prints starts
 *     with the browser's own helpers (timeout, sh), not with a bare shell.
 * SEE: man 5 proc
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int parent_of(int pid)
{
    char path[64];
    char line[1024];
    FILE *file;
    int parent = -1;

    snprintf(path, sizeof path, "/proc/%d/stat", pid);
    file = fopen(path, "r");
    if (file == NULL) {
        return -1;
    }
    if (fgets(line, sizeof line, file) != NULL) {
        const char *after_name = strrchr(line, ')');
        char state;
        if (after_name != NULL && sscanf(after_name + 1, " %c %d", &state, &parent) != 2) {
            parent = -1;
        }
    }
    fclose(file);
    return parent;
}

static int name_of(int pid, char *out, size_t size)
{
    char path[64];
    FILE *file;

    snprintf(path, sizeof path, "/proc/%d/comm", pid);
    file = fopen(path, "r");
    if (file == NULL || fgets(out, (int) size, file) == NULL) {
        if (file != NULL) {
            fclose(file);
        }
        return 0;
    }
    fclose(file);
    out[strcspn(out, "\n")] = '\0';
    return 1;
}

int main(void)
{
    const char *term = getenv("TERM");
    const char *program = getenv("TERM_PROGRAM");
    char name[64];

    printf("$TERM          %s\n", term != NULL ? term : "(not set)");
    printf("$TERM_PROGRAM  %s\n", program != NULL ? program : "(not set)");
    printf("ancestors, nearest first:\n");

    for (int pid = getppid(), depth = 0; pid > 1 && depth < 8; depth++) {
        if (!name_of(pid, name, sizeof name)) {
            break;
        }
        printf("  %-6d %s\n", pid, name);
        pid = parent_of(pid);
    }
    return 0;
}
