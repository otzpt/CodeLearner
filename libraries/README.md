# Libraries

A reference manual for the C and Python libraries, laid out like man pages.
One page per header, function family, module or system file: NAME, SYNOPSIS,
DESCRIPTION, STANDARDS, NOTES, APPLICATION USAGE, an EXAMPLE, and SEE ALSO.
The example is compiled and **run on this machine** each time the page is
opened, so the output shown is what this machine printed. There are no lessons
and no exercises.

The C manual is in six parts: the standard C library, POSIX, Linux, files
(`/proc`, `/sys` and `os-release`), terminal control sequences, and
third-party libraries (raylib). It has a reference page for each of 21
headers (stdio.h, stdlib.h, string.h, unistd.h, sys/statvfs.h, termios.h and
so on); searching for a header name, such as `sys/statvfs.h`, finds its page.
The Python manual has built-ins, the standard library, files, and terminal
control sequences. The first page of each (`00-index.txt`) lists the parts and
where each C header comes from (ISO C, POSIX or Linux).

## Running

```bash
cd libraries
make
./libraries            # or pick "Libraries" in the launcher, under its own tab
```

Needs `cc` (for the C pages) and `python3` (for the Python pages). The raylib
page also needs raylib (Arch: `pacman -S raylib`); without it that one page
does not compile. Linux only: some pages read `/proc` and `/sys`. Type a number
to open a page, or a word to search every page (try `memory`).

## A page is a file

Every page is one source file in `c/` or `python/`, with a header comment the
browser reads. Each key is shown under a man page section:

```c
/*
 * TITLE: sys/utsname.h: kernel, architecture and hostname (uname)   NAME
 * GROUP: POSIX                                                       (menu)
 * USES: #include <sys/utsname.h>                                     SYNOPSIS
 * PROVIDES:                                                          SYNOPSIS
 *   int uname(struct utsname *buf)    fills buf; -1 on error
 * LIBS: -lm                     (C only: extra linker flags)         SYNOPSIS
 * SUMMARY: One or two lines saying what it does.                     DESCRIPTION
 * STANDARD: POSIX.                                                   STANDARDS
 * NOTES:                                                             NOTES
 *   - a caveat per line
 * USEDFOR:                                                           APPLICATION USAGE
 *   - report the kernel version
 * SEE: man 2 uname                                                   SEE ALSO
 */
```

`TITLE`, `GROUP` and `SUMMARY` are required. The title is "name: what it is
for"; the page frame shows the name alone, as a man page header does. A C page
titled for one of the 21 documented headers must also have `STANDARD`,
`PROVIDES`, `USEDFOR` and `SEE`, and `check-pages.py` enforces that.

Python pages use a docstring (`"""`) with the same `KEY: value` lines. A page
ending in `.txt` is documentation only: the same header, a line with `---`,
then the text. Pages are listed in file-name order under their `GROUP`, and the
file numbers keep each part together (C: 10 standard C, 30 POSIX, 50 Linux,
60 files, 70 terminal, 90 third-party). To add a page, drop a file in the
folder: the browser needs no change. To add a language, add a row to `TRACKS`
in `src/main.c` and a folder of pages.

Keep the header free of the two-character sequences that end or start a C
comment, and keep Python headers free of backslashes. `check-pages.py` catches
both.

## How it was verified

```bash
python3 check-pages.py       # every page builds clean (C: -Wall -Wextra) and runs with output;
                             # a page whose library is not installed is skipped and counted;
                             # each of the 21 headers has a page that includes it
python3 check-browser.py     # ./libraries opens every page; search finds each header by name
```

The output a page shows is not copied into the file: the browser compiles and
runs the page each time, so it is always this machine's answer. Pages print a
plain "not available" for hardware that is missing (no battery on a desktop, no
display over ssh) and exit 0.

Sources used: the manual pages named in each page's `SEE:` line, `man 5 proc`,
`man 5 sysfs`, `man 5 os-release`, `man 4 console_codes`, the kernel
documentation for `/sys/class/drm` and `/sys/class/power_supply`, and the
Python standard library documentation.
