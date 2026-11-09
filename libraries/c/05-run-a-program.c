/*
 * TITLE: fork(2), execvp(3), waitpid(2): running another program
 * GROUP: Common tasks
 * USES: #include <unistd.h>, <sys/wait.h>
 * SUMMARY: fork makes a copy of the process. In the copy, execvp replaces it
 *   with another program. The original waits for it with waitpid and reads
 *   what it printed through a pipe.
 * SIGNATURE:
 *   pid_t fork(void)
 *   int execvp(const char *file, char *const argv[])
 *   pid_t waitpid(pid_t pid, int *status, int options)
 *   int pipe(int ends[2])
 *   int dup2(int oldfd, int newfd)
 * PARAMETERS:
 *   - file, argv: the program, found on PATH, and its arguments. argv[0] is the
 *     program's name, and the list ends with NULL.
 *   - pid, status, options: which child, where its result goes, usually 0.
 *   - ends: ends[0] reads what ends[1] writes.
 * RETURNS: fork returns twice: 0 in the child, the child's id in the parent,
 *   or -1. execvp returns only if it failed. waitpid: the child's id, or -1.
 *   Use WIFEXITED(status) and WEXITSTATUS(status) to read the result.
 * ERRORS:
 *   - fork: EAGAIN when there are too many processes, ENOMEM.
 *   - execvp: ENOENT no such program, EACCES not executable.
 *   - After a failed exec the child must call _exit, not return.
 * NOTES:
 *   - No shell is involved, so spaces and quotes in arguments are safe.
 *     system() and popen() pass your text to a shell: never give them outside
 *     input.
 *   - fflush(stdout) before fork, or buffered text is printed twice.
 *   - Close the ends of the pipe you do not use. Reading never sees the end
 *     of the data while a write end is still open anywhere.
 *   - Always waitpid, or the finished child stays as a zombie.
 *   - By custom, 127 is the status for a command that was not found.
 * USEDFOR:
 *   - Using a command line tool whose answer you can get no other way.
 * SEE: man 2 fork, man 3 exec, man 2 waitpid, man 2 pipe, man 3 popen
 */

#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

/* Runs argv, stores what it printed in output, and returns its exit status,
 * or -1 if it could not be run or did not exit normally. */
static int run(char *const argv[], char *output, size_t size)
{
    int ends[2];

    if (pipe(ends) != 0) {
        return -1;
    }

    fflush(stdout);
    pid_t child = fork();
    if (child < 0) {
        close(ends[0]);
        close(ends[1]);
        return -1;
    }

    if (child == 0) {
        dup2(ends[1], STDOUT_FILENO);
        close(ends[0]);
        close(ends[1]);
        execvp(argv[0], argv);
        _exit(127);
    }

    close(ends[1]);
    size_t used = 0;
    ssize_t got;
    while (used + 1 < size && (got = read(ends[0], output + used, size - used - 1)) > 0) {
        used += (size_t) got;
    }
    output[used] = '\0';
    close(ends[0]);

    int status;
    if (waitpid(child, &status, 0) < 0 || !WIFEXITED(status)) {
        return -1;
    }
    return WEXITSTATUS(status);
}

int main(void)
{
    char *echo[] = { "echo", "printed by another program", NULL };
    char *fails[] = { "false", NULL };
    char *missing[] = { "no-such-program-here", NULL };
    char output[256];

    int status = run(echo, output, sizeof output);
    output[strcspn(output, "\n")] = '\0';
    printf("echo: status %d, output \"%s\"\n", status, output);

    printf("false: status %d\n", run(fails, output, sizeof output));
    printf("missing program: status %d\n", run(missing, output, sizeof output));
    return 0;
}
