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

### Adding a lesson to an existing course

A lesson is one data file in the course's `lessons/` folder: screens of
text, examples, multiple-choice questions and one exercise, with a
Portuguese title and summary. The format and the rules lessons follow (from
zero, no other languages, at most one example per screen, every example run)
are in `docs/writing-a-course.md`. Two things a review will check:

- The exercise needs nothing a later lesson teaches.
- `node learn/check.js <course>` passes: every example runs, every solution
  passes the checker, every starter file runs.

### Adding a new course

Discuss it in an issue first. A course is a folder with `learn.js`,
`course.js` (how its language builds and runs code and what its errors mean)
and `lessons/`, plus one `COURSE(...)` line in `launcher/src/main.c` and its
name in `COURSES` in `.github/workflows/release.yml`. It runs on Node and on
that language's own toolchain, with no packages to install from npm or pip.
Where the subject needs something the terminal cannot show (a board, a
browser, a window), follow the Arduino, web and GTK courses: a stand-in
whose `sim` parts are marked PC-only, or a build-only course.

### Fixing a bug in the launcher or the shared tooling

Normal software contribution rules apply: explain the bug, show how to
reproduce it, fix it, and verify the fix with the existing verification
steps for whichever course(s) it touches.

## Verifying before you open a pull request

For any course you changed:

```bash
node learn/check.js <course>
python3 learn/check-course.py <course>
```

The first runs every example, question, solution and starter; the second
plays every lesson from start to finish. Both must pass. They need the
course's own toolchain installed (a compiler, an interpreter, `dotnet`).

## Code style

- No comments explaining *what* code does -- the code should already say
  that. A comment earns its place by explaining *why*: a non-obvious
  constraint, the reason a claim is verified rather than assumed, why a
  simpler-looking alternative was not used.
- No unrequested abstractions. A lesson is data; if something can be a
  sentence and one example, that is what it should be.
- Match the existing shape of the file you are editing before introducing
  a new one.

## Reporting a problem

Open an issue. If it's a security concern specifically (not a bug in a
teaching example -- an actual vulnerability in the launcher or tooling),
see [`SECURITY.md`](SECURITY.md) instead.

## License

By contributing, you agree that your contribution is licensed under this
project's [MIT License](LICENSE).
