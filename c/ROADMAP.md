# Roadmap: past the course

CodeLearner's C course (modules 1-13) covers stages 1-3 and has started
stage 4. This file is the rest of the path — what comes after module 13,
and where each stage of it actually lives. Nothing here is new course
content; it's a map, kept in the repo so the next stage isn't a mystery
once the course ends.

## 1. Core syntax — covered

Variables, types, `if`/`else`, `for`/`while`, functions, arrays, strings,
structs, compilation: modules 1-7 and 10. Not yet in the course: `enum`,
`switch`, `typedef`, and a dedicated module on splitting a program across
`.h`/`.c` files — real gaps, worth a future module rather than self-study,
since they're still core syntax.

## 2. Pointers — covered

`&`/`*`, `NULL`, pointer arithmetic, pointers into arrays and strings,
pointers to structs, pointer-to-pointer: modules 8 and 12. Function pointers
and `const` with pointers are not yet a module — same note as above.

Milestone: you can explain why `swap(int *a, int *b)` works without having
memorized it.

## 3. Memory management — covered, and the deepest module in the course

`malloc`/`calloc`/`realloc`/`free`, stack vs. heap, ownership, and all four
ways to get it wrong (leak, use-after-free, double-free, returning the
address of a local array) are module 9 — the longest module on purpose.
Module 12 goes further: a dynamically-sized 2D grid, growing an array with
`realloc`, and a linked list, each shown broken first so the fix means
something.

Milestone: pointers and memory make intuitive sense, not memorized rules.

## 4. Data structures and algorithms — started

Module 12 covers a dynamic array and a linked list. Module 13 adds a stack
and a queue, both array-based — a second underlying representation for
"a collection that grows," with its own tradeoff (fixed capacity instead
of one `malloc` per item) instead of just another linked structure.

Still beyond the course: your own string type, hash tables, trees, heaps,
graphs. Then algorithms: sorting, binary search, recursion, BFS, DFS,
greedy, dynamic programming. Competitive-programming judges (Codeforces,
beecrowd) in C are the practice loop here, not a course module — issues
[#1](https://github.com/otzpt/CodeLearner/issues/1) and
[#2](https://github.com/otzpt/CodeLearner/issues/2) track building the rest
of this as real modules.

Milestone: you can implement common data structures yourself.

## 5. The C standard library — beyond this course

Know what `memcpy`/`memmove`/`memset`/`memcmp`, `strlen`/`strcmp`/`strcpy`/
`strncpy`/`strchr`/`strstr`, and `fopen`/`fread`/`fwrite`/`fclose`/`fseek`
actually do, not just that they exist — and which string functions are
unsafe to reach for by default. `<stdint.h>`, `<stdbool.h>`, `<assert.h>`,
`<errno.h>` round this out.

## 6. Real C projects — beyond this course

Stop writing toy programs: a file organizer, hex viewer, CLI shell, HTTP
client, compression tool, mini database, config parser. "For each file,
determine category, create folder if needed, move it" is a genuinely good
one to start with — it forces directories, paths, structs, and errors
together.

Milestone: a substantial C program, without AI.

## 7. Linux / POSIX C — beyond this course

`open`/`read`/`write`/`close`, `fork`/`exec`/`wait`, `pipe`/`dup2`,
`stat`/`opendir`/`readdir`, `mmap`. Processes, file descriptors, signals,
permissions, virtual memory. Build a mini shell — that project alone covers
most of this list at once.

## 8. Debugging — beyond this course, but start immediately

`gdb`, `valgrind`, `strace`; `-Wall -Wextra -Wpedantic`; eventually
`-fsanitize=address,undefined`. (The course already builds warning-clean and
the verification steps in `docs/writing-a-course.md` run under both
sanitizers — the habit modules 1-12 model is exactly this one.) The goal is
seeing "Segmentation fault" and thinking about what you dereferenced, not
just reacting to it.

## 9. Assembly — beyond this course

Not fluency — enough to recognize registers, the stack, calling
conventions, function calls, return values, memory addressing, syscalls.
Then take a program you've already written for this course and read its
compiled output (`gcc -S` or `objdump -d`). That's the payoff, not assembly
for its own sake.

## 10. Reading other people's C — beyond this course

musl, BusyBox, Git, Linux userspace tools, eventually the kernel. Don't try
to understand a whole file at once: pick one function, trace what it calls,
what data structure it touches, what subsystem that belongs to. This is the
skill that makes a large unfamiliar codebase navigable instead of
overwhelming — the same skill this course's own `main.c` module table is a
small, safe example of reading.

## Milestones, in order

1. Write normal C without tutorials.
2. Pointers and memory make intuitive sense.
3. Implement common data structures yourself.
4. A substantial C program, without AI.
5. Debug segfaults and memory corruption yourself.
6. Read unfamiliar C and understand it.
7. Navigate a large C codebase.
8. Understand Linux userspace internals.
9. Read kernel code without feeling completely lost.
10. A first kernel patch gets reviewed.

Don't rush stage 1 because it looks like "only" the syntax. The depth is in
stages 2 and 3 — pointers, memory, undefined behaviour — and everything
past module 12 assumes those are solid, not memorized.
