# aprender-programar

Command-line programming courses, one per language. No browser, no account,
no internet. Open a terminal, run the program, learn.

Each course is written **in the language it teaches**, so the source is itself
worked example code the student can open and read.

## Status

| Language | Modules | Where |
| --- | --- | --- |
| C | 12 — first program through linked lists | [`c/`](c/) |
| C++ | 9 — cout through classes and RAII | [`cpp/`](cpp/) |
| Python | 10 — print() through classes and exceptions | [`python/`](python/) |
| JavaScript | 10 — console.log() through classes and errors | [`javascript/`](javascript/) |
| Launcher | done — pick a language, open its course | [`launcher/`](launcher/) |

All four languages are covered. The launcher and `tools/check-teaching-order.py`
both work over any number of courses without modification, so a fifth language
is a matter of writing it, not extending anything else.

The courses are independent programs. The launcher only launches them: it
does not know anything about what happens inside one.

## Running

```bash
cd launcher
make
./launcher
```

Pick a language and its course runs in place. Quitting a course (its own
`[0] Quit`) returns to this menu — that is a subprocess exiting, not
anything the launcher had to build. A language with no course yet shows as
"coming soon" instead of being selectable.

To run a course directly, without the launcher:

```bash
cd c
make
./c-course
```

```bash
cd python/src
python3 main.py
```

```bash
cd javascript/src
node main.js
```

Python and JavaScript need no build step — nothing to compile, nothing to
link.

You need a C compiler and `make` for the C and C++ courses. On Arch:

```bash
sudo pacman -S base-devel
```

On Debian or Ubuntu:

```bash
sudo apt install build-essential
```

## What a module looks like

Every module follows the same shape:

```
PART 1, 2, 3 ...   explanation, one screen at a time
EXERCISE           short questions, answered and checked here
                   then a task to write in a real file, stating the
                   output it must produce
SUMMARY            four or five lines worth remembering
```

Every task states two things: the input to type, if any, and the output it
must produce. Together they are the whole specification. Example code is
shown afterwards if you ask for it, but a solution that produces the same
output from the same input, by different means, is correct.

An exercise never needs anything the course has not taught yet. That is
enforced by `tools/check-teaching-order.py`, not by memory.

The short questions are checked as you answer them. Getting one wrong is not
a failure and does not block anything — the point is to show the right answer
and the reason, which appears either way.

## The C course

| # | Module | The thing it exists for |
| --- | --- | --- |
| 1 | Compiling and printing | C compiles before it runs |
| 2 | Variables and types | `5 / 2` is `2`, and casting afterwards is too late |
| 3 | Reading input | why `scanf` needs `&`, and why arrays do not |
| 4 | Conditions | `=` is not `==`, and missing braces |
| 5 | Loops | the off-by-one, and that C does not check bounds |
| 6 | Arrays and strings | `\0`, `strlen` vs `sizeof`, `==` does not compare text |
| 7 | Functions | arguments are copies — this is what explains `&` |
| 8 | Pointers | `&` and `*`, `NULL`, the segfault |
| 9 | **Memory: malloc and free** | stack vs heap, and the four ways to get it wrong |
| 10 | Structs | dot vs arrow, and why `sizeof` exceeds the sum of the fields |
| 11 | **Final test: 2 programs** | a game and a "normal" program, combining modules 1-10 |
| 12 | Going deeper: memory (extra) | pointer-to-pointer, `realloc`, linked lists |

Module 9 is the longest on purpose: leaks, use-after-free, double free, and
returning the address of a local array — each with what you actually see when
it happens.

Module 11 has no new theory beyond `rand()`/`srand()`, needed for its game to
be a game. Both of its tasks — Guess the Number, and a Student Grade Manager —
are stated as output the program must produce, but unlike every other exercise
in this course, that output depends on a random number and on what you type,
so it is shown as one example run rather than an exact string to match.

Module 12 is the extra requested after the final test: closing the gap between
"malloc and free work" and trusting your own ownership rules across something
bigger than a single block — a dynamically sized 2D grid, growing an array
with `realloc`, and a linked list, each freed correctly and each shown broken
first so the fix means something.

## The C++ course

