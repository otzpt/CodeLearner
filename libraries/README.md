# Libraries

A reference, not a course: how to use the libraries behind a job, one page per
job, in the style of a manual page or a good Stack Overflow answer. Each page
says what it is, which header or module it needs, shows the code, and then
**runs that code on this machine** and shows what it printed. There are no
lessons and no exercises.

Pages exist for C and Python. The second half of each list is everything a
system-information tool such as fastfetch needs: OS, kernel, CPU, GPU, memory,
disk, uptime, packages, shell and terminal, desktop, monitors, network,
battery, locale, terminal size, colours and layout, and a complete mini
fastfetch at the end.

The C list also has one reference page for each of 21 headers (stdio.h,
stdlib.h, string.h, unistd.h, sys/statvfs.h, termios.h and so on). Page
`01-headers-index.txt` lists them with where each comes from (ISO C, POSIX or
Linux) and what VoidFetch uses it for. Searching for a header name, such as
`sys/statvfs.h`, finds its page.

## Running

```bash
cd libraries
make
./libraries            # or pick "Libraries" in the launcher, under its own tab
```

Needs `cc` (for the C pages) and `python3` (for the Python pages). Linux only:
the pages read `/proc` and `/sys`. Type a number to open a page, or a word to
search (try `memory`). The first page of each language maps every fastfetch
field to its source.

## A page is a file

Every page is one source file in `c/` or `python/`, with a header comment the
browser reads:

```c
/*
 * TITLE: Kernel, architecture and hostname (uname)
 * GROUP: System information
 * USES: #include <sys/utsname.h>
 * SUMMARY: One or two lines saying what it does.
 * NOTES:
 *   - a gotcha per line
 * SEE: man 2 uname
 * LIBS: -lm                    (C only: extra linker flags)
 * STANDARD: ISO C, POSIX or Linux (optional)
 * PROVIDES:                    (optional: the functions, macros and types)
 *   uname()    fills a struct utsname
 * TOOL:                        (optional: how VoidFetch uses it)
 *   kernel line: sysname and release
 */
```

`STANDARD`, `PROVIDES` and `TOOL` are optional. The browser shows them as
"Standard", "Provides" and "In VoidFetch". The header reference pages (groups
whose name starts with "Header reference") must have all three, and
`check-pages.py` enforces that.

Python pages use a docstring (`"""`) with the same `KEY: value` lines. A page
ending in `.txt` is documentation only: the same header, a line with `---`,
then the text. Pages are listed in file-name order under their `GROUP`. To add
one, drop a file in the folder: the browser needs no change. To add a language,
add a row to `TRACKS` in `src/main.c` and a folder of pages.

Keep the header free of the two-character sequences that end or start a C
comment, and keep Python headers free of backslashes. `check-pages.py` catches
both.

## How it was verified

```bash
python3 check-pages.py       # every page builds clean (C: -Wall -Wextra) and runs with output;
                             # each of the 21 headers has a page that includes it
python3 check-browser.py     # ./libraries opens every page; search finds each header by name
python3 check-agreement.py   # the C and the Python mini fastfetch agree on every stable field
```

The output a page shows is not copied into the file: the browser compiles and
runs the page each time, so it is always this machine's answer. Pages print a
plain "not available" for hardware that is missing (no battery on a desktop, no
display over ssh) and exit 0.

Sources used: the manual pages named in each page's `SEE:` line, `man 5 proc`,
`man 5 sysfs`, `man 5 os-release`, and the kernel documentation for
`/sys/class/drm` and `/sys/class/power_supply`. The fastfetch field list is from
the [fastfetch project](https://github.com/fastfetch-cli/fastfetch).
