#!/usr/bin/env node
// Self-check for one course's lessons: every example runs (or fails, when it
// is marked as an error example, or prints its transcript, when it has one),
// every quiz answer points at a real choice, every exercise's solution passes
// the same checker the student meets, and every starter file builds and runs.
//
//   node learn/check.js <course folder> [more course folders...]

const path = require("node:path");
const app = require("./app.js");
const { loadCourse } = require("./main.js");

let failed = 0;
let checked = 0;

function fail(where, why) {
  failed++;
  console.log(`FAIL  ${where}: ${why}`);
}

function checkCourse(dir) {
  const course = loadCourse(path.resolve(dir));
  const ids = new Set();
  course.lessons.forEach((lesson, n) => {
    const name = `${course.id} lesson ${n + 1}`;
    if (ids.has(lesson.id)) fail(name, `duplicate lesson id ${lesson.id}`);
    ids.add(lesson.id);
    if (!lesson.titlePt || !lesson.recapPt || lesson.recapPt.length !== lesson.recap.length) {
      fail(name, "missing or uneven titlePt/recapPt");
    }
    lesson.screens.forEach((screen, s) => {
      const where = `${name} screen ${s + 1}`;
      if (screen.filter((b) => b && b.code).length > 1) fail(where, "more than one example on a screen");
      for (const b of screen) {
        if (typeof b === "string" || b.tip) continue;
        checked++;
        if (b.code && b.run !== false) {
          const full = b.setup ? b.setup + "\n\n" + b.code : b.code;
          const t = b.output ? app.transcript(b.output) : null;
          const r = app.runCode(course, full, t ? t.input : []);
          if (b.error && r.ok) fail(where, "example marked as an error ran fine");
          if (!b.error && !r.ok) fail(where, `example failed: ${r.errorLine || "timeout"}`);
          if (r.ok && t && !app.sameOutput(t.raw, r.output)) fail(where, `example printed ${JSON.stringify(r.output)}, transcript says ${JSON.stringify(t.raw)}`);
          if (r.ok && r.warnings && r.warnings.length && !b.warning) fail(where, `unexpected warning: ${r.warnings[0]}`);
        } else if (b.quiz) {
          if (!Number.isInteger(b.answer) || b.answer < 1 || b.answer > b.choices.length) fail(where, "quiz answer out of range");
        } else if (b.exercise) {
          const t = app.transcript(b.expected);
          const r = app.runCode(course, b.solution, t.input);
          if (!r.ok || !app.sameOutput(t.raw, r.output)) {
            fail(where, `solution printed ${JSON.stringify(r.output)} ${r.errorLine || ""}`);
          }
          const starter = b.starter || course.starter;
          if (starter) {
            const s2 = app.runCode(course, starter, t.input);
            if (!s2.ok && !s2.timeout) fail(where, `starter does not run: ${s2.errorLine}`);
          }
          if (!b.hints || b.hints.length === 0) fail(where, "exercise has no hints");
        }
      }
    });
  });
}

const dirs = process.argv.slice(2);
if (dirs.length === 0) {
  console.error("usage: node learn/check.js <course folder>...");
  process.exit(2);
}
for (const dir of dirs) {
  const before = failed;
  const start = checked;
  checkCourse(dir);
  console.log(`${path.basename(path.resolve(dir))}: ${checked - start} blocks checked, ${failed - before} failed`);
}
app.close();
process.exitCode = failed ? 1 : 0;
