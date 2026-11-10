/*
 * raylib course - main menu.
 *
 * Same shape as every other course's main.c: a table of modules, a loop.
 * Build:  make
 * Run:    ./raylib-course
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lessons.h"
#include "ui.h"

/* `tier` files a module under one of the three menu bands. Display
 * grouping only -- the lesson functions never see it. ADVANCED has no
 * modules yet; it still prints, empty, to show the arc this course is
 * headed toward. */
struct Module {
    const char *title;
    void (*run)(void);
    const char *tier;
};

static const struct Module MODULES[] = {
    { "raylib: your first window and the loop", lesson_01_raylib, "BASIC" },
    { "raygui: buttons, text and state", lesson_02_raygui, "BASIC" },
    { "raygui: a todo list", lesson_03_todo, "INTERMEDIATE" },
};

#define MODULE_COUNT (int)(sizeof MODULES / sizeof MODULES[0])

static const char *const EMPTY_TIERS[] = { "ADVANCED" };

static void show_menu(void)
{
    clear_screen();
    title(tr("RAYLIB COURSE"));
    printf("  %s\n", tr("NO GUI -- this course is a CLI program, same as every other"));
    printf("  %s\n", tr("course here. It teaches raylib and raygui; it does not open a window itself."));

    const char *shown_tier = "";
    for (int i = 0; i < MODULE_COUNT; i++) {
        if (strcmp(MODULES[i].tier, shown_tier) != 0) {
            shown_tier = MODULES[i].tier;
            printf("\n  -- %s --\n", tr(shown_tier));
        }
        printf("   [%2d]  %s\n", i + 1, tr(MODULES[i].title));
    }
    for (size_t i = 0; i < sizeof EMPTY_TIERS / sizeof EMPTY_TIERS[0]; i++) {
        printf("\n  -- %s -- (coming soon)\n", tr(EMPTY_TIERS[i]));
    }

    printf("\n   [ 0]  %s\n", tr("Quit"));
    rule();
}

int main(void)
{
    char choice[16];

    ui_init();

    for (;;) {
        show_menu();
        printf("\n  %s ", tr("Pick a module:"));
        fflush(stdout);

        if (!read_line(choice, sizeof choice)) {
            break;
        }

        if (strcmp(choice, "0") == 0) {
            break;
        }

        char *end;
        long n = strtol(choice, &end, 10);

        if (*end == '\0' && n >= 1 && n <= MODULE_COUNT) {
            clear_screen();
            MODULES[n - 1].run();
        } else {
            printf("\n  %s\n", tr("Not a valid option."));
            wait_enter();
        }
    }

    printf("\n  %s\n\n", tr("See you next time."));
    return 0;
}
