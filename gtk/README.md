# GTK course

4 modules: your first window, layout and the common widgets, signals and
keeping state, widgets that come and go (a todo list).

A terminal program in plain C, like `raylib/`. It teaches GTK4 and never opens
a window itself: the programs it teaches are ones you write and compile
yourself, with GTK4's development files installed (module 1 shows how to check).

## Running

```bash
make
./gtk-course
```

## How it was verified

Every example compiles with `-std=c11 -Wall -Wextra` and no warnings, and was
driven by a test program against the real window. Every bad use shown was
compiled or run, and the error quoted is its real output. See the header of
`src/lessons_gtk.c` and `src/lessons_widgets.c` for the full list.

`tools/check-teaching-order.py` covers the exercises. Output is coloured in a
terminal (`NO_COLOR=1` turns it off) and long screens pause with "more: press
ENTER" so they never scroll off (`CODELEARNER_NO_PAGER=1` turns that off).
