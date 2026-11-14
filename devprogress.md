# Development progress

What is built, what is partly done, and what is open. Dates come from `git log`.
Course contents and module tables live in `README.md`; plans live in `ROADMAP.md`.
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
| 2026-11-14 | JavaScript course: comparisons to C++ and advanced C removed, traps run live (`demo`), wrong summaries from the learner's notes shown failing, modules name the skipped modules they rely on, module 13's export list read live; `javascript/check-course.py` walks all 14 modules in CI; the two Libraries checks fixed after the Python manual reorganisation |

## Partly done

- **Rules 8 and 9 of `docs/writing-a-course.md`**: only the JavaScript course follows them. The other 16 courses still explain through other languages in places.
- **Security course**: modules 1 and 2 exist. The roadmap names the next module.
- **GTK / raylib courses**: four and six modules. The roadmap does not state an end point for either.
- **Desktop entry on Linux**: `install.sh` was tested against a fake release only. A real run needs the next release tag.
- **Windows support**: `install.ps1` has never been run (the README says so). No Start Menu shortcut exists. Needs a Windows machine.
- **Target arcs for the older courses** (Python, C++, JavaScript and others in `ROADMAP.md`): each lists next modules, none designed to the depth of `c/ROADMAP.md`.
- **Libraries reference**: C track has 21 header pages. Python pages were reorganised on 2026-11-12. Other languages are one folder and a `TRACKS` row each.
- **Learner notes**: `docs/learner-notes.md` records about four hours of practice, stated by the learner, tracker data not checked.

## Not committed anywhere

- Git stash `gui modules 2-4` on a local clone (4 files: `gui/Makefile`, `gui/src/lessons.h`, `gui/src/main.c`, `tools/check-teaching-order.py`). The `gui` course was later split into `gtk` and `raylib`, so it may be obsolete. Not diffed.

## Not started

TypeScript, Go, Bash, SQL, Lua and Pascal courses. HolyC is deliberately empty until a curriculum exists. C-specific backlog items are tracked as GitHub issues.
