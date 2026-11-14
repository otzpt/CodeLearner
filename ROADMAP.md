# Roadmap

Not scheduled: a map, so none of this gets lost or re-derived later. What is
already built is in `devprogress.md`; how a course is built is in
`docs/writing-a-course.md`.

## The rule every course follows

Every course starts from zero, for someone who has never programmed, and
explains each thing on its own terms: no comparison to another language. At
most one example per screen, every example run, every exercise checked by
running the student's own file. Each course still follows what matters in
its own language (ownership in Rust, pointers in C, the cascade in CSS)
rather than one shared list of topics.

## Where each course stops, and what comes next

Rewritten from zero on 2026-11-14; the old courses went further in places.
Next topics, roughly in order:

- **C** (14 lessons, to structs, files and AddressSanitizer): function
  pointers, bit operations, variadic functions, the preprocessor, inline
  assembly. Issues #1-#14.
- **C++** (12, to inheritance, exceptions and files): templates, RAII and
  smart pointers, std::map and algorithms, move semantics.
- **Python** (14, to classes, files and a project): modules and packages,
  comprehensions, the standard library (json, pathlib, argparse), testing.
- **JavaScript** (13, to classes, input and errors): promises and
  async/await in depth, fetch, modules, files with fs, JSON.
- **Java** (12, to interfaces, exceptions and files): collections (HashMap),
  generics, records, streams.
- **C#** (11, to interfaces, exceptions and files): collections
  (Dictionary), LINQ, records, async.
- **Rust** (12, to Result, files and a project): traits, generics,
  lifetimes, collections (HashMap), iterators, modules.
- **Assembly** (9, to text and a look at boot sectors): reading input with
  syscalls, calling C, then the OS track kept in `assembly/os` (two-stage
  loader, protected and long mode, a C kernel, interrupts) as lessons.
- **Arduino** (8) and **MicroPython** (7), on PC stand-ins: sensors over
  I2C/SPI, interrupts on Arduino, servo and motor control, state machines
  for a robot.
- **Git** (8): rebase, stash, bisect, pull requests on a real host.
- **HTML** (7), **CSS** (6), **Web JavaScript** (5): the box model and layout
  (flexbox, grid) need a stand-in that lays out boxes, which
  `learn/web.js` does not do yet; fetch from the page.
- **Security** (7): HTTP and headers against a local server, plain HTTP
  readable on the wire (the old roadmap's next module), session cookies.
- **GTK** (4) and **raylib** (4), build-only: lists and dialogs in GTK;
  sprites, sound and a full game loop in raylib.

## Libraries: a reference manual, not a course

[`libraries/`](libraries/) is a man-page style manual for C and Python, opening
with common tasks and then one page per header, function family, module or
system file, each giving the signature, parameters, return value and errors,
with an example run on this machine. It answers "which library, call or file does this, and how
do I use it" the way official documentation would, and has no lessons or
exercises on purpose. The C track includes a reference page for each of 21
headers (`stdio.h` to `termios.h`). Further languages are one folder of
pages and a row in `TRACKS` in `libraries/src/main.c`.

## New languages — not started, no folders yet

Each entry is a target arc, not a spec. Per the one rule above, each should
be shaped by what actually matters in that language, not forced into the
same module skeleton as C.

- **TypeScript** — types → interfaces → generics → narrowing → advanced
  types → projects.
- **Go** — structs → interfaces → concurrency → networking → CLI/backend
  projects.
- **Bash** — shell basics → variables → pipes → scripting → automation →
  system administration.
- **SQL** — queries → joins → aggregation → subqueries → indexes →
  transactions.
- **Lua** — syntax → tables → functions → modules → metatables → scripting
  projects.
- **Pascal** — fundamentals → procedures → records → pointers → data
  structures.
- **HolyC** — deliberately empty. No folder, no stub, until there's an
  actual, properly-thought-out curriculum for it — not added just to fill
  the list.

## Across every language, existing or new

- Real exercises, not just explanations.
- Progressive difficulty.
- Automatic tests where possible.
- Debugging challenges.
- Small projects after major sections, final projects combining everything.
- Common mistakes explicitly taught, not just correct usage.
- Language-specific best practices — not generic advice reworded per
  language.
- No filler modules just to increase the module count.
- Keep the Wiki → lesson → exercise connection strong — a concept
  introduced in a lesson should be findable and an exercise should exercise
  it, not the other way around.

## Packaging and distribution

**A desktop entry, so the launcher appears in application menus.** Done for
Linux: `assets/com.otzpt.codelearner.desktop` is the one tracked definition
(`Terminal=true`, because this is a TUI and a launcher that starts it without a
terminal runs it headless with nothing on screen). The release bundle carries it
and `install.sh` places it, with the icon, under `PREFIX/share/applications`
and rewrites `Exec=` to the wrapper's full path; the `.deb` and the Arch
package install it under `/usr/share`, and the AppImage uses the same file. CI
validates it with `desktop-file-validate` and checks that the `.deb` and the
Arch package contain the entry and the icon. `install.sh` was run against a
fake release (prefix with a space in it, uninstall, and a release without the
files). Not run against a real release yet: that needs the next release tag.

Windows has the same gap for the same reason: `install.ps1` adds a PATH
entry and no Start Menu shortcut. A `.lnk` in
`%APPDATA%\Microsoft\Windows\Start Menu\Programs` is the counterpart. Still
open: `install.ps1` has never been run (see the README note) and nothing here
can run PowerShell, so a shortcut written blind would be a second unverified
change in an unverified script. It needs someone with a Windows machine.

## C-specific backlog

Filed as tracked issues rather than left in a file, since they're scoped
enough to work individually: [issues #1-#14](https://github.com/otzpt/CodeLearner/issues).
`c/ROADMAP.md` places them in the C course's next steps. All 14 are still
open. The Libraries reference covers the headers named in #4 as reference
pages.
