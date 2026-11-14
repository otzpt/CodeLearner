# Writing a course

How a course is put together, and how to write lessons that teach. For how a
real learner used a course, and why the courses look the way they do, see
[`learner-notes.md`](learner-notes.md).

## How a course is built

Every course runs on one engine, `learn/`, with Node. A course is a folder:

```
python/
  learn.js       the entry: require("../learn/main.js").start(__dirname)
  course.js      how this language builds and runs code, and reads its errors
  lessons/       one file per lesson, as plain data, run in file-name order
```

The launcher starts a course with `node <folder>/learn.js`. The engine draws
the menu and the screens, runs every example, checks exercises, explains
errors and saves progress in `~/codelearner/<course>/`.

### course.js

| Field | What it is |
| --- | --- |
| `id`, `title`, `titlePt` | folder name, and the menu title in English and Portuguese |
| `file` | the name an example is written to, such as `main.c`; its extension names exercise files |
| `steps(file, dir, opts)` | the commands that build and run a file: every step but the last builds, the last runs and gets the typed input |
| `parseError(text, file)` | turns a compiler or runtime message into `{ errorLine, line }` |
| `errors` | `[pattern, English, Portuguese]` plain-word explanations, tried in order |
| `warnings(text)` | optional: compiler warnings from a build step, shown above the output |
| `starter` | the file an exercise starts from, unless the exercise gives its own |
| `howToRun(name)` | the command the student types to run an exercise file |
| `env`, `timeout` | optional extra environment and a time limit in milliseconds |
| `buildOnly`, `unavailable()` | for courses whose programs open windows (GTK, raylib): build, do not run; and say when the library is missing |

`errorsOnStdout` and `stdoutWarning` exist for toolchains that print errors or
warnings on standard output (dotnet, git). The C#, Git and web courses show
how they are used.

### A lesson

```js
module.exports = {
  id: "loops",                       // never change it: progress is saved by id
  title: "Repeating with loops",
  titlePt: "Repetir com ciclos",
  screens: [                         // one terminal screen each
    [
      "A paragraph of text.",
      { code: "...", },              // an example, run when shown
      { quiz: "...", choices: ["a", "b"], answer: 2, why: "..." },
      { tip: "One line to remember." },
    ],
    [
      { exercise: "count", task: "...", expected: ["1", "2"], hints: ["..."], solution: "..." },
    ],
  ],
  recap: ["...", "..."],             // WHAT YOU LEARNED, at the end
  recapPt: ["...", "..."],           // the same, in Portuguese
};
```

An example block can also have:

- `error: true`: the example is meant to fail; its error is shown and explained.
- `warning: true`: the example is meant to make the compiler warn.
- `run: false`: shown only, never run (a terminal command, for instance).
- `output: [...]`: for an example that reads input, the terminal transcript,
  with what the user types in braces at the end of a line: `"Name? {Ana}"`.
  The engine types the input, checks the real output against it, and shows
  the transcript.
- `setup` and `setupLabel`: code that runs before the example without being
  shown, such as a helper from an earlier screen.
- `flags` and `env`: extra build options or environment for this example.

An exercise's `expected` uses the same transcript form for input. The student
writes the program in a real file, the engine runs it and compares the output
line by line, and says which line differs.

## The rules

### 1. Start from zero, and explain each thing on its own terms

Every course is for someone who has never programmed. Never explain through
another language, not even C or Python: "the same rule as C's `==` on a
`char*`" taught a learner nothing about JavaScript and confused them about C.
A course may say which other course covers a prerequisite (the GTK course
assumes C basics), never compare to it.

### 2. At most one example per screen, minimal words, full detail

Too many examples on one screen lost a learner halfway through a lesson. Too
few words made lessons useless. Keep every detail that matters, in short
sentences, and give each example its own screen. `learn/check.js` fails a
screen with two examples.

### 3. Nothing is claimed without being shown

Every example runs when it is shown, so the output on screen is the real
output of the code above it. Where something cannot run here (a boot sector,
a window), say so and say why. The web courses use `learn/web.js`, a small
stand-in for a browser, and the hardware courses use stand-ins for the
board; anything that plays the outside world (`sim.click`, `sim_set_external`)
is called PC-only every time it appears.

### 4. Teach the trap, and show the wrong summary failing

What is worth a screen is the thing that makes people lose an afternoon:
`"5" + 1`, `=` inside `if`, positions starting at 0, `==` on strings. Where a
trap has an easy wrong summary, show the case where that summary gives the
wrong answer. A learner's notes said `for...in` gives `"1"`, `"2"`, `"3"` and
that `if (x = 5)` checks whether x is 5; the JavaScript lessons now show both
failing.

### 5. An exercise only needs what came before it

A task that needs something a later lesson teaches is a broken task, not a
hard one. When a lesson uses something from an earlier lesson that is easy to
skip, say which lesson it came from.

### 6. The output is the specification, not the solution

An exercise states what the program must print (and what is typed). Any code
that produces it is correct; the solution is one way, shown only on request.

### 7. Errors and warnings are teaching material

Show the real error, with the line, and explain it in one plain sentence from
`course.js`'s `errors`. Compiled courses build with warnings on.

## The Portuguese text

`CODELEARNER_LANG=pt-pt` translates the engine's menus, prompts and feedback
(`learn/pt.js`), and each lesson's `titlePt` and `recapPt`. The lesson text
and every piece of code stay as they are. `learn/check.js` fails a lesson
without them.

## Adding a lesson or a course

A lesson is one more file in `lessons/`, named so it sorts into place. A new
course is a folder with the three parts above, and one `COURSE(...)` line in
`launcher/src/main.c`, plus its name in `COURSES` in
`.github/workflows/release.yml`.

## Verifying before committing

```bash
node learn/check.js <course>          # every example, quiz, solution and starter
python3 learn/check-course.py <course> # every lesson played to the end
```

`check.js` runs every example (and checks error examples fail and
transcripts match), checks every quiz answer exists, runs every solution
through the same checker the student meets, and checks that every starter
file runs. `check-course.py` plays each lesson from the menu to WHAT YOU
LEARNED and confirms a correct practice file is accepted. CI runs both for
every course.
