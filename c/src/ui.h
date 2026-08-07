/*
 * ui.h - the pieces every lesson uses to draw the screen.
 *
 * Kept separate because clearing the screen is the only thing in this whole
 * program that differs between Windows and Linux. With it here, no lesson
 * needs to know which system it is running on.
 */

#ifndef UI_H
#define UI_H

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
 * gets() was removed from the language for the same reason. Module 3
 * explains why.
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

/* A task to write in a real file, followed by a solution. The solution only
 * appears after a confirmation, so there is a chance to try first. */
void challenge(const char *task[], int task_lines,
               const char *solution[], int solution_lines);

#endif
