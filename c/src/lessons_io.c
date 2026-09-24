/*
 * Module 18 - real input/output.
 *
 * Everything before this module read from the keyboard and wrote to this
 * program's own screen -- two channels this course never had to check for
 * failure, because neither one is ever simply not there. A real file can be:
 * the directory it lives in might not exist, the disk might be full, the
 * permissions might say no. FILE *, fopen, and checking fopen's own return
 * value for NULL are this module's answer to that, the same discipline
 * module 8 taught for pointers in general, aimed at the one kind of pointer
 * whose failure is completely ordinary instead of a bug.
 *
 * PART 4 hits a problem specific to a non-interactive teaching module: "keep
 * asking until the input is valid" cannot be demonstrated live here, because
 * this module has no real keyboard to wait on and cannot risk hanging. It
 * runs the exact same validation logic against a short, fixed sequence of
 * strings instead of live stdin, and says so out loud rather than pretending
 * otherwise -- rule 2 is "nothing is claimed without being shown," not
 * "nothing is shown honestly labelled as a stand-in."
 *
 * PART 5 hits a related problem: this course's own binary is always started
 * from a menu with no command-line arguments of its own to show. It follows
 * module 16 and 17's own precedent instead of inventing a new one -- write a
 * second, real .c file to /tmp, compile it with cc, and run the resulting
 * binary with real arguments through system(), checking its exit status
 * honestly instead of assuming success.
 *
 * Every scratch file this module writes lives under /tmp with a
 * codelearner_m18_ prefix and is removed again before the module returns,
 * the same cleanup discipline modules 16 and 17 use.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#include "lessons.h"
#include "ui.h"

#define M18_CFLAGS "-std=c11 -Wall -Wextra"

/* PART 1: a real file, and a real fopen failure. */
#define P_NOTES            "/tmp/codelearner_m18_notes.txt"
#define P_MISSING_DIR_FILE "/tmp/codelearner_m18_missing_dir/notes.txt"

/* PART 2 and PART 3 both read this same file back. */
#define P_SCORES "/tmp/codelearner_m18_scores.txt"

/* PART 5: a second real program, compiled and run for its argv. */
#define P_ARGV_C   "/tmp/codelearner_m18_argv_demo.c"
#define P_ARGV_BIN "/tmp/codelearner_m18_argv_demo"

/* P_MISSING_DIR_FILE is not created on purpose -- fopen() failing on it is
 * the whole point of PART 1 -- so it is not in this list. If that path
 * happens to already exist (e.g. a leftover directory), the fopen success
 * branch below removes it itself instead of leaving it behind. */
static const char *const M18_SCRATCH_FILES[] = {
    P_NOTES, P_SCORES, P_ARGV_C, P_ARGV_BIN,
};

/* Write `content` to `path` with plain fopen/fputs/fclose. Identical to the
 * helper module 16 and 17 each keep in their own file -- three lines, not
 * worth sharing across files at the cost of a fourth header. */
static void write_file(const char *path, const char *content)
{
    FILE *f = fopen(path, "w");
    if (f == NULL) {
        printf("  (could not write %s -- skipping)\n", path);
        return;
    }
    fputs(content, f);
    fclose(f);
}

/* Print `code` (one string, lines separated by '\n') the way every other
 * module prints a code sample: four spaces in front of each line. Reused so
 * a file's real bytes and what the student sees on screen can never drift
 * apart -- the same string literal is both. */
static void print_code(const char *code)
{
    const char *line = code;
    while (*line != '\0') {
        const char *end = strchr(line, '\n');
        int len = end ? (int) (end - line) : (int) strlen(line);
        printf("    %.*s\n", len, line);
        if (end == NULL) {
            break;
        }
        line = end + 1;
    }
}

/* Run a shell command for real, printing it first. Returns 1 for a clean
 * exit, 0 for anything else, including a missing tool. Same shape as module
 * 16 and 17's own run_shell -- cc is a hard dependency of this course's own
 * build either way. */
