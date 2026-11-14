#!/usr/bin/env node
// JavaScript from zero: the lesson menu.
//
// Run:  node main.js

const fs = require("node:fs");
const path = require("node:path");
const app = require("./app.js");

const LESSON_DIR = path.join(__dirname, "lessons");
const LESSONS = fs
  .readdirSync(LESSON_DIR)
  .filter((name) => name.endsWith(".js"))
  .sort()
  .map((name) => require(path.join(LESSON_DIR, name)));

function showMenu() {
  const { done } = app.loadProgress();
  const next = LESSONS.findIndex((lesson) => !done.includes(lesson.id));

  app.clear();
  console.log();
  console.log("  " + app.bold(app.tr("JAVASCRIPT FROM ZERO")));
  console.log(app.wrap(app.tr("No experience needed. Each lesson explains one idea, runs real examples, and ends with a small program you write yourself and the course checks.")));
  app.rule();
  console.log();
  LESSONS.forEach((lesson, i) => {
    const mark = done.includes(lesson.id) ? app.green("[x]") : "[ ]";
    const here = i === next ? app.bold(app.tr("  <- next")) : "";
    console.log(`   ${mark} ${String(i + 1).padStart(2)}  ${app.lessonTitle(lesson)}${here}`);
  });
  console.log();
  console.log(app.dim(`  ${app.tr("Your practice files go in")} ${app.PRACTICE_DIR}`));
  console.log();
  return next;
}

async function main() {
  for (;;) {
    const next = showMenu();
    const prompt = app.tr(next >= 0 ? "  Lesson number, [ENTER] for the next one, [q] quit: " : "  Lesson number, [q] quit: ");
    const choice = await app.ask(prompt);
    if (choice.toLowerCase() === "q" || choice === "0") {
      break;
    }
    const n = choice === "" ? next + 1 : Number(choice);
    if (Number.isInteger(n) && n >= 1 && n <= LESSONS.length) {
      await app.playLesson(LESSONS[n - 1], n);
    }
  }
}

main()
  .then(() => console.log(`\n  ${app.tr("See you next time.")}\n`))
  .catch((err) => {
    if (err instanceof app.InputClosed) {
      console.log(`\n  ${app.tr("See you next time.")}\n`);
      return;
    }
    console.error(`\n  ${app.tr("Something went wrong:")} ${err.message}`);
    process.exitCode = 1;
  })
  .finally(app.close);
