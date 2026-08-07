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

const MODULES = [
  { title: "Running Node and console.log()", run: lesson01Running },
  { title: "Variables and types", run: lesson02Variables },
  { title: "Reading input", run: lesson03Input },
  { title: "Conditions", run: lesson04Conditions },
  { title: "Loops", run: lesson05Loops },
  { title: "Arrays", run: lesson06Arrays },
  { title: "Functions and this", run: lesson07Functions },
  { title: "Objects", run: lesson08Objects },
  { title: "Classes", run: lesson09Classes },
  { title: "Errors", run: lesson10Errors },
];

function showMenu() {
  ui.clearScreen();
  ui.title("JAVASCRIPT COURSE - FROM ZERO TO CLASSES");

  MODULES.forEach((module, i) => {
    const n = String(i + 1).padStart(2, " ");
    console.log(`   [${n}]  ${module.title}`);
  });
  console.log("\n   [ 0]  Quit");
  ui.rule();
}

async function main() {
  for (;;) {
    showMenu();
    const choice = (await ui.ask("\n  Pick a module: ")).trim();

    if (choice === "0") {
      break;
    }

    const n = Number(choice);
    if (Number.isInteger(n) && n >= 1 && n <= MODULES.length) {
      ui.clearScreen();
      await MODULES[n - 1].run();
    } else {
      console.log("\n  Not a valid option.");
      await ui.waitEnter();
    }
  }

  console.log("\n  See you next time.\n");
  ui.closeInput();
}

main();
