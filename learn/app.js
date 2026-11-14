// The engine every course runs on: drawing screens, reading keys, running
// code, checking exercises and remembering progress. A course is a folder
// with course.js (how to run its language) and lessons/*.js (plain data), so
// writing a lesson never means touching this file.

const fs = require("node:fs");
const os = require("node:os");
const path = require("node:path");
const readline = require("node:readline");
const { spawnSync } = require("node:child_process");
const { PT, tr } = require("./pt.js");

const WIDTH = 66;

// ---------------------------------------------------------------- input

// readline's async iterator, not rl.question(): rl.question() silently drops
// every answer after the first when input is piped (checked on Node 26), and
// the walk test pipes its input.
let rl = null;
let lines = null;

class InputClosed extends Error {}

async function ask(prompt) {
  if (!rl) {
    rl = readline.createInterface({ input: process.stdin });
    lines = rl[Symbol.asyncIterator]();
  }
  process.stdout.write(prompt);
  const { value, done } = await lines.next();
  if (done) {
    throw new InputClosed();
  }
  return value.trim();
}

function close() {
  if (rl) {
    rl.close();
  }
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
    const lead = paragraph.match(/^ */)[0];
    let line = "";
    for (const word of paragraph.trim().split(" ")) {
      if (line && (line + " " + word).length > WIDTH - indent.length - lead.length) {
        out.push(indent + lead + line);
        line = word;
      } else {
        line = line ? line + " " + word : word;
      }
    }
    out.push(indent + lead + line);
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

// ---------------------------------------------------------------- transcripts

// An expected output is written the way it looks in a terminal, with what the
// user types in braces at the end of a line: "Name? {Ana}". A pipe does not
// echo typed text, so the program's real output is "Name? " followed
// directly by its next line. This turns a transcript into the typed input,
// the text to show, and the raw output to compare against.
function transcript(lines) {
  const input = [];
  const shown = [];
  let raw = "";
  for (const line of lines) {
    const m = line.match(/^(.*)\{(.*)\}$/);
    if (m) {
      input.push(m[2]);
      shown.push(m[1] + bold(m[2]));
      raw += m[1];
    } else {
      shown.push(line);
      raw += line + "\n";
    }
  }
  return { input, shown, raw: splitOutput(raw) };
}

function splitOutput(text) {
  const out = text.replace(/\s+$/, "");
  return out === "" ? [] : out.split("\n").map((l) => l.trimEnd());
}

function sameOutput(expected, got) {
  return expected.length === got.length && expected.every((line, i) => line === got[i]);
}

// ---------------------------------------------------------------- running code

// Write `code` to a fresh folder as the course's file and run the course's
// steps there. Every step but the last builds; the last runs and gets the
// input. Returns what happened in a shape the screens can show.
function runCode(course, code, input = []) {
  const dir = fs.mkdtempSync(path.join(os.tmpdir(), "codelearner-"));
  try {
    const file = path.join(dir, course.file);
    fs.writeFileSync(file, code + "\n");
    return runFile(course, file, input);
  } finally {
    fs.rmSync(dir, { recursive: true, force: true });
  }
}

function runFile(course, file, input = []) {
  const steps = course.steps(file, path.dirname(file));
  let warnings = [];
  for (let i = 0; i < steps.length; i++) {
    const [cmd, ...args] = steps[i];
    const last = i === steps.length - 1;
    const r = spawnSync(cmd, args, {
      cwd: path.dirname(file),
      input: last && input.length ? input.join("\n") + "\n" : "",
      encoding: "utf8",
      timeout: course.timeout || 10000,
      env: { ...process.env, ...(course.env || {}) },
    });
    if (r.error && r.error.code === "ENOENT") {
      return { ok: false, output: [], errorLine: `${cmd}: ${tr("not installed")}`, missing: cmd };
    }
    if (r.error && r.error.code === "ETIMEDOUT") {
      return { ok: false, output: splitOutput(r.stdout || ""), timeout: true };
    }
    if (!last) {
      if (r.status !== 0) {
        return { ok: false, output: [], compile: true, ...course.parseError(r.stderr + r.stdout, file) };
      }
      warnings = warnings.concat(course.warnings ? course.warnings(r.stderr) : []);
      continue;
    }
    const output = splitOutput(r.stdout);
    if (r.signal) {
      return { ok: false, output, errorLine: signalText(r.signal) };
    }
    if (r.status !== 0) {
      const parsed = course.parseError(r.stderr, file);
      return { ok: false, output, ...parsed, errorLine: parsed.errorLine || `${tr("exited with code")} ${r.status}` };
    }
    return { ok: true, output, warnings };
  }
  return { ok: true, output: [], warnings };
}

function signalText(signal) {
  const names = {
    SIGSEGV: "Segmentation fault",
    SIGABRT: "Aborted",
    SIGFPE: "Floating point exception",
    SIGBUS: "Bus error",
  };
  return names[signal] || signal;
}

// What an error means, in plain words: the course's own list first, then the
// general one.
function explainError(course, errorLine) {
  for (const [pattern, en, pt] of (course.errors || []).concat(GENERAL_ERRORS)) {
    if (pattern.test(errorLine)) {
      return PT && pt ? pt : en;
    }
  }
  return tr("read the message: it names the problem and the line.");
}

const GENERAL_ERRORS = [
  [/Segmentation fault/, "the program read or wrote memory it does not own, often an array index out of range or a bad pointer.", "o programa leu ou escreveu memória que não é dele, muitas vezes um índice fora do array ou um ponteiro errado."],
  [/Floating point exception/, "a whole-number division by zero.", "uma divisão inteira por zero."],
  [/not installed/, "this course needs that program to run code. Install it, then try again.", "este curso precisa desse programa para correr código. Instala-o e tenta outra vez."],
];

function showError(course, result, indent = "  ") {
  console.log(indent + red(result.errorLine));
  if (result.line) {
    console.log(indent + dim(`(${tr("line")} ${result.line})`));
  }
  console.log();
  say(yellow(tr("What this means: ")) + explainError(course, result.errorLine));
}

// ---------------------------------------------------------------- progress

function practiceDir(course) {
  const base = process.env.CODELEARNER_PRACTICE_DIR || path.join(os.homedir(), "codelearner");
  return process.env.CODELEARNER_PRACTICE_DIR ? base : path.join(base, course.id);
}

function loadProgress(course) {
  try {
    return JSON.parse(fs.readFileSync(path.join(practiceDir(course), ".progress.json"), "utf8"));
  } catch {
    return { done: [] };
  }
}

function saveProgress(course, progress) {
  fs.mkdirSync(practiceDir(course), { recursive: true });
  fs.writeFileSync(path.join(practiceDir(course), ".progress.json"), JSON.stringify(progress, null, 2) + "\n");
}

// ---------------------------------------------------------------- blocks

// An example: show it, run it, show what it printed. `output` (a transcript)
// is only for examples that read input: it says what to type and how the
// result looks on screen, and is checked against the real run.
// `setup` runs before the example without being shown; `run: false` only
// shows the code; `error: true` marks an example that is meant to fail.
async function codeBlock(course, block) {
  if (block.setup) {
    console.log("    " + dim(tr(block.setupLabel || "(the helper above goes here)")));
  }
  showCode(block.code);
  console.log();
  if (block.run === false) {
    return;
  }
  const full = block.setup ? block.setup + "\n\n" + block.code : block.code;
  const t = block.output ? transcript(block.output) : null;
  const result = runCode(course, full, t ? t.input : []);

  if (result.missing) {
    showError(course, result);
    return;
  }
  for (const w of result.warnings || []) {
    console.log("  " + yellow(w));
  }
  if (result.ok) {
    if (t && !sameOutput(t.raw, result.output)) {
      throw new Error(`an example printed ${JSON.stringify(result.output)}, not its transcript`);
    }
    console.log("  " + dim(tr("Output:")));
    for (const line of t ? t.shown : result.output) {
      console.log("    " + line);
    }
    console.log();
  } else if (block.error) {
    console.log("  " + dim(tr("Output:")));
    for (const line of result.output.filter((l) => l !== "")) {
      console.log("    " + line);
    }
    showError(course, result, "    ");
  } else {
    throw new Error(`an example failed: ${result.errorLine || "timeout"}`);
  }
}

async function quizBlock(block) {
  console.log("  " + bold(tr("Quick question: ")) + block.quiz);
  console.log();
  block.choices.forEach((choice, i) => console.log(`    ${i + 1}) ${choice}`));
  console.log();
  for (let attempt = 1; ; attempt++) {
    const answer = await ask(tr("  Your answer (number): "));
    if (Number(answer) === block.answer) {
      console.log("  " + green(tr("Right. ")) + block.why);
      break;
    }
    if (attempt === 2) {
      console.log("  " + red(tr("Not quite. ")) + `${tr("It is")} ${block.answer}) ${block.choices[block.answer - 1]}. ` + block.why);
      break;
    }
    console.log("  " + red(tr("Not quite.")) + tr(" Have another go."));
  }
  console.log();
}

function exerciseFile(course, block, lessonNumber) {
  return path.join(practiceDir(course), `${String(lessonNumber).padStart(2, "0")}-${block.exercise}${path.extname(course.file)}`);
}

async function exerciseBlock(course, block, lessonNumber) {
  const file = exerciseFile(course, block, lessonNumber);
  if (!fs.existsSync(file)) {
    fs.mkdirSync(path.dirname(file), { recursive: true });
    fs.writeFileSync(file, (block.starter || course.starter || "") + "\n");
  }
  const shownPath = file.replace(os.homedir(), "~");
  const t = transcript(block.expected);

  console.log("  " + bold(tr("YOUR TURN")));
  rule();
  say(block.task);
  console.log(tr(t.input.length ? "  It must look like this (you type the bold parts):" : "  It must print:"));
  for (const line of t.shown) {
    console.log("    " + line);
  }
  console.log();
  console.log(tr("  Write it in ") + bold(shownPath) + tr(" (open it in any text editor)."));
  if (course.howToRun) {
    console.log("  " + dim(tr("To run it yourself:") + " " + course.howToRun(path.basename(file))));
  }
  console.log();

  let hint = 0;
  for (;;) {
    const key = (await ask(tr("  [ENTER] check my file   [h] hint   [s] show a solution   [k] skip: "))).toLowerCase();
    console.log();
    if (key === "k") {
      return;
    }
    if (key === "h") {
      say(yellow(tr("Hint: ")) + block.hints[hint % block.hints.length]);
      hint++;
      continue;
    }
    if (key === "s") {
      console.log(tr("  One way to write it (yours can look different and still be right):"));
      console.log();
      showCode(block.solution);
      console.log();
      continue;
    }
    const result = runFile(course, file, t.input);
    if (result.timeout) {
      say(red(tr("Your program did not finish within 10 seconds.")) + tr(" Is there a loop that never stops, or is it waiting for more input?"));
    } else if (!result.ok) {
      console.log("  " + red(tr(result.compile ? "Your program did not build:" : "Your program stopped with an error:")));
      showError(course, result);
    } else if (sameOutput(t.raw, result.output)) {
      console.log("  " + green(tr("Correct! ")) + tr("Your program printed exactly what was asked."));
      console.log();
      return;
    } else {
      console.log("  " + red(tr("Not yet.")) + tr(" Your program printed:") + (t.input.length ? dim(tr(" (what was typed is not shown)")) : ""));
      for (const line of result.output.length ? result.output : [tr("(nothing)")]) {
        console.log("    " + line);
      }
      const at = t.raw.findIndex((line, i) => line !== result.output[i]);
      console.log();
      if (at >= 0) {
        console.log(`  ${PT ? "Linha" : "Line"} ${at + 1} ${tr("should be:")}  ${t.raw[at]}`);
      } else {
        console.log(tr("  The asked lines are all there, but there are extra lines after them."));
      }
      console.log();
    }
  }
}

async function block(course, b, lessonNumber) {
  if (typeof b === "string") {
    say(b);
  } else if (b.code) {
    await codeBlock(course, b);
  } else if (b.quiz) {
    await quizBlock(b);
  } else if (b.exercise) {
    await exerciseBlock(course, b, lessonNumber);
  } else if (b.tip) {
    say(yellow(tr("Remember: ")) + b.tip);
  } else {
    throw new Error("unknown block: " + JSON.stringify(b));
  }
}

// ---------------------------------------------------------------- lessons

function lessonTitle(lesson) {
  return PT && lesson.titlePt ? lesson.titlePt : lesson.title;
}

// Play one lesson. Returns when the student finishes it or asks for the menu.
async function playLesson(course, lesson, number) {
  let screen = 0;
  for (;;) {
    clear();
    console.log();
    console.log("  " + bold(`${tr("LESSON")} ${number}: ${lessonTitle(lesson).toUpperCase()}`) + dim(`   (${screen + 1}/${lesson.screens.length + 1})`));
    rule();
    console.log();

    if (screen === lesson.screens.length) {
      console.log("  " + bold(tr("WHAT YOU LEARNED")));
      console.log();
      for (const line of PT && lesson.recapPt ? lesson.recapPt : lesson.recap) {
        console.log(wrap("- " + line, "    ").replace(/^ {4}/, "  "));
      }
      console.log();
      const progress = loadProgress(course);
      if (!progress.done.includes(lesson.id)) {
        progress.done.push(lesson.id);
        saveProgress(course, progress);
      }
      await ask(tr("  [ENTER] back to the lessons: "));
      return;
    }

    for (const b of lesson.screens[screen]) {
      await block(course, b, number);
    }
    const key = (await ask(tr("  [ENTER] next   [b] back   [m] menu: "))).toLowerCase();
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
  tr,
  PT,
  transcript,
  sameOutput,
  runCode,
  runFile,
  exerciseFile,
  practiceDir,
  loadProgress,
  lessonTitle,
  playLesson,
  InputClosed,
};
