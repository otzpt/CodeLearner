/*
 * lessons.h - the modules of the GUI course.
 *
 * Each module is a function taking nothing and returning nothing. It draws
 * its own screens and returns when the student is done. The menu knows
 * nothing about what happens inside, so adding a module means writing the
 * function and adding one line to the table in main.c.
 */

#ifndef LESSONS_H
#define LESSONS_H

void lesson_01_gtk(void);

#endif
