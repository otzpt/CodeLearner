// ui.h - the pieces every lesson uses to draw the screen.
//
// Same purpose and the same visual style as the C course's ui.h, kept as a
// separate implementation rather than shared code -- see
// docs/writing-a-course.md for why. This one is written in idiomatic C++
// rather than translated from C: std::string instead of a fixed buffer,
// std::vector<std::string> instead of an array plus a count.

#ifndef UI_H
#define UI_H

#include <string>
#include <vector>

void clearScreen();

// Wait for ENTER. Used between parts of a lesson.
void waitEnter();

// A full-width rule.
void rule();

// A framed title, matching the C course's box style:
//
//   +======================================================+
//   |  MODULE 1 - COMPILING AND PRINTING                   |
//   +======================================================+
void title(const std::string &text);

// A heading inside a lesson (PART 1, SUMMARY, ...).
void heading(const std::string &text);

// Ask a yes/no question. Returns true for yes. ENTER means no.
bool askYes(const std::string &question);

// Header for a module's exercise section.
void exercise(int number);

// A short-answer question.
//
// Prints `text`, reads the answer, compares it with `correct` (ignoring
// surrounding spaces and letter case) and says whether it was right. `why`
// is shown either way. Returns true if correct; nobody is blocked from
// continuing.
bool question(const std::string &text, const std::string &correct, const std::string &why);

// A task to write in a real file, stating the output it must produce.
// Example code is shown afterwards only if asked for.
void challenge(const std::vector<std::string> &task,
               const std::vector<std::string> &expected,
               const std::vector<std::string> &solution);

#endif
