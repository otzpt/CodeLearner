module.exports = {
  id: "exceptions",
  title: "When things go wrong: exceptions",
  titlePt: "Quando as coisas correm mal: exceções",
  screens: [
    [
      "When something goes wrong while running, Java throws an exception. If nothing catches it, the program stops and prints the type, a message, and the line:",
      { code: 'public class Main {\n    public static void main(String[] args) {\n        int total = 10;\n        int people = 0;\n        System.out.println(total / people);\n    }\n}', error: true },
      "Read the first line for what happened and the \"at Main.main(...)\" line for where.",
    ],
    [
      "try runs code; if it throws, catch receives the exception and runs instead, and the program carries on. getMessage() gives the description:",
      { code: 'public class Main {\n    public static void main(String[] args) {\n        try {\n            System.out.println("before");\n            int n = Integer.parseInt("hello");\n            System.out.println("never printed");\n        } catch (NumberFormatException e) {\n            System.out.println("Problem: " + e.getMessage());\n        }\n        System.out.println("the program keeps going");\n    }\n}' },
      "As soon as something throws inside try, Java jumps straight to catch, skipping the rest of try.",
      { quiz: "An exception happens on the second of four lines in a try. Do lines 3 and 4 run?", choices: ["yes", "no"], answer: 2, why: "It jumps straight to catch." },
    ],
    [
      "Your own methods can throw, to refuse something that makes no sense. IllegalArgumentException is the usual one for a bad value:",
      { code: 'public class Main {\n    static int withdraw(int balance, int amount) {\n        if (amount > balance) {\n            throw new IllegalArgumentException("not enough money");\n        }\n        return balance - amount;\n    }\n\n    public static void main(String[] args) {\n        try {\n            System.out.println(withdraw(100, 30));\n            System.out.println(withdraw(100, 500));\n        } catch (IllegalArgumentException e) {\n            System.out.println("Refused: " + e.getMessage());\n        }\n    }\n}' },
      { tip: "throw new ...Exception(\"message\") to refuse; try/catch to handle." },
    ],
    [
      {
        exercise: "exceptions",
        task: 'Write static double divide(double a, double b) that throws new IllegalArgumentException("cannot divide by 0") when b is 0, and returns a / b otherwise. Inside one try, print divide(10, 4) and then divide(1, 0). In the catch, print "Error: " and the message. Then print "done".',
        expected: ["2.5", "Error: cannot divide by 0", "done"],
        hints: ['if (b == 0) throw new IllegalArgumentException("cannot divide by 0");', 'catch (IllegalArgumentException e) { System.out.println("Error: " + e.getMessage()); }'],
        solution: 'public class Main {\n    static double divide(double a, double b) {\n        if (b == 0) {\n            throw new IllegalArgumentException("cannot divide by 0");\n        }\n        return a / b;\n    }\n\n    public static void main(String[] args) {\n        try {\n            System.out.println(divide(10, 4));\n            System.out.println(divide(1, 0));\n        } catch (IllegalArgumentException e) {\n            System.out.println("Error: " + e.getMessage());\n        }\n        System.out.println("done");\n    }\n}',
      },
    ],
  ],
  recapPt: [
    "Uma exceção não apanhada para o programa e mostra o tipo, a mensagem e a linha.",
    "try { } catch (TipoException e) { } apanha-a e o programa continua.",
    "e.getMessage() dá a mensagem.",
    "throw new IllegalArgumentException(\"...\") recusa um valor errado.",
  ],
  recap: [
    "An uncaught exception stops the program and shows the type, message and line.",
    "try { } catch (SomeException e) { } catches it and the program continues.",
    "e.getMessage() gives the message.",
    "throw new IllegalArgumentException(\"...\") refuses a bad value.",
  ],
};
