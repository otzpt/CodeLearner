/*
 * GUI course - main menu.
 *
 * Same shape as every other course's main.c: a table of modules, a loop.
 * Build:  make
 * Run:    ./gui-course
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lessons.h"
#include "ui.h"

/* `tier` files a module under one of the three menu bands. Display
 * grouping only -- the lesson functions never see it. INTERMEDIATE and
 * ADVANCED have no modules yet; they still print, empty, to show the arc
 * this course is headed toward. */
struct Module {
    const char *title;
    void (*run)(void);
    const char *tier;
};

static const struct Module MODULES[] = {
    { "GTK: your first window", lesson_01_gtk, "BASIC" },
};

#define MODULE_COUNT (int)(sizeof MODULES / sizeof MODULES[0])

static const char *const EMPTY_TIERS[] = { "INTERMEDIATE", "ADVANCED" };

static void show_menu(void)
{
    clear_screen();
    title("GUI COURSE - GTK");
    printf("  NO GUI -- this course is a CLI program, same as every other\n");
    printf("  course here. It teaches GTK; it does not open a window itself.\n");

    const char *shown_tier = "";
    for (int i = 0; i < MODULE_COUNT; i++) {
        if (strcmp(MODULES[i].tier, shown_tier) != 0) {
            shown_tier = MODULES[i].tier;
            printf("\n  -- %s --\n", shown_tier);
        }
        printf("   [%2d]  %s\n", i + 1, MODULES[i].title);
    }
    for (size_t i = 0; i < sizeof EMPTY_TIERS / sizeof EMPTY_TIERS[0]; i++) {
        printf("\n  -- %s -- (coming soon)\n", EMPTY_TIERS[i]);
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

        if (!read_line(choice, sizeof choice)) {
            break;
        }

        if (strcmp(choice, "0") == 0) {
            break;
        }

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
