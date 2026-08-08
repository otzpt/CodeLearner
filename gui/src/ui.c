#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ui.h"

/* Inside width of the frame. Changing this changes every box in the course. */
#define WIDTH 54

void clear_screen(void)
{
#ifdef _WIN32
    /* The old cmd.exe does not understand ANSI codes; cls handles it. */
    system("cls");
#else
    /* ANSI codes rather than system("clear"): no second process per screen,
     * and no dependency on the TERM variable, which is missing when the
     * program runs outside a normal terminal. */
    fputs("\033[H\033[2J\033[3J", stdout);
    fflush(stdout);
#endif
}

void wait_enter(void)
{
    printf("\n  Press ENTER to continue...");
    fflush(stdout);

    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* discard */
    }
}

void rule(void)
{
    printf("  ");
    for (int i = 0; i < WIDTH; i++) {
        putchar('-');
    }
    putchar('\n');
}

static void padded_line(const char *text, char border)
{
    int written = (int) strlen(text);
    printf("  %c %s", border, text);

    for (int i = written; i < WIDTH - 3; i++) {
        putchar(' ');
    }
    printf("%c\n", border);
}

static void frame(char fill)
{
    printf("  +");
    for (int i = 0; i < WIDTH - 2; i++) {
        putchar(fill);
    }
    printf("+\n");
}

void title(const char *text)
{
    putchar('\n');
    frame('=');
    padded_line(text, '|');
    frame('=');
    putchar('\n');
}

void heading(const char *text)
{
    printf("\n  %s\n", text);
    rule();
}

int read_line(char *dest, int size)
{
    if (fgets(dest, size, stdin) == NULL) {
        dest[0] = '\0';
        return 0;
    }

    dest[strcspn(dest, "\n")] = '\0';
    return 1;
}

int ask_yes(const char *question_text)
{
    char answer[8];

    printf("\n  %s (y/N): ", question_text);
    fflush(stdout);

    if (!read_line(answer, sizeof answer)) {
        return 0;
    }
    return answer[0] == 'y' || answer[0] == 'Y';
}

void exercise(int number)
{
    printf("\n  >> EXERCISE - MODULE %d\n", number);
    rule();
}

static void normalise(char *dest, int size, const char *src)
{
    int end = (int) strlen(src);
    int start = 0;

    while (src[start] == ' ' || src[start] == '\t') {
        start++;
    }
    while (end > start && (src[end - 1] == ' ' || src[end - 1] == '\t')) {
        end--;
    }

    int n = 0;
    for (int i = start; i < end && n < size - 1; i++) {
        char c = src[i];
        dest[n++] = (c >= 'A' && c <= 'Z') ? (char) (c + 32) : c;
    }
    dest[n] = '\0';
}

int question(const char *text, const char *correct, const char *why)
{
    char answer[64];
    char cleaned[64];
    char expected[64];

    printf("\n  %s\n", text);
    printf("  Your answer: ");
    fflush(stdout);

    if (!read_line(answer, sizeof answer)) {
        answer[0] = '\0';
    }

    normalise(cleaned, sizeof cleaned, answer);
    normalise(expected, sizeof expected, correct);

    int right = strcmp(cleaned, expected) == 0;

    if (right) {
        printf("\n  CORRECT.  %s\n", why);
    } else {
        printf("\n  NOT QUITE. The answer is: %s\n", correct);
        printf("             %s\n", why);
    }
    return right;
}
