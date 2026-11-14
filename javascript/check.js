#!/usr/bin/env node
// Self-check for the lessons: every example runs (or fails, when it is
// marked as an error example), every quiz answer points at a real choice,
// and every exercise's solution passes the same checker the student meets.
// A starter file must run without crashing.
//
//   node check.js

const fs = require("node:fs");
const os = require("node:os");
const path = require("node:path");
const app = require("./src/app.js");

const dir = path.join(__dirname, "src", "lessons");
let failed = 0;
let checked = 0;

function fail(where, why) {
  failed++;
  console.log(`FAIL  ${where}: ${why}`);
}

for (const name of fs.readdirSync(dir).filter((n) => n.endsWith(".js")).sort()) {
  const lesson = require(path.join(dir, name));
  lesson.screens.forEach((screen, s) => {
    for (const b of screen) {
      const where = `${name} screen ${s + 1}`;
      if (typeof b === "string" || b.tip) {
        continue;
      }
      checked++;
      if (b.code && b.run !== false) {
        const full = b.setup ? b.setup + "\n\n" + b.code : b.code;
        const r = app.runCode(full, b.input || []);
        if (b.error && r.ok) fail(where, "example marked as an error ran fine");
        if (!b.error && !r.ok) fail(where, `example failed: ${r.errorLine || "timeout"}`);
      } else if (b.quiz) {
        if (!Number.isInteger(b.answer) || b.answer < 1 || b.answer > b.choices.length) fail(where, "quiz answer out of range");
      } else if (b.exercise) {
        const r = app.runCode(b.solution, b.input || []);
        if (!r.ok || !app.sameOutput(b.expected, r.output)) {
          fail(where, `solution printed ${JSON.stringify(r.output)} ${r.errorLine || ""}`);
        }
        if (b.starter) {
          const s2 = app.runCode(b.starter, b.input || []);
          if (!s2.ok) fail(where, `starter crashed: ${s2.errorLine}`);
        }
        if (!b.hints || b.hints.length === 0) fail(where, "exercise has no hints");
      }
    }
  });
}

console.log(`${checked} blocks checked, ${failed} failed`);
app.close();
process.exitCode = failed ? 1 : 0;
