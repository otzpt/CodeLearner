// Modules 6 to 10 - the part that sets JavaScript apart.
//
// No manual memory here either -- JavaScript collects its own garbage, same
// as Python. What replaces it: the exact same aliasing bug Python's lists
// have, a `this` that depends on how a function was called rather than
// where it was written, and privacy that Python only pretends to have.

const ui = require("./ui.js");

async function lesson06Arrays() {
  ui.title("MODULE 6 - ARRAYS");

  ui.heading("PART 1: const does not stop this -- the same bug Python has");

  console.log("    const a = [1, 2, 3];");
  console.log("    const b = a;");
  console.log("    b.push(4);");
  console.log("    console.log(a);");
  console.log();
  const a = [1, 2, 3];
  const b = a;
  b.push(4);
  console.log(`  Running: [${a}]`);
  console.log();
  console.log("  const stops b FROM BEING REASSIGNED to a different array.");
  console.log("  It says nothing about the array's own contents. b = a gives");
  console.log("  the same array a second name -- identical to Python's b = a");
  console.log("  aliasing bug, spelled with const instead of nothing at all.");
  console.log();
  console.log("  A real copy needs the spread operator:");
  console.log();
  console.log("    const c = [...a];");
  console.log("    c.push(5);");
  console.log();
  const c = [...a];
  c.push(5);
  console.log(`  Running: a=[${a}] c=[${c}]`);

  await ui.waitEnter();
  ui.clearScreen();
  ui.heading("PART 2: sort mutates, and sorts as text by default");

  console.log("    [10, 9, 2].sort()");
  console.log();
  console.log(`  Running: [${[10, 9, 2].sort()}]`);
  console.log();
  console.log("  Not [2, 9, 10]. sort() converts each element to a string");
  console.log('  first, and "10" sorts before "9" the same way "apple" sorts');
  console.log("  before \"b\" -- alphabetically, not numerically. For numbers,");
  console.log("  pass a comparator:");
  console.log();
  console.log("    [10, 9, 2].sort((x, y) => x - y)");
  console.log(`  Running: [${[10, 9, 2].sort((x, y) => x - y)}]`);
  console.log();
  console.log("  sort() also modifies the array in place AND returns it --");
  console.log("  unusual; most array methods do one or the other, not both.");
  console.log("  slice() (no arguments modify) makes a copy; splice() mutates.");

  await ui.waitEnter();
  ui.clearScreen();
  ui.heading("PART 3: map, filter, reduce");

  const nums = [1, 2, 3];
  console.log("    const nums = [1, 2, 3];");
  console.log("    nums.map(x => x * 2)");
  console.log("    nums.filter(x => x % 2 === 0)");
  console.log("    nums.reduce((sum, x) => sum + x, 0)");
  console.log();
  console.log(`  Running: map -> [${nums.map((x) => x * 2)}]`);
  console.log(`           filter -> [${nums.filter((x) => x % 2 === 0)}]`);
  console.log(`           reduce -> ${nums.reduce((sum, x) => sum + x, 0)}`);
  console.log();
  console.log("  Each returns a NEW array (map/filter) or a single value");
  console.log("  (reduce); none of the three touch nums itself.");

  await ui.waitEnter();
  ui.clearScreen();
  ui.exercise(6);

  await ui.question(
    "const a = [1,2]; const b = a; b.push(3);  -- does a change too?\n  (answer: yes or no)",
    "yes",
    "const only stops b being reassigned. b = a still aliases the same array."
  );
  await ui.question("What copies an array instead of aliasing it?", "[...a]", "The spread operator, or a.slice(), both make a real copy.");
  await ui.question(
    "[10, 9, 2].sort()  -- what order, by default?",
    "2 10 9",
    'sort() compares elements as text by default: "10" sorts before "9".'
  );

  await ui.challenge(
    ["Given nums = [3, 1, 2], print a SORTED COPY numerically", "(nums itself must stay [3, 1, 2]), then print nums."],
    [],
    ["[1, 2, 3]", "[3, 1, 2]"],
    ["const nums = [3, 1, 2];", "console.log([...nums].sort((a, b) => a - b));", "console.log(nums);"]
  );

  await ui.waitEnter();
  ui.clearScreen();
  ui.heading("SUMMARY");

  console.log("   - const stops reassignment, not mutation; b = a still aliases");
  console.log("   - [...a] copies; sort() mutates AND sorts as text by default");
  console.log("   - map/filter/reduce return something new; none mutate the original");
  await ui.waitEnter();
}

