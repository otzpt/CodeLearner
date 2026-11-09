/*
 * ui.h - the pieces every lesson uses to draw the screen.
 *
 * Kept separate because clearing the screen and making a scratch directory
 * differ between Windows and Linux. With them here, no lesson needs to know
 * which system it is running on.
 */

#ifndef UI_H
#define UI_H

/* Clear the screen. */
void clear_screen(void);

/* Move into a new private directory for a module's scratch files, so nobody
 * else on a shared machine can plant a file or symlink under a name the
 * module is about to write and run. scratch_leave() moves back and removes
 * the directory, which must be empty by then. */
void scratch_enter(void);
void scratch_leave(void);

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

/* A task to write in a real file.
 *
 * `input` is every line the program will read while producing `expected` --
 * empty (input_lines 0) for a task that reads nothing. Together they are the
 * actual specification: run the solution, type `input`, get `expected`, no
 * matter how the code that does it is written. `solution` is one way of
 * getting there and appears only after a confirmation, so there is a chance
 * to try first.
 *
 * Rule for whoever writes a challenge: the solution may only use what the
 * course has already taught by this module. A task needing `if` before module
 * 4 has taught `if` is a broken task, not a hard one.
 */
void challenge(const char *task[], int task_lines,
               const char *input[], int input_lines,
               const char *expected[], int expected_lines,
               const char *solution[], int solution_lines);

#endif
