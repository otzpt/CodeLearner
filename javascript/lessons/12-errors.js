module.exports = {
  id: "errors",
  title: "When things go wrong",
  titlePt: "Quando as coisas correm mal",
  screens: [
    [
      "Every programmer sees errors all day. What matters is reading them. An error message has a type, a description, and a line number:",
      { code: "const total = 10;\nconsole.log(totl);", error: true },
      "ReferenceError: totl is not defined, on line 2. The name is misspelled. The course explains the type underneath, but the message itself already says almost everything.",
    ],
    [
      "The three types you will meet most:",
      "ReferenceError: a name that does not exist (often a typo).\nTypeError: a value used in a way it does not allow.\nSyntaxError: code typed in a way JavaScript cannot read, like a missing bracket. Node finds these before running anything.",
      { code: "const price = 5;\nprice();", error: true },
      "price holds a number, and price() tried to run it as if it were a function. \"is not a function\" means exactly that.",
    ],
    [
      "The other TypeError you will meet all the time:",
      { code: 'const user = undefined;\nconsole.log(user.name);', error: true },
      "Reading a property from undefined is the most common TypeError. Usually something you expected to exist was never set.",
    ],
    [
      "Sometimes an error is expected, and the program should carry on instead of stopping. try runs code; if it throws an error, catch receives it and runs instead:",
      { code: 'try {\n  console.log("before");\n  JSON.parse("this is not JSON");\n  console.log("never printed");\n} catch (error) {\n  console.log("Problem: " + error.message);\n}\nconsole.log("the program keeps going");' },
      "As soon as an error happens inside try, JavaScript jumps straight to catch, skipping the rest of try. error.message holds the description.",
      { quiz: "In a try block, an error happens on the second of four lines. Do lines 3 and 4 run?", choices: ["yes", "no"], answer: 2, why: "The error jumps straight to catch; the rest of try is skipped." },
    ],
    [
      "Your own code can raise an error with throw, to refuse something that makes no sense:",
      { code: 'function withdraw(balance, amount) {\n  if (amount > balance) {\n    throw new Error("not enough money");\n  }\n  return balance - amount;\n}\n\ntry {\n  console.log(withdraw(100, 30));\n  console.log(withdraw(100, 500));\n} catch (error) {\n  console.log("Refused: " + error.message);\n}' },
      { tip: "Read the error type, the message and the line number before changing anything." },
    ],
    [
      {
        exercise: "errors",
        task: 'Write divide(a, b) that throws an Error with the message "cannot divide by 0" when b is 0, and returns a / b otherwise. Inside one try, print divide(10, 2) and then divide(1, 0). In catch, print "Error: " followed by the message. After the try/catch, print "done".',
        expected: ["5", "Error: cannot divide by 0", "done"],
        hints: ['if (b === 0) { throw new Error("cannot divide by 0"); }', 'catch (error) { console.log("Error: " + error.message); }'],
        solution: 'function divide(a, b) {\n  if (b === 0) {\n    throw new Error("cannot divide by 0");\n  }\n  return a / b;\n}\n\ntry {\n  console.log(divide(10, 2));\n  console.log(divide(1, 0));\n} catch (error) {\n  console.log("Error: " + error.message);\n}\nconsole.log("done");',
      },
    ],
  ],
  recapPt: [
    "Uma mensagem de erro dá um tipo, uma descrição e um número de linha. Lê os três.",
    "ReferenceError: nome desconhecido. TypeError: valor mal usado. SyntaxError: código que não se consegue ler.",
    "try { ... } catch (error) { ... } trata um erro e deixa o programa continuar.",
    "throw new Error(\"mensagem\") lança o teu próprio erro.",
  ],
  recap: [
    "An error message gives a type, a description and a line number. Read all three.",
    "ReferenceError: unknown name. TypeError: wrong use of a value. SyntaxError: code that cannot be read.",
    "try { ... } catch (error) { ... } handles an error and lets the program continue.",
    "throw new Error(\"message\") raises your own error.",
  ],
};