async function lesson07Functions() {
  ui.title("MODULE 7 - FUNCTIONS AND THIS");

  ui.heading("PART 1: default parameters and arrow functions");

  console.log("    function greet(name) { return `hello, ${name}`; }");
  console.log("    const add = (a, b) => a + b;");
  console.log("    function power(base, exp = 2) { return base ** exp; }");
  console.log();
  function greet(name) {
    return `hello, ${name}`;
  }
  const add = (a, b) => a + b;
  function power(base, exp = 2) {
    return base ** exp;
  }
  console.log(`  Running: ${greet("Ana")}, ${add(2, 3)}, ${power(3)}, ${power(3, 3)}`);

  await ui.waitEnter();
  ui.clearScreen();
  ui.heading("PART 2: this depends on how a function is called");

  console.log("    const obj = {");
  console.log('        name: "Ana",');
  console.log("        regular: function() { return this.name; },");
  console.log("        arrow: () => { return this.name; },");
  console.log("    };");
  console.log();
  const obj = {
    name: "Ana",
    regular: function () {
      return this.name;
    },
    arrow: () => {
      return this.name;
    },
  };
  console.log(`    obj.regular()  -> ${obj.regular()}`);
  console.log(`    obj.arrow()    -> ${obj.arrow()}`);
  console.log();
  console.log("  Both are called the same way: obj.method(). The difference");
  console.log("  is in how each kind of function decides what `this` means.");
  console.log("  A regular function's this is whatever sits left of the dot");
  console.log("  at the call site -- here, obj. An arrow function has no");
  console.log("  this of its own; it uses whatever this was where it was");
  console.log("  WRITTEN, not where it is called -- here, that is outside");
  console.log("  any object, so this.name is undefined.");
  console.log();
  console.log("  Rule of thumb: methods on an object use `function`. Arrow");
  console.log("  functions are for callbacks, where you usually WANT this to");
  console.log("  stay whatever it was outside -- map/filter callbacks, timers.");

  await ui.waitEnter();
  ui.clearScreen();
  ui.heading("PART 3: closures, and the var/let loop trap");

  console.log("    const fns = [];");
  console.log("    for (var i = 0; i < 3; i++) { fns.push(() => i); }");
  console.log("    console.log(fns.map(f => f()));");
  console.log();
  const fnsVar = [];
  for (var i = 0; i < 3; i++) {
    fnsVar.push(() => i);
  }
  console.log(`  Running with var:  [${fnsVar.map((f) => f())}]`);
  console.log();
  console.log("  Not [0, 1, 2]. var is not scoped to the loop body -- there");
  console.log("  is only ONE i for the whole loop, and by the time any of");
  console.log("  the three functions run, the loop has finished and i is 3.");
  console.log("  Every closure captured the SAME variable.");
  console.log();
  const fnsLet = [];
  for (let j = 0; j < 3; j++) {
    fnsLet.push(() => j);
  }
  console.log("    for (let j = 0; j < 3; j++) { fns.push(() => j); }");
  console.log(`  Running with let:  [${fnsLet.map((f) => f())}]`);
  console.log();
  console.log("  let creates a fresh binding on every iteration, so each");
  console.log("  closure captures its own j. This alone is reason enough to");
  console.log("  never use var in a loop whose body creates a function.");

  await ui.waitEnter();
  ui.clearScreen();
  ui.exercise(7);

  await ui.question(
    "obj.regular() where regular is a plain function -- what does\n  this refer to inside it?",
    "obj",
    "A regular function's this is decided by what is left of the dot at the call site."
  );
  await ui.question(
    "for (var i ...) { fns.push(() => i); }  three closures pushed --\n  do they each capture a different i?  (answer: yes or no)",
    "no",
    "var is not per-iteration; all three closures share the one i, which is 3 " +
      "by the time any of them runs."
  );

  await ui.challenge(
    ["Write swap(pair) that takes a two-element array and returns", "a new array with the elements reversed. Test it on [1, 2]."],
    [],
    ["[2, 1]"],
    ["function swap([a, b]) {", "    return [b, a];", "}", "", "console.log(swap([1, 2]));"]
  );

  await ui.waitEnter();
  ui.clearScreen();
  ui.heading("SUMMARY");

  console.log("   - regular function: this = whatever is left of the dot when called");
  console.log("   - arrow function: this = whatever it was where the arrow was written");
  console.log("   - let per iteration in a loop; var shares one binding across all of it");
  console.log();
  console.log("  Module 8: objects, and the one place == and === are both wrong.");
  await ui.waitEnter();
}