static int run_shell(const char *cmd)
{
    printf("  $ %s\n\n", cmd);
    fflush(stdout);

    int status = system(cmd);
    if (status == -1 || (WIFEXITED(status) && WEXITSTATUS(status) == 127)) {
        printf("  (command not found -- this command is expected to be on\n");
        printf("   PATH for this part of the lesson)\n");
        return 0;
    }
    return WIFEXITED(status) && WEXITSTATUS(status) == 0;
}

void lesson_18_file_io(void)
{
    title("MODULE 18 - REAL INPUT/OUTPUT");

    heading("PART 1: FILE *, fopen, fclose -- and checking for NULL");

    printf("  Every file this course has touched until now was one this\n");
    printf("  program wrote and read itself, in modules 16 and 17. FILE *\n");
    printf("  is the handle fopen hands back for one: an opaque pointer the\n");
    printf("  standard library uses to track position, buffering, and mode.\n");
    printf("  You never look inside it -- you only ever pass it to the\n");
    printf("  functions below.\n\n");

    printf("    FILE *f = fopen(path, \"w\");\n");
    printf("    if (f == NULL) {\n");
    printf("        /* could not open it -- do not use f */\n");
    printf("    }\n\n");

    FILE *f = fopen(P_NOTES, "w");
    if (f == NULL) {
        printf("  (unexpected: could not open %s)\n", P_NOTES);
    } else {
        fputs("first line\n", f);
        fclose(f);
        printf("  Running: fopen(\"%s\", \"w\") returned a real FILE *,\n",
               P_NOTES);
        printf("  wrote one line, and fclose closed it.\n");
    }

    printf("\n  fopen returns NULL exactly when it could not open the file:\n");
    printf("  a directory that does not exist, no permission, a full disk.\n");
    printf("  Nothing about that is exceptional -- checking for NULL right\n");
    printf("  after every fopen is not optional, the way checking malloc's\n");
    printf("  return was not optional in module 9. Proving the failure\n");
    printf("  instead of asserting it, by opening a path inside a directory\n");
    printf("  that genuinely is not there:\n\n");

    printf("    fopen(\"%s\", \"w\")\n\n", P_MISSING_DIR_FILE);

    errno = 0;
    FILE *bad = fopen(P_MISSING_DIR_FILE, "w");
    if (bad == NULL) {
        printf("  Running: fopen returned NULL. strerror(errno) says: %s\n",
               strerror(errno));
    } else {
        printf("  (unexpected: that path opened -- closing it)\n");
        fclose(bad);
        remove(P_MISSING_DIR_FILE);
    }

    printf("\n  Writing through that NULL anyway -- fputs(line, bad) -- would\n");
    printf("  be undefined behaviour, the exact same trap module 8 called a\n");
    printf("  NULL pointer dereference, wearing a FILE * instead of an\n");
    printf("  int *. The check above is what stands between fopen failing\n");
    printf("  and the program crashing three lines later for a reason that\n");
    printf("  looks unrelated.\n");

    wait_enter();
    clear_screen();
    heading("PART 2: fprintf and fgets -- writing structured data, and reading it back");

    printf("  fprintf works exactly like printf -- same format string, same\n");
    printf("  %%d/%%s markers -- except the first argument says where the\n");
    printf("  text goes, a FILE * instead of always the screen:\n\n");

    printf("    fprintf(f, \"%%s scored %%d\\n\", name, score);\n\n");

    FILE *out = fopen(P_SCORES, "w");
    if (out != NULL) {
        fprintf(out, "%s scored %d\n", "Ana", 90);
        fprintf(out, "%s scored %d\n", "Leo", 75);
        fclose(out);
        printf("  Running: wrote two lines to %s\n", P_SCORES);
    } else {
        printf("  (unexpected: could not write %s -- check disk space or\n"
               "   permissions)\n", P_SCORES);
    }

    printf("\n  Reading them back uses fgets -- the same function module 3\n");
    printf("  used on stdin, handed a different FILE * this time:\n\n");

    printf("    while (fgets(line, sizeof line, f) != NULL) {\n");
    printf("        printf(\"%%s\", line);\n");
    printf("    }\n\n");

    FILE *in = fopen(P_SCORES, "r");
    if (in != NULL) {
        char line[64];
        printf("  Running:\n\n");
        while (fgets(line, sizeof line, in) != NULL) {
            printf("    %s", line);
        }
        fclose(in);
    }

    printf("\n  fgets returns NULL at end of file -- the same NULL it\n");
    printf("  returns at end of input from the keyboard. That NULL is what\n");
    printf("  ends the while loop above; nothing here ever counts lines.\n");

    wait_enter();
    clear_screen();
    heading("PART 3: \"r\" vs \"rb\" -- a real distinction, invisible here");

    printf("  fopen's mode string can carry a 'b': \"rb\" instead of \"r\",\n");
    printf("  \"wb\" instead of \"w\". On Windows this matters: text mode\n");
    printf("  there silently translates \\r\\n on disk to \\n in memory on\n");
    printf("  read, and back on write. Binary mode skips that translation\n");
    printf("  and hands back the exact bytes on disk.\n\n");

    printf("  POSIX, which Linux follows, makes no such distinction --\n");
    printf("  \"r\" and \"rb\" behave identically here. Proving that instead\n");
    printf("  of asserting it: opening the same file %s\n", P_SCORES);
    printf("  both ways and comparing every byte read back:\n\n");

    char text_buf[64] = {0};
    char bin_buf[64] = {0};

    FILE *ft = fopen(P_SCORES, "r");
    size_t tn = ft != NULL ? fread(text_buf, 1, sizeof text_buf, ft) : 0;
    if (ft != NULL) {
        fclose(ft);
    }

    FILE *fb = fopen(P_SCORES, "rb");
    size_t bn = fb != NULL ? fread(bin_buf, 1, sizeof bin_buf, fb) : 0;
    if (fb != NULL) {
        fclose(fb);
    }

    printf("    fopen(path, \"r\")  read %zu bytes\n", tn);
    printf("    fopen(path, \"rb\") read %zu bytes\n\n", bn);

    if (tn == 0) {
        printf("  Running: both reads returned 0 bytes -- nothing was\n");
        printf("  actually compared, so this is not evidence either way.\n\n");
    } else {
        int same_bytes = (tn == bn) && (memcmp(text_buf, bin_buf, tn) == 0);
        printf("  Running: memcmp says the two buffers are %s.\n\n",
               same_bytes ? "identical" : "different");
    }

    printf("  That result does not make the 'b' pointless -- only a no-op\n");
    printf("  on THIS operating system. Code meant to run on Windows still\n");
    printf("  needs the right mode string; the flag is real, portable C\n");
    printf("  that one platform happens to ignore.\n");

    wait_enter();
    clear_screen();
    heading("PART 4: checking scanf's return value, and a real re-prompt");

    printf("  Module 3 covered the & scanf needs and %%s stopping at the\n");
    printf("  first space. It did not cover this: scanf returns a number --\n");
    printf("  how many items it actually matched and assigned. Ignoring\n");
    printf("  that return value is how a program ends up computing on\n");
    printf("  whatever garbage was already sitting in a variable, instead\n");
    printf("  of noticing the read failed at all:\n\n");

    printf("    int age;\n");
    printf("    int matched = scanf(\"%%d\", &age);\n");
    printf("    if (matched != 1) {\n");
    printf("        /* age was never written -- do not trust it */\n");
    printf("    }\n\n");

    printf("  matched is 1 when a whole number was read, 0 when the next\n");
    printf("  input cannot be read as one at all (letters, punctuation),\n");
    printf("  and EOF if input ran out entirely. In either failing case,\n");
    printf("  age is left holding whatever it held before the call --\n");
    printf("  scanf never touches its target unless the conversion\n");
    printf("  actually succeeded.\n\n");

    printf("  A real re-prompt loop tries again instead of giving up on the\n");
    printf("  first bad line:\n\n");

    printf("    int age, matched;\n");
    printf("    do {\n");
    printf("        printf(\"Age: \");\n");
    printf("        matched = scanf(\"%%d\", &age);\n");
    printf("        while (getchar() != '\\n') { }   /* drain the bad line */\n");
    printf("    } while (matched != 1);\n\n");

    printf("  That drain line matters as much as the retry itself: a failed\n");
    printf("  scanf(\"%%d\") leaves the bad text sitting in the input buffer\n");
    printf("  UNREAD. Skip draining it and the next scanf call reads that\n");
    printf("  same leftover text again -- the loop never advances and\n");
    printf("  never ends.\n\n");

    printf("  This module cannot run that loop live -- it has no real\n");
    printf("  keyboard here to wait on, and a teaching module that might\n");
    printf("  hang is a broken one. What follows runs the identical check\n");
    printf("  against a short, fixed sequence of strings standing in for\n");
    printf("  someone typing at a prompt, through sscanf instead of scanf --\n");
    printf("  same return-value convention, a string as the source instead\n");
    printf("  of stdin:\n\n");

    {
        const char *typed[] = { "abc", "-9", "banana", "23" };
        int accepted_age = -1;

        for (size_t i = 0; i < sizeof typed / sizeof typed[0]; i++) {
            printf("  Age: %s\n", typed[i]);

            int matched = sscanf(typed[i], "%d", &accepted_age);
            if (matched != 1) {
                printf("    -> not a number, try again\n");
                continue;
            }
            if (accepted_age <= 0) {
                printf("    -> %d is not a valid age, try again\n", accepted_age);
                continue;
            }
            printf("    -> accepted\n");
            break;
        }

        printf("\n  Running: final age = %d\n", accepted_age);
        printf("  Three rejections before one line finally passed both\n");
        printf("  checks -- the format check (did sscanf match a number at\n");
        printf("  all) and a second, ordinary if after it (is that number\n");
        printf("  sensible). scanf only ever answers the first question.\n");
    }

    wait_enter();
    clear_screen();
    heading("PART 5: command-line arguments -- argc, argv, and main's two forms");

    printf("  A C program can take information from outside itself: words\n");
    printf("  typed after the program's own name on the command line.\n\n");

    printf("    int main(int argc, char *argv[])\n\n");

    printf("  argc counts how many words there are, counting the program's\n");
    printf("  own name as the first one. argv is an array of that many\n");
    printf("  strings: argv[0] is the program's own name, argv[1] the first\n");
    printf("  real argument, and so on. argv[argc] is always a NULL\n");
    printf("  pointer, guaranteed by the language -- the same way a string\n");
    printf("  always ends in \\0, a list of arguments always ends in a NULL\n");
    printf("  entry, without argc needing to be consulted to find it.\n\n");

    printf("  That is why both forms of main are legal C:\n\n");
    printf("    int main(void)                    ignores argv entirely\n");
    printf("    int main(int argc, char *argv[])  reads it\n\n");

    printf("  Every module in this course used int main(void), because none\n");
    printf("  of them needed anything from outside the program -- this\n");
    printf("  course's own binary is one of them, started from a menu with\n");
    printf("  no arguments of its own to show here. What follows is a\n");
    printf("  second, real program instead, compiled and run the same way\n");
    printf("  module 16 and 17 each compiled and ran their own scratch\n");
    printf("  files, so argv[1] below is real content from a real run, not\n");
    printf("  a transcript typed to look like one:\n\n");

    {
        const char argv_src[] =
            "#include <stdio.h>\n"
            "\n"
            "int main(int argc, char *argv[])\n"
            "{\n"
            "    printf(\"argc = %d\\n\", argc);\n"
            "    for (int i = 0; i < argc; i++) {\n"
            "        printf(\"argv[%d] = %s\\n\", i, argv[i]);\n"
            "    }\n"
            "    return 0;\n"
            "}\n";

        print_code(argv_src);
        write_file(P_ARGV_C, argv_src);

        char cmd[256];
        snprintf(cmd, sizeof cmd, "cc %s -o %s %s",
                 M18_CFLAGS, P_ARGV_BIN, P_ARGV_C);
        int compiled = run_shell(cmd);

        if (compiled) {
            printf("\n  Running it with two real words after its own name:\n\n");
            snprintf(cmd, sizeof cmd, "%s hello world", P_ARGV_BIN);
            int ran = run_shell(cmd);

            if (ran) {
                printf("\n  argv[0] is the PROGRAM's own path, filled in by the\n");
                printf("  operating system, not something a caller chose to\n");
                printf("  send. argv[1] and argv[2] are the two words actually\n");
                printf("  typed after it, in order, each already a plain,\n");
                printf("  NUL-terminated string -- no parsing needed to split\n");
                printf("  them apart from each other or from argv[0].\n");
            } else {
                printf("\n  (the run failed above -- no argv output to read.)\n");
            }
        } else {
            printf("\n  (compile failed above -- skipping the run.)\n");
        }
    }

    wait_enter();
    clear_screen();
    exercise(18);

    question("fopen(path, \"w\") is checked with if (f == NULL) right after\n"
             "  the call. What does a NULL result mean there? (answer:\n"
             "  success or failure)",
             "failure",
             "fopen returns NULL exactly when it could not open the file --\n"
             "             a missing directory, no permission, a full disk. The\n"
             "             pointer itself is the check; there is no separate flag.");

    question("scanf(\"%d\", &age) is called and the user types \"twenty\".\n"
             "  What does scanf return? (a number)",
             "0",
             "0 items were converted and assigned -- \"twenty\" is not a\n"
             "             valid int, so scanf stops before ever writing to age.");

    question("On this Linux machine, does fopen(path, \"r\") ever read\n"
             "  different bytes than fopen(path, \"rb\") on the same file?\n"
             "  (answer: yes or no)",
             "no",
             "POSIX makes no text/binary distinction -- both modes behave\n"
             "             identically here. The 'b' still matters on Windows,\n"
             "             where text mode translates \\r\\n to \\n.");

    question("./prog first second is run. What is argc?",
             "3",
             "argc counts argv[0] too -- the program's own name -- so two\n"
             "             real arguments make argc 3, not 2.");

    {
        const char *task[] = {
            "Read a name from stdin. Write it to a real file with",
            "fprintf. Open that same file again, read the name back",
            "with fgets, and print it.",
        };
        const char *input[] = {"Alice"};
        const char *expected[] = {"Alice"};
        const char *solution[] = {
            "#include <stdio.h>",
            "",
            "int main(void)",
            "{",
            "    char name[50];",
            "",
            "    if (fgets(name, sizeof name, stdin) == NULL) {",
            "        return 1;",
            "    }",
            "",
            "    FILE *f = fopen(\"name.txt\", \"w\");",
            "    if (f == NULL) {",
            "        return 1;",
            "    }",
            "    fprintf(f, \"%s\", name);",
            "    fclose(f);",
            "",
            "    f = fopen(\"name.txt\", \"r\");",
            "    if (f == NULL) {",
            "        return 1;",
            "    }",
            "    if (fgets(name, sizeof name, f) == NULL) {",
            "        return 1;",
            "    }",
            "    fclose(f);",
            "",
            "    printf(\"%s\", name);",
            "    return 0;",
            "}",
            "",
            "fgets keeps the newline typed at the terminal, so writing name",
            "with %s and reading it back need no extra \\n anywhere here.",
        };
        challenge(task, 3, input, 1, expected, 1, solution,
                  (int) (sizeof solution / sizeof solution[0]));
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    printf("   - fopen returns NULL on failure; check it before using the\n");
    printf("     FILE * for anything, the same discipline module 9 taught\n");
    printf("     for malloc\n");
    printf("   - fprintf writes to a file the way printf writes to the\n");
    printf("     screen; fgets reads a file back the way it reads the\n");
    printf("     keyboard -- both return NULL/stop at end of input\n");
    printf("   - \"r\" and \"rb\" open identically on Linux, verified above --\n");
    printf("     the difference is real and only shows up on Windows's\n");
    printf("     \\r\\n translation\n");
    printf("   - checking scanf's return value catches bad input scanf\n");
    printf("     itself already saw; a real re-prompt loop also has to\n");
    printf("     drain the leftover bad text, or it repeats forever\n");
    printf("   - int main(int argc, char *argv[]) is why two main\n");
    printf("     signatures are legal C; argc counts argv[0], the\n");
    printf("     program's own name, and argv[argc] is always NULL\n");
    wait_enter();

    /* Single cleanup block, reached whether every step above succeeded or
     * some were skipped -- nothing above this point returns early. */
    for (size_t i = 0; i < sizeof M18_SCRATCH_FILES / sizeof M18_SCRATCH_FILES[0]; i++) {
        remove(M18_SCRATCH_FILES[i]);
    }
}
