// lessons_advanced.js - module 11 onward.
//
// Module 3 already made every module in this course async and showed
// `await` reading real keyboard input. What it never showed is the object
// underneath await -- a Promise -- or what came before Promises existed:
// a plain callback. This module fills that in, plus Promise.all for
// running independent awaits concurrently instead of one after another.

const ui = require("./ui.js");
const fs = require("node:fs");
const os = require("node:os");
const path = require("node:path");
const { spawnSync } = require("node:child_process");

async function lesson11Promises() {
  ui.title("MODULE 11 - PROMISES");

  ui.heading("PART 1: a callback -- the way this used to be done");

  console.log("  Before Promises existed, an async result was handed back");
  console.log("  through a function you passed in, called when the work");
  console.log("  finished:");
  console.log();
  console.log("    function afterOneSecond(callback) {");
  console.log('      setTimeout(() => callback("done"), 1000);');
  console.log("    }");
  console.log();
  console.log('    afterOneSecond((result) => console.log(result));');
  console.log();

  await new Promise((resolve) => {
    function afterOneSecond(callback) {
      setTimeout(() => callback("done"), 200);
    }
    afterOneSecond((result) => {
      console.log(`  Running: ${result}`);
      resolve();
    });
  });

  console.log();
  console.log("  Nesting two of these (do A, then in A's callback do B) is");
  console.log("  how \"callback hell\" happens -- each step indented inside");
  console.log("  the last, with error handling repeated at every level.");

  await ui.waitEnter();
  ui.clearScreen();
  ui.heading("PART 2: a Promise is the object await is actually unwrapping");

  console.log("  await has been used since module 3 without ever naming");
  console.log("  what it unwraps. This is it:");
  console.log();
  console.log("    const p = new Promise((resolve, reject) => {");
  console.log('      setTimeout(() => resolve("done"), 1000);');
  console.log("    });");
  console.log();
  console.log("  A Promise is a value that does not exist yet -- it is in");
  console.log("  one of three states: pending, then fulfilled (resolve was");
  console.log("  called) or rejected (reject was called), and it can only");
  console.log("  move from pending to one of those two, once, ever.");
  console.log();
  console.log("  Two ways to get the value back out. `.then()`/`.catch()`,");
  console.log("  the pre-async/await style:");
  console.log();
  console.log("    p.then((value) => console.log(value))");
  console.log('     .catch((err) => console.log("failed:", err));');
  console.log();
  console.log("  Or `await` -- exactly the same Promise, unwrapped inline");
  console.log("  instead of in a callback passed to .then():");
  console.log();
  console.log("    const value = await p;");
  console.log();

  const p = new Promise((resolve) => {
    setTimeout(() => resolve("done"), 200);
  });
  const value = await p;
  console.log(`  Running: ${value}`);
  console.log();
  console.log("  A rejected Promise, awaited, throws -- catchable with the");
  console.log("  same try/catch module 10 already covers, not a new");
  console.log("  mechanism:");
  console.log();
  console.log("    try {");
  console.log('      await Promise.reject(new Error("boom"));');
  console.log("    } catch (e) {");
  console.log('      console.log("caught:", e.message);');
  console.log("    }");
  console.log();

  try {
    await Promise.reject(new Error("boom"));
  } catch (e) {
    console.log(`  Running: caught: ${e.message}`);
  }

  await ui.waitEnter();
  ui.clearScreen();
  ui.heading("PART 3: Promise.all -- independent awaits, run together");

  console.log("  Two sequential awaits that don't depend on each other");
  console.log("  waste the gap between them:");
  console.log();
  console.log("    const a = await delay(100, \"a\");   // waits 100ms");
  console.log("    const b = await delay(100, \"b\");   // THEN waits another 100ms");
  console.log();
  console.log("  Promise.all starts both immediately and waits for whichever");
  console.log("  finishes last, once:");
  console.log();
  console.log("    const [a, b] = await Promise.all([");
  console.log('      delay(100, "a"),');
  console.log('      delay(100, "b"),');
  console.log("    ]);");
  console.log();

  function delay(ms, tag) {
    return new Promise((resolve) => setTimeout(() => resolve(tag), ms));
  }

  const start = Date.now();
  const [a, b] = await Promise.all([delay(100, "a"), delay(100, "b")]);
  const elapsed = Date.now() - start;
  console.log(`  Running: got [${a}, ${b}] in ~${elapsed < 180 ? "100" : elapsed}ms,`);
  console.log("  not ~200ms -- both delays ran at the same time, not one");
  console.log("  after the other.");

  await ui.waitEnter();
  ui.clearScreen();
  ui.exercise(11);

  await ui.question(
    "A Promise starts pending. After resolve() is called once, can\n" +
      "  it later become rejected instead?  (answer: yes or no)",
    "no",
    "A Promise settles once, to fulfilled or rejected, and never changes state again.",
  );

  await ui.question(
    "await p, where p is a Promise that already rejected. What\n" +
      "  happens if there is no surrounding try/catch?",
    "throws",
    "An awaited rejection throws in the awaiting function, uncaught if nothing catches it -- " +
      "the same as any other thrown error.",
  );

  await ui.question(
    "Two independent 100ms delays, awaited one after another,\n" +
      "  take about how long in total -- 100ms or 200ms?",
    "200ms",
    "Sequential awaits wait fully for the first before even starting the second. " +
      "Promise.all runs them concurrently instead.",
  );

  await ui.challenge(
    [
      "Write delay(ms, value) returning a Promise that resolves to",
      "value after ms milliseconds. Using Promise.all, run",
      'delay(50, "x") and delay(50, "y") concurrently and print the',
      "resulting array.",
    ],
    [],
    ['["x","y"]'],
    [
      "function delay(ms, value) {",
      "  return new Promise((resolve) => setTimeout(() => resolve(value), ms));",
      "}",
      "",
      "(async () => {",
      '  const result = await Promise.all([delay(50, "x"), delay(50, "y")]);',
      "  console.log(JSON.stringify(result));",
      "})();",
    ],
  );

  await ui.waitEnter();
  ui.clearScreen();
  ui.heading("SUMMARY");

  console.log("   - a Promise is pending, then settles once, to fulfilled or rejected");
  console.log("   - await is unwrapping that same object -- .then()/.catch() is the");
  console.log("     older syntax for the identical thing");
  console.log("   - an awaited rejection throws, catchable with ordinary try/catch");
  console.log("   - Promise.all runs independent awaits concurrently instead of");
  console.log("     one after another");
  console.log();
  console.log("  Module 12 is working with a real API: fetch and JSON, both");
  console.log("  already usable with everything covered here.");
  await ui.waitEnter();
}