async function lesson08Objects() {
  ui.title("MODULE 8 - OBJECTS");

  ui.heading("PART 1: property access, and JSON");

  const aluno = { nome: "Ana", nota: 15 };
  console.log('    const aluno = { nome: "Ana", nota: 15 };');
  console.log(`    aluno.nome    -> ${aluno.nome}`);
  console.log(`    aluno["nota"] -> ${aluno["nota"]}`);
  console.log();
  aluno.curso = "Info";
  console.log('    aluno.curso = "Info";');
  console.log(`    Object.keys(aluno) -> [${Object.keys(aluno)}]`);
  console.log();
  console.log("    JSON.stringify(aluno)");
  console.log(`  Running: ${JSON.stringify(aluno)}`);
  console.log(`           JSON.parse(...).nota -> ${JSON.parse(JSON.stringify(aluno)).nota}`);

  await ui.waitEnter();
  ui.clearScreen();
  ui.heading("PART 2: == and === are both wrong for comparing objects");

  console.log(`    [1, 2] == [1, 2]   -> ${[1, 2] == [1, 2]}`);
  console.log(`    [1, 2] === [1, 2]  -> ${[1, 2] === [1, 2]}`);
  console.log();
  console.log("  Both false. Neither == nor === looks inside an array or");
  console.log("  object -- both compare REFERENCES, same rule as C's == on a");
  console.log("  char*, and neither operator has a content-comparing mode");
  console.log("  the way Python's == or C++'s std::string == do.");
  console.log();
  const x = [1, 2];
  console.log(`    const x = [1, 2]; x === x  -> ${x === x}`);
  console.log();
  console.log("  True -- same array, same reference. To compare contents,");
  console.log("  compare a serialised form, or write the comparison by hand:");
  console.log();
  console.log("    JSON.stringify([1,2]) === JSON.stringify([1,2])");
  console.log(`  Running: ${JSON.stringify([1, 2]) === JSON.stringify([1, 2])}`);

  await ui.waitEnter();
  ui.clearScreen();
  ui.heading("PART 3: optional chaining");

  console.log("    aluno.morada?.rua");
  console.log(`  Running: ${aluno.morada?.rua}`);
  console.log();
  console.log("  aluno has no morada property. Without ?., aluno.morada.rua");
  console.log("  would throw -- reading .rua off undefined is a TypeError,");
  console.log("  module 10 shows exactly that message. ?. stops at the first");
  console.log("  missing link and gives undefined instead of throwing.");

  await ui.waitEnter();
  ui.clearScreen();
  ui.exercise(8);

  await ui.question("[1, 2] === [1, 2]   Equal?", "no", "Two different arrays, two different references -- never equal.");
  await ui.question(
    "const x = [1,2]; x === x   Equal?",
    "yes",
    "Same reference this time -- === is true when both sides are the exact same object."
  );
  await ui.question("What does JSON.stringify turn an object into?", "a string", "A text representation, suitable for comparing contents or sending over a network.");

  await ui.challenge(
    ['Given aluno = { nome: "Ana", notas: [15, 18] }, print it as', "JSON, then print the second grade after parsing it back."],
    [],
    ['{"nome":"Ana","notas":[15,18]}', "18"],
    [
      'const aluno = { nome: "Ana", notas: [15, 18] };',
      "const texto = JSON.stringify(aluno);",
      "console.log(texto);",
      "console.log(JSON.parse(texto).notas[1]);",
    ]
  );

  await ui.waitEnter();
  ui.clearScreen();
  ui.heading("SUMMARY");

  console.log("   - both == and === compare objects/arrays by reference, not contents");
  console.log("   - JSON.stringify/.parse round-trips data, and compares contents indirectly");
  console.log("   - ?. stops at a missing link instead of throwing");
  await ui.waitEnter();
}

