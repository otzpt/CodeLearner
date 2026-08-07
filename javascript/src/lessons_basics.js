// Modules 1 to 5 - the basics.
//
// Rule used throughout: nothing is claimed without being shown. Where an
// example can run here, it runs.

const ui = require("./ui.js");

async function lesson01Running() {
  ui.title("MODULE 1 - RUNNING NODE AND CONSOLE.LOG()");

  ui.heading("PART 1: no compile step");

  console.log("  Like Python, JavaScript run through Node has no separate");
  console.log("  build step. Node reads your file and runs it directly:");
  console.log();
  console.log("    node program.js");
  console.log();
  console.log("  That is the whole command. A mistake later in the file is");
  console.log("  only caught once execution actually reaches it -- there is");
  console.log("  no compiler checking the whole file up front the way gcc");
  console.log("  or g++ does.");

  await ui.waitEnter();
  ui.clearScreen();
  ui.heading("PART 2: console.log() and template literals");

  console.log('    console.log("Hello world")');
  console.log('    console.log("a", "b", "c")');
  console.log();
  process.stdout.write("  Running:  ");
  console.log("a", "b", "c");
  console.log();
  console.log("  Several arguments are joined with a space, same as Python's");
  console.log("  print(). For building a string with values inside, use a");
  console.log("  template literal -- backticks, not quotes:");
  console.log();
  console.log('    const name = "Ana";');
  console.log('    console.log(`Hello, ${name}`);');
  console.log();
  const name = "Ana";
  console.log(`  Running: Hello, ${name}`);
  console.log();
  console.log("  ${expr} inside backticks is evaluated and inserted. Regular");
  console.log("  quotes do not do this -- \"Hello, ${name}\" prints literally.");

  await ui.waitEnter();
  ui.clearScreen();
  ui.heading("PART 3: let, const, and no semicolon-or-not debate here");

  console.log("  const for a name you will not reassign, let for one you");
  console.log("  will. Both exist because of var, an older keyword with");
  console.log("  scoping rules that surprise people -- module 7 shows the");
  console.log("  specific way it does. Avoid var in code you write.");
  console.log();
  console.log("  // starts a line comment, same as most languages here.");
  console.log("  There is no main() -- top-level code runs as the file is");
  console.log("  read, same as Python.");

  await ui.waitEnter();
  ui.clearScreen();
  ui.exercise(1);

  await ui.question(
    'console.log("a", "b");  -- what appears between them by default?',
    "a space",
    "console.log() joins its arguments with a single space, same as Python's print()."
  );
  await ui.question(
    'const name = "Ana"; console.log("Hi, ${name}");  -- does ${name} get replaced?\n' +
      "  (answer: yes or no)",
    "no",
    "Regular quotes do not interpolate. Only backtick template literals do."
  );

  await ui.challenge(
    ["Print your name and the current year, each on its own line."],
    [],
    ["Jose", "2026"],
    ['console.log("Jose");', 'console.log("2026");']
  );

  await ui.waitEnter();
  ui.clearScreen();
  ui.heading("SUMMARY");

  console.log("   - node file.js runs directly, no build step");
  console.log("   - console.log() joins arguments with a space");
  console.log("   - `${expr}` inside backticks interpolates; regular quotes do not");
  console.log();
  console.log("  Module 2: variables, and a coercion rule Python does not have.");
  await ui.waitEnter();
}

