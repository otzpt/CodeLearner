# CodeLearner

Programming courses for the terminal, written for someone who has never
programmed: C, C++, Assembly, Rust, Arduino and MicroPython (on a PC, no board
needed), Python, JavaScript, Java, C#, HTML, CSS, Web JavaScript, Security,
GTK, raylib and Git. No browser, no account, nothing online except the update
check, which only runs when you pick it.

Every lesson runs its examples while you read them, explains each error in
plain words, and ends with a small program you write in a real file. The
course runs it and tells you whether it printed what was asked.

```sh
curl -fsSL https://raw.githubusercontent.com/otzpt/CodeLearner/main/install.sh | sh
codelearner
```

That opens the launcher (this is its real output; Windows and the other
install options are under [Installing](#installing)):

```text
  +======================================================+
  |                  CODELEARNER                         |
  +======================================================+
  version 2.0.0

   -- LOW-LEVEL AND SYSTEMS --
   [1]  C
   [2]  C++
   [3]  Assembly
   [4]  Rust
   [5]  Arduino
   [6]  MicroPython

   -- LANGUAGES --
   [7]  Python
   [8]  JavaScript
   [9]  Java
   [10]  C#

   -- WEB --
   [11]  HTML
   [12]  CSS
   [13]  Web JavaScript

   -- SECURITY --
   [14]  Security

   -- GUI --
   [15]  GTK
   [16]  raylib

   -- TOOLING --
   [17]  Git

   -- REFERENCE --
   [18]  Libraries

   [u]  Check for updates
   [l]  Portugues (PT-PT)
   [0]  Exit
  ------------------------------------------------------
```

The Libraries entry is a reference manual, not a course: man-page style pages
for C and Python libraries, each with an example that runs on your machine.

## What a lesson looks like

A lesson is a few screens, each with one idea and at most one example. The
example runs as you read it, and what it prints is real. This is a screen from
Python's lesson 2:

```text
  LESSON 2: TEXT AND NUMBERS   (4/6)
  ----------------------------------------------------------------

  Text and numbers do not mix with +. Python refuses rather than
  guess what you meant:

    print("5" + 1)

  Output:
    TypeError: can only concatenate str (not "int") to str
    (line 1)

  What this means: a value used in a way its type does not allow,
  like adding text to a number.
```

Short multiple-choice questions check the idea as you go; a wrong answer gets
a second try and then the reason. At the end, YOUR TURN gives a task, the exact
output it must print, and a file to write it in. Press ENTER and the course
runs your file:

```text
  Not yet. Your program printed:
    60
    3.4
    2

  Line 2 should be:  3
```

`h` gives a hint, `s` shows one solution (yours can look different and still
be right), `k` skips. Each lesson ends with WHAT YOU LEARNED, a few lines worth
remembering. Finished lessons are ticked in the course's menu, and your files
and progress stay in `~/codelearner/<course>/`.

## The courses

| Group | Course | Lessons |
| --- | --- | --- |
| Low-level and systems | C | 14: first program and the compiler through pointers, `malloc`/`free`, structs, files and AddressSanitizer |
| Low-level and systems | C++ | 12: `std::cout` through `std::vector`, references, classes, inheritance and exceptions |
| Low-level and systems | Assembly | 9: NASM for 64-bit Linux, from syscalls and registers through the stack, functions and text, then a look at boot sectors |
| Low-level and systems | Rust | 12: `println!` through ownership and borrowing, structs, `match`, `Option` and `Result` |
| Low-level and systems | Arduino | 8: `setup`/`loop`, pins, buttons, analog, timing with `millis`, Serial, a traffic light, on a PC stand-in for the board |
| Low-level and systems | MicroPython | 7: pins, the clock, buttons, ADC and PWM, interrupts, a traffic light, on a PC stand-in for a Pico |
| Languages | Python | 14: `print` through lists, dictionaries, classes, files and a guessing game |
| Languages | JavaScript | 13: `console.log` through arrays, functions, objects, classes, input and errors |
| Languages | Java | 12: `System.out` through `ArrayList`, classes, interfaces, exceptions and files |
| Languages | C# | 11: top-level statements through `List`, classes, interfaces, exceptions and files |
| Web | HTML | 7: tags, a whole page, nesting, links, lists, forms and structure |
| Web | CSS | 6: rules, class and id, the cascade, inheritance, `!important`, combinators |
| Web | Web JavaScript | 5: the DOM, changing the page, click events, inputs, adding elements |
| Security | Security | 7: password hashing and salt, unguessable tokens, SQL and command injection, path traversal, escaping HTML |
| GUI | GTK | 4: a window, widgets in boxes, signals, a text entry |
| GUI | raylib | 4: the game loop, shapes, keys and movement, a catching game |
| Tooling | Git | 8: commits, history, undoing, branches, conflicts, remotes, `.gitignore` and tags |

No lesson explains anything by comparing it to another language: every course
starts from zero on its own. Some subjects cannot be shown in a terminal, and
those courses say so:

- **HTML, CSS and Web JavaScript** use a small stand-in for a browser. It shows
  the page's element tree, each element's final CSS and the rule that won it,
  and runs the page's scripts. `sim.click` and `sim.type` play the user; they
  exist only on the PC.
- **Arduino and MicroPython** run on a PC stand-in for the board: pins are
  variables and time is simulated. Anything named `sim` is PC-only.
- **GTK and raylib** open windows, which the course cannot show: it builds your
  program and you run it.
- **Assembly** ends with a boot sector you can try in QEMU; the files in
  `assembly/os` go on from there to a small kernel.

[`ROADMAP.md`](ROADMAP.md) lists where each course stops and what comes next.

## Installing

One command, and `codelearner` is on your PATH. Neither installer needs
root or Administrator: both write only inside your own home directory, and
both replace an existing install rather than merging into it.

Linux, any distribution:

```sh
curl -fsSL https://raw.githubusercontent.com/otzpt/CodeLearner/main/install.sh | sh
```

`wget -qO- <same URL> | sh` works identically if curl isn't installed. It
unpacks the release into `~/.local/share/codelearner`, writes a wrapper to
`~/.local/bin/codelearner`, and adds a CodeLearner entry (with the icon) to
your application menu, which opens a terminal. Pass `--prefix DIR` to put
everything elsewhere, or `--uninstall` to remove it.

Windows:

```powershell
irm https://raw.githubusercontent.com/otzpt/CodeLearner/main/install.ps1 | iex
```

Installs to `%LOCALAPPDATA%\Programs\CodeLearner` and adds that directory
to your user PATH, so `codelearner` works in a new terminal. Download the
script and run it directly for `-Uninstall` or `-Prefix`.

> **`install.ps1` is untested.** It has never been run, because it was written
> on a Linux machine with no PowerShell on it, and CI builds the Windows bundle
> without touching the installer. Installing with `-Prefix` into a throwaway
> directory exercises everything except its PATH write.

Piping a script from the internet into a shell is worth reading first
either way. Both are a single file at the root of this repository:
[`install.sh`](install.sh), [`install.ps1`](install.ps1).

### What you need installed

The courses run on **Node.js**, so install it first. Each course also runs its
examples, and your programs, with that language's own tools:

| Course | Needs |
| --- | --- |
| C, GTK, raylib | a C compiler (`gcc`); GTK 4.10 or later, or raylib, for those two |
| C++, Arduino | `g++` |
| Python, Security, MicroPython | Python 3 |
| JavaScript, HTML, CSS, Web JavaScript | nothing beyond Node |
| Java | a JDK, 11 or later |
| C# | the .NET 10 SDK |
| Rust | `rustc` |
| Assembly | `nasm` and `ld`, on 64-bit x86 Linux |
| Git | `bash` and `git` |

A course whose tools are missing still opens; its examples say which program
is not installed. On Arch, for example: `sudo pacman -S nodejs gcc python
jdk-openjdk dotnet-sdk rust nasm git gtk4 raylib`.

### Or without an installer

Every artifact is on the [Releases page](https://github.com/otzpt/CodeLearner/releases):

| Download | What it is |
| --- | --- |
| `codelearner-x86_64.AppImage` | one file, `chmod +x` and run, nothing installed |
| `codelearner-amd64.deb` | Debian/Ubuntu package, `sudo dpkg -i` |
| `codelearner-x86_64.pkg.tar.zst` | Arch package, `sudo pacman -U` |
| `codelearner-linux-x86_64.tar.gz` | the plain bundle, extract and run `codelearner.sh` |
| `codelearner-windows-x86_64.zip` | the plain bundle, extract and run `codelearner.bat` |

The `.deb` and Arch packages install to `/opt/codelearner`, depend on Node, and
put the same `codelearner` command on PATH system-wide. The AppImage does not
touch PATH at all. Assembly, Arduino, Git and Libraries are Linux only.

### Updating

CodeLearner does not update itself. The launcher shows its version under the
title, and `[u] Check for updates` asks GitHub, once and only when you pick
it, whether a newer release exists. It prints the release page and installs
nothing. It needs `curl` (or `wget` on Linux). Like any web request it shows
GitHub your IP address, and the request carries a `User-Agent` of
`CodeLearner/<version>`; nothing else is sent.

To update, reinstall the way you installed it: run `install.sh` again, or
install the new package or AppImage.

## Running from the source

```bash
cd launcher
make
./launcher
```

Pick a course and it runs in place; quitting it (`q`) returns to this menu.
To run one course directly, without the launcher:

```bash
node python/learn.js
```

## Portuguese

The menus, prompts, lesson titles and lesson summaries are also available in
European Portuguese. Press `l` in the launcher to switch, and every course
started from it follows. To run a course directly, set the variable yourself:

```bash
CODELEARNER_LANG=pt-pt node python/learn.js
```

The lessons themselves are still in English, and code is never translated.

## The Libraries reference

The launcher lists **Libraries** under **REFERENCE**, after the courses. It is a
reference manual, not a course: pick C or Python, then a page. Each page is
laid out like a man page (NAME, SYNOPSIS, DESCRIPTION, PARAMETERS, RETURN VALUE,
ERRORS, STANDARDS, NOTES, APPLICATION USAGE, EXAMPLE, SEE ALSO) and runs its
example on your machine. Both manuals open with Common tasks: reading a file,
parsing text, sorting, timeouts and running another program.
The C manual then covers the standard C library, POSIX, Linux, the files in /proc,
/sys and /etc, terminal control sequences, and raylib; the Python manual
covers the built-ins, the standard library, the same files, and terminal
control sequences.
See [`libraries/README.md`](libraries/README.md) for how a page is written and
how it was verified.

## How it is built and checked

Every course is a folder of lessons written as plain data, run by one shared
engine in [`learn/`](learn/). A course's `course.js` says how its language
builds and runs a file and what its errors mean; `lessons/` holds the lessons.
[`docs/writing-a-course.md`](docs/writing-a-course.md) has the format and the
rules every lesson follows.

Two checks run on every course, locally and in CI:

```bash
node learn/check.js <course>           # every example, question, solution and starter
python3 learn/check-course.py <course> # every lesson played to the end
```

The first runs every example (and confirms the ones meant to fail do fail),
checks every question has its answer among the choices, and runs every
exercise's solution through the same checker you meet. The second plays each
lesson from the menu to WHAT YOU LEARNED. CI runs both for every course on
Linux and for eight of them on Windows; GTK and raylib are checked only on a
machine that has them, since the CI runner has an older GTK and no raylib.

## Layout

```
learn/        the shared lesson engine, the two checks, and web.js
<course>/     learn.js, course.js and lessons/ for each course
launcher/     the menu that starts every course (C)
libraries/    the reference manual
arduino/shim  the PC stand-in for the Arduino core
micropython/  sim.py, the PC stand-in for a Pico
assembly/os   the boot sector to kernel files, checked in QEMU by check-os.py
docs/         how a course is written, and notes on how a learner used one
```

## Contributing

Fixes to a lesson are the easiest contribution: point at the claim, show what
you ran and what it actually printed, fix the text. Larger changes, such as a
new course, start with an issue.

One rule matters more than the rest: **every claim in a lesson must be
something you actually ran.** See [`CONTRIBUTING.md`](CONTRIBUTING.md) for the
process, and [`docs/writing-a-course.md`](docs/writing-a-course.md) for how
lessons are written.

## License

[MIT](LICENSE). The courses use nothing beyond Node and each language's own
tools: no packages to install from npm or pip.
