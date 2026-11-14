module.exports = {
  id: "errors",
  title: "When things go wrong",
  screens: [
    [
      "An error gives a type, a message and a line. Read all three:",
      { code: "const total = 10;\nconsole.log(totl);", error: true },
    ],
    [
      "TypeError means a value was used in a way it does not allow:",
      { code: "const price = 5;\nprice();", error: true },
      "price is a number, and () tried to run it. Reading from undefined is the other common one.",
    ],
    [
      "try runs code. If it throws, catch runs instead, and the program keeps going:",
      { code: 'try {\n  JSON.parse("not json");\n  console.log("skipped");\n} catch (error) {\n  console.log("Problem: " + error.message);\n}\nconsole.log("still running");' },
      { quiz: "An error happens on line 2 of a try block. Does line 3 run?", choices: ["yes", "no"], answer: 2, why: "It jumps straight to catch." },
    ],
    [
      "throw raises your own error:",
      { code: 'function withdraw(amount) {\n  if (amount > 100) {\n    throw new Error("not enough money");\n  }\n  return 100 - amount;\n}\ntry {\n  console.log(withdraw(500));\n} catch (error) {\n  console.log(error.message);\n}' },
    ],
    [
      {
        exercise: "errors",
        task: 'Write divide(a, b): throw an Error "cannot divide by 0" when b is 0, else return a / b. In one try, print divide(10, 2) then divide(1, 0). In catch, print "Error: " + the message. Then print "done".',
        expected: ["5", "Error: cannot divide by 0", "done"],
        hints: ['if (b === 0) throw new Error("cannot divide by 0");', 'catch (error) { console.log("Error: " + error.message); }'],
        solution: 'function divide(a, b) {\n  if (b === 0) {\n    throw new Error("cannot divide by 0");\n  }\n  return a / b;\n}\n\ntry {\n  console.log(divide(10, 2));\n  console.log(divide(1, 0));\n} catch (error) {\n  console.log("Error: " + error.message);\n}\nconsole.log("done");',
      },
    ],
  ],
  recap: [
    "Read the error type, message and line.",
    "ReferenceError: unknown name. TypeError: wrong use of a value. SyntaxError: unreadable code.",
    "try { } catch (error) { } handles an error; the program continues.",
    "throw new Error(\"...\") raises your own.",
  ],
};
