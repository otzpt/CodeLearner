# CodeLearner

Command-line programming courses, one per language. No browser, no account,
no internet. Open a terminal, run the program, learn.

Each course is written **in the language it teaches**, so the source is itself
worked example code the student can open and read.

## Status

| Language | Modules | Where |
| --- | --- | --- |
| C | 13 — first program through stacks and queues | [`c/`](c/) |
| C++ | 9 — cout through classes and RAII | [`cpp/`](cpp/) |
| Python | 10 — print() through classes and exceptions | [`python/`](python/) |
| JavaScript | 10 — console.log() through classes and errors | [`javascript/`](javascript/) |
| Java | 10 — System.out through checked exceptions | [`java/`](java/) |
| GUI (GTK) | 1 — your first window | [`gui/`](gui/) |
| Assembly | 1 — registers, syscalls, your first program | [`assembly/`](assembly/) |
| Launcher | done — pick a language, open its course | [`launcher/`](launcher/) |

Seven courses are covered. GUI and Assembly are not languages in the same
sense C/C++/Python/JavaScript/Java are, but both get the same treatment as
one: their own directory, their own binary, their own entry in the launcher,
rather than being folded into `c/`'s module list. Assembly goes further
than GUI does, though -- its own delivery program (`assembly/src/ui.s`,
`main.s`) is genuinely written in x86-64 assembly, the same "written in the
language it teaches" rule every other course follows literally, just with
no standard library underneath it (raw `read`/`write`/`exit` syscalls, no
libc at all). GUI's own delivery stays C, since there is no separate "GTK
language" to write a menu in. The launcher and `tools/check-teaching-order.py`
both work over any number of courses without modification, so a new entry
is a matter of writing it, not extending anything else.
[`ROADMAP.md`](ROADMAP.md) has the target arc for the five languages and
the ones not started yet.

The courses are independent programs. The launcher only launches them: it
does not know anything about what happens inside one.

## Running

Prebuilt releases (Windows `.zip`, Linux `.tar.gz`/`.deb`/Arch
`.pkg.tar.zst`) are on the [Releases page](https://github.com/otzpt/CodeLearner/releases)
— extract and run `codelearner.bat`/`codelearner.sh`, or `codelearner` on
your PATH after installing the `.deb`/Arch package. Only the C and C++
courses are inside; Python, JavaScript and Java need that language's own
toolchain installed to run (see below). Want just one course, standalone,
with no launcher and no other four? See
[`docs/building-a-single-course.md`](docs/building-a-single-course.md).

To build it yourself instead:

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

```bash
cd java
./run
```

Python and JavaScript need no build step — nothing to compile, nothing to
link. Java's `./run` compiles and runs in one command too (`java Main.java`,
no separate `javac` step, no `.class` files left behind) — see the Java
course's own module 1 for why that is not the two-step model most people
expect from a compiled language.

You need a C compiler and `make` for the C and C++ courses. On Arch:

```bash
sudo pacman -S base-devel
```

On Debian or Ubuntu:

```bash
sudo apt install build-essential
```

You need a JDK for the Java course. On Arch:

```bash
sudo pacman -S jdk-openjdk
```

On Debian or Ubuntu:

```bash
sudo apt install default-jdk
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
| 13 | Stacks and queues | array-based, not another linked list — capacity checks that actually run |

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

Module 13 is the first data-structures module: a stack and a queue, both
array-based rather than another linked list, so the same idea — "a
collection that grows" — is shown with a different tradeoff (fixed
capacity instead of one `malloc` per item). "Stack overflow" and a naive
queue reporting full with free slots behind `front` are both real, checked
conditions that actually run, not just described.

Modules 1-13 are stages 1-4 (started) of a longer path — more data
structures and algorithms, real projects, POSIX, debugging tools, assembly,
reading other people's C. [`c/ROADMAP.md`](c/ROADMAP.md) maps the rest of it
and where each stage picks up.

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

Module 9's `#field` syntax is worth reading against the other courses'
answers to the same question. C++'s `private` and Java's `private` are both
enforced by the compiler. Python's leading underscore is a convention
enforced by nobody — confirmed in that course by reading `a._grade` from
outside the class and watching it just work. JavaScript's `#field` sits with
C++ and Java: trying to reference `s.#grade` from outside the class is a
`SyntaxError`, caught before the program runs at all, verified on Node 26.

