#!/usr/bin/env node
// JavaScript course - main menu.
//
// Written in JavaScript, the same way the other three courses are written
// in the language they teach: the source is itself worked example code.
// This file uses an array of objects and async/await, both taught properly
// in modules 7 and 3.
//
// Run:  node main.js

const ui = require("./ui.js");
const {
  lesson01Running,
  lesson02Variables,
  lesson03Input,
  lesson04Conditions,
  lesson05Loops,
} = require("./lessons_basics.js");
const {
  lesson06Arrays,
  lesson07Functions,
  lesson08Objects,
  lesson09Classes,
  lesson10Errors,
} = require("./lessons_more.js");
const { lesson11Promises, lesson12Apis, lesson13Modules, lesson14NodeStdlib } = require("./lessons_advanced.js");

// `tier` files a module under one of the three menu bands. Display
// grouping only -- the lesson functions never see it.
//
// `uses` lists the modules after the BASIC band whose code this module shows
// again, so a student who skipped one is told before meeting it. Written by
// hand from the lesson text: module 10's own error class is a module 9 class.
const MODULES = [
  { title: "Running Node and console.log()", run: lesson01Running, tier: "BASIC" },
  { title: "Variables and types", run: lesson02Variables, tier: "BASIC" },
  { title: "Reading input", run: lesson03Input, tier: "BASIC" },
  { title: "Conditions", run: lesson04Conditions, tier: "BASIC" },
  { title: "Loops", run: lesson05Loops, tier: "BASIC" },
  { title: "Arrays", run: lesson06Arrays, tier: "INTERMEDIATE" },
  { title: "Functions and this", run: lesson07Functions, tier: "INTERMEDIATE", uses: [6] },
  { title: "Objects", run: lesson08Objects, tier: "INTERMEDIATE" },
  { title: "Classes", run: lesson09Classes, tier: "ADVANCED", uses: [7, 8] },
  { title: "Errors", run: lesson10Errors, tier: "ADVANCED", uses: [7, 9] },
  { title: "Promises", run: lesson11Promises, tier: "ADVANCED", uses: [7, 10] },
  { title: "Working with APIs", run: lesson12Apis, tier: "ADVANCED", uses: [10] },
  { title: "Modules", run: lesson13Modules, tier: "ADVANCED" },
  { title: "Node-specific stdlib", run: lesson14NodeStdlib, tier: "ADVANCED", uses: [11, 13] },
];

function showUses(module) {
  if (!module.uses) {
    return;
  }
  console.log("\n  " + ui.tr("This module uses:"));
  for (const n of module.uses) {
    console.log(`    ${ui.tr("module")} ${n}  ${ui.tr(MODULES[n - 1].title)}`);
  }
  console.log("  " + ui.tr("Skipped one? Expect a few lines you have not seen yet."));
}

function showMenu() {
  ui.clearScreen();
  ui.title(ui.tr("JAVASCRIPT COURSE - FROM ZERO TO CLASSES"));

  let shownTier = null;
  MODULES.forEach((module, i) => {
    if (module.tier !== shownTier) {
      shownTier = module.tier;
      console.log(`\n  -- ${ui.tr(shownTier)} --`);
    }
    const n = String(i + 1).padStart(2, " ");
    console.log(`   [${n}]  ${ui.tr(module.title)}`);
  });
  console.log("\n   [ 0]  " + ui.tr("Quit"));
  ui.rule();
}

async function main() {
  for (;;) {
    showMenu();
    const choice = (await ui.ask("\n  " + ui.tr("Pick a module:") + " ")).trim();

    if (choice === "0" || ui.isInputClosed()) {
      break;
    }

    const n = Number(choice);
    if (Number.isInteger(n) && n >= 1 && n <= MODULES.length) {
      ui.clearScreen();
      showUses(MODULES[n - 1]);
      await MODULES[n - 1].run();
    } else {
      console.log("\n  " + ui.tr("Not a valid option."));
      await ui.waitEnter();
    }
  }

  console.log("\n  " + ui.tr("See you next time.") + "\n");
  ui.closeInput();
}

main().catch((err) => {
  // A bug in a lesson should not leave a raw stack trace where every other
  // exit path in this course prints "See you next time." -- state what
  // broke, still close the readline interface so the process actually
  // exits instead of hanging on an open stdin handle, then fail loudly
  // enough that it is not mistaken for a normal quit.
  console.error(`\n  ${ui.tr("Something went wrong:")} ${err.message}`);
  ui.closeInput();
  process.exitCode = 1;
});
