/*
 * Module 16 - compilation and project structure.
 *
 * A new theme, not a new language feature: every module before this one
 * has been a single .c file the course's own Makefile already compiled
 * for you. This one is about what actually happens between typing `make`
 * and getting ./c-course -- .c versus .h, header guards, separate
 * compilation, linking, warnings, -g, Makefiles, and static libraries.
 * It comes after module 11 (the final test) conceptually: everything it
 * needs -- functions, and nothing past them -- is already taught, and
 * "more than one file" is the thing this course has quietly relied on
 * the whole time (its own Makefile links six .c files) without ever
 * explaining out loud.
 *
 * The centerpiece, PART 4, is a real linker error: an "undefined
 * reference" this program produces by actually invoking `cc`, not a
 * string typed to look like one. Rule 3 (never demonstrate undefined
 * behaviour) does not forbid this -- a compiler refusing to compile, or
 * a linker refusing to link, is the tool working exactly as documented,
 * the same way a bounds-checked array access refusing an out-of-range
 * index would be. What rule 3 forbids is running a broken program
 * afterwards, and linking failure means there is never anything to run.
 *
 * Every scratch file this module writes lives under /tmp with a
 * codelearner_m16_ prefix, and every one of them is removed again before
 * the module returns -- see the cleanup block at the end of
 * lesson_16_compilation.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#include "lessons.h"
#include "ui.h"

#define M16_CFLAGS "-std=c11 -Wall -Wextra"

/* PART 2: a header with no guard, and the fixed version. */
#define P_SHAPE_H        "/tmp/codelearner_m16_shape.h"
#define P_GUARD_BAD_C    "/tmp/codelearner_m16_guard_bad.c"
#define P_SHAPE_FIXED_H  "/tmp/codelearner_m16_shape_fixed.h"
#define P_GUARD_OK_C     "/tmp/codelearner_m16_guard_ok.c"

/* PART 3: two files, compiled separately, then linked into one program. */
#define P_UTIL_H         "/tmp/codelearner_m16_util.h"
#define P_UTIL_C         "/tmp/codelearner_m16_util.c"
#define P_UTIL_O         "/tmp/codelearner_m16_util.o"
#define P_MAIN3_C        "/tmp/codelearner_m16_main3.c"
#define P_MAIN3_O        "/tmp/codelearner_m16_main3.o"
#define P_COMBINED       "/tmp/codelearner_m16_combined"

/* PART 4: a header promising a function nobody ever defines. */
#define P_CALC_H         "/tmp/codelearner_m16_calc.h"
#define P_CALLER_C       "/tmp/codelearner_m16_caller.c"
#define P_CALLER_O       "/tmp/codelearner_m16_caller.o"
#define P_LINKED         "/tmp/codelearner_m16_linked"

/* PART 5: a real -Wall warning. */
#define P_WARN_C         "/tmp/codelearner_m16_warn.c"

/* PART 6: util.c (PART 3) compiled once with -g and once without. */
#define P_UTIL_G_O       "/tmp/codelearner_m16_util_g.o"
#define P_UTIL_NOG_O     "/tmp/codelearner_m16_util_nog.o"

/* PART 8: util.o (PART 3) archived into a static library. The archive
 * itself has to be named libNAME.a for -lNAME to find it -- that naming
 * rule belongs to ar and the linker, not to this module's own scratch
 * prefix, so it is the one path here without codelearner_m16_ in it. */
#define P_LIBRARY        "/tmp/libcodelearner_m16.a"
#define P_STATIC_MAIN_C  "/tmp/codelearner_m16_static_main.c"
#define P_STATIC_MAIN_O  "/tmp/codelearner_m16_static_main.o"
#define P_STATIC_PROG    "/tmp/codelearner_m16_static_prog"

/* Every scratch path this module can create, for the cleanup block at the
 * end of lesson_16_compilation. remove() is plain stdio, already taught,
 * and does exactly what unlink() does to a regular file -- no reason to
 * bring in a second header just to delete one. */
static const char *const M16_SCRATCH_FILES[] = {
    P_SHAPE_H, P_GUARD_BAD_C, P_SHAPE_FIXED_H, P_GUARD_OK_C,
    P_UTIL_H, P_UTIL_C, P_UTIL_O, P_MAIN3_C, P_MAIN3_O, P_COMBINED,
    P_CALC_H, P_CALLER_C, P_CALLER_O, P_LINKED,
    P_WARN_C,
    P_UTIL_G_O, P_UTIL_NOG_O,
    P_LIBRARY, P_STATIC_MAIN_C, P_STATIC_MAIN_O, P_STATIC_PROG,
};

/* Write `content` to `path` with plain fopen/fputs/fclose. If it fails,
 * say so and move on -- whatever tries to compile `path` next fails with
 * its own real, honest "No such file" instead of this program faking the
 * file into existing. */
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
 * module prints a code sample: four spaces in front of each line. Reused
 * so a file's real bytes and what the student sees on screen can never
 * drift apart -- the same string literal is both. */
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

/* Run a shell command for real, printing it first and then whatever it
 * writes -- stdout and stderr already land on this program's own screen,
 * since a child started by system() inherits both from its parent.
 * Returns 1 for a clean exit, 0 for anything else, including a missing
 * tool. cc/ar are a hard dependency of this course's own build (see
 * c/Makefile); gdb (module 17) is not -- it is only ever invoked from
 * here, so the not-found message stays true for both instead of pointing
 * every caller at the Makefile. */
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

