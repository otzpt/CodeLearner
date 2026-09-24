# Roadmap: past the course

CodeLearner's C course (modules 1-19) covers a full 13-section target
curriculum (fundamentals through advanced C). Sections 1-9 and 13 are
built; section 10 (data structures) is started and continues past this
course; sections 11 (algorithms) and 12 (practical C) are deliberately
out of scope for an interactive course, not gaps. This file maps that full
curriculum onto exact module numbers: what's built, and what's genuinely
beyond an interactive course. Nothing here is new course content; it's the
map, kept in the repo so the next module isn't a mystery.

Numbering rule: new modules are appended after the current last module
(19) and tagged tier ADVANCED, regardless of how introductory their content
actually is. This is a deliberate trade-off, not an oversight — see
"Numbering vs. tiers" at the end of this file.

## 1. C fundamentals — mostly covered

Compiling, `main`, variables, types, operators, `if`/`else`, `for`/`while`,
arrays, strings, structs: modules 1-7 and 10. Preprocessing (what actually
happens before compilation: macro expansion, `#include`, conditional
compilation) is not yet taught anywhere and belongs with module 16
(compilation and project structure, below) rather than here, since it's
inseparable from how a real build actually runs.

Gaps folded into existing modules rather than new ones (small, and each
belongs with content already there):
- `switch` — a PART in module 4 (Conditions).
- `do`/`while` — a PART in module 5 (Loops).
- Scope and lifetime (block scope, when a local variable's storage
  actually exists) — a PART in module 2 (Variables and types), since it's
  inseparable from what a variable *is*.

## 2. Functions — mostly covered

Parameters, return values, pass-by-value: module 7. Gaps, folded into
module 7 as new PARTs rather than a new module, since they extend it
directly rather than starting something new:
- Function prototypes/declarations (why a `.c` file needs to see a
  function's signature before its first call, and what happens without
  one).
- Local vs. global variables (their own PART, distinct from scope/lifetime
  above, which is about a single variable's existence; this is about two
  places a name can live and which one wins).
- Recursion (a base case, a recursive case, and a real stack-depth
  failure shown — not simulated — the same way module 9 shows real leak
  output rather than describing one).

Organizing a program into multiple `.c` files belongs with module 16
(compilation and project structure) — it is inseparable from linking and
header files, not a functions topic on its own.

## 3. Arrays and strings — covered

1D arrays, character arrays, C strings, `strlen`/`strcmp`/`strcpy`, arrays
decaying to pointers when passed to functions: module 6, revisited in
module 8 (pointers) and module 12 (a 2D grid, genuinely
multidimensional). No new module needed; 2D arrays before pointers were
judged not worth a separate module, since module 12's grid already covers
the concept properly once pointers make `int **grid` legible — introducing
`int grid[3][4]` earlier, without pointers to explain how it actually
decays, would be teaching around the trap this course exists to teach.

## 4. Pointers — covered, deliberately the biggest section