async function lesson09Classes() {
  ui.title("MODULE 9 - CLASSES");

  ui.heading("PART 1: class, constructor, and real private fields");

  console.log("    class Student {");
  console.log("        #grade;");
  console.log("        constructor(name, grade) {");
  console.log("            this.name = name;");
  console.log("            this.#grade = grade;");
  console.log("        }");
  console.log("        describe() { return `${this.name}: ${this.#grade}`; }");
  console.log("    }");
  console.log();

  class Student {
    #grade;
    constructor(name, grade) {
      this.name = name;
      this.#grade = grade;
    }
    describe() {
      return `${this.name}: ${this.#grade}`;
    }
  }

  const st = new Student("Ana", 15);
  console.log(`  Running: ${st.describe()}`);
  console.log();
  console.log("  #grade is a REAL private field -- unlike Python's leading");
  console.log("  underscore, which is only a convention. Trying to read");
  console.log("  s.#grade from outside the class is not just wrong, it is a");
  console.log("  SyntaxError caught before the program even runs:");
  console.log();
  console.log("    SyntaxError: Private field '#grade' must be declared in");
  console.log("                 an enclosing class");
  console.log();
  console.log("  Verified on Node 26. This is real enforcement, closer to");
  console.log("  C++'s private than to Python's convention-only underscore.");

  await ui.waitEnter();
  ui.clearScreen();
  ui.heading("PART 2: inheritance");

  console.log("    class Bolseiro extends Student {");
  console.log("        describe() { return `${super.describe()} (bolseiro)`; }");
  console.log("    }");
  console.log();

  class Bolseiro extends Student {
    describe() {
      return `${super.describe()} (bolseiro)`;
    }
  }

  const bo = new Bolseiro("Bea", 18);
  console.log(`  Running: ${bo.describe()}`);
  console.log(`           b instanceof Student -> ${bo instanceof Student}`);
  console.log();
  console.log("  extends and super work the same way C++ inheritance does --");
  console.log("  super.describe() reaches the parent's version explicitly.");

  await ui.waitEnter();
  ui.clearScreen();
  ui.heading("PART 3: garbage collection, same as Python -- no RAII here either");

  console.log("  Same story as Python: there is no destructor tied to a");
  console.log("  predictable moment the way C++'s is. An object is collected");
  console.log("  sometime after nothing references it any more -- not at a");
  console.log("  scope's closing brace, not on any schedule you control.");
  console.log();
  console.log("  For something that must close at a known point -- a file");
  console.log("  handle, a database connection -- Node's answer is `using`");
  console.log("  (a newer, less common feature) or, far more commonly, an");
  console.log("  explicit try/finally that calls .close() yourself:");
  console.log();
  console.log("    try {");
  console.log("        // use the resource");
  console.log("    } finally {");
  console.log("        resource.close();   // always runs, guaranteed");
  console.log("    }");

  await ui.waitEnter();
  ui.clearScreen();
  ui.exercise(9);

  await ui.question(
    "class Student { #grade; ... }  Can code outside the class read\n  s.#grade?  (answer: yes or no)",
    "no",
    "It is a SyntaxError, caught before the program runs -- real enforcement."
  );
  await ui.question(
    "Is a JS object destroyed at a specific, predictable moment, the\n  way a C++ destructor is?  (answer: yes or no)",
    "no",
    "Garbage collection timing is not guaranteed. Use try/finally for anything " +
      "that must close on time."
  );

  await ui.challenge(
    [
      "Write a class Counter with a private #count starting at 0,",
      "a method increment() that adds 1, and a method value() that",
      "returns the count. Create one, call increment() three times,",
      "and print value().",
    ],
    [],
    ["3"],
    [
      "class Counter {",
      "    #count = 0;",
      "    increment() { this.#count++; }",
      "    value() { return this.#count; }",
      "}",
      "",
      "const c = new Counter();",
      "c.increment();",
      "c.increment();",
      "c.increment();",
      "console.log(c.value());",
    ]
  );

  await ui.waitEnter();
  ui.clearScreen();
  ui.heading("SUMMARY");

  console.log("   - #field is real private -- a SyntaxError from outside, not a convention");
  console.log("   - extends/super work like C++ inheritance");
  console.log("   - no guaranteed destructor timing -- use try/finally for deterministic cleanup");
  await ui.waitEnter();
}