## The Java course

| # | Module | The thing it exists for |
| --- | --- | --- |
| 1 | Compiling and printing | `javac`/`java`, two steps not one; a public class's name must match its file |
| 2 | Variables and types | primitives vs objects; **boxed `Integer` caches -128..127, then `==` lies** |
| 3 | Reading input | `Scanner`; `nextInt()` then `nextLine()` — same bug shape as C++'s `cin >>`/`getline` |
| 4 | Conditions | a condition must be `boolean` — `if (x = 5)` is a **compile error**, not a C-style warning |
| 5 | Loops | `for`/`while`; the enhanced `for` gives values, no index |
| 6 | Arrays and Strings | `arr.length` is a field, `str.length()` is a method; `==` vs `.equals()` |
| 7 | Methods and overloading | no default arguments — overloading fills the gap, resolved at compile time |
| 8 | `ArrayList` and boxing | grows, unlike an array; `ArrayList<int>` does not compile — needs `Integer` |
| 9 | Classes, interfaces, encapsulation | `private` is **enforced**, not a convention; an unimplemented interface method won't compile |
| 10 | Exceptions: checked vs unchecked | an unhandled **checked** exception is a compile error |

No memory-management thread here either — garbage collected, same as Python
and JavaScript. What replaces it is Java's two-tier type system: eight
primitive types that hold a raw value directly, and everything else — String,
Integer, any class — held by reference. Module 2's boxed-`Integer` trap only
exists because of that split: `Integer a = 127, b = 127; a == b` is `true`
(both happen to be the same cached object), but the identical code with `200`
instead of `127` is `false` — two separate objects with equal values, `==`
comparing identity, not value, the same rule as C's `==` on a `char*`. Both
sides of that claim were run against the installed JDK before being written
down, not assumed from "Java caches small integers."

Module 4 is the sharpest cross-course callback in this collection: `if (x = 5)`
— typing `=` where `==` was meant — is the exact bug C and C++'s own module 4
warn about, and in both of those courses it compiles, with at most a warning.
In Java it is a hard compile error (`incompatible types: int cannot be
converted to boolean`), because a condition must be `boolean` and an
assignment evaluates to `int`. Same typo, opposite failure mode — caught
before the program can run at all rather than caught only if you are reading
compiler warnings carefully.

Module 10 is the other half of that same idea, working in reverse. A checked
exception (`throws IOException`) left uncaught anywhere on its call path is
also a compile error — `unreported exception IOException; must be caught or
declared to be thrown` — enforced no matter how deeply it is buried, unlike
every exception in C++, Python, or JavaScript, all of which let any exception
fly straight past every caller with zero compile-time warning. `RuntimeException`
and its subclasses are unchecked, behaving exactly like exceptions in the
other three courses — checked exceptions are the one place this course's
compiler is stricter than the others, not the whole story.

Module 9 lines up directly against Python's module 9 and C++'s module 9 on
the same question: is `private` real? Python's leading underscore is a
naming convention nobody enforces — reading or writing `a._grade` from
outside the class just works, shown running in that course. Java's `private`
is a compiler-enforced access modifier: `s.grade = 20` on a private field
does not compile (`grade has private access in Student`), and neither does a
class that implements an interface without providing every method it
declares. For deterministic cleanup, Java's answer is try-with-resources —
the same opt-in idea as Python's `with`, not the automatic-for-every-object
guarantee C++'s RAII gives.

The launcher runs this course via `java/run` (`java/run.bat` on Windows),
not a path straight to a `.java` file — verified against the installed JDK
that a `#!` shebang line on a `.java` file does not compile here, despite
some descriptions of JEP 330 suggesting it should.

## The GUI course

