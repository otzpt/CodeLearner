// The engine behind the lessons: drawing screens, reading keys, running
// code, checking exercises and remembering progress. The lessons themselves
// are plain data in lessons/, so writing a lesson never means touching this.

const fs = require("node:fs");
const os = require("node:os");
const path = require("node:path");
const readline = require("node:readline");
const { spawnSync } = require("node:child_process");

const WIDTH = 66;
const PRACTICE_DIR = process.env.CODELEARNER_JS_DIR || path.join(os.homedir(), "codelearner-js");
const PROGRESS_FILE = path.join(PRACTICE_DIR, ".progress.json");

// ---------------------------------------------------------------- input

// readline's async iterator, not rl.question(): rl.question() silently drops
// every answer after the first when input is piped (checked on Node 26),
// and the walk test pipes its input.
const rl = readline.createInterface({ input: process.stdin });
const lines = rl[Symbol.asyncIterator]();

class InputClosed extends Error {}

async function ask(prompt) {
  process.stdout.write(prompt);
  const { value, done } = await lines.next();
  if (done) {
    throw new InputClosed();
  }
  return value.trim();
}

function close() {
  rl.close();
}

// ---------------------------------------------------------------- output

const COLOR = process.stdout.isTTY && !process.env.NO_COLOR;
const paint = (code) => (text) => (COLOR ? `\x1b[${code}m${text}\x1b[0m` : text);
const bold = paint("1");
const dim = paint("2");
const green = paint("32");
const red = paint("31");
const cyan = paint("36");
const yellow = paint("33");

function clear() {
  process.stdout.write("\x1b[H\x1b[2J\x1b[3J");
}

// Break a paragraph into lines no wider than the screen.
function wrap(text, indent = "  ") {
  const out = [];
  for (const paragraph of text.split("\n")) {
    let line = "";
    for (const word of paragraph.split(" ")) {
      if (line && (line + " " + word).length > WIDTH - indent.length) {
        out.push(indent + line);
        line = word;
      } else {
        line = line ? line + " " + word : word;
      }
    }
    out.push(indent + line);
  }
  return out.join("\n");
}

function say(text) {
  console.log(wrap(text));
  console.log();
}

function rule() {
  console.log("  " + dim("-".repeat(WIDTH - 2)));
}

function showCode(code) {
  for (const line of code.split("\n")) {
    console.log("    " + cyan(line));
  }
}

// ---------------------------------------------------------------- running code

// Run a file with node, typing `input` into it, and report what happened.
function runFile(file, input = []) {
  const result = spawnSync(process.execPath, [file], {
    cwd: path.dirname(file),
    input: input.length ? input.join("\n") + "\n" : "",
    encoding: "utf8",
    timeout: 5000,
  });
  const output = result.stdout.replace(/\n+$/, "").split("\n").map((l) => l.trimEnd());
  if (result.error && result.error.code === "ETIMEDOUT") {
    return { ok: false, output, timeout: true };
  }
  if (result.status !== 0) {
    const errorLine = result.stderr.split("\n").find((l) => /^\w*Error\b/.test(l)) || result.stderr.trim();
    const where = result.stderr.match(/\.js:(\d+)/);
    return { ok: false, output, errorLine, line: where ? Number(where[1]) : null };
  }
  return { ok: true, output: result.stdout === "" ? [] : output };
}

// Run a snippet the same way, from a temporary file.
function runCode(code, input = []) {
  const dir = fs.mkdtempSync(path.join(os.tmpdir(), "codelearner-js-"));
  try {
    const file = path.join(dir, "example.js");
    fs.writeFileSync(file, code + "\n");
    return runFile(file, input);
  } finally {
    fs.rmSync(dir, { recursive: true, force: true });
  }
}

// What an error message means, in plain words.
function explainError(errorLine) {
  const name = (errorLine.match(/^(\w*Error)/) || [])[1];
  const help = {
    ReferenceError: "a name that does not exist. Check the spelling and capitals.",
    SyntaxError: "code JavaScript cannot read: often a missing quote, bracket or brace.",
    TypeError: "a value used in a way it does not allow.",
    RangeError: "a number outside what is allowed.",
  };
  return help[name] || "read the message: it names the problem and the line.";
}

function showError(result, indent = "  ") {
  console.log(indent + red(result.errorLine));
  if (result.line) {
    console.log(indent + dim(`(line ${result.line})`));
  }
  console.log();
  say(yellow("What this means: ") + explainError(result.errorLine));
}

// Same lines, ignoring spaces at the end of a line.
function sameOutput(expected, got) {
  return expected.length === got.length && expected.every((line, i) => line.trimEnd() === got[i]);
}

// ---------------------------------------------------------------- progress

function loadProgress() {
  try {
    return JSON.parse(fs.readFileSync(PROGRESS_FILE, "utf8"));
  } catch {
    return { done: [] };
  }
}

function saveProgress(progress) {
  fs.mkdirSync(PRACTICE_DIR, { recursive: true });
  fs.writeFileSync(PROGRESS_FILE, JSON.stringify(progress, null, 2) + "\n");
}

// ---------------------------------------------------------------- blocks