| # | Module | The thing it exists for |
| --- | --- | --- |
| 1 | Compiling and printing | `cout`/`<<`, `g++`, `.cpp` — what changed and what did not |
| 2 | Variables and types | `std::string`, `auto` — and `int / int` still truncates |
| 3 | Reading input | `cin >>` needs no `&`; the `>>` then `getline` bug |
| 4 | Conditions | `bool` is real, but `cout` still prints it as `1`/`0` |
| 5 | Loops | range-based `for`; `auto x` copies, `auto &x` does not |
| 6 | `std::string` | `==` finally compares content; `npos`, not `-1` |
| 7 | Functions and references | `&` removes the need for `&` at the call site |
| 8 | `std::vector` | grows itself; `[]` is still unchecked, `at()` still throws |
| 9 | **Classes and RAII** | `private`/`public`; a destructor that fires on its own |

This course assumes nothing about the C course, but is written to be read
right after it: every module is framed as *what changed, and what did not*.
Integer division survives unchanged into module 2. `if (x = 5)` still
compiles and still only warns, in module 4. `new`/`delete` are `malloc`/`free`
under a different name in module 9, verified by leaking 20 bytes on purpose
and quoting the real LeakSanitizer report — not run live, since it would leak
inside the same process this README's own verification claims is clean.

Module 8 makes a point of the gap between *documented* and *observed*:
`v[100]` on a small vector is undefined behaviour by the C++ standard — no
guarantee at all — and on this specific compiler it happens to abort with an
assertion. Both facts are stated, and kept separate.

## The Python course

| # | Module | The thing it exists for |
| --- | --- | --- |
| 1 | Running Python and `print()` | no compile step; `sep=`/`end=` |
| 2 | Variables and types | rebinding, not redeclaring; `/` vs `//`, floor rounds toward `-inf` |
| 3 | Reading input | `input()` always returns `str`; the forgot-to-convert `TypeError` |
| 4 | Conditions | indentation *is* the block; truthiness |
| 5 | Loops | `range()`'s end is exclusive; `enumerate()` |
| 6 | Lists | **`b = a` does not copy** |
| 7 | Functions and default arguments | **the mutable default argument trap** |
| 8 | Dictionaries | `[]` raises, `.get()` doesn't; insertion order since 3.7 |
| 9 | Classes | `self` is explicit; no enforced privacy; no destructor timing guarantee |
| 10 | Exceptions | `try`/`except`/`finally`; catch a type, not everything |

No memory-management thread here — Python collects its own garbage, so what
module 9 in the C course spent five sections on does not exist as a topic.
What replaces it, module for module, is a different class of bug: `b = a`
aliasing a list instead of copying it (module 6), and a default argument list
built once, at definition time, then silently shared by every call that
relies on it (module 7) — confirmed by running `add_bad("a")` then
`add_bad("b")` and watching the second call's result still contain `"a"`.

Module 9 states plainly what Python does not promise: unlike a C++ destructor
firing at a predictable, specific point, CPython's garbage collection timing
is an implementation detail, not a language guarantee. For anything that must
close at a known moment, the module points at `with`/context managers instead
of pretending Python has RAII.

## The JavaScript course

| # | Module | The thing it exists for |
| --- | --- | --- |
| 1 | Running Node and `console.log()` | no compile step; template literals |
| 2 | Variables and types | always `===`, never `==`; `NaN !== NaN` |
| 3 | Reading input | **reading a line is asynchronous**; `async`/`await` all the way up |
| 4 | Conditions | `[]` and `{}` are **truthy** — the opposite of Python |
| 5 | Loops | `for...of` gives values, `for...in` gives keys as strings |
| 6 | Arrays | `const` doesn't stop `b = a` aliasing; default `.sort()` sorts as text |
| 7 | Functions and `this` | a regular function's `this` depends on the call site; an arrow's does not |
| 8 | Objects | `==` and `===` are *both* wrong for comparing objects — both compare by reference |
| 9 | Classes | `#field` is **real** private, enforced by a `SyntaxError` |
| 10 | Errors | `try`/`catch`/`finally`; `extends Error` for your own catchable type |

No memory-management thread here either — same as Python, garbage collected.
Module 3 exists because of a genuine difference from every other course in
this repo: Node has no built-in blocking read the way `scanf`, `input()` and
even `cin` are. Reading a line is asynchronous, so `async`/`await` is not
optional syntax to learn eventually — it is required from the first `input()`
equivalent onward, and it propagates upward through every function that
calls one.