async function lesson02Variables() {
  ui.title("MODULE 2 - VARIABLES AND TYPES");

  ui.heading("PART 1: typeof, and == vs ===");

  console.log("    let age = 25;");
  console.log("    const height = 1.75;");
  console.log();
  let age = 25;
  const height = 1.75;
  console.log(`    typeof age    -> ${typeof age}`);
  console.log(`    typeof height -> ${typeof height}`);
  console.log(`    typeof "x"    -> ${typeof "x"}`);
  console.log();
  console.log("  Two equals operators, and they are not interchangeable:");
  console.log();
  console.log(`    1 == "1"   -> ${1 == "1"}    // converts, then compares`);
  console.log(`    1 === "1"  -> ${1 === "1"}   // compares type and value, no conversion`);
  console.log();
  console.log("  == quietly converts one side to match the other before");
  console.log("  comparing, with rules nobody has memorised:");
  console.log();
  console.log(`    0 == ""    -> ${0 == ""}`);
  console.log(`    0 == "0"   -> ${0 == "0"}`);
  console.log(`    "" == "0"  -> ${"" == "0"}`);
  console.log();
  console.log("  Use === always. It is not a style preference -- == has");
  console.log("  genuine surprises baked into the language specification.");

  await ui.waitEnter();
  ui.clearScreen();
  ui.heading("PART 2: NaN, and a variable's type can change");

  console.log(`    NaN === NaN         -> ${NaN === NaN}`);
  console.log(`    Number.isNaN(NaN)   -> ${Number.isNaN(NaN)}`);
  console.log();
  console.log("  NaN famously does not equal itself. Number.isNaN() is the");
  console.log("  correct check; === against NaN directly is always false.");
  console.log();
  age = "now a string";
  console.log("    age = 25;");
  console.log('    age = "now a string";    // legal -- let rebinds freely');
  console.log(`    typeof age -> ${typeof age}`);
  console.log();
  console.log("  const stops REASSIGNMENT, not mutation, and has nothing to");
  console.log("  say about type -- module 6 covers what const does and does");
  console.log("  not protect on an array or object.");

  await ui.waitEnter();
  ui.clearScreen();
  ui.heading("PART 3: floating point, unchanged from every other language here");

  console.log(`    0.1 + 0.2  -> ${0.1 + 0.2}`);
  console.log();
  console.log("  Same binary floating point as C, Python and everything");
  console.log("  else here. Not a JavaScript bug -- for money, work in whole");
  console.log("  cents, same rule as always.");

  await ui.waitEnter();
  ui.clearScreen();
  ui.exercise(2);

  await ui.question('1 == "1"   Does this compare equal?', "yes", "== converts types before comparing.");
  await ui.question(
    '1 === "1"   Does this compare equal?',
    "no",
    "=== never converts. Different types are never equal under ===."
  );
  await ui.question("NaN === NaN   Equal?", "no", "NaN never equals anything, including itself. Use Number.isNaN().");

  await ui.challenge(
    ["Declare text = \"5\" and number = 5. Print typeof text,", "typeof number, then text == number, then text === number."],
    [],
    ["string number", "true false"],
    [
      'const text = "5";',
      "const number = 5;",
      "console.log(typeof text, typeof number);",
      "console.log(text == number, text === number);",
    ]
  );

  await ui.waitEnter();
  ui.clearScreen();
  ui.heading("SUMMARY");

  console.log("   - always ===, never == -- == converts with rules nobody remembers");
  console.log("   - NaN !== NaN; check with Number.isNaN()");
  console.log("   - let/const control reassignment, not type or mutability");
  await ui.waitEnter();
}

