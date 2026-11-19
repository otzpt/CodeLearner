# Accessibility

CodeLearner is a set of terminal programs. This file records what the code
does today, what has not been tested, and how to report a barrier. It is not
a conformance claim.

## What the code does

- **Text only.** Everything is plain text on stdout, read from stdin. There is
  no GUI, no mouse input and no browser. Menus are numbered; you type a
  number or letter and press Enter.
- **Color is optional.** The lesson engine (`learn/app.js`) and the library
  reference print color only when stdout is a terminal and `NO_COLOR` is
  unset. With `NO_COLOR=1` the output has no escape codes except the
  screen-clear below.
- **Color is never the only signal.** Quiz results are written out ("Right.",
  "Not quite."), as are hints, warnings and "What this means:" lines.
- **Screens are cleared.** The launcher and the lesson engine clear the
  terminal between screens (`ESC[H ESC[2J ESC[3J`). There is no option to
  turn this off.
- **Language.** The launcher's `[l]` entry switches menu and interface text to
  European Portuguese; `CODELEARNER_LANG=pt-pt` does the same. Lesson text
  availability in Portuguese varies by course.
- **Editor.** Exercises are written in a normal file you open in any editor,
  so the editor, font, size and screen reader are your own.

## Not tested

No screen reader, braille display or voice-control tool has been used with
these courses. Whether the screen clearing interferes with any of them is
unknown. Reports are the way this gets answered.

## Known limitations

- Screen clearing cannot be disabled.
- The launcher draws a box of `+`, `=` and `|` characters as a header, which
  a screen reader may read out symbol by symbol.
- Terminal width is assumed to fit the layout; very narrow windows may wrap.
  This was not measured.

## Reporting a barrier

Open an issue at <https://github.com/otzpt/CodeLearner/issues> with the
terminal, OS, assistive tool (if any) and what you could not do. This is a
one-person project, so fixes are made as time allows.