The course's own stance already matches the request that this not be
"here's `*`, good luck": `&`/`*`, `NULL`, pointer arithmetic, pointers into
arrays and strings, pointers to structs, `.` vs `->`, pointers to
pointers: modules 8 and 12. `const` with pointers is the one real gap —
folded into module 8 as a new PART (`const int *`, `int *const`, and the
difference, with a compiler warning shown for getting it backwards, per
the course's "warnings are teaching material" rule). Function pointers as
*taught content* (not just the menu-table mechanism `main.c` already uses)
is covered separately in module 19 (advanced C), since they depend on
nothing pointers-specific beyond what module 8 already gives and are more
at home next to callbacks and variadic functions.

Milestone: you can explain why `swap(int *a, int *b)` works without having
memorized it.

## 5. Structs, enums, and unions — partially covered, module 15 fills the gap

`struct`, arrays of structs, nested structs, structs + pointers: module
10. `enum`, `union`, `typedef`, and struct padding/alignment are module
15 (new) — enough of a jump from module 10 (a genuinely new keyword and a
genuinely new memory-layout idea, not a variation on struct) to deserve
its own module rather than another PART bolted onto module 10.

## 6. Memory management — covered, and the deepest module in the course

`malloc`/`calloc`/`realloc`/`free`, stack vs. heap, ownership, and all four
ways to get it wrong (leak, use-after-free, double-free, returning the
address of a local array) are module 9 — the longest module on purpose.
Module 12 goes further: a dynamically-sized 2D grid and growing an array
with `realloc`, each shown broken first so the fix means something. Buffer
overflows specifically (as distinct from the four ways module 9 already
names) are the one gap — worth a PART in module 9 if it's ever revisited,
not urgent enough for a new module on their own.

Milestone: pointers and memory make intuitive sense, not memorized rules.

## 7. Real input/output — module 18 fills this gap entirely

`printf`/`scanf`/`fgets` (modules 1, 3), input validation (scattered
across `question()`'s own answer-checking, but never taught as its own
idea for a program the student writes). `FILE *`, reading/writing files,
text vs. binary, and command-line arguments (`argc`/`argv`) do not exist
anywhere in the course yet — all of module 18 (new).

## 8. Compilation and project structure — module 16 fills this gap entirely

Everything here is currently undocumented inside the course itself (it's
described in `docs/writing-a-course.md` and this repo's own Makefiles, but
never taught to the student): `.c` vs `.h`, header guards, separate
compilation, linking, `gcc` flags, compiler warnings, debug builds,
Makefiles, static libraries, and reading a real linker error. All of
module 16 (new) — the single highest-value gap in the course, since every
module from 1 onward has been a single `.c` file the student never had to
link, and every real C program is not.

## 9. Debugging — module 17 fills this gap entirely

Reading compiler errors and warnings (already modeled by the course's own
"warnings are teaching material" rule, but never taught directly), `gdb`
(breakpoints, inspecting variables, stack traces), debugging memory
problems, and sanitizers (`-fsanitize=address,undefined` — already how
this course verifies itself, per `docs/writing-a-course.md`, but never
shown to the student as a tool they can run themselves). All of module 17
(new). Ordered after module 16 (compilation) since you need to understand
compiler flags before `-g` and `-fsanitize=...` mean anything, and after
module 9 (memory) since most of what's worth debugging here is a memory
bug module 9 already named.

## 10. Data structures — started, continues past this course

Module 13 covers a linked list. Module 14 adds a stack and a queue, both
array-based — a second underlying representation for "a collection that
grows," with its own tradeoff (fixed capacity instead of one `malloc` per
item) instead of just another linked structure.

Trees, hash tables, and graphs are still beyond the course — real enough
data structures that each deserves its own module done right (a broken
version shown first, same as every memory-management trap in this course),
not a rushed single pass. Tracked as future modules rather than designed
here; see the C-specific backlog below.

Milestone: you can implement common data structures yourself.

## 11. Algorithms — beyond this course

Searching, sorting (quicksort/mergesort), binary search, Big-O basics,
graph traversal, basic dynamic programming. Recursion (module 7, folded
in above) is the one prerequisite this course now provides; the rest
needs the data structures from section 10 built first, so it comes after
that, not before.

## 12. Practical C — beyond this course, deliberately

A CLI argument parser, text processing tool, file search utility, config
parser, mini shell, mini database, HTTP client, process/system utility, a
custom allocator, a small data-structure library. Explicitly NOT games —
real programs. This is exactly what issue #3 already tracks ("a file
organizer... forces directories, paths, structs, and errors together"),
and stays a project-practice track rather than a numbered module: a
project is open-ended by nature, and this course's own format (a fixed
`challenge()` with one exact expected output) is the wrong shape for it.

Milestone: a substantial C program, without AI.

Note: GUI programming (GTK) is not filed here — it got its own course,
[`gui/`](../gui), rather than a C-course module. See the top-level
README's course table.

## 13. Advanced C — module 19 fills part of this, most stays beyond the course

Function pointers (as taught content), callbacks, variadic functions,
bit manipulation, and the preprocessor/macros belong together in module 19
(new) — a natural set: each is "C doing something with functions or text
that looks like magic until you see the mechanism," and none needs
anything past module 9's memory model.

