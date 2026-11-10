# raylib course

6 modules: your first window and the loop, shapes and text, keyboard and mouse input, time and movement, raygui buttons and state, a todo list.

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
own, and was started and ran. Drawing results were read back from an
off-screen texture. Collision, vector maths, frame-time movement, the pong
rules and the todo list's data functions were run by small programs that
include the example file and never open a window. How real key presses,
mouse clicks and raygui widgets behave under a person's hands was not driven
by a test; the lessons say so. The reference screens are generated from
`raylib.h` and `raymath.h`, not retyped.

`tools/check-teaching-order.py` covers the exercises. Output is coloured in a
terminal (`NO_COLOR=1` turns it off) and long screens pause with "more: press
ENTER" so they never scroll off (`CODELEARNER_NO_PAGER=1` turns that off).
