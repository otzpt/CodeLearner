/*
 * TITLE: Language, locale and character encoding
 * GROUP: System information
 * USES: #include <locale.h>, <langinfo.h>
 * SUMMARY: The locale decides the language, date formats and the character
 *   encoding. It comes from the environment (LC_ALL, then LC_*, then LANG).
 *   setlocale(LC_ALL, "") adopts it.
 * NOTES:
 *   - A program starts in the "C" locale until it calls setlocale(LC_ALL, "").
 *   - nl_langinfo(CODESET) is the encoding. If it is "UTF-8" you can print
 *     box-drawing characters and accents. In "ANSI_X3.4-1968" (plain ASCII) you
 *     should not.
 *   - LANG looks like en_US.UTF-8 or pt_PT.UTF-8: language, country, encoding.
 *   - Over ssh or in cron, LANG is often unset and the answer is "C".
 * SEE: man 7 locale, man 3 setlocale, man 3 nl_langinfo
 */

#include <langinfo.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>

static const char *env_or(const char *name)
{
    const char *value = getenv(name);
    return value != NULL ? value : "(not set)";
}

int main(void)
{
    printf("LANG      %s\n", env_or("LANG"));
    printf("LC_ALL    %s\n", env_or("LC_ALL"));
    printf("before setlocale: %s\n", setlocale(LC_ALL, NULL));

    const char *chosen = setlocale(LC_ALL, "");
    printf("after setlocale:  %s\n", chosen != NULL ? chosen : "(locale not installed)");
    printf("encoding:         %s\n", nl_langinfo(CODESET));
    return 0;
}