| # | Module | The thing it exists for |
| --- | --- | --- |
| 1 | GTK: your first window | `pkg-config` in the compile line; `GtkApplication`/`activate`, not `gtk_init`; `G_CALLBACK` hides a signature mismatch from the compiler |

Not a language — GTK is a C library — but it gets the same treatment as
one: its own directory, its own zero-dependency binary teaching it, its own
entry in the launcher next to C, C++, Python, JavaScript and Java, rather
than being folded into `c/`'s module list. That separation is real, not
cosmetic: `gui-course` itself (the menu, this module's text) is plain C
with no dependency beyond a compiler, same as every other course, but the
GTK code it teaches needs GTK4's development files installed to actually
compile and run — a dependency none of the five language courses have.
`gui/src/lessons_gtk.c`'s own header comment has the exact verification:
every example was compiled and run for real (`gcc -Wall -Wextra
$(pkg-config --cflags gtk4) ... $(pkg-config --libs gtk4)`), and running it
opened a real window, confirmed via the window manager's own client list.

Module 1 leads with what GTK actually is — a C library, called the same
way any C library is called, not a black box behind a friendlier
language — before any code, since that framing is the difference between
"here is unfamiliar syntax" and "here is C you already know, aimed at a
new library." Its exercise has no fixed transcript to match the way every
other course's does: the result is a window, not text, so the module
describes the real observed behaviour (a button that relabels itself on
click) instead of diffing against captured stdout — the same accommodation
module 11 in the C course already makes for output that depends on a
random number.

## The Assembly course

| # | Module | The thing it exists for |
| --- | --- | --- |
| 1 | Registers, syscalls, your first program | AT&T syntax reads backwards from most tutorials; a syscall is a number in `rax` plus arguments in the same registers a call would use |

Linux/x86-64 only — see [`ROADMAP.md`](ROADMAP.md)'s note on why there is
no Windows path to translate this one to. This is the one course where
"written in the language it teaches" is taken completely literally: unlike
the GUI course, there genuinely is a language here to write a menu in, so
`assembly/src/ui.s` and `main.s` are real x86-64 assembly, not C that talks
about assembly. No libc, no C runtime — `title`/`heading`/`question`/
`ask_yes`/`read_line` are all built from nothing but the `read`, `write`
and `exit` syscalls, called directly.

Every function that needs an argument to survive more than one call saves
it into a real stack frame (`push %rbp; mov %rsp, %rbp; sub $N, %rsp`)
rather than juggling `push`/`pop` pairs around each call — the same shape
`gcc -O0`'s own output uses, which module 1 shows side by side with
`ui.s`'s hand-written version of the same thing. Two bugs caught during
verification, not by inspection: an early draft of `question()` tried to
print an argument through a register that had already been overwritten,
and a piped `printf | ./asm-course` test appeared to break `ask_yes()`
because a pipe (unlike a real terminal) can hand `read()` more than one
line in a single call — both are why every module in this project is
walked with a real FIFO and delays between lines before being trusted, not
just assembled and eyeballed.

The exercise's expected answers are one word each (`second`, `rsi`,
`number`) on purpose: grading is an exact string comparison (`ui.s`'s
`trim_and_lower` plus `streq` -- case-insensitive and whitespace-trimmed,
nothing fuzzier), the same "keep it short and unambiguous" rule
`docs/writing-a-course.md` already states for every other course's
`question()`.

## Design rules

**Nothing is claimed without being shown.** Where an example can run, it runs.
Module 2 prints `sizeof` as computed by *your* compiler; module 6 prints the
real addresses of an array going up 4 bytes at a time; module 8 changes a
variable through a pointer; module 9 calls `malloc` for real and shows the
address it got back. Module 3 reads your actual keyboard.

**Memory bugs are explained, never demonstrated.** A double free or a
use-after-free is undefined behaviour: running one would either take the
course down or — worse — appear to work and teach the wrong lesson.

**Warnings are teaching material.** C and C++ build with `-Wall -Wextra`,
Java with `-Xlint:all -Werror`, and the courses themselves have none.

