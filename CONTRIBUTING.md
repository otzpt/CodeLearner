# Contributing to CodeLearner

Thanks for considering a contribution. This document covers how the project
is organized, what a contribution needs to include, and how to verify one
before opening a pull request.

Read [`docs/writing-a-course.md`](docs/writing-a-course.md) first. It covers
the design rules every course follows and is the fuller reference; this file
covers the process around a contribution.

## No commit lands without review

No change of any kind -- a typo fix, a new module, a dependency bump, a CI
change -- is committed directly to `main`. Every change goes through a pull
request and gets reviewed by the project lead before it merges. This
applies regardless of how small the change looks or how confident the
verification steps above made it feel.

## Before you start

Open an issue for anything beyond a small fix -- a typo, a wrong claim in a
lesson, a missing test. For a new module or a new language, open an issue
first and describe what you are planning. It is much cheaper to agree on
scope before code exists than to rework a finished pull request.

## The one rule that matters most

**Every claim in a lesson must be something you actually ran.** Not
something you are confident about, not something you read once -- run it,
on the toolchain version you have, and write down what actually happened.
This project has a documented history of claims that were wrong in exactly
the way "everyone knows this" claims are usually wrong: `-lm` needing to
come after source files (false, verified on GCC 16.1), a syntax form being
"invalid in Python 3" (false as of 3.14, per PEP 758), `int main()` versus
`int main(void)` behaving identically in C (false, verified with
`-std=c11` versus `-std=c23`). All three were caught by running the code,
not by reading about it.

If you cannot run something -- a Windows-only behavior on a Linux
machine, for instance -- say so explicitly in the lesson text or the PR
description rather than stating it as fact.

## Kinds of contributions

### Fixing a lesson

The fastest path to merged. Point at the exact claim, show what you ran and
what it actually produced, and fix the text. No other process required.

### Adding a module to an existing course

Follow the shape every module already uses: `PART 1/2/3` explanation with
live output where possible, then `EXERCISE` with a few short questions and
one `challenge()` stating both the input and the exact output expected.
Full details, including the helper functions available in each course's
`ui` module, are in `docs/writing-a-course.md`.

Two things a review will check specifically:

- Every construct a `challenge()`'s solution uses must already have been
  shown somewhere earlier in the course -- run
  `python3 tools/check-teaching-order.py` before opening the PR, and add
  the new construct to that language's table in
  `tools/check-teaching-order.py` if it introduces something new.
- Every array or vector passed to `challenge()` with an explicit count (the
  C course only -- C++, Python and JavaScript pass a vector/list/array with
  no separate count to get wrong) must have that count match the real
  number of elements. This has been the source of every buffer-overflow-
  class bug found in this project so far.

### Adding a new language

A larger undertaking, and one to discuss in an issue first. The
requirements:

- Written in the language it teaches. The course's own source is meant to
  be readable, worked example code by the time a student reaches its later
  modules.
- A single, self-contained CLI program with no runtime dependency beyond
  that language's own toolchain. No npm packages, no pip packages, nothing
  to install beyond a compiler or interpreter.
- The same module shape as the other courses: explanation, then an
  exercise with input-stated, output-stated challenges.
- Registered in `launcher/src/main.c`'s `LANGUAGES` table (a two-line
  change) and in `tools/check-teaching-order.py`'s `COURSES` table (that
  language's own construct-to-module map).

### Adding a course for something that is not a language

`gui/` (GTK) is the precedent: a real C library, not a language, but
registered exactly like one -- its own directory, its own entry in the
launcher next to C/C++/Python/JavaScript/Java, not folded into an existing
course's module list. Two things make this different from "adding a new
language" above, not exceptions to the no-dependency rule but a distinction
worth being precise about:

- The lesson *program itself* still has zero runtime dependency beyond its
  host language's toolchain -- `gui-course` is plain C, no `#include
  <gtk/gtk.h>` anywhere in it, same as every language course.
- What the lesson *teaches* is allowed to need something extra installed
  (GTK4's development files here), since that is the whole subject matter,
  not incidental. State this plainly in the README and the course's own
  source header, the way `gui/src/lessons_gtk.c` does, so nobody installs
  the extra dependency thinking it is needed just to browse the course.
- Every claim about that external code is still held to the same "you
  actually ran it" standard as everything else in this file -- installing
  GTK4 to verify a module before writing it down is not optional just
  because the course binary itself does not need it.

### Fixing a bug in the launcher or the shared tooling

Normal software contribution rules apply: explain the bug, show how to
reproduce it, fix it, and verify the fix with the existing verification
steps for whichever course(s) it touches.

## Verifying before you open a pull request

For C:

```bash
cd c
make clean && make                     # must build with zero warnings
cc -std=c11 -Wall -Wextra -g -fsanitize=address,undefined -o /tmp/course src/*.c
/tmp/course                            # walk the module(s) you changed
```

For C++, the same with `g++ -std=c++20` in place of `cc -std=c11`.

For Python and JavaScript, there is no compiler to catch a mistake, so the
check is running every module and confirming nothing raises or throws. See
the "Verifying before committing" section of `docs/writing-a-course.md` for
the pattern this project uses (mocking the input function, capturing
output, calling each lesson directly) -- a real terminal or a FIFO with
delays between lines is required for any interactive testing; a single
pre-filled pipe does not behave like a real user typing and produces false
failures.

For Java:

```bash
cd java
javac -Xlint:all -Werror src/*.java   # must compile with zero warnings
./run                                  # walk the module(s) you changed
```

Java's own `Scanner` reads directly from `System.in`, the same as C's
`scanf`/C++'s `cin` -- the same FIFO-with-delays or real-terminal rule
above applies, not a pre-filled pipe.

For everything:

```bash
python3 tools/check-teaching-order.py
```

This must report every exercise clean before a PR is opened.

## Code style

- No comments explaining *what* code does -- the code should already say
  that. A comment earns its place by explaining *why*: a non-obvious
  constraint, the reason a claim is verified rather than assumed, why a
  simpler-looking alternative was not used.
- No unrequested abstractions. If a lesson can be one function with a
  handful of `printf`/`console.log`/`print` calls, that is what it should
  be.
- Match the existing shape of the file you are editing before introducing
  a new one.

## Reporting a problem

Open an issue. If it's a security concern specifically (not a bug in a
teaching example -- an actual vulnerability in the launcher or tooling),
see [`SECURITY.md`](SECURITY.md) instead.

## License

By contributing, you agree that your contribution is licensed under this
project's [MIT License](LICENSE).
