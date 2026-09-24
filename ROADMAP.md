# Roadmap

Not scheduled, not started — a map so none of this gets lost or re-derived
from scratch later. `c/ROADMAP.md` is a different, narrower thing: the path
past the existing C course's 19 modules. This file is the whole project:
new languages, and what "done" looks like for the languages that already
exist.

Two courses outside the original five languages this file tracks have
since been started -- see the README's own course tables, not this file,
for their current state: **GUI (GTK)**, delivered in C since there is no
separate "GTK language" to write a menu in; and **Assembly**, which is
started below and whose own entry stays in this file since it was one of
the "new languages" this roadmap already named.

## The one rule everything else follows

**Don't make every language follow the same curriculum structure.** Python
should feel like Python, C should feel like C, Rust should teach ownership
properly on Rust's own terms. The common framework — write the course in
the language it teaches, verify every output, teach the trap not the
syntax, no filler modules — is the quality bar every course is held to.
It is not a template every course's *content* has to match. A Rust course
structured like the C course would be teaching Rust as if it were C with
different syntax, which is a worse Rust course than one that starts from
what actually makes Rust Rust.

## Target arcs for the languages already in the repo

Not commitments to build in this order — a statement of what a *finished*
version of each course covers, since none of these currently states an end
point beyond its current module count. Each arc below is broken into
concrete next modules, numbered as a continuation of that course's current
last module, the same level of detail `c/ROADMAP.md` now has for C. None
of this is designed as deeply as the C plan yet — it's the map for where to
go next, not finished module content.

A constraint that applies to every language here, from
`docs/writing-a-course.md`: each course must stay "a single self-contained
CLI program with no runtime dependencies beyond that language's own
toolchain." That rules out reaching for a third-party package (Python's
`requests`, a JS `npm install`) for anything below — an "APIs" module
means the language's own stdlib HTTP client, not the ergonomic library
everyone actually reaches for in real code. Worth teaching that trade-off
explicitly when it comes up (the stdlib way is uglier and that's exactly
why the ecosystem library exists), not silently working around it.