async function lesson03Input() {
  ui.title("MODULE 3 - READING INPUT");

  ui.heading("PART 1: reading a line is asynchronous");

  console.log("  C's scanf, Python's input(), even C++'s cin -- all block");
  console.log("  until a line arrives. Node has nothing built in that does");
  console.log("  that. Reading input is an operation that takes time, and");
  console.log("  Node is built around never blocking while waiting -- so");
  console.log("  reading a line is asynchronous, and you await it:");
  console.log();
  console.log('    const readline = require("node:readline");');
  console.log('    const rl = readline.createInterface({ input: process.stdin });');
  console.log("    const it = rl[Symbol.asyncIterator]();");
  console.log();
  console.log("    async function ask(prompt) {");
  console.log("        process.stdout.write(prompt);");
  console.log("        const { value } = await it.next();");
  console.log("        return value;");
  console.log("    }");
  console.log();
  console.log("  Every function that calls ask() must itself be async, and");
  console.log("  every caller of THAT function must await it -- async is");
  console.log("  contagious upward through the whole call chain, all the");
  console.log("  way to the top-level code that starts everything.");

  await ui.waitEnter();
  ui.clearScreen();
  ui.heading("PART 2: try it, for real");

  const name = await ui.ask("\n  What is your name? ");
  console.log(`\n  Hello, ${name}. This course's own ui.js is the ask()`);
  console.log("  function shown above, reading your actual keyboard.");

  await ui.waitEnter();
  ui.clearScreen();
  ui.heading("PART 3: a real gotcha, specific to testing non-interactively");

  console.log("  If you test a script by piping input instead of typing it:");
  console.log();
  console.log('    printf "a\\nb\\n" | node script.js');
  console.log();
  console.log("  and the script asks two questions with the readline/promises");
  console.log("  library's rl.question(), the SECOND question can silently");
  console.log("  hang forever with no error. Verified: it happens because");
  console.log("  printf closes the pipe the instant it finishes writing, and");
  console.log("  once stdin has already hit that end-of-input, a second");
  console.log("  pending question never resolves.");
  console.log();
  console.log("  A real terminal never closes stdin while you are sitting");
  console.log("  there, so this never affects the course itself. It does");
  console.log("  affect testing a script non-interactively -- if you pipe");
  console.log("  input to test something you wrote, and it hangs on the");
  console.log("  second prompt, this is why.");

  await ui.waitEnter();
  ui.clearScreen();
  ui.exercise(3);

  await ui.question(
    "Does Node have a built-in function that blocks and returns a\n" +
      "  typed line the way Python's input() does?  (answer: yes or no)",
    "no",
    "Reading a line is asynchronous in Node; there is no blocking equivalent built in."
  );
  await ui.question(
    "If function b() awaits an async function a(), can b() itself\n" +
      "  be a plain, non-async function?  (answer: yes or no)",
    "no",
    "await only works inside an async function -- the requirement propagates upward " +
      "through every caller."
  );

  await ui.challenge(
    ["Ask for the user's name and greet them."],
    ["Jose"],
    ["What is your name? Jose", "Hello, Jose"],
    [
      'const readline = require("node:readline");',
      "const rl = readline.createInterface({ input: process.stdin });",
      "const it = rl[Symbol.asyncIterator]();",
      "",
      "async function main() {",
      '    process.stdout.write("What is your name? ");',
      "    const { value: name } = await it.next();",
      "    console.log(`Hello, ${name}`);",
      "    rl.close();",
      "}",
      "",
      "main();",
    ]
  );

  await ui.waitEnter();
  ui.clearScreen();
  ui.heading("SUMMARY");

  console.log("   - reading input is async; every caller up the chain must be too");
  console.log("   - a real terminal is fine; a pre-closed test pipe can hang a second read");
  console.log("   - this course's own ask() avoids that by draining readline's iterator by hand");
  await ui.waitEnter();
}

async function lesson04Conditions() {
  ui.title("MODULE 4 - CONDITIONS");

  ui.heading("PART 1: if / else if / else");

  console.log("    const score = 85;");
  console.log("    if (score >= 90) {");
  console.log('        console.log("A");');
  console.log("    } else if (score >= 80) {");
  console.log('        console.log("B");');
  console.log("    } else {");
  console.log('        console.log("F");');
  console.log("    }");
  console.log();
  const score = 85;
  process.stdout.write(`  Running with score = ${score}:  `);
  if (score >= 90) {
    console.log("A");
  } else if (score >= 80) {
    console.log("B");
  } else {
    console.log("F");
  }
  console.log();
  console.log("  Braces mark the block, same as C. Only the first true");
  console.log("  branch runs, so order matters, same rule as everywhere else.");

  await ui.waitEnter();
  ui.clearScreen();
  ui.heading("PART 2: truthy and falsy -- and this is where JS and Python disagree");

  console.log("  Falsy in JavaScript: false, 0, \"\", null, undefined, NaN.");
  console.log("  So far, similar to Python. Here is where they diverge:");
  console.log();
  console.log(`    Boolean([])   -> ${Boolean([])}`);
  console.log(`    Boolean({})   -> ${Boolean({})}`);
  console.log();
  console.log("  TRUE. An empty array and an empty object are both truthy in");
  console.log("  JavaScript -- the opposite of Python, where [] and {} are");
  console.log("  falsy. If you know Python: `if (arr)` is never a length");
  console.log("  check here. Use `if (arr.length)` or `if (arr.length === 0)`.");

  await ui.waitEnter();
  ui.clearScreen();
  ui.heading("PART 3: = is still not ==, and still just a warning at best");

  console.log("    let x = 3;");
  console.log("    if (x = 5) { ... }");
  console.log();
  console.log("  Assigns 5, evaluates to 5, 5 is truthy, the block runs.");
  console.log("  Verified: this executes with no error at all -- unlike gcc");
  console.log("  and g++, Node does not even warn about it by default.");

  await ui.waitEnter();
  ui.clearScreen();
  ui.exercise(4);

  await ui.question("Boolean([])  -- truthy or falsy?", "truthy", "Empty arrays and objects are truthy in JS, unlike Python.");
  await ui.question(
    "To check if an array is empty, do you write `if (arr)` or\n  `if (arr.length === 0)`?",
    "if (arr.length === 0)",
    "if (arr) is always true for an array, empty or not -- arrays are objects, and objects are truthy."
  );

  await ui.challenge(
    ["Read a number and say whether it is positive, negative or zero."],
    ["-5"],
    ["Number: -5", "negative"],
    [
      "const readline = require(\"node:readline\");",
      "const rl = readline.createInterface({ input: process.stdin });",
      "const it = rl[Symbol.asyncIterator]();",
      "",
      "async function main() {",
      '    process.stdout.write("Number: ");',
      "    const { value } = await it.next();",
      "    const n = Number(value);",
      "",
      "    if (n > 0) console.log(\"positive\");",
      "    else if (n < 0) console.log(\"negative\");",
      "    else console.log(\"zero\");",
      "    rl.close();",
      "}",
      "",
      "main();",
    ]
  );

  await ui.waitEnter();
  ui.clearScreen();
  ui.heading("SUMMARY");

  console.log("   - only the first true branch runs; order matters");
  console.log("   - [] and {} are TRUTHY in JS -- opposite of Python");
  console.log("   - = still is not ==, and Node does not even warn about it");
  await ui.waitEnter();
}

