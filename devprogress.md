# Development progress

What is built, what is partly done, and what is open. Dates come from `git log`.
How courses are built is in `docs/writing-a-course.md`; plans live in `ROADMAP.md`.
This file is a status log, not a plan. Last updated 2026-11-14.

## Done

| Date | Item |
|------|------|
| 2026-08-07 | C course (modules 1-12), Python, C++, JavaScript courses, universal launcher, exercises in every module, project docs |
| 2026-08-08 | Java course, release pipeline for the launcher, project-wide `ROADMAP.md`, GTK as its own course |
| 2026-08-09 | Assembly course (x86-64) as its own course |
| 2026-08-15 | C# course, release artifacts (standalone binary, Linux zip, AppImage), Arch package version fix |
| 2026-08-26 | Git course, written in Bash against real repos |
| 2026-09-24 | C modules 12-19, stricter teaching-order checker, one-command installers (`install.sh`, `install.ps1`), Windows build fix |
| 2026-10-08 | Arduino, MicroPython and Rust courses, inline assembly and NASM modules in C and C++, AVR and Cortex-M0+ code is executed by the checks |
| 2026-10-10 | Libraries tab (reference pages), "Check for updates" in the launcher, desktop entry |
| 2026-10-17 | Assembly course rewritten in NASM Intel syntax, 16 modules up to a kernel; `vm.py` talks to QEMU over a UNIX socket |
| 2026-10-23 | Security course module 1, raylib reference page, man-page style libraries manual, CI action upgrades |
| 2026-11-09 | Bug sweep (lesson claims, EOF crashes, installer icon cache), Security module 2, launcher groups courses by topic, HTML, CSS and Web JavaScript courses, PT-PT mode, libraries as a normal reference manual |
| 2026-11-10 | GTK course (4 modules), raylib + raygui course (6 modules), split into two courses, module 1 rewritten as a beginner walkthrough |
| 2026-11-12 | Python manual reorganised by topic with a page generator, notes on how a learner worked through the JavaScript course, learner JavaScript practice files in `docs/learner-examples/` |
| 2026-11-14 | Every course rewritten "from zero" on one shared lesson engine (`learn/`, Node): written for a first-time programmer, no other language mentioned, at most one example per screen, every example run live, plain-word error explanations, exercises written in real files and checked by running them, saved progress, PT-PT menus and summaries. 17 courses, 153 lessons: C 14, Python 14, JavaScript 13, C++ 12, Java 12, Rust 12, C# 11, Assembly 9, Arduino 8, Git 8, HTML 7, MicroPython 7, Security 7, CSS 6, Web JavaScript 5, GTK 4, raylib 4. `learn/web.js` stands in for a browser; Arduino and MicroPython run on PC stand-ins for the board; GTK and raylib are build-only. CI checks every course with `learn/check.js` and `learn/check-course.py`. The launcher starts every course with `node`. The Libraries checks fixed after the Python manual reorganisation |

## Partly done

- **Course coverage after the rewrite**: the new courses are shorter than the old ones. Not yet covered again: JavaScript promises, fetch, modules, files and JSON; Python and Java beyond the basics of files; C's function pointers, bit operations, variadic functions and inline assembly; the old Assembly course's OS modules as lessons (the files and `check-os.py` are kept in `assembly/`); Rust traits, lifetimes and collections; the old Security course's HTTP modules.
- **Windows**: the Windows CI leg checks only C, C++, MicroPython, Python, JavaScript, HTML, CSS and Web JavaScript. Assembly, Arduino and Git are Linux-only. `install.ps1` has never been run, and no Start Menu shortcut exists.
- **GTK and raylib**: build-only. The examples and solutions are compiled, but the windows they open are not run by any check. CI's ubuntu-22.04 runner has GTK 4.6 and no raylib, so both are skipped there and only checked on a machine that has them.
- **Assembly OS track**: `assembly/check-os.py` passes 31 of 33 checks on GCC 16.2 and QEMU 11.1 (2026-11-14); module 15's volatile listing and module 16's interrupt kernel fail, on `main` too, so the toolchain changed since 2026-10-17. Not investigated.
- **Desktop entry on Linux**: `install.sh` was tested against a fake release only. A real run needs the next release tag.
- **Libraries reference**: C track has 21 header pages. Python pages were reorganised on 2026-11-12. Other languages are one folder and a `TRACKS` row each.
- **Learner notes**: `docs/learner-notes.md` records about four hours of practice, stated by the learner, tracker data not checked.

## Not committed anywhere

- Git stash `gui modules 2-4` on a local clone (4 files under `gui/` and `tools/check-teaching-order.py`). Both the old `gui` course and that checker are gone since the 2026-11-14 rewrite, so it is obsolete.

## Not started

TypeScript, Go, Bash, SQL, Lua and Pascal courses. HolyC is deliberately empty until a curriculum exists. C-specific backlog items are tracked as GitHub issues.
