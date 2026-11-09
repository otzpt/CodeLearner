/*
 * HTML course - main menu.
 *
 * Same shape as every other course's main.c: a table of modules, a loop.
 * Build:  make
 * Run:    ./html-course
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lessons.h"
#include "ui.h"

/* `tier` files a module under one of the three menu bands. Display
 * grouping only: the lesson functions never see it. */
struct Module {
    const char *title;
    void (*run)(void);
    const char *tier;
};

static const struct Module MODULES[] = {
    { "Structure and meaning", lesson_01_structure, "BASIC" },
    { "Forms", lesson_02_forms, "BASIC" },
    { "Accessibility", lesson_03_accessibility, "INTERMEDIATE" },
    { "Metadata", lesson_04_metadata, "INTERMEDIATE" },
    { "Practical pages", lesson_05_practical, "ADVANCED" },
};

#define MODULE_COUNT (int)(sizeof MODULES / sizeof MODULES[0])

static void show_menu(void)
{
    clear_screen();
    title(tr("HTML COURSE"));
    printf("  %s\n", tr("NO BROWSER -- this course is a CLI program, same as every"));
    printf("  %s\n", tr("other course here. It teaches HTML; it never opens a page."));

    const char *shown_tier = "";
    for (int i = 0; i < MODULE_COUNT; i++) {
        if (strcmp(MODULES[i].tier, shown_tier) != 0) {
            shown_tier = MODULES[i].tier;
            printf("\n  -- %s --\n", tr(shown_tier));
        }
        printf("   [%2d]  %s\n", i + 1, tr(MODULES[i].title));
    }

    printf("\n   [ 0]  %s\n", tr("Quit"));
    rule();
}

int main(void)
{
    char choice[16];

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
