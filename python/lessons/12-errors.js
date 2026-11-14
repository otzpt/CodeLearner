module.exports = {
  id: "errors",
  title: "When things go wrong",
  titlePt: "Quando as coisas correm mal",
  screens: [
    [
      "Every programmer sees errors all day. What matters is reading them. Python's error report is called a traceback. Read it from the bottom: the last line has the type and the description, and the lines above it say where:",
      { code: "total = 10\nprint(totl)", error: true },
      "NameError: name 'totl' is not defined, on line 2. The name is misspelled. Python often even suggests the right one.",
    ],
    [
      "The types you will meet most:",
      "NameError: a name that does not exist (often a typo).\nTypeError: a value used in a way its type does not allow.\nValueError: the right type with a value that makes no sense.\nIndexError and KeyError: a position or key that is not there.\nSyntaxError and IndentationError: code Python cannot read.",
      { code: "prices = [2, 15, 30]\nprint(prices[3])", error: true },
    ],
    [
      "Sometimes an error is expected, and the program should carry on instead of stopping. try runs code; if it raises an error, except catches it and runs instead:",
      { code: 'try:\n    print("before")\n    number = int("hello")\n    print("never printed")\nexcept ValueError as error:\n    print("Problem:", error)\nprint("the program keeps going")' },
      "As soon as an error happens inside try, Python jumps straight to except, skipping the rest of try. Name the error type you expect, so other mistakes still show up.",
      { quiz: "In a try block, an error happens on the second of four lines. Do lines 3 and 4 run?", choices: ["yes", "no"], answer: 2, why: "The error jumps straight to except; the rest of try is skipped." },
    ],
    [
      "Your own code can raise an error, to refuse something that makes no sense:",
      { code: 'def withdraw(balance, amount):\n    if amount > balance:\n        raise ValueError("not enough money")\n    return balance - amount\n\ntry:\n    print(withdraw(100, 30))\n    print(withdraw(100, 500))\nexcept ValueError as error:\n    print("Refused:", error)' },
      { tip: "Read a traceback from the bottom: type, message, then the line." },
    ],
    [
      {
        exercise: "errors",
        task: 'Write divide(a, b) that raises a ValueError with the message "cannot divide by 0" when b is 0, and returns a / b otherwise. Inside one try, print divide(10, 2) and then divide(1, 0). In the except, print "Error:" followed by the message. After it, print "done".',
        expected: ["5.0", "Error: cannot divide by 0", "done"],
        hints: ['if b == 0:\n    raise ValueError("cannot divide by 0")', 'except ValueError as error:\n    print("Error:", error)'],
        solution: 'def divide(a, b):\n    if b == 0:\n        raise ValueError("cannot divide by 0")\n    return a / b\n\ntry:\n    print(divide(10, 2))\n    print(divide(1, 0))\nexcept ValueError as error:\n    print("Error:", error)\nprint("done")',
      },
    ],
  ],
  recapPt: [
    "Lê um traceback de baixo para cima: tipo, mensagem, depois a linha.",
    "NameError, TypeError, ValueError, IndexError, KeyError e SyntaxError são os mais comuns.",
    "try: ... except TipoDeErro as error: ... trata um erro e deixa o programa continuar.",
    "raise ValueError(\"mensagem\") lança o teu próprio erro.",
  ],
  recap: [
    "Read a traceback from the bottom: type, message, then the line.",
    "NameError, TypeError, ValueError, IndexError, KeyError and SyntaxError are the common ones.",
    "try: ... except SomeError as error: ... handles an error and lets the program continue.",
    "raise ValueError(\"message\") raises your own error.",
  ],
};
