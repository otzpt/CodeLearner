# C course roadmap

Where the C course stops now, and what comes next. The whole project's plan is
in [`../ROADMAP.md`](../ROADMAP.md); how a lesson is written is in
[`../docs/writing-a-course.md`](../docs/writing-a-course.md).

## Where it stops

14 lessons, for someone who has never programmed: the first program and the
compiler, numbers and int division, `scanf`, `if`, loops, arrays, functions,
strings and `strcmp`, pointers, `malloc`/`free`, structs, files, debugging
with AddressSanitizer, and a guessing game.

## Next, roughly in order

Each is a lesson or two, written the same way: from zero, one example per
screen, every example run, an exercise checked by running the student's file.

1. **Programs in several files** and building them with `make`: headers,
   declarations, `static`, why the order of `-l` options can matter.
2. **More of the standard library** (#4): `stdint.h` fixed-size types,
   `string.h` beyond `strlen` and `strcmp` (`strncpy`'s missing terminator,
   `memcpy`), `qsort` with a comparison function.
3. **Function pointers** and callbacks, which `qsort` needs.
4. **Bits**: `&`, `|`, `^`, `~`, shifts, masks, and `enum` and `union`.
5. **The preprocessor**: `#define` with parameters and why they need
   brackets, `#ifdef`, include guards.
6. **Debugging** (#6): `gdb` breakpoints and backtraces, `valgrind`, core
   dumps; the AddressSanitizer lesson is the start.
7. **Data structures** (#1): a stack, a queue, a linked list, a hash table,
   a binary tree, each with `malloc`/`free` and a leak check.
8. **Algorithms** (#2): sorting, binary search, recursion, BFS and DFS.
9. **POSIX and Linux** (#5): file descriptors, `fork`/`exec`/`wait`, pipes,
   signals, `errno`.
10. **Assembly from C** (#7): reading the compiler's output, the calling
    convention, inline assembly. The Assembly course covers the assembly side.
11. **Projects** (#3, #13): a file organiser, a small shell, a text editor,
    each combining several lessons.

## Across the course

- Bug challenges (#10) and code-reading challenges (#8): a program that is
  wrong or unfamiliar, to fix or extend.
- Less help in later exercises (#9): fewer hints, a task stated only by its
  output.
- More checking of exercises (#12): a second set of input, edge cases, input
  that is not a number.
- Progress by concept (#11): the course already ticks finished lessons; this
  would name what each lesson taught.
- A Linux-kernel preparation track (#14): GNU extensions, kernel style,
  intrusive lists, bit manipulation.

## Found in a read-through (2026-11-24)

Gaps and small fixes, not yet filed as issues.

- **Arrays and functions.** Lesson 6 says the `sizeof` trick fails inside a
  function the array was passed to, but no lesson teaches passing an array to a
  function, or how arrays and pointers relate.
- **Strings.** No copying (`strcpy`, `snprintf`) and no reading a whole line from
  the keyboard (`fgets` on `stdin`). A learner who tries to assign a name to a
  struct member in lesson 11 hits an error the course never explains.
- **Missing basics.** `switch`, `enum`, `char` and the other number types, header
  files with several source files, `argc` and `argv`. That any non-zero value
  counts as true is never said in lesson 4.
- **Lesson 10 exercise.** `n` is read with `scanf` and never checked, so a bad
  input leaves it uninitialised before `malloc`. This goes against what lesson 3
  teaches.
- **Lesson 13.** `-fsanitize=address` probably does not work with the MinGW gcc
  that `course.js` uses on Windows. Needs checking on a Windows machine.
- **Style.** A few one-line `if (...) return 1;` forms in lessons 10 and 12 break
  the always-braces style used elsewhere.