More detail in [`docs/writing-a-course.md`](docs/writing-a-course.md).

## Linux and Windows

The C course and the launcher build and run on both. The only
platform-specific code is the three lines that clear the screen (in
`c/src/ui.c` and duplicated in `launcher/src/main.c` — kept separate rather
than shared, since the launcher and the courses are meant to stay
independent programs) and, in the launcher, which relative path and file
extension a course binary uses. Java follows the same split: `java/run` on
Linux, `java/run.bat` on Windows, both doing the same `cd src && java
Main.java`.

Tested on Linux. The Windows path compiles and is exercised by CI's release
build (see `.github/workflows/release.yml`), including a smoke test that
launches `launcher.exe` and confirms it exits cleanly -- but that only
proves it starts and stops, not that every menu path behaves correctly
end to end, which still needs a human on real Windows. One real bug was
already caught by scrutinizing this path rather than trusting the comment
that called it untested: Windows' default handler for `.js` is Windows
Script Host, not Node -- unlike Python's installer, Node's does not set
itself as the default for a bare path, only as an "Open With" option (see
`launcher/src/main.c`'s comment on `run_command` for the verified detail).
The JavaScript and Python entries now invoke `python`/`node` explicitly
rather than relying on file association for either. `java/run.bat` still
has had no equivalent scrutiny beyond compiling.

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

Java compiles clean with `-Xlint:all -Werror` too, and every module was run
through a real pty end to end (all 10 modules visited, no crash, no hang).

```bash
cd java
javac -Xlint:all -Werror src/*.java
```

Every solution shown in a challenge, in any course, has been run and checked
against the output its task promises. Python's and JavaScript's own solutions
are included, verified by intercepting `input()` (Python) or the `ui.js`
prompt functions (JavaScript) and driving every module through directly
rather than by compiling anything.

Every compile-error claim across every course — C/C++'s warnings, and every
one of Java's genuine compile errors (`if (x = 5)`, a private field touched
from outside its class, an unhandled checked exception, `ArrayList<int>`,
and the rest) — was produced by actually running `javac` against it, not
recalled from documentation.

`python3 tools/check-teaching-order.py` covers all five courses in one run.

## Layout

```
README.md                   this file
LICENSE                     MIT
CONTRIBUTING.md             how to add a module, a course, or a fix
CODE_OF_CONDUCT.md          Contributor Covenant 2.1
SECURITY.md                 what counts as a vulnerability here
.github/
└── workflows/
    └── release.yml         builds + packages the launcher on a version tag
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
    └── lessons_advanced.c  modules 11-13
gui/
├── Makefile
└── src/
    ├── main.c              menu: a table of modules and a loop
    ├── ui.h  ui.c          screen, input, questions (no challenge() --
    │                        a window is not text to diff against)
    ├── lessons.h           one prototype per module
    └── lessons_gtk.c       module 1
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
java/
├── Makefile
├── run  run.bat             launcher entry point: java Main.java
└── src/
    ├── Main.java             menu: an array of a title+Runnable record, a loop
    ├── Ui.java               screen, input, questions, challenges
    ├── LessonsBasics.java    modules 1-5
    └── LessonsMore.java      modules 6-10
tools/
└── check-teaching-order.py  fails if an exercise needs something not yet
                              taught, in any course
docs/
├── writing-a-course.md     how to add a module or a new language
└── building-a-single-course.md   just one course, standalone, no launcher
```

## Contributing

Fixes to a lesson are the easiest contribution: point at the claim, show what
you ran and what it actually printed, fix the text. Larger changes — a new
module, a new language — start with an issue.

One rule matters more than the rest: **every claim in a lesson must be
something you actually ran.** This project has already caught three claims
that "everyone knows" and that were wrong when tested. See
[`CONTRIBUTING.md`](CONTRIBUTING.md) for the process and the per-language
verification commands, and [`docs/writing-a-course.md`](docs/writing-a-course.md)
for the design rules.

## License

[MIT](LICENSE). The courses depend on nothing beyond each language's own
toolchain — no packages to install, in any of the five.
