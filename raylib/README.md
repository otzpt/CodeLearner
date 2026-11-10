# raylib course

3 modules: your first window and the loop, raygui buttons and state, a todo list.

A terminal program in plain C, like `gtk/`. It teaches raylib and its
companion header raygui. It never opens a window itself: the programs it
teaches are ones you write and compile yourself. raylib is built from source
(module 1 gives the steps and pins the commits this was tested with).

## Running

```bash
make
./raylib-course
```

## How it was verified

Every example compiles with `-std=c11 -Wall -Wextra` and no warnings of its
own, and was started and ran. The todo list's data functions were tested by a
program that included the file without opening a window. Clicking and typing in
the raygui windows were not driven by a test. The measurements the lessons
quote (frames per second with and without `SetTargetFPS`, a missing-file
warning, `TextFormat`'s reused buffer, `strncpy` not terminating, deleting
inside a draw loop) were read back by small programs on the author's machine.

`tools/check-teaching-order.py` covers the exercises. Output is coloured in a
terminal; `NO_COLOR=1` turns it off.