void lesson_16_compilation(void)
{
    title("MODULE 16 - COMPILATION AND PROJECT STRUCTURE");

    heading("PART 1: .c and .h -- a promise, and its fulfillment");

    printf("  Every module so far has been one .c file. A real program is\n");
    printf("  usually many files, of two kinds: a .c file holds\n");
    printf("  DEFINITIONS -- the actual code that runs -- and a .h file\n");
    printf("  holds DECLARATIONS -- a promise that something exists,\n");
    printf("  without saying where or how.\n\n");

    {
        const char add_h[] =
            "#ifndef ADD_H\n"
            "#define ADD_H\n"
            "\n"
            "int add(int a, int b);\n"
            "\n"
            "#endif\n";
        const char add_c[] =
            "#include \"add.h\"\n"
            "\n"
            "int add(int a, int b)\n"
            "{\n"
            "    return a + b;\n"
            "}\n";

        printf("  add.h:\n");
        print_code(add_h);
        printf("\n  add.c:\n");
        print_code(add_c);
    }

    printf("\n  add.h promises: somewhere, a function called add exists,\n");
    printf("  taking two ints and returning an int. add.c keeps that\n");
    printf("  promise. Any other .c file that #includes add.h can call\n");
    printf("  add() -- the compiler only needs the promise to check the\n");
    printf("  call looks right; the real code arrives later, at the\n");
    printf("  linking step PART 3 covers.\n\n");

    printf("  #include \"add.h\" uses quotes, not #include <stdio.h>'s\n");
    printf("  angle brackets. Quotes look in the including file's own\n");
    printf("  folder first; angle brackets go straight to the system's\n");
    printf("  header folders. Your own headers use quotes; the standard\n");
    printf("  library's use angle brackets.\n");

    wait_enter();
    clear_screen();
    heading("PART 2: header guards -- what breaks without one");

    printf("  This program writes its own scratch files under /tmp with a\n");
    printf("  codelearner_m16_ prefix, so they can never collide with\n");
    printf("  anything real on your machine, and deletes them again before\n");
    printf("  this module ends. In your own code the names below would\n");
    printf("  just be shape.h and main.c.\n\n");

    printf("  A header with no guard, defining a struct:\n\n");

    {
        const char shape_h_bad[] =
            "struct Point {\n"
            "    int x;\n"
            "    int y;\n"
            "};\n";
        const char guard_bad_c[] =
            "#include \"codelearner_m16_shape.h\"\n"
            "#include \"codelearner_m16_shape.h\"\n"
            "\n"
            "int main(void)\n"
            "{\n"
            "    struct Point p;\n"
            "    p.x = 1;\n"
            "    p.y = 2;\n"
            "    return p.x + p.y;\n"
            "}\n";

        print_code(shape_h_bad);
        printf("\n  #include-d twice into the same .c file -- easy to do\n");
        printf("  by accident once two of your own headers both include a\n");
        printf("  third one:\n\n");
        print_code(guard_bad_c);

        write_file(P_SHAPE_H, shape_h_bad);
        write_file(P_GUARD_BAD_C, guard_bad_c);

        printf("\n  Compiling this for real:\n\n");
        char cmd[256];
        snprintf(cmd, sizeof cmd, "cc %s -fsyntax-only %s",
                 M16_CFLAGS, P_GUARD_BAD_C);
        run_shell(cmd);

        printf("\n  That is a real error from a real compiler: `struct\n");
        printf("  Point` got DEFINED twice in one translation unit, once\n");
        printf("  per #include. A function prototype can legally appear\n");
        printf("  more than once -- it is only a declaration. A struct\n");
        printf("  body, a typedef, or a variable with an initializer\n");
        printf("  cannot; the second copy is a redefinition.\n\n");

        printf("  The fix wraps the header so the second #include expands\n");
        printf("  to nothing:\n\n");
    }

    {
        const char shape_h_fixed[] =
            "#ifndef CODELEARNER_M16_SHAPE_H\n"
            "#define CODELEARNER_M16_SHAPE_H\n"
            "\n"
            "struct Point {\n"
            "    int x;\n"
            "    int y;\n"
            "};\n"
            "\n"
            "#endif\n";
        const char guard_ok_c[] =
            "#include \"codelearner_m16_shape_fixed.h\"\n"
            "#include \"codelearner_m16_shape_fixed.h\"\n"
            "\n"
            "int main(void)\n"
            "{\n"
            "    struct Point p;\n"
            "    p.x = 1;\n"
            "    p.y = 2;\n"
            "    return p.x + p.y;\n"
            "}\n";

        print_code(shape_h_fixed);

        write_file(P_SHAPE_FIXED_H, shape_h_fixed);
        write_file(P_GUARD_OK_C, guard_ok_c);

        printf("\n  #ifndef asks \"is CODELEARNER_M16_SHAPE_H not defined\n");
        printf("  yet?\". The first #include says yes, defines it, and\n");
        printf("  keeps the body. The second #include sees it already\n");
        printf("  defined and skips straight to #endif -- the struct is\n");
        printf("  declared once no matter how many times the header gets\n");
        printf("  pulled in. Compiling the same double-#include again,\n");
        printf("  guarded this time:\n\n");

        char cmd[256];
        snprintf(cmd, sizeof cmd, "cc %s -fsyntax-only %s",
                 M16_CFLAGS, P_GUARD_OK_C);
        int ok = run_shell(cmd);
        if (ok) {
            printf("  (no output at all -- for a compiler, silence is\n");
            printf("  success. Exit status 0, no warnings, no errors.)\n");
        }
    }

    wait_enter();
    clear_screen();
    heading("PART 3: separate compilation -- .o files, then linking");

    printf("  `cc -c` compiles ONE file into an object file: real machine\n");
    printf("  code, but not a program yet -- calls to anything outside\n");
    printf("  that one file are left as unresolved names for later.\n\n");

    {
        const char util_h[] =
            "#ifndef CODELEARNER_M16_UTIL_H\n"
            "#define CODELEARNER_M16_UTIL_H\n"
            "\n"
            "int square(int n);\n"
            "\n"
            "#endif\n";
        const char util_c[] =
            "#include \"codelearner_m16_util.h\"\n"
            "\n"
            "int square(int n)\n"
            "{\n"
            "    return n * n;\n"
            "}\n";
        const char main3_c[] =
            "#include <stdio.h>\n"
            "#include \"codelearner_m16_util.h\"\n"
            "\n"
            "int main(void)\n"
            "{\n"
            "    printf(\"6 squared is %d\\n\", square(6));\n"
            "    return 0;\n"
            "}\n";

        printf("  util.h / util.c:\n");
        print_code(util_h);
        print_code(util_c);
        printf("\n  main3.c:\n");
        print_code(main3_c);

        write_file(P_UTIL_H, util_h);
        write_file(P_UTIL_C, util_c);
        write_file(P_MAIN3_C, main3_c);

        printf("\n  Compiling each one on its own -- neither step needs to\n");
        printf("  see the other file's body, only util.h's promise:\n\n");

        char cmd[256];
        snprintf(cmd, sizeof cmd, "cc %s -c %s -o %s",
                 M16_CFLAGS, P_UTIL_C, P_UTIL_O);
        run_shell(cmd);
        snprintf(cmd, sizeof cmd, "cc %s -c %s -o %s",
                 M16_CFLAGS, P_MAIN3_C, P_MAIN3_O);
        run_shell(cmd);

        printf("\n  Two real .o files now exist on disk:\n\n");
        snprintf(cmd, sizeof cmd, "ls -la %s %s", P_UTIL_O, P_MAIN3_O);
        run_shell(cmd);

        printf("\n  Linking combines them into one executable -- this is\n");
        printf("  the step that finally matches main3.c's call to\n");
        printf("  square() with util.o's real definition of it:\n\n");
        snprintf(cmd, sizeof cmd, "cc %s %s -o %s",
                 P_UTIL_O, P_MAIN3_O, P_COMBINED);
        run_shell(cmd);

        printf("\n  And it runs, built from two pieces compiled apart:\n\n");
        run_shell(P_COMBINED);

        printf("\n  Changing only main3.c, in a real project, would only\n");
        printf("  need main3.c recompiled and everything relinked --\n");
        printf("  util.c's already-compiled .o does not need touching.\n");
        printf("  PART 7 covers a Makefile doing exactly that\n");
        printf("  automatically.\n");
    }

    wait_enter();
    clear_screen();
    heading("PART 4: a real linker error -- \"undefined reference\"");

    printf("  A header can promise a function that nobody ever keeps the\n");
    printf("  promise for. This compiles fine -- and fails only when\n");
    printf("  something tries to link it into a program:\n\n");

    {
        const char calc_h[] =
            "#ifndef CODELEARNER_M16_CALC_H\n"
            "#define CODELEARNER_M16_CALC_H\n"
            "\n"
            "int double_it(int n);\n"
            "\n"
            "#endif\n";
        const char caller_c[] =
            "#include <stdio.h>\n"
            "#include \"codelearner_m16_calc.h\"\n"
            "\n"
            "int main(void)\n"
            "{\n"
            "    printf(\"%d\\n\", double_it(21));\n"
            "    return 0;\n"
            "}\n";

        printf("  calc.h declares double_it. No calc.c anywhere defines\n");
        printf("  it -- on purpose:\n\n");
        print_code(calc_h);
        printf("\n  caller.c:\n");
        print_code(caller_c);

        write_file(P_CALC_H, calc_h);
        write_file(P_CALLER_C, caller_c);

        printf("\n  Compiling caller.c ALONE, first:\n\n");
        char cmd[256];
        snprintf(cmd, sizeof cmd, "cc %s -c %s -o %s",
                 M16_CFLAGS, P_CALLER_C, P_CALLER_O);
        int compiled = run_shell(cmd);

        if (compiled) {
            printf("\n  That succeeded. The compiler only ever saw calc.h's\n");
            printf("  promise and had no reason to doubt it -- checking\n");
            printf("  that double_it is ever actually DEFINED is not the\n");
            printf("  compiler's job at all.\n\n");
        }

        printf("  Now linking caller.o into a program:\n\n");
        snprintf(cmd, sizeof cmd, "cc %s -o %s", P_CALLER_O, P_LINKED);
        int linked = run_shell(cmd);

        if (!linked) {
            printf("\n  \"undefined reference to `double_it'\" -- the real\n");
            printf("  message a real linker (ld, invoked here by cc) just\n");
            printf("  printed. This is a LINKER error, not a compiler\n");
            printf("  error: it names a symbol, not a line number, because\n");
            printf("  linking no longer has any source text to point at --\n");
            printf("  only object code and a list of names nobody supplied.\n");
            printf("  Linking failed, so there is no %s to run. That is the\n", P_LINKED);
            printf("  whole point of this module's rule 3 note: there is\n");
            printf("  nothing broken left to accidentally execute.\n");
        }
    }

    wait_enter();
    clear_screen();
    heading("PART 5: compiler warnings -- -Wall -Wextra, on purpose");

    printf("  This course has compiled with -Wall -Wextra since module 1.\n");
    printf("  Here is the exact kind of bug they exist to catch:\n\n");

    {
        const char warn_c[] =
            "#include <stdio.h>\n"
            "\n"
            "int main(void)\n"
            "{\n"
            "    int x = 0;\n"
            "\n"
            "    if (x = 5) {\n"
            "        printf(\"was set\\n\");\n"
            "    }\n"
            "\n"
            "    return 0;\n"
            "}\n";

        print_code(warn_c);
        write_file(P_WARN_C, warn_c);

        printf("\n  `if (x = 5)` is a single `=` -- assignment, not `==`\n");
        printf("  comparison. It sets x to 5 and then tests 5, which is\n");
        printf("  never zero, so the branch always runs. The compiler\n");
        printf("  still accepts it: it is legal C. Compiling with the\n");
        printf("  warnings this course always uses:\n\n");

        char cmd[256];
        snprintf(cmd, sizeof cmd, "cc %s -fsyntax-only %s",
                 M16_CFLAGS, P_WARN_C);
        run_shell(cmd);

        printf("\n  That warning is the real reason this course turns\n");
        printf("  -Wall -Wextra on for every single example: it does not\n");
        printf("  stop the program from compiling -- a warning is advice,\n");
        printf("  not an error, and the compiler still produces a working\n");
        printf("  executable either way. Reading it anyway is what catches\n");
        printf("  this before it ships.\n");
    }

    wait_enter();
    clear_screen();
    heading("PART 6: debug builds -- -g");

    printf("  -g asks the compiler to also write DEBUG SYMBOLS into the\n");
    printf("  object file: variable names, function names, and a table\n");
    printf("  mapping machine instructions back to source lines. None of\n");
    printf("  it changes what the program computes -- it is information\n");
    printf("  FOR a debugger (module 17 covers using one), not for the\n");
    printf("  program itself.\n\n");

    printf("  Compiling util.c (from PART 3) once with -g and once without\n");
    printf("  it, everything else identical:\n\n");

    {
        char cmd[256];
        snprintf(cmd, sizeof cmd, "cc %s -g -c %s -o %s",
                 M16_CFLAGS, P_UTIL_C, P_UTIL_G_O);
        run_shell(cmd);
        snprintf(cmd, sizeof cmd, "cc %s -c %s -o %s",
                 M16_CFLAGS, P_UTIL_C, P_UTIL_NOG_O);
        run_shell(cmd);

        printf("\n");
        snprintf(cmd, sizeof cmd, "ls -la %s %s", P_UTIL_G_O, P_UTIL_NOG_O);
        run_shell(cmd);
    }

    printf("\n  The -g version is larger -- that extra size is the debug\n");
    printf("  information, not more program. This course's own Makefile\n");
    printf("  (PART 7) already builds with -g always: it costs disk space,\n");
    printf("  nothing at runtime, and a release build strips it later with\n");
    printf("  `strip`, once a debugger is no longer needed.\n");

    wait_enter();
    clear_screen();
    heading("PART 7: Makefiles -- this course's own");

    printf("  Typing the right cc command by hand for six source files,\n");
    printf("  every time one of them changes, does not scale. `make`\n");
    printf("  reads a Makefile describing what depends on what, and only\n");
    printf("  rebuilds what actually needs it. This is not a toy example\n");
    printf("  -- it is c/Makefile, copy-pasted, unedited:\n\n");

    {
        const char makefile_text[] =
            "# C course\n"
            "#\n"
            "#   make          builds ./c-course\n"
            "#   make run      builds and runs\n"
            "#   make clean    removes build output\n"
            "#\n"
            "# -Wall -Wextra stay on always. In a C course a compiler warning is\n"
            "# teaching material: nearly all of them catch a real bug before it happens.\n"
            "\n"
            "CC      = cc\n"
            "CFLAGS  = -std=c11 -Wall -Wextra -g\n"
            "TARGET  = c-course\n"
            "SOURCES = src/main.c src/ui.c src/lessons_basics.c src/lessons_memory.c src/lessons_advanced.c src/lessons_tooling.c\n"
            "OBJECTS = $(SOURCES:.c=.o)\n"
            "\n"
            "$(TARGET): $(OBJECTS)\n"
            "\t$(CC) $(CFLAGS) -o $@ $(OBJECTS)\n"
            "\n"
            "# Rebuild a .o when its .c or any header changes. Without the header\n"
            "# dependency, editing ui.h would rebuild nothing and leave a stale binary.\n"
            "%.o: %.c src/ui.h src/lessons.h\n"
            "\t$(CC) $(CFLAGS) -c $< -o $@\n"
            "\n"
            "run: $(TARGET)\n"
            "\t./$(TARGET)\n"
            "\n"
            "clean:\n"
            "\trm -f $(OBJECTS) $(TARGET)\n"
            "\n"
            ".PHONY: run clean\n";

        print_code(makefile_text);
    }

    printf("\n  $@ and $< are automatic variables, only meaningful inside a\n");
    printf("  recipe: $@ is the TARGET being built (left of the colon,\n");
    printf("  e.g. src/main.o); $< is the first PREREQUISITE (right of the\n");
    printf("  colon, its matching src/main.c). `%%.o: %%.c ...` is a\n");
    printf("  pattern rule -- one recipe, reused for every .o this course\n");
    printf("  needs, because $@ and $< are different each time it fires.\n\n");

    printf("  \"only rebuilds what changed\" is a real file-timestamp\n");
    printf("  check, not a figure of speech: make compares each target's\n");
    printf("  modification time against every prerequisite's. If the\n");
    printf("  target is newer than all of them, its recipe is skipped\n");
    printf("  entirely. That is also why ui.h and lessons.h are listed as\n");
    printf("  prerequisites of every .o above -- without that line,\n");
    printf("  editing ui.h would leave every .o looking up to date, and\n");
    printf("  `make` would link a stale binary and say nothing.\n");

    wait_enter();
    clear_screen();
    heading("PART 8: static libraries, briefly");

    printf("  A static library is a bundle of already-compiled .o files in\n");
    printf("  one archive, so a program can link against many functions\n");
    printf("  with one flag instead of naming every .o by hand. Archiving\n");
    printf("  PART 3's util.o:\n\n");

    {
        char cmd[256];
        snprintf(cmd, sizeof cmd, "ar rcs %s %s", P_LIBRARY, P_UTIL_O);
        run_shell(cmd);

        printf("\n  r = insert/replace this member, c = create the archive\n");
        printf("  without warning if it is new, s = write a symbol index,\n");
        printf("  so the linker can find square() inside it without\n");
        printf("  scanning every member. The result is a real archive:\n\n");

        snprintf(cmd, sizeof cmd, "ls -la %s", P_LIBRARY);
        run_shell(cmd);

        const char static_main_c[] =
            "#include <stdio.h>\n"
            "#include \"codelearner_m16_util.h\"\n"
            "\n"
            "int main(void)\n"
            "{\n"
            "    printf(\"7 squared is %d\\n\", square(7));\n"
            "    return 0;\n"
            "}\n";

        printf("\n  static_main.c, calling square() but never seeing\n");
        printf("  util.c, only the library:\n\n");
        print_code(static_main_c);

        write_file(P_STATIC_MAIN_C, static_main_c);

        printf("\n  Compiling it, then linking against the library instead\n");
        printf("  of naming util.o directly: -L says where to look,\n");
        printf("  -lcodelearner_m16 says link against\n");
        printf("  libcodelearner_m16.a (drop \"lib\" and \".a\"):\n\n");

        snprintf(cmd, sizeof cmd, "cc %s -c %s -o %s",
                 M16_CFLAGS, P_STATIC_MAIN_C, P_STATIC_MAIN_O);
        run_shell(cmd);
        snprintf(cmd, sizeof cmd, "cc %s -L/tmp -lcodelearner_m16 -o %s",
                 P_STATIC_MAIN_O, P_STATIC_PROG);
        run_shell(cmd);

        printf("\n  And it runs -- the linker copied square()'s object code\n");
        printf("  straight out of the archive and into this executable:\n\n");
        run_shell(P_STATIC_PROG);

        printf("\n  \"Static\" is the key word: the code is copied in at\n");
        printf("  link time, so the finished program never needs\n");
        printf("  libcodelearner_m16.a to exist again to run. A shared\n");
        printf("  library (.so) is the opposite tradeoff -- smaller\n");
        printf("  executables that all reuse one copy on disk, but only if\n");
        printf("  that .so is still there every time -- beyond this\n");
        printf("  module's scope.\n");
    }

    wait_enter();
    clear_screen();
    exercise(16);

    question("A .c file calls a function that is DECLARED in a header it\n"
             "  #includes, but never DEFINED in any .c file. Does building\n"
             "  it fail while compiling, or while linking? (answer:\n"
             "  compiling or linking)",
             "linking",
             "The compiler only checks that the header's promise makes the\n"
             "             call well-formed. Only the linker needs the real\n"
             "             function body to exist, which is why the error names\n"
             "             a symbol, not a line of source.");

    question("A header defines `struct Point { ... };` with no header\n"
             "  guard, and gets #included twice into the same .c file.\n"
             "  What kind of error does this cause? (one word)",
             "redefinition",
             "A function prototype can legally appear more than once; a\n"
             "             struct BODY cannot. The second #include's copy\n"
             "             collides with the first.");

    question("Does -Wall -Wextra ever stop code with real mistakes in it\n"
             "  from compiling into a working executable? (yes or no)",
             "no",
             "A warning is advice, not an error -- the compiler still\n"
             "             produces a binary either way. That is exactly why\n"
             "             this course reads every one instead of ignoring them.");

    question("You compile the exact same source once with -g and once\n"
             "  without it. Does what the resulting program PRINTS differ\n"
             "  between the two? (yes or no)",
             "no",
             "-g only adds debug symbols alongside the compiled code, for\n"
             "             a debugger to read later. It changes the file on\n"
             "             disk, never what the program computes.");

    heading("TASK: A THREE-FILE PROGRAM");

    printf("  Read two integers. Print their sum. Split the work across\n");
    printf("  three real files: math.h declares add(int, int), guarded\n");
    printf("  against double inclusion; math.c defines it; main.c reads\n");
    printf("  the two numbers, calls add(), and prints the result.\n\n");

    printf("  Unlike every challenge() so far, this task genuinely cannot\n");
    printf("  be one file -- that is the entire point of it. Module 11's\n");
    printf("  final test hit the same wall for a different reason (no\n");
    printf("  single output to match) and used this same by-hand format.\n\n");

    printf("  Example run:\n\n");
    rule();
    printf("  First number: 3\n");
    printf("  Second number: 4\n");
    printf("  Sum: 7\n");
    rule();

    printf("\n  Try it first. Compile and link all three .c files in one\n");
    printf("  command:\n\n");
    printf("    gcc -Wall -Wextra math.c main.c -o test && ./test\n\n");
    printf("  gcc finds add()'s prototype in math.h (via main.c's\n");
    printf("  #include), compiles math.c and main.c separately under the\n");
    printf("  hood, then links both into one executable -- PART 3's whole\n");
    printf("  pipeline, run with a single command instead of three.\n");

    if (ask_yes("Want to see example code?")) {
        printf("\n  math.h:\n");
        rule();
        printf("  #ifndef MATH_H\n");
        printf("  #define MATH_H\n");
        printf("  \n");
        printf("  int add(int a, int b);\n");
        printf("  \n");
        printf("  #endif\n");
        rule();

        printf("\n  math.c:\n");
        rule();
        printf("  #include \"math.h\"\n");
        printf("  \n");
        printf("  int add(int a, int b)\n");
        printf("  {\n");
        printf("      return a + b;\n");
        printf("  }\n");
        rule();

        printf("\n  main.c:\n");
        rule();
        printf("  #include <stdio.h>\n");
        printf("  #include \"math.h\"\n");
        printf("  \n");
        printf("  int main(void)\n");
        printf("  {\n");
        printf("      int a, b;\n");
        printf("  \n");
        printf("      printf(\"First number: \");\n");
        printf("      if (scanf(\"%%d\", &a) != 1) {\n");
        printf("          return 1;\n");
        printf("      }\n");
        printf("      printf(\"Second number: \");\n");
        printf("      if (scanf(\"%%d\", &b) != 1) {\n");
        printf("          return 1;\n");
        printf("      }\n");
        printf("  \n");
        printf("      printf(\"Sum: %%d\\n\", add(a, b));\n");
        printf("      return 0;\n");
        printf("  }\n");
        rule();
        printf("  This is EXAMPLE CODE. main.c never sees add()'s body,\n");
        printf("  only math.h's promise -- exactly PART 1's add.h/add.c,\n");
        printf("  used for real this time instead of just shown.\n");
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    printf("   - a .h is a promise (declarations); a .c is the fulfillment\n");
    printf("     (definitions) -- the same .h can be #included by many .c\n");
    printf("     files\n");
    printf("   - a header guard (#ifndef/#define/#endif) stops a header's\n");
    printf("     own contents from being defined twice when it ends up\n");
    printf("     #included more than once\n");
    printf("   - cc -c compiles one file into an unlinked .o; the linker\n");
    printf("     combines .o files into one executable, and is where\n");
    printf("     \"undefined reference\" comes from, not the compiler\n");
    printf("   - -Wall -Wextra catch real bugs without ever refusing to\n");
    printf("     compile them -- a warning is advice, not an error\n");
    printf("   - -g adds debug symbols for a debugger (module 17); it\n");
    printf("     never changes what the program computes\n");
    printf("   - a Makefile automates the same compile-then-link pipeline\n");
    printf("     PART 3 ran by hand; a static library bundles already-\n");
    printf("     compiled code so a linker flag can pull it in by name\n\n");

    printf("  Module 17 picks up right where PART 4 and PART 5 left off:\n");
    printf("  reading these same kinds of diagnostics under a real\n");
    printf("  debugger and a sanitizer, instead of only in the terminal.\n");
    wait_enter();

    /* Single cleanup block, reached whether every step above succeeded or
     * some were skipped -- nothing above this point returns early, so
     * this always runs before the module hands control back to main(). */
    for (size_t i = 0; i < sizeof M16_SCRATCH_FILES / sizeof M16_SCRATCH_FILES[0]; i++) {
        remove(M16_SCRATCH_FILES[i]);
    }
}

/*
 * Module 17 - debugging.
 *
 * Module 16 was about tools that read source text and either produce a
 * program or refuse to. This one is about tools that watch a program
 * actually run: gdb and a sanitizer both know things no reading of the
 * source ever could -- the exact path taken, and the exact values held,
 * at the moment something went wrong.
 *
 * Rule 3 (never demonstrate undefined behaviour) still applies, and PART 3
 * and PART 4 use it two different ways on purpose. PART 3's NULL-pointer
 * write is undefined behaviour that happens to be reliable on Linux --
 * address 0 is never mapped, so the write faults every single time,
 * loudly, the same way module 16's own broken link always failed loudly.
 * PART 4's use-after-free is undefined behaviour that is NOT reliable:
 * freed heap memory is still ordinary, mapped memory, so the same write
 * might crash, might corrupt something unrelated, or might silently
 * "work" if nothing has reused that address yet -- module 9's own "worst
 * case". Running it raw and hoping it crashes would teach exactly the
 * wrong lesson, so this module never does: PART 4's use-after-free is
 * compiled with AddressSanitizer from its very first build, every time,
 * and is never executed any other way.
 *
 * Every scratch file lives under /tmp with a codelearner_m17_ prefix and
 * is removed again at the end of lesson_17_debugging, the same discipline
 * module 16 uses -- write_file(), print_code() and run_shell() above are
 * reused as-is, not reimplemented.
 */

#define M17_CFLAGS      "-std=c11 -Wall -Wextra"
#define M17_ASAN_CFLAGS "-std=c11 -Wall -Wextra -fsanitize=address"

/* Every -ex list below starts by turning off gdb's one-time "download
 * debug info from the internet?" prompt -- housekeeping for this course's
 * screen, unrelated to debugging any of these programs. */
#define M17_GDB_QUIET "set debuginfod enabled off"

/* PART 2: a breakpoint, stepping, and reading variables. */
#define P_GDB_BASIC_C   "/tmp/codelearner_m17_gdb_basic.c"
#define P_GDB_BASIC_BIN "/tmp/codelearner_m17_gdb_basic"

/* PART 3: a real NULL-pointer crash and its backtrace. */
#define P_CRASH_C   "/tmp/codelearner_m17_crash.c"
#define P_CRASH_BIN "/tmp/codelearner_m17_crash"

/* PART 4: module 9's use-after-free, compiled with ASan from the start. */
#define P_UAF_C   "/tmp/codelearner_m17_uaf.c"
#define P_UAF_BIN "/tmp/codelearner_m17_uaf"

/* PART 5: an out-of-bounds write on the student's own future code. */
#define P_OOB_C   "/tmp/codelearner_m17_oob.c"
#define P_OOB_BIN "/tmp/codelearner_m17_oob"

static const char *const M17_SCRATCH_FILES[] = {
    P_GDB_BASIC_C, P_GDB_BASIC_BIN,
    P_CRASH_C, P_CRASH_BIN,
    P_UAF_C, P_UAF_BIN,
    P_OOB_C, P_OOB_BIN,
};

void lesson_17_debugging(void)
{
    title("MODULE 17 - DEBUGGING");

    heading("PART 1: same convention, one more reader");

    printf("  Module 16 ran real compiler diagnostics shaped like this:\n\n");
    printf("    shape.h:1:8: error: redefinition of 'struct Point'\n\n");
    printf("  FILE:LINE:COLUMN:, then a message, then a caret at the exact\n");
    printf("  token. Every tool below speaks the same convention -- gdb\n");
    printf("  names a file and a line the same way, and a sanitizer's\n");
    printf("  report ends with one line built out of the same pieces.\n\n");

    printf("  What changes in this module is WHEN the tool speaks. A\n");
    printf("  compiler only reads source text, so it can only complain\n");
    printf("  about what is written there. gdb and a sanitizer both watch\n");
    printf("  the program actually execute, so they can report something\n");
    printf("  no reading of the source ever could: the exact path that was\n");
    printf("  taken, and the exact values variables held, right at the\n");
    printf("  moment something went wrong.\n");

    wait_enter();
    clear_screen();
    heading("PART 2: gdb basics -- breakpoints, stepping, variables");

    printf("  gdb runs a program under its own control: pause it at a\n");
    printf("  chosen line (a BREAKPOINT), run it one line at a time (STEP\n");
    printf("  into a call, or NEXT over one), and read whatever a variable\n");
    printf("  holds at that exact instant (PRINT, INFO LOCALS). None of\n");
    printf("  that is possible from source text alone -- a value only\n");
    printf("  exists once the program is actually running.\n\n");

    printf("  This course drives gdb in BATCH mode: -batch runs a fixed\n");
    printf("  list of -ex 'COMMAND' arguments and exits, instead of\n");
    printf("  dropping into gdb's interactive prompt. The commands\n");
    printf("  themselves are identical either way -- batch mode exists\n");
    printf("  here only because this course has no terminal to type into.\n");
    printf("  At your own keyboard, `gdb ./program` and typing each\n");
    printf("  command by hand does exactly the same thing, one at a time.\n\n");

    {
        const char gdb_basic_c[] =
            "#include <stdio.h>\n"
            "\n"
            "int square(int n)\n"
            "{\n"
            "    int result = n * n;\n"
            "    return result;\n"
            "}\n"
            "\n"
            "int main(void)\n"
            "{\n"
            "    int x = 5;\n"
            "    int y = square(x);\n"
            "    printf(\"y = %d\\n\", y);\n"
            "    return 0;\n"
            "}\n";

        printf("  square.c, compiled with -g so gdb has line numbers and\n");
        printf("  variable names to work with:\n\n");
        print_code(gdb_basic_c);

        write_file(P_GDB_BASIC_C, gdb_basic_c);

        char cmd[320];
        snprintf(cmd, sizeof cmd, "cc %s -g -o %s %s",
                 M17_CFLAGS, P_GDB_BASIC_BIN, P_GDB_BASIC_C);
        int compiled = run_shell(cmd);

        if (compiled) {
            printf("\n  break square      stop the instant square() is\n");
            printf("                    entered\n");
            printf("  run               start the program\n");
            printf("  print n           read a variable's current value\n");
            printf("  next              run the current line without\n");
            printf("                    stepping INTO a further call\n");
            printf("                    (step would; there is none here)\n");
            printf("  print result\n");
            printf("  info locals       every local in the current frame\n");
            printf("  continue          resume until the program ends or\n");
            printf("                    hits another breakpoint\n");
            printf("  quit\n\n");

            snprintf(cmd, sizeof cmd,
                     "gdb -batch -ex '%s' -ex 'break square' -ex run "
                     "-ex 'print n' -ex next -ex 'print result' "
                     "-ex 'info locals' -ex continue -ex quit %s",
                     M17_GDB_QUIET, P_GDB_BASIC_BIN);
            int ok = run_shell(cmd);

            if (ok) {
                printf("\n  'Breakpoint 1, square (n=5) at\n");
                printf("  /tmp/codelearner_m17_gdb_basic.c:5' -- gdb already\n");
                printf("  knew n's value before a single print\n");
                printf("  command ran, straight from the debug symbols -g\n");
                printf("  wrote. print n and print result both read real\n");
                printf("  memory at that exact instant; info locals lists\n");
                printf("  every local the same way, without asking for each\n");
                printf("  one by name.\n\n");
                printf("  Then continue let the program run to the end on\n");
                printf("  its own -- gdb only pauses and reads memory, it\n");
                printf("  never changes what the program computes.\n");
            }
        } else {
            printf("\n  (compile failed above -- skipping gdb.)\n");
        }
    }

    wait_enter();
    clear_screen();
    heading("PART 3: a real crash, and where gdb says it happened");

    printf("  *p = 42 through a NULL p is undefined behaviour too -- but\n");
    printf("  on Linux it is a RELIABLE kind: the first page of a\n");
    printf("  process's address space is never mapped, on purpose, so\n");
    printf("  writing through address 0 faults every single time instead\n");
    printf("  of maybe working. That reliability is why this module runs\n");
    printf("  it for real, the same way module 16 ran its own broken\n");
    printf("  linker example for real -- PART 4 covers a memory bug that\n");
    printf("  does NOT get this same treatment, and why.\n\n");

    {
        const char crash_c[] =
            "#include <stdio.h>\n"
            "\n"
            "void crash(void)\n"
            "{\n"
            "    int *p = NULL;\n"
            "    *p = 42;\n"
            "}\n"
            "\n"
            "int main(void)\n"
            "{\n"
            "    printf(\"about to crash\\n\");\n"
            "    crash();\n"
            "    return 0;\n"
            "}\n";

        print_code(crash_c);
        write_file(P_CRASH_C, crash_c);

        char cmd[320];
        snprintf(cmd, sizeof cmd, "cc %s -g -o %s %s",
                 M17_CFLAGS, P_CRASH_BIN, P_CRASH_C);
        int compiled = run_shell(cmd);

        if (compiled) {
            printf("\n  Running it under gdb instead of directly, so the\n");
            printf("  crash is caught instead of just killing the shell:\n\n");

            snprintf(cmd, sizeof cmd,
                     "gdb -batch -ex '%s' -ex run -ex backtrace -ex kill "
                     "-ex quit %s",
                     M17_GDB_QUIET, P_CRASH_BIN);
            int ok = run_shell(cmd);

            if (ok) {
                printf("\n  'Program received signal SIGSEGV' names the\n");
                printf("  exact signal the operating system delivered.\n");
                printf("  backtrace then lists every frame still on the\n");
                printf("  call stack, #0 innermost: #0 is crash() itself, at\n");
                printf("  the *p = 42 line; #1 is main(), at the line that\n");
                printf("  called crash(). Read a backtrace from #0 down --\n");
                printf("  it answers \"what was running, who called it, and\n");
                printf("  who called them\", all the way back to main().\n");
            }
        } else {
            printf("\n  (compile failed above -- skipping gdb.)\n");
        }
    }

    wait_enter();
    clear_screen();
    heading("PART 4: a memory bug too unreliable to just run");

    printf("  Module 9 introduced USE AFTER FREE: freeing memory, then\n");
    printf("  reading or writing through the same pointer anyway. It is\n");
    printf("  undefined behaviour exactly like PART 3's NULL write -- but\n");
    printf("  it does not get PART 3's treatment, and the difference is\n");
    printf("  the whole point of this part.\n\n");

    printf("  A NULL write faults every time because address 0 is never\n");
    printf("  mapped. Freed heap memory is still ordinary, mapped memory --\n");
    printf("  free() only marks it reusable. Writing through it afterward\n");
    printf("  might crash, might corrupt something else entirely, or might\n");
    printf("  just \"work\", unnoticed, if nothing has reused that address\n");
    printf("  yet. Module 9 called that the worst case. Running this raw\n");
    printf("  and hoping it crashes would teach exactly the wrong lesson,\n");
    printf("  so this module never does -- the build below has\n");
    printf("  AddressSanitizer on from the very first compile, and there\n");
    printf("  is no earlier, unsanitized version of this file anywhere.\n\n");

    {
        const char uaf_c[] =
            "#include <stdio.h>\n"
            "#include <stdlib.h>\n"
            "\n"
            "int main(void)\n"
            "{\n"
            "    int *p = malloc(sizeof(int));\n"
            "\n"
            "    if (p == NULL) {\n"
            "        return 1;\n"
            "    }\n"
            "\n"
            "    *p = 42;\n"
            "    free(p);\n"
            "    *p = 99;\n"
            "    printf(\"%d\\n\", *p);\n"
            "    return 0;\n"
            "}\n";

        print_code(uaf_c);
        write_file(P_UAF_C, uaf_c);

        printf("\n  Compiling WITH -fsanitize=address, first and only time:\n\n");

        char cmd[320];
        snprintf(cmd, sizeof cmd, "cc %s -g -o %s %s",
                 M17_ASAN_CFLAGS, P_UAF_BIN, P_UAF_C);
        int compiled = run_shell(cmd);

        if (compiled) {
            printf("\n  Some compilers can spot this exact use-after-free\n");
            printf("  shape just by reading the source, no run needed --\n");
            printf("  gcc's -Wuse-after-free is one such warning; whether\n");
            printf("  it printed above depends on which compiler cc runs\n");
            printf("  on your machine. Either way it only catches simple,\n");
            printf("  visible cases like this one -- a bonus, not a\n");
            printf("  replacement for the sanitizer below.\n\n");

            printf("  Running the ASan-instrumented binary -- this is the\n");
            printf("  only way this bug ever executes in this course:\n\n");

            int clean = run_shell(P_UAF_BIN);

            if (!clean) {
                printf("\n  'heap-use-after-free' names the bug; the WRITE\n");
                printf("  line gives the exact address and size. 'freed by\n");
                printf("  thread T0 here' and 'previously allocated by\n");
                printf("  thread T0 here' are two more real backtraces\n");
                printf("  inside the same report -- one for the free() that\n");
                printf("  made this memory off-limits, one for the malloc()\n");
                printf("  that created it. The SUMMARY line at the bottom is\n");
                printf("  the one to read first: file, line, kind of bug.\n\n");

                printf("  gdb can attach to this same ASan binary too, but\n");
                printf("  not by just running it again: ASan's default on\n");
                printf("  Linux is ASAN_OPTIONS=abort_on_error=0, meaning\n");
                printf("  the run just above ended with a plain _exit(1),\n");
                printf("  not a signal -- there was never a SIGABRT for gdb\n");
                printf("  to catch. Setting abort_on_error=1 for this one\n");
                printf("  invocation is what turns the same report into a\n");
                printf("  real SIGABRT gdb can stop on, instead of the\n");
                printf("  SIGSEGV PART 3 caught:\n\n");

                snprintf(cmd, sizeof cmd,
                         "ASAN_OPTIONS=abort_on_error=1 gdb -batch -ex '%s' "
                         "-ex 'catch signal SIGABRT' -ex run -ex backtrace "
                         "-ex kill -ex quit %s",
                         M17_GDB_QUIET, P_UAF_BIN);
                int attached = run_shell(cmd);

                if (attached) {
                    printf("\n  The frames near the top are glibc's and\n");
                    printf("  libasan's own internals getting from the write\n");
                    printf("  to the abort() call. The deepest frame that\n");
                    printf("  names uaf.c instead of a library is this\n");
                    printf("  program's own code -- the same line the\n");
                    printf("  SUMMARY line above already named, this time\n");
                    printf("  reached by walking the real call stack instead\n");
                    printf("  of reading a report.\n");
                }
            } else {
                printf("\n  (unexpected: this run exited cleanly instead of\n");
                printf("  reporting a use-after-free. ASan's default here\n");
                printf("  is still to detect the bug -- abort_on_error only\n");
                printf("  changes how it exits once found, not whether it\n");
                printf("  finds it -- so a clean exit would mean this\n");
                printf("  binary was not actually built with\n");
                printf("  -fsanitize=address.)\n");
            }
        } else {
            printf("\n  (compile failed above -- skipping the run.)\n");
        }
    }

    wait_enter();
    clear_screen();
    heading("PART 5: running a sanitizer on code you just wrote");

    printf("  Any code written after this course can be checked the exact\n");
    printf("  same way -- not just this course's own examples. One added\n");
    printf("  flag turns a chosen kind of undefined behaviour into an\n");
    printf("  instant, reported crash instead of a silent maybe:\n\n");

    printf("    cc -fsanitize=address,undefined yourfile.c -o test && ./test\n\n");

    printf("  -fsanitize=address (ASan) catches out-of-bounds reads and\n");
    printf("  writes, use-after-free, and double-free. -fsanitize=undefined\n");
    printf("  (UBSan) catches a different family -- signed overflow,\n");
    printf("  division by zero, misaligned access -- that ASan does not.\n");
    printf("  Both turn on together, as above, for the price of one flag\n");
    printf("  each.\n\n");

    {
        const char oob_c[] =
            "#include <stdio.h>\n"
            "\n"
            "int main(void)\n"
            "{\n"
            "    int arr[5] = {1, 2, 3, 4, 5};\n"
            "\n"
            "    arr[5] = 100;\n"
            "\n"
            "    printf(\"%d\\n\", arr[5]);\n"
            "    return 0;\n"
            "}\n";

        printf("  arr has 5 slots, 0 through 4 -- module 5's own\n");
        printf("  off-by-one rule, written directly instead of through a\n");
        printf("  loop this time:\n\n");
        print_code(oob_c);
        write_file(P_OOB_C, oob_c);

        char cmd[320];
        snprintf(cmd, sizeof cmd, "cc %s -g -o %s %s",
                 M17_ASAN_CFLAGS, P_OOB_BIN, P_OOB_C);
        int compiled = run_shell(cmd);

        if (compiled) {
            printf("\n  Running it:\n\n");
            int clean = run_shell(P_OOB_BIN);

            if (!clean) {
                printf("\n  'stack-buffer-overflow' -- a different name from\n");
                printf("  PART 4's 'heap-use-after-free', because this\n");
                printf("  happened on the STACK, a local array, instead of\n");
                printf("  the heap. The report even names the declared\n");
                printf("  variable itself, \"'arr' (line 5) <== Memory access\n");
                printf("  ... overflows this variable\" -- it does not just\n");
                printf("  say something went wrong nearby, it names which\n");
                printf("  variable the bad write landed inside.\n");
            } else {
                printf("\n  (unexpected: this should have aborted -- see\n");
                printf("  PART 4's note above.)\n");
            }
        } else {
            printf("\n  (compile failed above -- skipping the run.)\n");
        }
    }

    wait_enter();
    clear_screen();
    exercise(17);

    question("A gdb backtrace prints:\n"
             "    #0  crash () at crash.c:6\n"
             "    #1  main () at crash.c:12\n"
             "  Which frame is where the program actually stopped -- #0 or\n"
             "  #1? (answer: 0 or 1)",
             "0",
             "#0 is always the innermost frame: the function that was\n"
             "             actually running when gdb stopped it. Every frame\n"
             "             after it is only who called who, working outward.");

    question("Does pausing a program at a gdb breakpoint and stepping\n"
             "  through it change what values the program computes?\n"
             "  (answer: yes or no)",
             "no",
             "gdb only reads memory and the debug symbols -g wrote; it\n"
             "             does not rewrite the program. The same run,\n"
             "             watched or not, computes the same result.");

    question("A use-after-free, run without a sanitizer, is guaranteed to\n"
             "  crash every single time, the same way a NULL-pointer write\n"
             "  is. (answer: yes or no)",
             "no",
             "A NULL write always faults because address 0 is never\n"
             "             mapped; freed heap memory is still ordinary,\n"
             "             mapped memory, so writing through it might crash,\n"
             "             might corrupt something else, or might silently\n"
             "             \"work\" -- exactly why this module never ran that\n"
             "             example without AddressSanitizer already on.");

    question("AddressSanitizer only reports a bug on code that actually\n"
             "  executes. Would it catch an out-of-bounds write sitting\n"
             "  inside an if-branch that never runs during a given test?\n"
             "  (answer: yes or no)",
             "no",
             "ASan instruments real memory accesses as the program runs;\n"
             "             a line that never executes is never checked. A\n"
             "             compiler warning reads the source text instead,\n"
             "             so it can flag a mistake on a branch that never\n"
             "             once ran.");

    heading("TASK: NAME THE LINE");

    printf("  Write this to a real file, task.c -- module 5's own\n");
    printf("  off-by-one rule (\"an array of 5 has no index 5\"), except\n");
    printf("  this time it actually runs:\n\n");

    rule();
    printf("  #include <stdio.h>\n");
    printf("  \n");
    printf("  int main(void)\n");
    printf("  {\n");
    printf("      int scores[5];\n");
    printf("  \n");
    printf("      for (int i = 0; i <= 5; i++) {\n");
    printf("          scores[i] = i * 10;\n");
    printf("      }\n");
    printf("  \n");
    printf("      for (int i = 0; i < 5; i++) {\n");
    printf("          printf(\"%%d \", scores[i]);\n");
    printf("      }\n");
    printf("      printf(\"\\n\");\n");
    printf("  \n");
    printf("      return 0;\n");
    printf("  }\n");
    rule();

    printf("\n  Try it first: compile it with AddressSanitizer and read the\n");
    printf("  report before reading any further.\n\n");
    printf("    cc -std=c11 -Wall -Wextra -g -fsanitize=address task.c -o task && ./task\n\n");
    printf("  Report which line number the SUMMARY line names. Is it the\n");
    printf("  line with <= 5, or a different one?\n");

    if (ask_yes("Want to see the answer?")) {
        printf("\n  AddressSanitizer names line 8, `scores[i] = i * 10;` --\n");
        printf("  the actual out-of-bounds WRITE -- not line 7, where the\n");
        printf("  <= that causes it lives. A sanitizer reports the moment\n");
        printf("  of harm, not the earlier mistake that led to it; reading\n");
        printf("  up from there to find the <= is a step you still do\n");
        printf("  yourself.\n\n");
        printf("  Change <= to < on line 7 and recompile. The report\n");
        printf("  should be gone -- and unlike this module's own examples,\n");
        printf("  you can check that for real, on your own machine, right\n");
        printf("  now.\n");
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    printf("   - a compiler only reads source text; gdb and a sanitizer\n");
    printf("     both watch a program actually run, so they report things\n");
    printf("     only run time ever produces\n");
    printf("   - gdb pauses a program with a breakpoint, moves one line at\n");
    printf("     a time with step/next, and reads real values with print\n");
    printf("     and info locals -- without ever changing what it computes\n");
    printf("   - a backtrace lists every frame still on the call stack,\n");
    printf("     #0 innermost -- the function that was actually running,\n");
    printf("     then who called it, all the way back to main()\n");
    printf("   - a NULL-pointer write faults every time and is safe to run\n");
    printf("     for real; a use-after-free is not, which is why this\n");
    printf("     module compiled it with AddressSanitizer from the start\n");
    printf("   - AddressSanitizer only checks code that actually executes,\n");
    printf("     and only certain bug classes; -fsanitize=undefined covers\n");
    printf("     a different family -- together, one flag each\n");
    printf("   - the same recipe works on anything written next:\n");
    printf("     cc -fsanitize=address,undefined yourfile.c -o test && ./test\n");
    wait_enter();

    for (size_t i = 0; i < sizeof M17_SCRATCH_FILES / sizeof M17_SCRATCH_FILES[0]; i++) {
        remove(M17_SCRATCH_FILES[i]);
    }
}
