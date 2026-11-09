/* strings_pt - Portuguese text for the menus, prompts and summaries. */

#ifndef STRINGS_PT_H
#define STRINGS_PT_H

/* True when CODELEARNER_LANG is pt-pt. */
int lang_pt(void);

/* The Portuguese text for `text` in that mode, otherwise `text` itself. */
const char *tr(const char *text);

/* Switches language here and for every course started from now on. */
void toggle_language(void);

/* Windows needs telling that text is UTF-8. Courses inherit the setting. */
void use_chosen_language(void);

#endif