// The two live endpoints this module calls, plus the timeout wrapping every
// call to them: a student with no network, or a slow one, must see a
// labelled fallback and keep going, never a hang and never a raw stack
// trace where the rest of the course prints a lesson.
const FETCH_TIMEOUT_MS = 5000;
const POST_URL = "https://jsonplaceholder.typicode.com/posts/1";
const MISSING_URL = "https://jsonplaceholder.typicode.com/posts/999999";

async function lesson12Apis() {
  ui.title("MODULE 12 - WORKING WITH APIS");

  ui.heading("PART 1: fetch and the two Promises inside it");

  console.log("  fetch(url) returns a Promise -- but not of the parsed body.");
  console.log("  It resolves to a Response object: status, headers, and a");
  console.log("  body that has not been read yet.");
  console.log();
  console.log("    const response = await fetch(url);");
  console.log("    const data = await response.json();  // itself a Promise");
  console.log();
  console.log("  Two awaits, because .json() has to read and parse the body,");
  console.log("  which is its own asynchronous step -- not part of what the");
  console.log("  first await already did.");
  console.log();
  console.log("  A real request, right now:");
  console.log();
  console.log(`    await fetch("${POST_URL}")`);
  console.log();

  try {
    const response = await fetch(POST_URL, {
      signal: AbortSignal.timeout(FETCH_TIMEOUT_MS),
    });
    if (!response.ok) {
      // PART 2 covers this properly -- checking .ok before trusting the
      // body is the module's whole point, so even this first, otherwise
      // happy-path example does it, not just the part that names the trap.
      console.log(`  Running: status ${response.status}, not ok -- skipping .json()`);
    } else {
      const post = await response.json();
      console.log(`  Running: status ${response.status}, title: "${post.title}"`);
    }
  } catch (e) {
    console.log("  Couldn't reach the network just now -- here's what a");
    console.log("  successful call returns (a real response, captured");
    console.log("  earlier, not invented):");
    console.log();
    console.log(
      '  Running (fallback): status 200, title: "sunt aut facere repellat ' +
        'provident occaecati excepturi optio reprehenderit"',
    );
  }

  await ui.waitEnter();
  ui.clearScreen();
  ui.heading("PART 2: the trap -- fetch does not reject on a 404");

  console.log("  A rejected Promise means the REQUEST failed: no route, no");
  console.log("  DNS, connection refused. A 404 or a 500 is a completed,");
  console.log("  successful HTTP exchange -- the server answered, it just");
  console.log("  answered \"not found\". fetch has no way to know that means");
  console.log("  failure to your program, so it does not reject on one.");
  console.log();
  console.log("  Checking is the caller's job: response.ok (true only for");
  console.log("  200-299) or response.status directly.");
  console.log();
  console.log("  A real request to a URL that does not exist:");
  console.log();
  console.log(`    await fetch("${MISSING_URL}")`);
  console.log();

  try {
    const response = await fetch(MISSING_URL, {
      signal: AbortSignal.timeout(FETCH_TIMEOUT_MS),
    });
    console.log("  Running: await fetch(...) completed. No throw.");
    console.log(`           response.ok     = ${response.ok}`);
    console.log(`           response.status = ${response.status}`);
  } catch (e) {
    console.log("  Couldn't reach the network just now -- here's what a");
    console.log("  real 404 looks like (a real response, captured earlier,");
    console.log("  not invented):");
    console.log();
    console.log("  Running (fallback): await fetch(...) completed. No throw.");
    console.log("                      response.ok     = false");
    console.log("                      response.status = 404");
  }

  await ui.waitEnter();
  ui.clearScreen();
  ui.heading("PART 3: try/catch around a GENUINE network failure");

  console.log("  Contrast with PART 2: that 404 did not throw. This one");
  console.log("  does -- a host that cannot be resolved at all is a real");
  console.log("  network failure. Module 10's try/catch catches it, no new");
  console.log("  mechanism:");
  console.log();
  console.log("    try {");
  console.log('      await fetch("https://does-not-exist.invalid/");');
  console.log("    } catch (e) {");
  console.log('      console.log("caught:", e.message);');
  console.log("    }");
  console.log();

  try {
    await fetch("https://does-not-exist.invalid/", {
      signal: AbortSignal.timeout(FETCH_TIMEOUT_MS),
    });
    console.log("  Running: no throw -- that host was not supposed to resolve.");
  } catch (e) {
    console.log(`  Running: caught: ${e.message}`);
  }

  console.log();
  console.log("  Same fetch, same await, side by side with PART 2: one");
  console.log("  completes normally carrying bad news inside it, the other");
  console.log("  never completes at all.");

  await ui.waitEnter();
  ui.clearScreen();
  ui.exercise(12);

  await ui.question(
    "A fetch reaches the server, and the server answers with a 404\n" +
      "  Not Found. Does the Promise fetch() returns reject?  (answer: yes or no)",
    "no",
    "fetch only rejects on a genuine network failure -- a 404 is a completed, " +
      "successful HTTP exchange with bad news inside it.",
  );

  await ui.question(
    "response.json() -- does it return the parsed data directly, or\n" +
      "  a Promise of it?",
    "a promise",
    "Reading and parsing the body is its own asynchronous step, so .json() " +
      "needs its own await, exactly like fetch() itself.",
  );

  await ui.question(
    "Which Response property is true only for a 200-299 status?",
    "ok",
    "response.ok covers any 2xx status; response.status gives the exact code " +
      "(404, 500, ...) when ok alone isn't specific enough.",
  );

  await ui.challenge(
    [
      "Here is what a fetch response's .json() already gave you, as a",
      "plain string:",
      "",
      '  const raw = \'{"userId":1,"id":1,"title":"real API demo"}\';',
      "",
      "Parse it with JSON.parse and print the title.",
    ],
    [],
    ["real API demo"],
    [
      'const raw = \'{"userId":1,"id":1,"title":"real API demo"}\';',
      "const post = JSON.parse(raw);",
      "console.log(post.title);",
    ],
  );

  await ui.waitEnter();
  ui.clearScreen();
  ui.heading("SUMMARY");

  console.log("   - fetch(url) resolves to a Response, not the parsed body --");
  console.log("     response.json() is a second, separate Promise");
  console.log("   - fetch rejects only on a genuine network failure (DNS,");
  console.log("     connection refused, timeout) -- never on a 404 or a 500");
  console.log("   - a 404/500 is a completed, successful exchange; checking");
  console.log("     response.ok or response.status is the caller's job");
  console.log("   - try/catch (module 10) still catches the failures that");
  console.log("     really are rejections -- no new mechanism, same as always");
  await ui.waitEnter();
}

