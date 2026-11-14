# GTK course

GTK 4 from C, for someone who knows C's basics (the C course covers them).
GTK programs open windows, which a terminal cannot show, so the course builds
every example and exercise with the real GTK and you run them yourself.

## Lessons

1. A window
2. Widgets and boxes
3. Reacting to clicks: signals
4. Reading typed text: GtkEntry

## Building yourself

```bash
cc -o program main.c $(pkg-config --cflags --libs gtk4) && ./program
```

Needs a C compiler and GTK 4.10 or later (every lesson uses
`G_APPLICATION_DEFAULT_FLAGS`, from GLib 2.74). Without it, the course says
so and its checks report it skipped. The windows themselves are not run by
any check.

## Running

From the launcher, or directly:

```bash
node gtk/learn.js
```

Your practice files and progress go in `~/codelearner/gtk/`.

## Checking

```bash
node learn/check.js gtk            # every example, question, solution and starter
python3 learn/check-course.py gtk  # every lesson played to the end
```