async function lesson05Loops() {
  ui.title("MODULE 5 - LOOPS");

  ui.heading("PART 1: for, while, unchanged from C");

  console.log("    for (let i = 1; i <= 5; i++) {");
  console.log('        process.stdout.write(i + " ");');
  console.log("    }");
  console.log();
  process.stdout.write("  Running:  ");
  for (let i = 1; i <= 5; i++) {
    process.stdout.write(i + " ");
  }
  console.log();
  console.log();
  console.log("  Same three-part shape, same off-by-one risk with <= where");
  console.log("  < was meant. `let`, not `var`, for the counter -- module 7");
  console.log("  covers exactly why that choice matters inside a loop.");

  await ui.waitEnter();
  ui.clearScreen();
  ui.heading("PART 2: for...of and for...in are not the same loop");

  const fruits = ["apple", "banana", "cherry"];
  console.log('    const fruits = ["apple", "banana", "cherry"];');
  console.log("    for (const f of fruits) console.log(f);      // VALUES");
  console.log("    for (const i in fruits) console.log(i);      // KEYS, as strings");
  console.log();
  for (const f of fruits) {
    console.log(`    - ${f}`);
  }
  console.log();
  for (const i in fruits) {
    console.log(`    in: ${i}  (${typeof i})`);
  }
  console.log();
  console.log("  for...in gives the indices, as strings -- \"0\", \"1\", \"2\", not");
  console.log("  the elements. Using for...in on an array is almost always a");
  console.log("  mistake; remember it as: of for values, in for keys.");

  await ui.waitEnter();
  ui.clearScreen();
  ui.heading("PART 3: while");

  console.log("    let n = 3;");
  console.log("    while (n > 0) {");
  console.log('        process.stdout.write(n + " ");');
  console.log("        n--;");
  console.log("    }");
  console.log();
  let n = 3;
  process.stdout.write("  Running:  ");
  while (n > 0) {
    process.stdout.write(n + " ");
    n--;
  }
  console.log();

  await ui.waitEnter();
  ui.clearScreen();
  ui.exercise(5);

  await ui.question("for (const x of arr)  -- does x hold values or indices?", "values", "of walks the values directly.");
  await ui.question("for (const x in arr)  -- does x hold values or indices?", "indices", "in walks the keys/indices, as strings.");

  await ui.challenge(
    [
      "Print 1 through 5, one per line. Then print each of",
      '"apple", "banana", "cherry" prefixed with "- ".',
    ],
    [],
    ["1", "2", "3", "4", "5", "- apple", "- banana", "- cherry"],
    [
      "for (let i = 1; i <= 5; i++) {",
      "    console.log(i);",
      "}",
      "",
      'const fruits = ["apple", "banana", "cherry"];',
      "for (const f of fruits) {",
      '    console.log("- " + f);',
      "}",
    ]
  );

  await ui.waitEnter();
  ui.clearScreen();
  ui.heading("SUMMARY");

  console.log("   - for/while carry over unchanged, off-by-one included");
  console.log("   - of -> values, in -> keys as strings; do not mix them up on arrays");
  console.log();
  console.log("  Module 6: arrays, and the same aliasing bug Python has too.");
  await ui.waitEnter();
}

module.exports = {
  lesson01Running,
  lesson02Variables,
  lesson03Input,
  lesson04Conditions,
  lesson05Loops,
};
