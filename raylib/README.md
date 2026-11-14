# raylib course

raylib from C, for someone who knows C's basics (the C course covers them).
raylib programs open windows, which a terminal cannot show, so the course
builds every example and exercise with the real raylib and you run them
yourself.

## Lessons

1. A window and the game loop
2. Shapes and colours
3. Keys, mouse and movement
4. Project: a catching game

## Building yourself

```bash
cc -o program main.c $(pkg-config --cflags --libs raylib) && ./program
```

Needs a C compiler and raylib. Without it, the course says so and its checks
report it skipped (the CI runner has no raylib package). The windows
themselves are not run by any check.

## Running

From the launcher, or directly:

```bash
node raylib/learn.js
```

Your practice files and progress go in `~/codelearner/raylib/`.

## Checking

```bash
node learn/check.js raylib            # every example, question, solution and starter
python3 learn/check-course.py raylib  # every lesson played to the end
```