// `setup` is code that runs before the example without being shown, such as
// the ask() helper of lesson 10.
async function codeBlock(block) {
  if (block.setup) {
    console.log("    " + dim(block.setupLabel || "(the ask helper goes here)"));
  }
  showCode(block.code);
  if (block.run === false) {
    console.log();
    return;
  }
  const full = block.setup ? block.setup + "\n\n" + block.code : block.code;
  const result = runCode(full, block.input || []);
  console.log();
  if (block.input) {
    console.log("  " + dim(`Run, typing: ${block.input.join(", ")}`));
  }
  if (result.ok) {
    console.log("  " + dim("Output:"));
    for (const line of result.output) {
      console.log("    " + line);
    }
  } else if (block.error) {
    console.log("  " + dim("Output:"));
    for (const line of result.output.filter((l) => l !== "")) {
      console.log("    " + line);
    }
    showError(result, "    ");
    return;
  } else {
    throw new Error(`an example failed: ${result.errorLine || "timeout"}`);
  }
  console.log();
}

async function quizBlock(block) {
  console.log("  " + bold("Quick question: ") + block.quiz);
  console.log();
  block.choices.forEach((choice, i) => console.log(`    ${i + 1}) ${choice}`));
  console.log();
  for (let attempt = 1; ; attempt++) {
    const answer = await ask("  Your answer (number): ");
    if (Number(answer) === block.answer) {
      console.log("  " + green("Right. ") + block.why);
      break;
    }
    if (attempt === 2) {
      console.log("  " + red("Not quite. ") + `It is ${block.answer}) ${block.choices[block.answer - 1]}. ` + block.why);
      break;
    }
    console.log("  " + red("Not quite.") + " Have another go.");
  }
  console.log();
}

async function exerciseBlock(block, lessonNumber) {
  const file = path.join(PRACTICE_DIR, `${String(lessonNumber).padStart(2, "0")}-${block.exercise}.js`);
  if (!fs.existsSync(file)) {
    fs.mkdirSync(PRACTICE_DIR, { recursive: true });
    fs.writeFileSync(file, (block.starter || `// ${block.task.split("\n")[0]}\n`) + "\n");
  }
  const shownPath = file.replace(os.homedir(), "~");

  console.log("  " + bold("YOUR TURN"));
  rule();
  say(block.task);
  if (block.input) {
    console.log("  When the checker runs it, it types: " + block.input.join(", "));
  }
  console.log("  It must print:");
  for (const line of block.expected) {
    console.log("    " + line);
  }
  console.log();
  console.log("  Write it in " + bold(shownPath) + " (open it in any text editor).");
  console.log();

  let hint = 0;
  for (;;) {
    const key = (await ask("  [ENTER] check my file   [h] hint   [s] show a solution   [k] skip: ")).toLowerCase();
    console.log();
    if (key === "k") {
      return;
    }
    if (key === "h") {
      say(yellow("Hint: ") + block.hints[hint % block.hints.length]);
      hint++;
      continue;
    }
    if (key === "s") {
      console.log("  One way to write it (yours can look different and still be right):");
      console.log();
      showCode(block.solution);
      console.log();
      continue;
    }
    const result = runFile(file, block.input || []);
    if (result.timeout) {
      say(red("Your program did not finish within 5 seconds.") + " Is there a loop that never stops?");
    } else if (!result.ok) {
      console.log("  " + red("Your program stopped with an error:"));
      showError(result);
    } else if (sameOutput(block.expected, result.output)) {
      console.log("  " + green("Correct! ") + "Your program printed exactly what was asked.");
      console.log();
      return;
    } else {
      console.log("  " + red("Not yet.") + " Your program printed:");
      for (const line of result.output.length ? result.output : ["(nothing)"]) {
        console.log("    " + line);
      }
      const at = block.expected.findIndex((line, i) => line.trimEnd() !== result.output[i]);
      if (at >= 0) {
        console.log();
        console.log(`  Line ${at + 1} should be:  ${block.expected[at]}`);
      } else {
        console.log();
        console.log("  The asked lines are all there, but there are extra lines after them.");
      }
      console.log();
    }
  }
}

async function block(b, lessonNumber) {
  if (typeof b === "string") {
    say(b);
  } else if (b.code) {
    await codeBlock(b);
  } else if (b.quiz) {
    await quizBlock(b);
  } else if (b.exercise) {
    await exerciseBlock(b, lessonNumber);
  } else if (b.tip) {
    say(yellow("Remember: ") + b.tip);
  } else {
    throw new Error("unknown block: " + JSON.stringify(b));
  }
}

// ---------------------------------------------------------------- lessons

// Play one lesson. Returns when the student finishes it or asks for the menu.
async function playLesson(lesson, number) {
  let screen = 0;
  for (;;) {
    clear();
    console.log();
    console.log("  " + bold(`LESSON ${number}: ${lesson.title.toUpperCase()}`) + dim(`   (${screen + 1}/${lesson.screens.length + 1})`));
    rule();
    console.log();

    if (screen === lesson.screens.length) {
      console.log("  " + bold("WHAT YOU LEARNED"));
      console.log();
      for (const line of lesson.recap) {
        console.log(wrap("- " + line, "    ").replace(/^ {4}/, "  "));
      }
      console.log();
      const progress = loadProgress();
      if (!progress.done.includes(lesson.id)) {
        progress.done.push(lesson.id);
        saveProgress(progress);
      }
      await ask("  [ENTER] back to the lessons: ");
      return;
    }

    for (const b of lesson.screens[screen]) {
      await block(b, number);
    }
    const key = (await ask("  [ENTER] next   [b] back   [m] menu: ")).toLowerCase();
    if (key === "m") {
      return;
    }
    screen = key === "b" ? Math.max(0, screen - 1) : screen + 1;
  }
}

module.exports = {
  ask,
  close,
  clear,
  bold,
  dim,
  green,
  rule,
  wrap,
  runCode,
  runFile,
  sameOutput,
  loadProgress,
  playLesson,
  InputClosed,
  PRACTICE_DIR,
};
