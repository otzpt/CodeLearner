/*
 * C course - main menu.
 *
 * Written in C on purpose. A C course whose own source the student can open
 * and read is worth more than one describing the language from the outside,
 * and everything this file uses is taught in the first modules: arrays, a
 * struct, a function pointer and a loop.
 *
 * Build:  make
 * Run:    ./c-course
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lessons.h"
#include "ui.h"

/* One menu entry: what is shown, and the function that runs.
 *
 * `void (*run)(void)` is a function pointer - a variable holding the address
 * of a function instead of a number. It is what lets the module list be a
 * table rather than a giant switch. Module 8 covers pointers; this is the
 * same idea applied to code.
 */
struct Module {
    const char *title;
    void (*run)(void);
};

static const struct Module MODULES[] = {
    { "Compiling and printing",   lesson_01_compiling },
    { "Variables and types",      lesson_02_variables },
    { "Reading input",            lesson_03_input },
    { "Conditions",               lesson_04_conditions },
    { "Loops",                    lesson_05_loops },
    { "Arrays and strings",       lesson_06_arrays_strings },
    { "Functions",                lesson_07_functions },
    { "Pointers",                 lesson_08_pointers },
    { "Memory: malloc and free",  lesson_09_memory },
    { "Structs",                  lesson_10_structs },
    { "Final test: 2 programs",   lesson_11_final_test },
    { "Going deeper: memory (extra)", lesson_12_deeper_memory },
    { "Stacks and queues",        lesson_13_stacks_queues },
    { "GTK: your first window (extra)", lesson_14_gtk },
};

/* Number of elements in the array: the size of the whole array divided by
 * the size of one element. This works here because MODULES really is an
 * array; on a pointer it would give the size of the pointer instead, which
 * module 6 comes back to. */
#define MODULE_COUNT (int)(sizeof MODULES / sizeof MODULES[0])

static void show_menu(void)
{
    clear_screen();
    title("C COURSE - FROM ZERO TO MEMORY");

    for (int i = 0; i < MODULE_COUNT; i++) {
        printf("   [%2d]  %s\n", i + 1, MODULES[i].title);
    }

    printf("\n   [ 0]  Quit\n");
    rule();
}

int main(void)
{
    char choice[16];

    for (;;) {
        show_menu();
        printf("\n  Pick a module: ");
        fflush(stdout);

        /* End of input (Ctrl-D, or the program being fed from a file) counts
         * as quitting. Without this the program would spin forever reading
         * nothing. */
        if (!read_line(choice, sizeof choice)) {
            break;
        }

        if (strcmp(choice, "0") == 0) {
            break;
        }

        /* atoi returns 0 for anything that is not a number, and 0 already
         * means quit, so "abc" falls through to the invalid branch. */
        int n = atoi(choice);

        if (n >= 1 && n <= MODULE_COUNT) {
            clear_screen();
            MODULES[n - 1].run();
        } else {
            printf("\n  Not a valid option.\n");
            wait_enter();
        }
    }

    printf("\n  See you next time.\n\n");
    return 0;
}
