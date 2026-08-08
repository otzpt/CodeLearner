# Writing a course

How the courses are built, and how to add a module or a whole new language.
Read this before writing lessons — most of it is about what *not* to do.

## The rules

### 1. Write the course in the language it teaches

The C course is written in C. The Python course will be written in Python.

This is not a gimmick. A student who has just been shown function pointers can
open `c/src/main.c` and see one holding the module table. The course stops
being a description of the language and becomes a program written in it.

It also keeps the author honest: you cannot teach something the course itself
does badly.

### 2. Nothing is claimed without being shown

If an example can run, run it and print the real result.

```c
printf("    sizeof(int)    = %zu bytes\n", sizeof(int));
```

Not "an int is usually 4 bytes" — the number the student's own compiler just
produced. The same goes for addresses, for `5 / 2`, for what `malloc`
returned. A course that asserts is a worse book; a course that demonstrates is
something a book cannot be.

Where the language runs elsewhere, verify before writing it down. Every output
in the C course was produced by running the code first. Two claims changed as
a result.

### 3. Never demonstrate undefined behaviour

A double free, a use-after-free, reading past an array. Explain them, show the
error message they produce, and do not run them. Undefined behaviour either
crashes the course or — much worse — appears to work, teaching exactly the
wrong lesson.

Show the diagnostic instead:

```
free(): double free detected in tcache 2
```

### 4. An exercise may only need what the course has already taught

A task requiring `if` before module 4 has taught `if` is a broken task, not a
hard one. It reads as "you should have known this" to somebody who could not
have.

This has slipped through twice, both times caught by a student rather than by
the author, so it is checked automatically now:

```bash
python3 tools/check-teaching-order.py
```

It maps every construct to the module that first teaches it and flags any
exercise reaching ahead. Run it before committing a new module. Adding a
construct to a lesson means adding it to that map too.

### 5. The input and the output are the specification, not your code

Every `challenge` states two things: the input the program will read, if any,
and the output it must produce from that input. That pair is what the student
checks against — not the code shown afterwards, which is labelled example code
and described as one way of getting there. A different solution that reads the
same input and produces the same output is correct, and may well be better.

