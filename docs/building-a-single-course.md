# Running a single course

Every course is a folder that runs on the shared lesson engine in `learn/`.
To use just one course, you need three things from the repository or the
release bundle:

```
learn/            the engine (app.js, main.js, pt.js, web.js)
<course>/         learn.js, course.js and lessons/
```

plus, for two courses, the stand-in they run on: `arduino/shim/` for Arduino
and `micropython/sim.py` for MicroPython.

Then, from anywhere:

```bash
node <path>/<course>/learn.js
```

## What each course needs installed

Node runs every course. Each course also runs its examples, and your
exercises, with its own language's toolchain:

| Course | Needs |
| --- | --- |
| C, GTK, raylib | a C compiler (`cc`, or `gcc` on Windows); GTK 4.10+ or raylib for those two |
| C++, Arduino | `g++` |
| Python, Security, MicroPython | `python3` (`python` on Windows) |
| JavaScript, HTML, CSS, Web JavaScript | nothing beyond Node |
| Java | a JDK, 11 or later |
| C# | the .NET 10 SDK |
| Rust | `rustc` |
| Assembly | `nasm` and `ld`, on 64-bit x86 Linux |
| Git | `bash` and `git` |

A course that is missing its toolchain still opens; its examples say which
program is not installed.

## Where your work is kept

Practice files and progress go in `~/codelearner/<course>/`. Set
`CODELEARNER_PRACTICE_DIR` to put them somewhere else.