// Every temp file module 13 writes, and the child-process discipline that
// mirrors the C course's own module 16: spawnSync with an argument array --
// never a shell string assembled by hand, so there is nothing to quote or
// escape even though every path here is one this module wrote itself -- and
// the exit code checked before any output from the child is shown as real.
// Files are removed in a `finally`, unconditionally, whether the child
// succeeded, failed, or the write itself never happened.
const P_LOCAL_MODULE = path.join(os.tmpdir(), "codelearner_m13_greet.js");
const P_MIXED_JS = path.join(os.tmpdir(), "codelearner_m13_mixed.js");
const P_ESM_DEMO = path.join(os.tmpdir(), "codelearner_m13_esm_demo.mjs");

// Runs `scriptPath` in a fresh node process -- the same executable already
// running this course (process.execPath), not a "node" looked up on PATH,
// so there is no missing-tool case to handle here the way the C course's
// run_shell() has to for cc/ar/ls. `exitedCleanly` is what callers check
// before treating stdout as real output; a caller checking exitCode alone
// can still narrate a crash as a teaching example. `scriptArgs` (module 14's
// own addition) land in the child's process.argv after the script path,
// exactly where a real command-line invocation would put them.
function runNode(scriptPath, scriptArgs = []) {
  const result = spawnSync(process.execPath, [scriptPath, ...scriptArgs], {
    encoding: "utf8",
  });
  return {
    exitedCleanly: !result.error && result.status === 0,
    exitCode: result.status,
    stdout: result.stdout ?? "",
    stderr: result.stderr ?? "",
  };
}

