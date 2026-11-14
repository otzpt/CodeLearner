# How a learner used the JavaScript course

A record of how the author worked through the JavaScript course as a student,
and what it suggests for how the courses are built. Read it next to
[`writing-a-course.md`](writing-a-course.md).

Each statement below is labelled by where it comes from:

- **Observed**: read directly in the learner's practice files.
- **Stated**: said by the learner. Not measured.
- **Suggestion**: a conclusion drawn from the above. Not tested.

## Source

Ten practice files made while following the JavaScript course:
`first.js` to `seventh.js`, `tenth.js`, `game.js` and `ccalc.js`, plus a
`package.json` for one npm package (`prompt-sync`). The files are dated 1 to
5 September.

They are copied here unchanged, typos included, in
[`learner-examples/javascript/`](learner-examples/javascript/). They are the
learner's work, not course material, and they are not run by any check. The
`node_modules` folder and `package-lock.json` were left out.

The learner's time tracker, Hackatime, shows about four hours of work on the
basics (**stated** by the learner, the tracker data was not checked here). The
file dates neither confirm nor contradict that: they show only the last time
each file was saved, and a tracker counts time spent working, not the span
between the first and the last save.

## What the practice files show

**Observed:**

1. **One file per module.** The files are numbered in the course's order.
   Modules 8 and 9 have no file. `tenth.js` opens with a comment saying they
   were skipped on purpose because they cover objects.
2. **Notes in the learner's own words.** Comments explain why something works
   that way, in Portuguese or English, with typos left in.
3. **Comparisons to languages the learner already knew.** For example `const`
   against `const int` in C, a missing `main` against Python, and `try/catch`
   against Python's `try/except`.
4. **The traps are written down.** `==` against `===`, `NaN !== NaN`,
   `sort()` ordering numbers as strings, `var` in a loop capturing one
   variable, `this` inside an arrow function, two names for one array, and
   `if (x = 5)` running without a warning.
5. **The result next to each demo**, as a comment such as
   `// outputs [10, 2, 9]`. These are not predictions: the learner copied
   them from the course's own output so the file could be read without
   switching windows (**stated**, 2026-11-14).
6. **An exercise at the end of most files**, written by the learner.
   `second.js` also ends with a short summary.
7. **Two small programs after the modules**: a number-guessing game and a
   calculator. Both read input with the `prompt-sync` package.
8. **Reading input was a hurdle.** `third.js` spends about 25 lines of
   comments on the fact that JavaScript has no `scanf` and that reading a line
   is asynchronous. The two later programs read input synchronously instead.
9. **Three notes are wrong.** `fifth.js` says `for...in` over an array gives
   the indices `"1"`, `"2"`, `"3"`. Running it in Node 24 gives `"0"`, `"1"`,
   `"2"`. `fourth.js` says `if (x = 5)` sets x to 5 "and then checks whether
   it is 5"; it tests whether 5 is truthy and compares nothing. `tenth.js`
   reads TypeError as "called something that is not in the function"; the
   course says "called something not a function". The rewritten course
   shows each one going wrong: lessons 7, 5 and 12.
10. **A module reaches into a skipped one.** `tenth.js` extends `Error` with a
    class and notes that this is "the same mechanism as module 9, which I
    skipped".

## What the learner said

**Stated:**

- Typing short snippets by hand gave muscle memory and a rough picture of how
  things look before the details were learned.
- Copying code without understanding it is not learning.
- The learner decides which modules to skip.
- Comparisons to a language the learner does not know make it more
  confusing, not less: the course's "same rule as C's `==` on a `char*`" and
  C++'s `std::string` taught nothing about JavaScript (2026-11-14).
- After comparing the old course with the rewrite (2026-11-14): the old one
  "felt good, and precise while keeping wording minimal". The rewrite's
  level of detail was right, but too many examples on one screen lost them
  halfway through the classes lesson. At most one example per terminal
  screen. Cutting the text itself in half went too far: the first lessons
  became useless.

## What this suggests for the courses

**Suggestions.** Item 4 is in the rewritten JavaScript course (rule 9 of
`writing-a-course.md`); item 3 became a plain mention of the earlier lesson. Items 1 and 2 were built and then removed at the
learner's request: the extra snippet repeated what they already did with the
lesson's own examples, and item 2 rested on misreading item 5 above.

1. **Keep a short example before the explanation, one the student can type.**
   The learner's stated benefit was muscle memory, which reading output does
   not give. Removed: the learner already types the lesson's own examples,
   one file per module, so a separate snippet only repeated that.
2. **Ask for a prediction before running.** Removed: it assumed the
   comments in item 5 were predictions, which they were not.
3. **Say when a module depends on one a student may skip.** Item 10 above: a
   student who skips module 9 still meets its mechanism in module 10.
   `tools/check-teaching-order.py` checks the numbered sequence only and
   does not account for skipped modules.
4. **Check the common wrong summaries.** Item 9 shows a student ending up with
   a wrong rule even though the course was available. Where a trap has an easy
   wrong summary (indices starting at 1), a question about it would catch it.
5. **Give input reading its own early note.** Item 8: the first obstacle for a
   student coming from C or Python was how to read a line. Checked on
   2026-11-13: reading input is module 3 of 14, and module 3 now opens with a
   short program to type that reads one line.

## Limits

- This is one learner, who already knew C and Python. The patterns may not hold
  for a first language.
- The practice files are the learner's, and the course text was not compared
  against them line by line.