`volatile`, `static` (file scope, not the keyword's other two meanings
this course hasn't touched), `restrict`, alignment, and undefined
behaviour as its own topic (distinct from the specific instances of UB
already named throughout the course) are a plausible module 20, not
designed here yet.

Atomics, threads, POSIX APIs, and sockets are genuinely beyond an
interactive single-process CLI course — they need a real multi-process or
multi-threaded environment to demonstrate honestly, which breaks this
course's "one program, one terminal, real output" format. These stay filed
under `c/ROADMAP.md`'s existing stage 7 (Linux/POSIX) below, as a
next-step-after-this-course area, same as it already was.

## Linux / POSIX C — beyond this course

`open`/`read`/`write`/`close`, `fork`/`exec`/`wait`, `pipe`/`dup2`,
`stat`/`opendir`/`readdir`, `mmap`, atomics, threads, sockets. Processes,
file descriptors, signals, permissions, virtual memory. Build a mini
shell — that project alone covers most of this list at once, and doubles
as the "practical C" project from section 12.

## Assembly — beyond this course

Not fluency — enough to recognize registers, the stack, calling
conventions, function calls, return values, memory addressing, syscalls.
Then take a program you've already written for this course and read its
compiled output (`gcc -S` or `objdump -d`). That's the payoff, not assembly
for its own sake.

## Reading other people's C — beyond this course

musl, BusyBox, Git, Linux userspace tools, eventually the kernel. Don't try
to understand a whole file at once: pick one function, trace what it calls,
what data structure it touches, what subsystem that belongs to. This is the
skill that makes a large unfamiliar codebase navigable instead of
overwhelming — the same skill this course's own `main.c` module table is a
small, safe example of reading.

## Milestones, in order

1. Write normal C without tutorials.
2. Pointers and memory make intuitive sense.
3. Read your own compiler's warnings and errors, and your own program's
   linker errors, without help.
4. Debug segfaults and memory corruption yourself, with `gdb` and
   sanitizers, not by guessing.
5. Implement common data structures yourself.
6. A substantial C program, without AI.
7. Read unfamiliar C and understand it.
8. Navigate a large C codebase.
9. Understand Linux userspace internals.
10. Read kernel code without feeling completely lost.
11. A first kernel patch gets reviewed.

Don't rush the fundamentals because they look like "only" the syntax. The
depth is in pointers and memory, and everything past module 14 assumes
those are solid, not memorized.

## Tiers

The menu groups modules into three bands: BASIC (1-7, no pointers yet),
INTERMEDIATE (8-11, pointers through the final test), and ADVANCED (12
onward). It is a display grouping in `main.c`'s `MODULES` table, not a
separate track — the modules still run in one numbered sequence, and
`tools/check-teaching-order.py` checks that sequence exactly as it did
before tiers existed.

## Numbering vs. tiers

Every module added after 14 is tagged ADVANCED, even ones that are not,
by content, advanced (enums and unions, in module 15, are genuinely closer
to module 10's difficulty than module 14's). This is a deliberate,
documented trade-off:

The menu's tier headers are printed by walking `MODULES` in array order
and emitting a new `-- TIER --` line whenever the tier field changes from
the previous row (see `c/src/main.c`'s `show_menu`). A module's number
IS its position in that array, and that position is also what
`check-teaching-order.py` treats as "how much has been taught so far" —
module 15's exercise is allowed to use anything modules 1-14 taught,
nothing more. Both of those meanings (array position, and "taught by
now") depend on new modules only ever being appended at the end.

Inserting module 15 in the *middle* of the array (next to module 10,
where it's conceptually closer in difficulty) would require renumbering
every module after it — updating every `exercise(N)` call, every
`lesson_NN_*` function and file name, every cross-reference in this file
and in `docs/writing-a-course.md`, and re-verifying the whole course
compiles and passes the checker again. That cost is real and was judged
not worth paying for a cosmetic accuracy gain (one tier label being
slightly generous) versus a correctness risk (a renumbering mistake
breaking a module's own exercise or another module's cross-reference to
it, the same class of mistake `check-teaching-order.py` exists to catch
in the first place).

If a genuinely BASIC or INTERMEDIATE gap is found later that must sit
before an existing module to make sense (unlike enums/unions, which don't
need to), that is the point to reconsider this trade-off — not before.