async function lesson13Modules() {
  ui.title("MODULE 13 - MODULES");

  ui.heading("PART 1: require and module.exports -- this course's own pattern");

  console.log("  Every module in this course has been using this already.");
  console.log("  main.js's own first lines:");
  console.log();
  console.log('    const ui = require("./ui.js");');
  console.log();
  console.log("  and ui.js's own last lines -- the exact thing that call");
  console.log("  gets back:");
  console.log();
  console.log("    module.exports = {");
  console.log("      ask,");
  console.log("      isInputClosed,");
  console.log("      clearScreen,");
  console.log("      waitEnter,");
  console.log("      rule,");
  console.log("      title,");
  console.log("      heading,");
  console.log("      askYes,");
  console.log("      exercise,");
  console.log("      question,");
  console.log("      challenge,");
  console.log("      closeInput,");
  console.log("    };");
  console.log();
  console.log('  require("./ui.js") runs ui.js once, top to bottom, then hands');
  console.log("  back whatever it assigned to module.exports -- that object,");
  console.log("  and nothing else about ui.js, is visible to the file that");
  console.log("  required it. The `ui.` in every ui.title(...), ui.heading(...)");
  console.log("  call in this course is that object.");
  console.log();
  console.log("  This very file does the identical thing in both directions --");
  console.log("  requiring ui.js at the top:");
  console.log();
  console.log('    const ui = require("./ui.js");');
  console.log();
  console.log("  and, at the bottom, being required BY main.js in turn:");
  console.log();
  console.log("    module.exports = { lesson11Promises, lesson12Apis, lesson13Modules };");
  console.log();
  console.log("  CommonJS -- this require()/module.exports pair -- is not a");
  console.log("  topic being introduced now so much as named: it is what this");
  console.log("  whole course has been written in since module 1.");

  await ui.waitEnter();
  ui.clearScreen();
  ui.heading("PART 2: writing your own module, required live");

  console.log("  A file becomes a module the moment it assigns to");
  console.log("  module.exports -- no special extension, no declaration.");
  console.log("  Writing one to a real file right now:");
  console.log();

  const greetSource =
    "function shout(text) {\n" +
    '  return text.toUpperCase() + "!";\n' +
    "}\n" +
    "\n" +
    "function repeat(text, times) {\n" +
    "  return text.repeat(times);\n" +
    "}\n" +
    "\n" +
    "module.exports = { shout, repeat };\n";

  for (const line of greetSource.trimEnd().split("\n")) {
    console.log(`    ${line}`);
  }
  console.log();
  console.log("  Requiring it from another file works exactly like requiring");
  console.log("  ui.js -- a relative path, destructured on the way in:");
  console.log();
  console.log('    const { shout, repeat } = require("./greet.js");');
  console.log('    console.log(shout("modules work"));');
  console.log('    console.log(repeat("ab", 3));');
  console.log();

  let wroteGreet = false;
  try {
    fs.writeFileSync(P_LOCAL_MODULE, greetSource);
    wroteGreet = true;
  } catch {
    console.log(`  (could not write ${P_LOCAL_MODULE} -- skipping this demonstration)`);
  }

  if (wroteGreet) {
    try {
      const { shout, repeat } = require(P_LOCAL_MODULE);
      console.log(`  Running: shout("modules work") = "${shout("modules work")}"`);
      console.log(`  Running: repeat("ab", 3) = "${repeat("ab", 3)}"`);
    } finally {
      fs.rmSync(P_LOCAL_MODULE, { force: true });
    }
  }

  await ui.waitEnter();
  ui.clearScreen();
  ui.heading("PART 3: import/export -- and why it needs a fresh process");

  console.log("  CommonJS is not the only module system Node runs. ESM --");
  console.log("  import/export -- is JavaScript's own standardized module");
  console.log("  syntax, added to the language after CommonJS already existed");
  console.log("  and Node already depended on it, so Node supports both");
  console.log("  instead of replacing one with the other.");
  console.log();
  console.log("  Node decides which one a file uses ONCE, for the whole file,");
  console.log("  before running any of it: a .mjs extension always means ESM,");
  console.log("  a .cjs extension always means CommonJS, and a plain .js file");
  console.log('  falls back to whatever "type" a package.json above it');
  console.log('  declares ("module" for ESM, anything else -- or no');
  console.log("  package.json at all, which is this course's own");
  console.log("  javascript/ folder -- for CommonJS).");
  console.log();
  console.log("  The trap: adding one `import` line to a file that already");
  console.log("  requires things -- the shape every file in this course is");
  console.log("  already in -- does not add ESM syntax to a CommonJS file. It");
  console.log("  makes Node re-decide the WHOLE file is ESM, and require only");
  console.log("  exists inside CommonJS. Two lines, written to a real file and");
  console.log("  run for real, right now:");
  console.log();
  console.log('    const fs = require("node:fs");');
  console.log('    import { basename } from "node:path";');
  console.log();
  console.log("    console.log(basename(__filename));");
  console.log();

  const mixedSource =
    'const fs = require("node:fs");\n' +
    'import { basename } from "node:path";\n' +
    "\n" +
    "console.log(basename(__filename));\n";

  let wroteMixed = false;
  try {
    fs.writeFileSync(P_MIXED_JS, mixedSource);
    wroteMixed = true;
  } catch {
    console.log(`  (could not write ${P_MIXED_JS} -- skipping this demonstration)`);
  }

  if (wroteMixed) {
    try {
      const result = runNode(P_MIXED_JS);
      const referenceErrorLine = result.stderr
        .split("\n")
        .find((line) => line.includes("ReferenceError: require is not defined"));
      const failedTheExpectedWay = result.exitCode === 1 && referenceErrorLine !== undefined;

      if (failedTheExpectedWay) {
        console.log("  A real node process, run right now, refusing to start:");
        console.log();
        console.log(`    ${referenceErrorLine.trim()}`);
        console.log();
        console.log("  Not a SyntaxError -- Node parsed the file fine, decided");
        console.log("  the whole thing is an ES module because of that one");
        console.log("  import line, and only then hit a name -- require --");
        console.log("  that CommonJS defines and ESM never does.");
      } else {
        console.log("  (that demonstration did not fail the way it was expected");
        console.log("   to on this node install -- skipping the claim rather");
        console.log("   than showing output that is not real)");
      }
    } finally {
      fs.rmSync(P_MIXED_JS, { force: true });
    }
  }

  console.log();
  console.log("  The extension-based version needs no mixing and no");
  console.log("  package.json at all -- a .mjs file is ESM on its own, free to");
  console.log("  use both import and export in the same file. It cannot be");
  console.log("  loaded into this already-running process the way PART 2's");
  console.log("  greet.js was -- main.js was already decided to be CommonJS");
  console.log("  the moment it started, and import is syntax Node's parser");
  console.log("  only accepts while it is already parsing a file as an ES");
  console.log("  module. Running it in its own, fresh node process instead,");
  console.log("  for real, right now:");
  console.log();

  const esmSource =
    'import { basename } from "node:path";\n' +
    "\n" +
    "export function shout(text) {\n" +
    '  return text.toUpperCase() + "!";\n' +
    "}\n" +
    "\n" +
    'console.log("file:", basename(import.meta.url));\n' +
    'console.log(`shout("esm works") = "${shout("esm works")}"`);\n';

  for (const line of esmSource.trimEnd().split("\n")) {
    console.log(`    ${line}`);
  }
  console.log();
  console.log(`    $ node ${path.basename(P_ESM_DEMO)}`);
  console.log();

  let wroteEsm = false;
  try {
    fs.writeFileSync(P_ESM_DEMO, esmSource);
    wroteEsm = true;
  } catch {
    console.log(`  (could not write ${P_ESM_DEMO} -- skipping this demonstration)`);
  }

  if (wroteEsm) {
    try {
      const result = runNode(P_ESM_DEMO);
      if (result.exitedCleanly) {
        console.log("  Real output from that real, freshly started process:");
        console.log();
        for (const line of result.stdout.trimEnd().split("\n")) {
          console.log(`    ${line}`);
        }
      } else {
        console.log(`  That demonstration did not exit cleanly on this node`);
        console.log(`  install (exit code ${result.exitCode}) -- skipping the`);
        console.log("  claim rather than showing output that is not real.");
      }
    } finally {
      fs.rmSync(P_ESM_DEMO, { force: true });
    }
  }

  await ui.waitEnter();
  ui.clearScreen();
  ui.exercise(13);

  await ui.question(
    "To require a file you wrote yourself, does the path need to\n" +
      '  start with ./ or ../, or can it be a bare name like "myModule"?',
    "./ or ../",
    "A bare name looks up an installed package in node_modules; your own files need an " +
      "explicit relative (or absolute) path.",
  );

  await ui.question(
    "require(\"./foo.js\") hands its caller back the file's local\n" +
      "  variables, or just whatever was assigned to module.exports --\n" +
      "  which one?",
    "module.exports",
    "Whatever module.exports is set to when the file finishes running is exactly what " +
      "require(...) hands back -- nothing else about that file is visible outside it.",
  );

  await ui.question(
    "A file already full of require() calls gets one bare import\n" +
      "  statement added at the top, no package.json anywhere above it.\n" +
      "  Does only that one line fail, or does the WHOLE file stop being\n" +
      "  CommonJS?",
    "the whole file",
    "Node decides a file's module type once, before running any of it -- one import line " +
      "flips the entire file to ESM, breaking every require() already in it, not just itself.",
  );

  await ui.challenge(
    [
      "Write math.js, exporting add(a, b) and subtract(a, b) with",
      "module.exports. Write test.js requiring math.js and printing",
      "add(2, 3), then subtract(5, 2).",
    ],
    [],
    ["5", "3"],
    [
      "// math.js",
      "function add(a, b) {",
      "  return a + b;",
      "}",
      "",
      "function subtract(a, b) {",
      "  return a - b;",
      "}",
      "",
      "module.exports = { add, subtract };",
      "",
      "// test.js",
      'const { add, subtract } = require("./math.js");',
      "",
      "console.log(add(2, 3));",
      "console.log(subtract(5, 2));",
    ],
  );

  await ui.waitEnter();
  ui.clearScreen();
  ui.heading("SUMMARY");

  console.log("   - require(path) runs that file once and returns whatever it assigned");
  console.log("     to module.exports -- this course's own ui.js, and every");
  console.log("     lessons_*.js file, already work exactly this way");
  console.log("   - a bare require() name looks up an installed package; your own files");
  console.log("     need an explicit ./ or ../ path");
  console.log("   - import/export is a second, standardized module system Node also");
  console.log('     supports -- a .mjs file (or "type": "module" in package.json) is ESM');
  console.log("   - the two do not mix inside one file: one import statement makes Node");
  console.log("     treat the WHOLE file as ESM, where require is not defined at all");
  await ui.waitEnter();
}