State input explicitly even when it already appears inside the expected
transcript (a prompt like `What is your name? ` followed by the typed `Jose`
on the same line is realistic, but does not by itself tell a reader that
`Jose` was theirs to type rather than the program's to print). Leave it empty
for a task that reads nothing — most of the early ones do.

### 6. Teach the trap, not the syntax

Syntax is in every reference. What is worth a module is the thing that makes
people lose an afternoon:

- `5 / 2` giving `2`, and casting the result being too late
- `scanf` needing `&`, and arrays not
- `==` on strings comparing addresses
- an array of 5 having no index 5
- one `free` per `malloc`, and who owes it

If a module has no trap in it, it probably does not need to be a module.

### 7. Warnings are teaching material

Build with `-Wall -Wextra`. When the compiler catches something the module is
about, quote it. `if (x = 5)` is a better lesson with the warning next to it.

## Adding a module to an existing course

Three steps.

**1. Write the function.** In `c/src/lessons_basics.c` or
`c/src/lessons_memory.c`, following the shape every other module uses:

```c
void lesson_11_files(void)
{
    title("MODULE 11 - FILES");

    heading("PART 1: ...");
    /* explanation, and run whatever can be run */

    wait_enter();
    clear_screen();
    exercise(11);

    question("...", "answer", "why");

    {
        const char *task[] = { "...", };
        const char *expected[] = { "what it must print", };
        const char *solution[] = { "...", };
        challenge(task, 1, expected, 1, solution, 1);
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");
    /* four or five lines */
    wait_enter();
}
```

**2. Declare it** in `c/src/lessons.h`.

**3. Add one line** to the `MODULES` table in `c/src/main.c`. The menu
numbers itself and `MODULE_COUNT` counts itself, so there is nothing else to
update.

If the file is getting long, split it the way `lessons_basics.c` and
`lessons_memory.c` are split — by theme, not by line count.

## The helpers

From `ui.h`. They exist so a lesson contains teaching and nothing else.

| Function | Use |
| --- | --- |
| `title("MODULE 1 - ...")` | framed heading, once per module |
| `heading("PART 1: ...")` | section heading inside a module |
| `wait_enter()` | between screens |
| `clear_screen()` | after `wait_enter()`, before the next part |
| `rule()` | a plain horizontal line |
| `exercise(n)` | the exercise header |
| `question(text, correct, why)` | short answer, checked immediately |
| `challenge(task, n, expected, e, solution, m)` | task, the output it must produce, and example code on request |
| `read_line(buf, sizeof buf)` | read input safely |
| `ask_yes("...")` | yes/no |

`question` compares answers ignoring case and surrounding spaces. Keep the
expected answer short and unambiguous — `"3"`, `"-o"`, `"yes"`. If a question
has two reasonable phrasings, rewrite the question.

## Adding a new language

Create a directory next to `c/`, build it the same way, and add it to the
table in the README.

The course must:

- be written in the language it teaches
- be a single self-contained CLI program with no runtime dependencies beyond
  that language's own toolchain
- follow the module shape above
- state in its own README how to run it and how it was verified

Do not share code between courses. They are separate programs on purpose: a
Python course importing a helper written in C would be worse in every way that
matters, and the duplication is a menu loop.

Once the course builds -- or, for an interpreted language, once its entry
script exists and is runnable -- register it in `launcher/src/main.c`: change
its `LANGUAGES` entry from `NULL` to the relative path. Nothing else changes
— the launcher checks the path exists and runs it with `system()`. It does
not need to know the course's module count, its language, or whether that
path is a compiled binary or a script. Python's and JavaScript's entries are
scripts, `"../python/src/main.py"` and `"../javascript/src/main.js"`: no
build step, but the same file_exists() check and the same system() call as C
and C++. On Linux this relies on the script's own shebang line and its
execute bit (`chmod +x`); on Windows, on the extension being associated with
the right interpreter, which the standard installers set up. C++, Python and
JavaScript are all working examples of registering a language with a
two-line change.

Java could not use that same direct-to-entry-file pattern: verified against
the installed JDK that a `#!` shebang line on a `.java` file does not
compile, so unlike a `.py`/`.js` script, `java/src/Main.java` cannot be
marked executable and pointed at directly. `java/run` (`run.bat` on Windows)
is a small wrapper doing `cd src && java Main.java`, and that wrapper is
what is registered instead -- worth checking for early if a future
language's own launch story turns out not to be a single runnable file
either.

Also add the course to `COURSES` in `tools/check-teaching-order.py` — its
own `first_taught` table, in that language's own vocabulary. The five
existing entries are working examples of the shape: a construct's regex
mapped to the module that first shows it *anywhere in that module*, including
the module's own explanation, not just its formally-named topic. `std::vector`'s
declaration syntax is credited to the C++ course's module 5, where it first
appears on screen as something to loop over, even though its method
vocabulary (`push_back`, `sort`) is not taught until module 8; JavaScript's
arrow-function syntax `=>` is credited to module 6 for the identical reason
-- it appears as a `map`/`filter`/`sort` argument before module 7 makes
arrow functions a named topic -- the check
follows what a reader has actually seen, not which module "owns" a topic.

## Verifying before committing

For the C course:

```bash
cd c
make clean && make                     # must be warning-free
cc -std=c11 -Wall -Wextra -g -fsanitize=address,undefined -o /tmp/course src/*.c
/tmp/course                            # walk the modules you changed
```

And compile every solution you added to a `challenge`. A solution shown to a
student that does not compile is worse than no solution. Same idea for C++,
with `g++ -std=c++20` in place of `cc -std=c11`.

For an interpreted language with no compiler to catch mistakes, a syntax
check plus running every module without crashing is the minimum, not
optional:

```bash
cd python/src
python3 -c "import ast; ast.parse(open('lessons_basics.py').read())"
python3 main.py
```

For coverage without hand-counting how many prompts each module has,
intercept `input()` and call each module's function directly rather than
driving the menu through a pipe -- a real terminal, or a FIFO with delays
between lines, is the only thing that behaves like actual typing; a plain
`printf ... | prog` pre-fills the whole pipe at once and lets one process's
buffered reads consume input meant for another, which looks like a hang or a
skipped module and is not one. See the Python course's own verification
history for the working pattern: mock `input()` with an iterator, redirect
`stdout` to a buffer, call the lesson function, assert no exception and that
`"SUMMARY"` appears in the captured output.
