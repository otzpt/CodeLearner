module.exports = {
  id: "exceptions",
  title: "When things go wrong: exceptions",
  titlePt: "Quando as coisas correm mal: exceções",
  screens: [
    [
      "When something goes wrong while running, C# throws an exception. If nothing catches it, the program stops and prints the type, a message and the line:",
      { code: "int total = 10;\nint people = 0;\nConsole.WriteLine(total / people);", error: true },
    ],
    [
      "try runs code; if it throws, catch receives the exception and runs instead, and the program carries on. .Message gives the description:",
      { code: 'try\n{\n    Console.WriteLine("before");\n    int n = int.Parse("hello");\n    Console.WriteLine("never printed");\n}\ncatch (FormatException e)\n{\n    Console.WriteLine("Problem: " + e.Message);\n}\nConsole.WriteLine("the program keeps going");' },
      "As soon as something throws inside try, C# jumps straight to catch, skipping the rest of try.",
      { quiz: "An exception happens on the second of four lines in a try. Do lines 3 and 4 run?", choices: ["yes", "no"], answer: 2, why: "It jumps straight to catch." },
    ],
    [
      "For typed numbers, int.TryParse avoids the exception: it gives back true or false, and puts the number in an out variable:",
      { code: 'foreach (string text in new[] { "42", "hello" })\n{\n    if (int.TryParse(text, out int n))\n    {\n        Console.WriteLine($"number {n}");\n    }\n    else\n    {\n        Console.WriteLine($"not a number: {text}");\n    }\n}' },
      "Your own code throws with throw new ArgumentException(\"message\") to refuse a bad value.",
      { tip: "throw to refuse; try/catch to handle; TryParse to check typed numbers." },
    ],
    [
      {
        exercise: "exceptions",
        task: 'Write double Divide(double a, double b) that throws new ArgumentException("cannot divide by 0") when b is 0, and returns a / b otherwise. Inside one try, print Divide(10, 4) and then Divide(1, 0). In the catch, print "Error: " and the message. Then print "done".',
        expected: ["2.5", "Error: cannot divide by 0", "done"],
        hints: ['if (b == 0) throw new ArgumentException("cannot divide by 0");', 'catch (ArgumentException e) { Console.WriteLine("Error: " + e.Message); }'],
        solution: 'double Divide(double a, double b)\n{\n    if (b == 0)\n    {\n        throw new ArgumentException("cannot divide by 0");\n    }\n    return a / b;\n}\n\ntry\n{\n    Console.WriteLine(Divide(10, 4));\n    Console.WriteLine(Divide(1, 0));\n}\ncatch (ArgumentException e)\n{\n    Console.WriteLine("Error: " + e.Message);\n}\nConsole.WriteLine("done");',
      },
    ],
  ],
  recapPt: [
    "Uma exceção não apanhada para o programa e mostra o tipo, a mensagem e a linha.",
    "try { } catch (TipoException e) { } apanha-a e o programa continua; e.Message dá a mensagem.",
    "int.TryParse(texto, out int n) verifica sem lançar exceção.",
    "throw new ArgumentException(\"...\") recusa um valor errado.",
  ],
  recap: [
    "An uncaught exception stops the program and shows the type, message and line.",
    "try { } catch (SomeException e) { } catches it; e.Message gives the message.",
    "int.TryParse(text, out int n) checks without throwing.",
    "throw new ArgumentException(\"...\") refuses a bad value.",
  ],
};
