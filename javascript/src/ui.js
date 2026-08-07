// ui.js - the pieces every lesson uses to draw the screen.
//
// Same purpose and the same visual style as the other three courses' ui
// modules, kept as a separate implementation rather than shared code -- see
// docs/writing-a-course.md for why.
//
// Node has no built-in synchronous input the way C's scanf, Python's
// input() or even C++'s cin do -- reading a line is asynchronous, so every
// function here that reads one is `async` and every lesson that calls one
// has to be `async` too, all the way up to main.js's own loop.
//
// SIMPLIFICATION: readline's promise-based rl.question() looks like the
// obvious choice and is not used here. Verified: once stdin has already
// reached EOF -- which happens immediately with a pre-filled, closing pipe
// like `printf "a\nb\n" | node script.js` -- a SECOND rl.question() call
// silently never resolves, hanging the program with no error. A real
// terminal never closes stdin mid-session, so this never bites a student
// running the course normally, but it does bite the exact way people test a
// script non-interactively. `ask()` below drives readline's async iterator
// by hand instead, which returns "" once input is exhausted rather than
// hanging -- verified against both a closed pipe and a real terminal.

const readline = require("node:readline");
const { stdin, stdout } = require("node:process");

const WIDTH = 54; // inside width of the frame, matching the other courses

const rl = readline.createInterface({ input: stdin });
const lines = rl[Symbol.asyncIterator]();

async function ask(prompt) {
  stdout.write(prompt);
  const { value, done } = await lines.next();
  return done ? "" : value;
}

function clearScreen() {
  // ANSI codes, not console.clear(): console.clear() relies on the same
  // terminal capability that made the C course's system("clear") print
  // "TERM environment variable not set" outside a real terminal.
  stdout.write("\x1b[H\x1b[2J\x1b[3J");
}

async function waitEnter() {
  await ask("\n  Press ENTER to continue...");
}

function rule() {
  console.log("  " + "-".repeat(WIDTH));
}

function paddedLine(text, border) {
  console.log(`  ${border} ` + text.padEnd(WIDTH - 3) + border);
}

function frame(fill) {
  console.log("  +" + fill.repeat(WIDTH - 2) + "+");
}

function title(text) {
  console.log();
  frame("=");
  paddedLine(text, "|");
  frame("=");
  console.log();
}

function heading(text) {
  console.log(`\n  ${text}`);
  rule();
}

async function askYes(questionText) {
  const answer = await ask(`\n  ${questionText} (y/N): `);
  return answer.slice(0, 1).toLowerCase() === "y";
}

function exercise(number) {
  console.log(`\n  >> EXERCISE - MODULE ${number}`);
  rule();
}

async function question(text, correct, why) {
  console.log(`\n  ${text}`);
  const answer = await ask("  Your answer: ");

  const right = answer.trim().toLowerCase() === correct.trim().toLowerCase();
  if (right) {
    console.log(`\n  CORRECT.  ${why}`);
  } else {
    console.log(`\n  NOT QUITE. The answer is: ${correct}`);
    console.log(`             ${why}`);
  }
  return right;
}

// A task to write in a real file.
//
// `input` is every line the program will read while producing `expected` --
// empty for a task that reads nothing. Together they are the actual
// specification: run the solution, type `input`, get `expected`, however
// the code that does it is written. `solution` is one way of getting there
// and appears only after a confirmation.
async function challenge(task, input, expected, solution) {
  console.log("\n  >> WRITE THIS YOURSELF, in a real file");
  rule();

  for (const line of task) {
    console.log(`  ${line}`);
  }

  if (input.length > 0) {
    console.log("\n  Try it with this input:\n");
    for (const line of input) {
      console.log(`      ${line}`);
    }
  }

  if (expected.length > 0) {
    console.log("\n  It must print:\n");
    for (const line of expected) {
      console.log(`      ${line}`);
    }
    console.log("\n  That output is the whole specification. Any code that");
    console.log("  produces it is correct.");
  }

  console.log("\n  Try it first. Run with:");
  console.log("    node test.js");

  if (!(await askYes("Want to see example code?"))) {
    return;
  }

  console.log();
  rule();
  for (const line of solution) {
    console.log(`  ${line}`);
  }
  rule();
  console.log("  This is EXAMPLE CODE, not the answer. It is one way to get");
  console.log("  that output; yours may look nothing like it and still be");
  console.log("  right -- or better. Compare the output, not the code.");
}

function closeInput() {
  rl.close();
}

module.exports = {
  ask,
  clearScreen,
  waitEnter,
  rule,
  title,
  heading,
  askYes,
  exercise,
  question,
  challenge,
  closeInput,
};
