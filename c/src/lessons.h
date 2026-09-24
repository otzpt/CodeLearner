/*
 * lessons.h - the modules of the course.
 *
 * Each module is a function taking nothing and returning nothing. It draws
 * its own screens and returns when the student is done. The menu knows
 * nothing about what happens inside, so adding a module means writing the
 * function and adding one line to the table in main.c.
 */

#ifndef LESSONS_H
#define LESSONS_H

/* Basics */
void lesson_01_compiling(void);
void lesson_02_variables(void);
void lesson_03_input(void);
void lesson_04_conditions(void);
void lesson_05_loops(void);

/* Memory and structures */
void lesson_06_arrays_strings(void);
void lesson_07_functions(void);
void lesson_08_pointers(void);
void lesson_09_memory(void);
void lesson_10_structs(void);

/* Past the core course */
void lesson_11_final_test(void);
void lesson_12_growing_memory(void);
void lesson_13_linked_lists(void);
void lesson_14_stacks_queues(void);
void lesson_15_enums_unions(void);

/* The toolchain */
void lesson_16_compilation(void);
void lesson_17_debugging(void);
void lesson_18_file_io(void);
void lesson_19_advanced(void);

#endif
