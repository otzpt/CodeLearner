/* strings_pt - Portuguese text for the menus, prompts and summaries. */

#ifndef STRINGS_PT_H
#define STRINGS_PT_H

/* True when CODELEARNER_LANG is pt-pt. */
int lang_pt(void);

/* The Portuguese text for `text` in that mode, otherwise `text` itself. */
const char *tr(const char *text);

/* Prints a module's summary in Portuguese and returns 1, or returns 0. */
int print_summary_pt(int module);

#endif
