# Roadmap

Not scheduled, not started — a map so none of this gets lost or re-derived
from scratch later. `c/ROADMAP.md` is a different, narrower thing: the path
past the existing C course's 12 modules. This file is the whole project:
new languages, and what "done" looks like for the languages that already
exist.

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

## Target arcs for the four languages already in the repo

Not commitments to build in this order — a statement of what a *finished*
version of each course covers, since none of the four currently states an
end point beyond its current module count.

- **Python**: fundamentals → OOP → modules → files → APIs → practical
  projects.
- **C++**: C++ basics → STL → OOP → templates → modern C++ → projects.
  (Module 9, classes and RAII, is where the current course ends — templates
  and modern C++ are past that.)
- **Java**: OOP → collections → exceptions → generics → concurrency →
  projects.
- **JavaScript**: fundamentals → DOM → async → APIs → modules → Node.js.

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
- **Assembly** — registers → memory → instructions → stack → syscalls →
  calling conventions. (Distinct from `c/ROADMAP.md` section 9, which is
  "read enough asm to understand your own compiled C" — this would be a
  full course in its own right.)
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

## C-specific backlog

Filed as tracked issues rather than left in a file, since they're scoped
enough to work individually: [issues #1-#14](https://github.com/otzpt/CodeLearner/issues).
See `c/ROADMAP.md` for how they fit the existing course's path.
