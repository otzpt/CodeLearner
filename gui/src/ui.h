/*
 * ui.h - the pieces every lesson uses to draw the screen.
 *
 * Kept separate because clearing the screen is the only thing in this whole
 * program that differs between Windows and Linux. With it here, no lesson
 * needs to know which system it is running on.
 *
 * Duplicated from c/src/ui.h rather than shared with it on purpose -- see
 * docs/writing-a-course.md: courses do not share code with each other, and
 * the duplication here is one small menu loop, not a library worth the
 * coupling.
 */

#ifndef UI_H
#define UI_H

#include "strings_pt.h"

/* Clear the screen. */
void clear_screen(void);

/* Wait for ENTER. Used between parts of a lesson, so nobody has to read
 * three screens at once. */
void wait_enter(void);

/* A full-width rule. */
void rule(void);

/* A framed title:
 *
 *   +======================================================+
 *   |  MODULE 1 - COMPILING AND PRINTING                   |
 *   +======================================================+
 */
void title(const char *text);

/* A heading inside a lesson (PART 1, SUMMARY, ...). */
void heading(const char *text);

/* Read one line into `dest`, never writing more than `size` bytes. Returns 1
 * if something was read, 0 at end of input.
 *
 * Exists because scanf("%s") does not know the size of its destination and
 * gets() was removed from the language for the same reason.
 */
int read_line(char *dest, int size);

/* Ask a yes/no question. Returns 1 for yes. ENTER means no. */
int ask_yes(const char *question);

/* Header for a module's exercise section. */
void exercise(int number);

/* A short-answer question.
 *
 * Prints `text`, reads the answer, compares it with `correct` (ignoring
 * surrounding spaces and letter case) and says whether it was right. `why`
 * is shown either way -- somebody who guessed correctly still needs the
 * reason.
 *
 * Returns 1 if correct. Nobody is blocked from continuing: the point is to
 * show the right answer, not to fail people.
 */
int question(const char *text, const char *correct, const char *why);


/* ---- colour -----------------------------------------------------------
 *
 * Colour is on only when stdout is a terminal and NO_COLOR is not set (and,
 * on Windows, only inside Windows Terminal). CODELEARNER_COLOR=1 forces it on
 * and =0 forces it off. When it is off every colour string below is "", so
 * piped output and CI logs stay plain text. Call ui_init() once, first. */
void ui_init(void);

extern const char *C_RESET, *C_BOLD, *C_DIM, *C_RED, *C_GREEN, *C_YELLOW,
                  *C_BLUE, *C_MAGENTA, *C_CYAN;

/* Number of lines in a string array, for the calls below:
 *     code(LINES(EXAMPLE));                                              */
#define LINES(a) (a), (int) (sizeof (a) / sizeof (a)[0])

/* Print example C code, one array element per line. Comments, strings,
 * #preprocessor lines and a few keywords are coloured. */
void code(const char *const *lines, int n);

/* The same, with a green "+" or red "x" in the margin: code worth copying
 * and code that is wrong. */
void code_good(const char *const *lines, int n);
void code_bad(const char *const *lines, int n);

/* One-line notes: a coloured label, then the text. more() continues the
 * previous note on the next line, indented to match. */
void good(const char *text);
void bad(const char *text);
void tip(const char *text);
void more(const char *text);

/* A reference entry in the style of a C book's function index:
 *
 *     gtk_window_set_title(window, title)
 *       Does:     sets the text in the title bar.
 *       Takes:    ...
 *       Returns:  nothing.
 *
 * ref_line() with an empty label continues the line above. */
void ref_head(const char *signature);
void ref_line(const char *label, const char *text);

#endif