That module also states a real, verified gotcha about *testing* a script
non-interactively rather than about the language itself: piping input with
`printf "a\nb\n" | node script.js` closes stdin the instant it finishes
writing, and a second pending `readline`-promises `question()` call after
that point silently never resolves — no error, just a hang. A real terminal
never closes stdin mid-session, so this never affects the course; it does
affect testing a script by piping to it, which is why this course's own
`ui.js` drives `readline`'s async iterator by hand instead of using
`question()`, verified against both a closed pipe and realistic typed timing.

Module 4 is the one place this course actively disagrees with the Python
course sitting next to it: `Boolean([])` and `Boolean({})` are both `true`.
`if (arr)` is never a length check in JavaScript, unlike Python where an
empty list is falsy.

Module 9's `#field` syntax is worth reading against the other three courses'
answers to the same question. C++'s `private` is enforced by the compiler.
Python's leading underscore is a convention enforced by nobody — confirmed in
that course by reading `a._grade` from outside the class and watching it
just work. JavaScript's `#field` sits with C++: trying to reference `s.#grade`
from outside the class is a `SyntaxError`, caught before the program runs at
all, verified on Node 26.

## Design rules

**Nothing is claimed without being shown.** Where an example can run, it runs.
Module 2 prints `sizeof` as computed by *your* compiler; module 6 prints the
real addresses of an array going up 4 bytes at a time; module 8 changes a
variable through a pointer; module 9 calls `malloc` for real and shows the
address it got back. Module 3 reads your actual keyboard.

**Memory bugs are explained, never demonstrated.** A double free or a
use-after-free is undefined behaviour: running one would either take the
course down or — worse — appear to work and teach the wrong lesson.

**Warnings are teaching material.** Everything builds with `-Wall -Wextra`
and the course itself has none.

More detail in [`docs/writing-a-course.md`](docs/writing-a-course.md).

## Linux and Windows

The C course and the launcher build and run on both. The only
platform-specific code is the three lines that clear the screen (in
`c/src/ui.c` and duplicated in `launcher/src/main.c` — kept separate rather
than shared, since the launcher and the courses are meant to stay
independent programs) and, in the launcher, which relative path and file
extension a course binary uses.

Tested on Linux. The Windows path is written but has not been run yet.

## Verification

The C and C++ courses compile with `-Wall -Wextra` without a single warning,
and run clean under AddressSanitizer and UndefinedBehaviorSanitizer — no
leaks, no undefined behaviour. A course that teaches `free` (or `delete`)
cannot leak.

```bash
cd c
cc -std=c11 -Wall -Wextra -g -fsanitize=address,undefined -o /tmp/course src/*.c
/tmp/course
```

```bash
cd cpp
g++ -std=c++20 -Wall -Wextra -g -fsanitize=address,undefined -o /tmp/course src/*.cpp
/tmp/course
```

Every solution shown in a challenge, in any course, has been run and checked
against the output its task promises. Python's and JavaScript's own solutions
are included, verified by intercepting `input()` (Python) or the `ui.js`
prompt functions (JavaScript) and driving every module through directly
rather than by compiling anything.

`python3 tools/check-teaching-order.py` covers all four courses in one run.

## Layout

```
launcher/
├── Makefile
└── src/
    └── main.c              pick a language, system() its binary, loop
c/
├── Makefile
└── src/
    ├── main.c              menu: a table of modules and a loop
    ├── ui.h  ui.c          screen, input, questions, challenges
    ├── lessons.h           one prototype per module
    ├── lessons_basics.c    modules 1-5
    ├── lessons_memory.c    modules 6-10
    └── lessons_advanced.c  modules 11-12
cpp/
├── Makefile
└── src/
    ├── main.cpp             menu: a vector of modules and a loop
    ├── ui.h  ui.cpp         screen, input, questions, challenges
    ├── lessons.h            one prototype per module
    ├── lessons_basics.cpp   modules 1-5
    └── lessons_modern.cpp   modules 6-9
python/
└── src/
    ├── main.py              menu: a list of modules and a loop
    ├── ui.py                screen, input, questions, challenges
    ├── lessons_basics.py    modules 1-5
    └── lessons_more.py      modules 6-10
javascript/
└── src/
    ├── main.js              menu: an array of modules and an async loop
    ├── ui.js                screen, input, questions, challenges
    ├── lessons_basics.js    modules 1-5
    └── lessons_more.js      modules 6-10
tools/
└── check-teaching-order.py  fails if an exercise needs something not yet
                              taught, in any course
docs/
└── writing-a-course.md     how to add a module or a new language
```
