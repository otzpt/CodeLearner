// The lesson menu, shared by every course. A course's learn.js calls
// start(__dirname); everything course-specific is in that folder's course.js
// and lessons/.

const fs = require("node:fs");
const path = require("node:path");
const app = require("./app.js");

function loadCourse(dir) {
  const course = require(path.join(dir, "course.js"));
  const lessonDir = path.join(dir, "lessons");
  course.lessons = fs
    .readdirSync(lessonDir)
    .filter((name) => name.endsWith(".js"))
    .sort()
    .map((name) => require(path.join(lessonDir, name)));
  return course;
}

function showMenu(course) {
  const { done } = app.loadProgress(course);
  const next = course.lessons.findIndex((lesson) => !done.includes(lesson.id));

  app.clear();
  console.log();
  console.log("  " + app.bold(app.PT && course.titlePt ? course.titlePt : course.title));
  console.log(app.wrap(app.tr("No experience needed. Each lesson explains one idea, runs real examples, and ends with a small program you write yourself and the course checks.")));
  app.rule();
  console.log();
  course.lessons.forEach((lesson, i) => {
    const mark = done.includes(lesson.id) ? app.green("[x]") : "[ ]";
    const here = i === next ? app.bold(app.tr("  <- next")) : "";
    console.log(`   ${mark} ${String(i + 1).padStart(2)}  ${app.lessonTitle(lesson)}${here}`);
  });
  console.log();
  console.log(app.dim(`  ${app.tr("Your practice files go in")} ${app.practiceDir(course)}`));
  console.log();
  return next;
}

async function menu(course) {
  for (;;) {
    const next = showMenu(course);
    const prompt = app.tr(next >= 0 ? "  Lesson number, [ENTER] for the next one, [q] quit: " : "  Lesson number, [q] quit: ");
    const choice = await app.ask(prompt);
    if (choice.toLowerCase() === "q" || choice === "0") {
      break;
    }
    const n = choice === "" ? next + 1 : Number(choice);
    if (Number.isInteger(n) && n >= 1 && n <= course.lessons.length) {
      await app.playLesson(course, course.lessons[n - 1], n);
    }
  }
}

function start(dir) {
  const course = loadCourse(dir);
  menu(course)
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
}

module.exports = { start, loadCourse };
