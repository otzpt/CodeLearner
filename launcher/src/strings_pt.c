/*
 * Portuguese (PT-PT) text for the menus, prompts and module summaries.
 * CODELEARNER_LANG=pt-pt turns it on. The lessons themselves stay in English.
 */
#ifndef _WIN32
#define _POSIX_C_SOURCE 200809L
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#endif

#include "strings_pt.h"

static int pt_mode = -1;

int lang_pt(void)
{
    if (pt_mode < 0) {
        const char *value = getenv("CODELEARNER_LANG");
        pt_mode = value != NULL && strcmp(value, "pt-pt") == 0;
    }
    return pt_mode;
}

static const char *const TEXT[][2] = {
    { "Check for updates", "Procurar atualizações" },
    { "Exit", "Sair" },
    { "Pick an entry:", "Escolhe uma entrada:" },
    { "(coming soon)", "(em breve)" },
    { "LOW-LEVEL AND SYSTEMS", "BAIXO NIVEL E SISTEMAS" },
    { "LANGUAGES", "LINGUAGENS" },
    { "WEB", "WEB" },
    { "SECURITY", "SEGURANCA" },
    { "GUI", "GUI" },
    { "TOOLING", "FERRAMENTAS" },
    { "REFERENCE", "REFERENCIA" },
    { "Press ENTER to continue...", "Prime ENTER para continuar..." },
    { "Not a valid option.", "Opção inválida." },
    { "See you next time.", "Até à próxima." },
};

const char *tr(const char *text)
{
    if (lang_pt()) {
        for (size_t i = 0; i < sizeof TEXT / sizeof TEXT[0]; i++) {
            if (strcmp(TEXT[i][0], text) == 0) {
                return TEXT[i][1];
            }
        }
    }
    return text;
}

void toggle_language(void)
{
    pt_mode = !lang_pt();
#ifdef _WIN32
    _putenv_s("CODELEARNER_LANG", pt_mode ? "pt-pt" : "");
    if (pt_mode) {
        SetConsoleOutputCP(CP_UTF8);
    }
#else
    if (pt_mode) {
        setenv("CODELEARNER_LANG", "pt-pt", 1);
    } else {
        unsetenv("CODELEARNER_LANG");
    }
#endif
}

void use_chosen_language(void)
{
#ifdef _WIN32
    if (lang_pt()) {
        SetConsoleOutputCP(CP_UTF8);
    }
#endif
}