async function lesson10Errors() {
  ui.title("MODULE 10 - ERRORS");

  ui.heading("PART 1: try / catch / finally, and throw");

  console.log("    function dividir(a, b) {");
  console.log('        if (b === 0) throw new Error("divisao por zero");');
  console.log("        return a / b;");
  console.log("    }");
  console.log();
  console.log("    try {");
  console.log("        console.log(dividir(10, 2));");
  console.log("        console.log(dividir(1, 0));");
  console.log("    } catch (e) {");
  console.log("        console.log(`apanhado: ${e.message}`);");
  console.log("    } finally {");
  console.log('        console.log("sempre corre");');
  console.log("    }");
  console.log();

  function dividir(a, b) {
    if (b === 0) throw new Error("divisao por zero");
    return a / b;
  }

  console.log("  Running:");
  try {
    console.log(`    ${dividir(10, 2)}`);
    console.log(`    ${dividir(1, 0)}`);
  } catch (e) {
    console.log(`    apanhado: ${e.message}`);
  } finally {
    console.log("    sempre corre");
  }
  console.log();
  console.log("  The second dividir() never returns -- the throw jumps");
  console.log("  straight to catch. finally runs either way.");

  await ui.waitEnter();
  ui.clearScreen();
  ui.heading("PART 2: the errors you will actually meet");

  console.log("    error        typically means");
  ui.rule();
  console.log("    TypeError    called something not a function, or read a");
  console.log("                 property of undefined/null");
  console.log("    RangeError   a number outside an allowed range");
  console.log("    SyntaxError  invalid code, or invalid JSON to parse");
  console.log();

  try {
    null.foo;
  } catch (e) {
    console.log(`  Running: ${e.constructor.name}: ${e.message}`);
  }
  try {
    JSON.parse("{bad json}");
  } catch (e) {
    console.log(`           ${e.constructor.name}: ${e.message}`);
  }

  await ui.waitEnter();
  ui.clearScreen();
  ui.heading("PART 3: your own error types");

  console.log("    class ValidationError extends Error {");
  console.log("        constructor(msg) {");
  console.log("            super(msg);");
  console.log('            this.name = "ValidationError";');
  console.log("        }");
  console.log("    }");
  console.log();

  class ValidationError extends Error {
    constructor(msg) {
      super(msg);
      this.name = "ValidationError";
    }
  }

  try {
    throw new ValidationError("idade invalida");
  } catch (e) {
    console.log(`  Running: ${e.name}: ${e.message}, instanceof Error -> ${e instanceof Error}`);
  }
  console.log();
  console.log("  extends Error, plus super(msg) to set .message, is the");
  console.log("  standard shape -- the same class mechanics from module 9,");
  console.log("  applied to something you can catch specifically:");
  console.log();
  console.log("    catch (e) {");
  console.log("        if (e instanceof ValidationError) { ... }");
  console.log("        else throw e;   // not mine -- pass it on");
  console.log("    }");

  await ui.waitEnter();
  ui.clearScreen();
  ui.exercise(10);

  await ui.question(
    "try { risky(); } finally { cleanup(); }  -- if risky() throws\n  and is never caught, does cleanup() still run?  (answer: yes or no)",
    "yes",
    "finally always runs, caught or not."
  );
  await ui.question("null.foo   Which error does this raise?", "TypeError", "Reading a property off null or undefined is a TypeError.");
  await ui.question(
    "class MyError extends Error { ... }  -- what must the\n  constructor call to set .message correctly?",
    "super(msg)",
    "super(msg) runs Error's own constructor, which is what sets .message."
  );

  await ui.challenge(
    [
      "Write dividir(a, b) that throws an Error on b === 0. Call it",
      "with (10, 2) then (1, 0) inside one try, catching the error",
      "and printing its message, with a finally that always prints.",
    ],
    [],
    ["5", "apanhado: divisao por zero", "sempre corre"],
    [
      "function dividir(a, b) {",
      '    if (b === 0) throw new Error("divisao por zero");',
      "    return a / b;",
      "}",
      "",
      "try {",
      "    console.log(dividir(10, 2));",
      "    console.log(dividir(1, 0));",
      "} catch (e) {",
      "    console.log(`apanhado: ${e.message}`);",
      "} finally {",
      '    console.log("sempre corre");',
      "}",
    ]
  );

  await ui.waitEnter();
  ui.clearScreen();
  ui.heading("SUMMARY");

  console.log("   - throw to signal a problem; try/catch to handle it; finally always runs");
  console.log("   - TypeError, RangeError, SyntaxError -- know what each one means");
  console.log("   - extends Error + super(msg) is how you make your own catchable type");
  console.log();
  console.log("  That is the core course. Like Python, no manual memory --");
  console.log("  garbage collected, with try/finally standing in for RAII.");
  await ui.waitEnter();
}

module.exports = {
  lesson06Arrays,
  lesson07Functions,
  lesson08Objects,
  lesson09Classes,
  lesson10Errors,
};