// Files module 14 writes for its own two, unrelated demonstrations: PART
// 1/2's sync/async read-write round trip, and PART 3's child script that
// prints its own process.argv. Same discipline as module 13's block above --
// explicit paths under os.tmpdir(), removed in a finally regardless of
// outcome.
const P_M14_SYNC_FILE = path.join(os.tmpdir(), "codelearner_m14_sync.txt");
const P_M14_ASYNC_FILE = path.join(os.tmpdir(), "codelearner_m14_async.txt");
const P_M14_ARGV_SCRIPT = path.join(os.tmpdir(), "codelearner_m14_argv.js");

async function lesson14NodeStdlib() {
  ui.title("MODULE 14 - NODE-SPECIFIC STDLIB");

  console.log("  Module 13 already called fs.writeFileSync and fs.rmSync");
  console.log("  three times each: once for a file it required, twice more");
  console.log("  for files it ran as separate node processes instead,");
  console.log("  cleaning each one up afterward -- fs was plumbing there,");
  console.log("  in service of teaching require()/module.exports. This module");
  console.log("  teaches fs itself: what it can do, and where Node's version");
  console.log("  of \"standard library\" differs from what a browser's");
  console.log("  JavaScript is allowed to touch at all.");

  ui.heading("PART 1: fs.readFileSync/writeFileSync -- blocking, and that's fine here");

  console.log("  fs is Node's filesystem module -- direct disk access a");
  console.log("  browser's JavaScript never gets (PART 4 comes back to why).");
  console.log("  The Sync functions do exactly what their name says: block");
  console.log("  the whole process until the read or write finishes.");
  console.log();
  console.log('    fs.writeFileSync(path, "hello\\n");');
  console.log('    const contents = fs.readFileSync(path, "utf8");');
  console.log();
  console.log("  Fine for a short CLI tool with nothing else to do while it");
  console.log("  waits -- a problem for a server handling other requests at");
  console.log("  the same time, which is what PART 2 is for. A real file,");
  console.log("  written and read back right now:");
  console.log();

  let wroteSyncFile = false;
  try {
    fs.writeFileSync(P_M14_SYNC_FILE, "hello from fs.writeFileSync\n");
    wroteSyncFile = true;
  } catch {
    console.log(`  (could not write ${P_M14_SYNC_FILE} -- skipping this demonstration)`);
  }

  if (wroteSyncFile) {
    try {
      const contents = fs.readFileSync(P_M14_SYNC_FILE, "utf8");
      console.log(`  Running: wrote and read back ${JSON.stringify(contents)}`);
    } finally {
      fs.rmSync(P_M14_SYNC_FILE, { force: true });
    }
  }

  await ui.waitEnter();
  ui.clearScreen();
  ui.heading("PART 2: fs.promises.readFile/writeFile -- the non-blocking twin");

  console.log("  Same two operations, returning Promises instead of blocking");
  console.log("  -- the exact await pattern module 11 named and module 12");
  console.log("  used on fetch, applied to the filesystem instead of the");
  console.log("  network:");
  console.log();
  console.log('    await fs.promises.writeFile(path, "hello\\n");');
  console.log('    const contents = await fs.promises.readFile(path, "utf8");');
  console.log();
  console.log("  While one of these awaits is pending, the rest of the");
  console.log("  process is free to do other work -- exactly what PART 1's");
  console.log("  Sync pair never allows. A real file, written and read back");
  console.log("  the async way, right now:");
  console.log();

  let wroteAsyncFile = false;
  try {
    await fs.promises.writeFile(P_M14_ASYNC_FILE, "hello from fs.promises.writeFile\n");
    wroteAsyncFile = true;
  } catch {
    console.log(`  (could not write ${P_M14_ASYNC_FILE} -- skipping this demonstration)`);
  }

  if (wroteAsyncFile) {
    try {
      const contents = await fs.promises.readFile(P_M14_ASYNC_FILE, "utf8");
      console.log(`  Running: wrote and read back ${JSON.stringify(contents)}`);
    } finally {
      fs.rmSync(P_M14_ASYNC_FILE, { force: true });
    }
  }

  await ui.waitEnter();
  ui.clearScreen();
  ui.heading("PART 3: process.argv -- Node's version of argc/argv");

  console.log("  Every argument typed after `node script.js` on the command");
  console.log("  line lands in one array, process.argv. Index 0 is always");
  console.log("  the node executable running the script, index 1 is the");
  console.log("  script's own path, and your own arguments start at index 2:");
  console.log();
  console.log("    $ node script.js alpha beta");
  console.log("    process.argv[0] -> the node executable");
  console.log("    process.argv[1] -> script.js");
  console.log("    process.argv[2] -> \"alpha\"");
  console.log("    process.argv[3] -> \"beta\"");
  console.log();
  console.log("  A real child process, written to a real file and started");
  console.log("  right now with two real arguments, printing its own");
  console.log("  process.argv:");
  console.log();

  const argvSource =
    'const path = require("node:path");\n' +
    'console.log("process.argv.length            =", process.argv.length);\n' +
    'console.log("process.argv[0] (node itself)  =", path.basename(process.argv[0]));\n' +
    'console.log("process.argv[1] (this script)  =", path.basename(process.argv[1]));\n' +
    'console.log("process.argv.slice(2) (yours)  =", JSON.stringify(process.argv.slice(2)));\n';

  for (const line of argvSource.trimEnd().split("\n")) {
    console.log(`    ${line}`);
  }
  console.log();
  console.log(`    $ node ${path.basename(P_M14_ARGV_SCRIPT)} alpha beta`);
  console.log();

  let wroteArgvScript = false;
  try {
    fs.writeFileSync(P_M14_ARGV_SCRIPT, argvSource);
    wroteArgvScript = true;
  } catch {
    console.log(`  (could not write ${P_M14_ARGV_SCRIPT} -- skipping this demonstration)`);
  }

  if (wroteArgvScript) {
    try {
      const result = runNode(P_M14_ARGV_SCRIPT, ["alpha", "beta"]);
      if (result.exitedCleanly) {
        console.log("  Real output from that real, freshly started process:");
        console.log();
        for (const line of result.stdout.trimEnd().split("\n")) {
          console.log(`    ${line}`);
        }
      } else {
        console.log(`  That demonstration did not exit cleanly on this node`);
        console.log(`  install (exit code ${result.exitCode}) -- skipping the`);
        console.log("  claim rather than showing output that is not real.");
      }
    } finally {
      fs.rmSync(P_M14_ARGV_SCRIPT, { force: true });
    }
  }

  await ui.waitEnter();
  ui.clearScreen();
  ui.heading("PART 4: two things a browser's JavaScript cannot do");

  console.log("  (a) Direct filesystem access. PART 1 and PART 2 work");
  console.log("  because Node trusts the process it is running in -- a");
  console.log("  script you started yourself, on your own machine. A");
  console.log("  browser runs code from any website you happen to visit, so");
  console.log("  it sandboxes fs away entirely: there is no fs.readFileSync");
  console.log("  in browser JavaScript, at any privilege level, on purpose.");
  console.log();
  console.log("  (b) process vs window. A Node script has no browser to run");
  console.log("  in, so it gets process instead: process.argv (PART 3),");
  console.log("  process.env, process.exit(code). A browser script gets");
  console.log("  window and location instead -- the current page's URL, not");
  console.log("  a command line, because there is no command line to have");
  console.log("  one.");

  await ui.waitEnter();
  ui.clearScreen();
  ui.exercise(14);

  await ui.question(
    "fs.readFileSync(path) -- does it block the whole process until\n" +
      "  the read finishes, or does it return a Promise immediately?",
    "blocks",
    "The Sync functions block by design -- fine for a short CLI tool, a problem for anything " +
      "else with work to do while it waits.",
  );

  await ui.question(
    "In process.argv, is index 0 the path to the script being run, or\n" +
      "  the path to the node executable running it?",
    "the node executable",
    "process.argv[0] is always the node executable; process.argv[1] is the script -- your own " +
      "arguments start at index 2.",
  );

  await ui.question(
    "Can a browser's JavaScript read an arbitrary file off the\n" +
      "  visitor's disk the way Node's fs module just did in PART 1?\n" +
      "  (answer: yes or no)",
    "no",
    "Browsers sandbox real filesystem access away entirely -- a website's JavaScript cannot do " +
      "what a Node script just did.",
  );

  await ui.challenge(
    [
      "Read a word from stdin. Write it to a file with",
      "fs.writeFileSync. Read that same file back with",
      'await fs.promises.readFile(path, "utf-8"), and print it.',
    ],
    ["hello"],
    ["hello"],
    [
      'const readline = require("node:readline");',
      'const fs = require("node:fs");',
      "const rl = readline.createInterface({ input: process.stdin });",
      "const it = rl[Symbol.asyncIterator]();",
      "",
      "async function main() {",
      "  const { value: word } = await it.next();",
      '  fs.writeFileSync("word.txt", word);',
      '  const contents = await fs.promises.readFile("word.txt", "utf-8");',
      "  console.log(contents);",
      "  rl.close();",
      "}",
      "",
      "main();",
    ],
  );

  await ui.waitEnter();
  ui.clearScreen();
  ui.heading("SUMMARY");

  console.log("   - fs.readFileSync/writeFileSync block the process until they finish --");
  console.log("     fine for a short CLI tool, not for anything else with work to do");
  console.log("   - fs.promises.readFile/writeFile return Promises instead, the same");
  console.log("     await pattern modules 11 and 12 already taught");
  console.log("   - process.argv[0] is the node executable, [1] is the script, and your");
  console.log("     own arguments start at index 2");
  console.log("   - direct filesystem access and the process object are both things a");
  console.log("     browser's JavaScript never gets");
  await ui.waitEnter();
}

module.exports = {
  lesson11Promises,
  lesson12Apis,
  lesson13Modules,
  lesson14NodeStdlib,
};