- **Python**: fundamentals → OOP → modules → files → APIs → practical
  projects.
  - Module 11 — modules and packages: built. `import`, `if __name__ ==
    "__main__":` (demonstrated live against this course's own main.py, not
    described), writing your own importable module, a brief note on
    `pip`/venv for when a real project needs a dependency (without the
    course itself ever depending on one).
  - Module 12 — files and context managers: built. `open`/`.read`/`.write`/
    `.close` and the trap (forgetting to close, or an exception before
    close leaking the handle), `with open(...) as f:` proven — not just
    asserted — to close on the way out even when an exception is raised
    and caught inside the block, `.readlines()` keeping `\n`, `.write()`
    never adding one, and `json.dump`/`json.load` round-tripping a dict
    through a real file.
  - Module 13, working with APIs: built. `urllib.request` + `json`, both
    stdlib, deliberately not `requests` (see the constraint above); a real
    call to a real endpoint the student can inspect, with real error
    handling for a network failure (`HTTPError` vs. `URLError`, caught
    differently), not a happy-path-only demo.
  - Practical projects (project-practice track, not a numbered module,
    same reasoning as the C course's issue-tracked "Practical C" backlog):
    a CLI tool that reads a config file, calls an API, and writes a
    report — combines 11-13 in one program.
- **C++**: C++ basics → STL → OOP → templates → modern C++ → projects.
  - Module 10 — maps and sets: built. `std::map` (and the trap: `[]` on a
    missing key silently inserts a default value rather than telling you
    it was missing — `.count()` is the safe check), `std::set` (unique,
    sorted, `.insert()` on an existing value is a silent no-op), and
    `std::find` (`end()` as the "not found" signal, the same role `NULL`
    plays for a pointer).
  - Module 11 — templates: built. A function template (`myMax<T>`) and a
    class template (`Box<T>`), motivated the same way module 9's classes
    are: what the duplicated version looks like first, then the fix; a
    real, actually-triggered template compile error (a type missing
    `operator+`) shown by invoking `g++` live and printing its real error
    text, honestly gated on the compiler actually having run and failed
    the expected way rather than narrating any failure as if it were that.
  - Module 12: modern C++ idioms — smart pointers (`unique_ptr`,
    `shared_ptr`) as the RAII idea from module 9 applied to dynamically
    allocated objects specifically, move semantics (`std::move`, why a
    copy is sometimes wasted work), `auto` and structured bindings beyond
    what module 2 already introduced.
  - Practical projects track, same shape as every other language here.
- **Java**: OOP → collections → generics → concurrency → projects.
  - Module 11 — the Collections Framework: built. `List`/`Map`/`Set` as
    interfaces with real implementations, and the trap Java hides
    somewhere different from every sibling course: `HashMap.get()` on a
    missing key returns `null` — no exception, no inserted default —
    with `getOrDefault()` as the fix.
  - Module 12 — generics: built. The `Object`-typed container and its
    real, caught `ClassCastException`, then a generic class/method/bounded
    type parameter, then type erasure verified at runtime (`new
    ArrayList<String>().getClass() == new ArrayList<Integer>().getClass()`
    actually prints `true`), contrasted with C++'s per-type template
    instantiation from that course's own module 11.
  - Module 13, concurrency: built. `Thread`, `Runnable`, a real, observed
    race condition on a shared counter (not simulated — module 9's
    leak-detector-output precedent from the C course is the model: run the
    broken version, show what actually happens), then `synchronized` as
    the fix, measured before and after.
  - Practical projects track.
- **JavaScript**: fundamentals → async → APIs → modules → Node.js
  specifics → practical projects. Correction to an earlier draft of this
  arc: `async`/`await` is not a gap — module 3 (reading input) already
  introduces it, out of necessity (Node's own stdin is asynchronous), and
  every module since has been an `async function` as a result. Module 11
  (built) is what module 3 never named: the Promise object `await` has
  been unwrapping the whole time.
  - Module 11 — Promises: built. A callback (the pre-Promise way), what a
    Promise actually is (pending → fulfilled/rejected, once, via
    `.then()`/`.catch()` or `await`, the same object either way), and
    `Promise.all` for running independent awaits concurrently instead of
    one after another.
  - Module 12 — working with APIs: built. Real, live `fetch` calls (not
    a captured-once string) against `jsonplaceholder.typicode.com`, each
    wrapped in `AbortSignal.timeout(...)` with a clearly-labeled,
    already-verified-real fallback if the live attempt genuinely fails;
    the central trap proven live — a 404 does not reject the Promise,
    only `response.ok`/`.status` reveals it; a genuine DNS failure
    contrasted right after, which does throw. The challenge itself stays
    network-independent (a literal JSON string), keeping its expected
    output exact per this project's own rule 5.
  - Module 13, modules: built. `require`/CommonJS vs. `import`/ESM, since
    Node's own ecosystem genuinely uses both and a student will meet both
    in real code; `module.exports`/`export` for writing your own.
  - Module 14, Node-specific stdlib: built. `fs.readFileSync` (blocking)
    vs. `fs.promises.readFile` (async), `process.argv` (command-line
    arguments, verified via a real child process printing its own argv),
    and what makes Node different from browser-JavaScript at the platform
    level, not just "no DOM here."
  - Practical projects track. DOM/browser JavaScript is deliberately not
    in this arc: this course is a Node.js CLI program per the "no
    dependencies beyond the language's own toolchain" rule, and DOM APIs
    need a browser, not a terminal — worth its own course or module
    format if it's ever built, not squeezed into this one.
- **C#**: fundamentals → nullable/IDisposable → LINQ → async/await →
  generics and interfaces in depth → delegates and events → practical
  projects. Deliberately built to sit next to Java's course rather than
  duplicate it — see the README's own C# section for where the two
  genuinely diverge (default parameters, `string ==`, checked exceptions,
  value vs reference types) and where they don't.
  - Module 11 — LINQ: built. The manual loop first, then `Where`/`Select`
    method syntax, `Sum`/`Count`/`OrderBy(Descending)`, and the trap: a
    LINQ query is lazy, running at enumeration time rather than creation
    time — proven by mutating the source list after building the query
    and showing the addition is still included.
  - Module 12 — async/await: built. `Task` as C#'s Promise, deliberately
    compared against JavaScript's own module 11; `Task.Delay` vs. blocking
    `Thread.Sleep`; `Task.WhenAll` with a REAL `Stopwatch`-measured timing
    difference (sequential ~200ms vs. concurrent ~100ms for two 100ms
    delays); an awaited faulted `Task` caught with ordinary `try`/`catch`.
    The one new module in this batch that needed a structural decision:
    rather than converting the whole menu's module table from `Action` to
    `Func<Task>` (touching all 11 other modules), the one async module is
    wrapped in `() => LessonsAdvanced.Lesson12Async().GetAwaiter().GetResult()`
    — a single new array row, the standard safe pattern for a console app
    with no captured `SynchronizationContext`, with a note in the module
    itself pointing at this exact line as the deliberate exception to
    "async propagates all the way to the entry point."
  - Module 13, generics and interfaces in depth: built. A generic class
    with constraints (`where T : IComparable<T>`), a default interface
    method that dispatches on runtime type (not the compile-time reference
    type), going past what module 8's structs/records already touched;
    a real, verified `CS7036` compiler error from `MemoryExtensions`'s
    always-in-scope `CompareTo` extension method colliding on arity.
  - Module 14, delegates and events: built. `Action`/`Func` as behavior
    passed by value, a custom `delegate` type, the `event` keyword
    restricting outside code to `+=`/`-=` (a real `CS0070` for anything
    else), bridging back to the function-pointer-style dispatch table
    every course's own `Program.cs`/`main.c`/etc. already uses.
  - Practical projects track.

## Assembly — started

[`assembly/`](assembly/), modules 1-4: registers and syscalls, comparing
and branching, loops, then reading input and building a number-guessing
game out of the three before it. Module 4 is where the course stops being
read-only: everything up to it prints, and it is the first one whose own
example program waits for you. Target arc from here: memory addressing →
the stack in more depth (currently only what a function's own
prologue/epilogue needs) → more instructions → calling conventions beyond
"arguments in rdi/rsi/...".
Distinct from `c/ROADMAP.md` section 9, which is "read enough asm to
understand your own compiled C" for someone taking the C course, not a
full course in its own right.

Linux/x86-64 only, and not planned to become cross-platform: the whole
course is built on Linux syscalls made directly (`syscall` plus a Linux
syscall number in `rax`), which have no Windows equivalent to translate
to -- a Windows version would need a real second implementation against
a completely different ABI (different argument registers, no directly
callable stable syscall interface), not a path or flag change the way
every other course's Windows support is. The launcher reflects this:
Assembly shows as unavailable on a Windows build, the same "coming soon"
treatment as a language with no course yet, not a broken link.

## New languages — not started, no folders yet

Each entry is a target arc, not a spec. Per the one rule above, each should
be shaped by what actually matters in that language, not forced into the
same module skeleton as C.

- **Rust** — ownership → borrowing → lifetimes → traits → `Result`/`Option`
  → async → projects. Ownership is the whole point of Rust; it should get
  the same weight module 9 (memory) gets in the C course, not a single
  module in passing.
- **TypeScript** — types → interfaces → generics → narrowing → advanced
  types → projects.
- **Go** — structs → interfaces → concurrency → networking → CLI/backend
  projects.
- **Bash** — shell basics → variables → pipes → scripting → automation →
  system administration.
- **HTML** — semantic HTML → forms → accessibility → metadata → practical
  pages.
- **CSS** — layout → Flexbox → Grid → responsive design → modern CSS.
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
- Cross-language comparisons where genuinely useful (e.g. `==` on strings:
  C compares addresses, C++'s `std::string` compares content, Python
  always compares content, Java is the C++ trap again). Only where the
  comparison teaches something; not for its own sake.
- No filler modules just to increase the module count.
- Keep the Wiki → lesson → exercise connection strong — a concept
  introduced in a lesson should be findable and an exercise should exercise
  it, not the other way around.

## Packaging and distribution

**A desktop entry, so the launcher appears in application menus.** Today
`codelearner` is a terminal command and nothing else: rofi's `drun` mode,
wofi, and every GNOME/KDE application menu read `.desktop` files, and this
project installs none. `rofi -show run` finds it, because that mode searches
PATH, but that is not where anyone looks for an application.

One `.desktop` file already exists, written inline by the AppImage step in
`.github/workflows/release.yml` and living only inside the AppImage, where
nothing registers it with the system unless the user integrates the AppImage
by hand. The work is to lift that file out into the repository as a real
tracked file next to `assets/icon.png`, and install it from every path that
installs anything: `install.sh` into `~/.local/share/applications`, the .deb
and the Arch package into `/usr/share/applications`, with the AppImage step
then copying the same file instead of printing its own copy. One definition,
four consumers.

`Terminal=true` is not optional in it. This is a TUI, and a launcher that
starts it without a terminal runs the process headless with nothing visible
on screen.

Windows has the same gap for the same reason: `install.ps1` adds a PATH
entry and no Start Menu shortcut. A `.lnk` in
`%APPDATA%\Microsoft\Windows\Start Menu\Programs` is the counterpart, and it
should land in the same change so the two platforms do not drift.

## C-specific backlog

Filed as tracked issues rather than left in a file, since they're scoped
enough to work individually: [issues #1-#14](https://github.com/otzpt/CodeLearner/issues).
See `c/ROADMAP.md` for how they fit the existing course's path.
