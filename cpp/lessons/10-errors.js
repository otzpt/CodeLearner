module.exports = {
  id: "errors",
  title: "When things go wrong: exceptions",
  titlePt: "Quando as coisas correm mal: exceções",
  screens: [
    [
      "When something goes wrong that the code at that spot cannot fix, it can throw an exception. If nothing catches it, the program stops and says what was thrown:",
      { code: '#include <iostream>\n#include <stdexcept>\n\nint main()\n{\n    throw std::runtime_error("something broke");\n    return 0;\n}', error: true },
    ],
    [
      "try runs code; if it throws, catch receives the exception and runs instead, and the program carries on. .what() gives the message:",
      { code: '#include <iostream>\n#include <stdexcept>\n#include <vector>\n\nint main()\n{\n    std::vector<int> v = {1, 2, 3};\n    try {\n        std::cout << "before\\n";\n        std::cout << v.at(10) << "\\n";\n        std::cout << "never printed\\n";\n    } catch (const std::out_of_range& e) {\n        std::cout << "Problem: index out of range\\n";\n    }\n    std::cout << "the program keeps going\\n";\n    return 0;\n}' },
      "As soon as something throws inside try, C++ jumps straight to catch, skipping the rest of try.",
      { quiz: "An exception is thrown on the second of four lines in a try. Do lines 3 and 4 run?", choices: ["yes", "no"], answer: 2, why: "It jumps straight to catch." },
    ],
    [
      "Your own functions can throw, to refuse something that makes no sense. std::invalid_argument and std::runtime_error, from <stdexcept>, take a message:",
      { code: '#include <iostream>\n#include <stdexcept>\n\nint withdraw(int balance, int amount)\n{\n    if (amount > balance) {\n        throw std::invalid_argument("not enough money");\n    }\n    return balance - amount;\n}\n\nint main()\n{\n    try {\n        std::cout << withdraw(100, 30) << "\\n";\n        std::cout << withdraw(100, 500) << "\\n";\n    } catch (const std::exception& e) {\n        std::cout << "Refused: " << e.what() << "\\n";\n    }\n    return 0;\n}' },
      "catch (const std::exception& e) catches every standard exception.",
      { tip: "Compile errors first, warnings next. throw to refuse; try/catch to handle." },
    ],
    [
      {
        exercise: "errors",
        task: 'Write double divide(double a, double b) that throws std::invalid_argument("cannot divide by 0") when b is 0, and returns a / b otherwise. Inside one try, print divide(10, 4) and then divide(1, 0). In catch, print "Error: " and the message. Then print "done".',
        expected: ["2.5", "Error: cannot divide by 0", "done"],
        hints: ['if (b == 0) throw std::invalid_argument("cannot divide by 0");', 'catch (const std::exception& e) { std::cout << "Error: " << e.what() << "\\n"; }'],
        solution: '#include <iostream>\n#include <stdexcept>\n\ndouble divide(double a, double b)\n{\n    if (b == 0) {\n        throw std::invalid_argument("cannot divide by 0");\n    }\n    return a / b;\n}\n\nint main()\n{\n    try {\n        std::cout << divide(10, 4) << "\\n";\n        std::cout << divide(1, 0) << "\\n";\n    } catch (const std::exception& e) {\n        std::cout << "Error: " << e.what() << "\\n";\n    }\n    std::cout << "done\\n";\n    return 0;\n}',
      },
    ],
  ],
  recapPt: [
    "throw lança uma exceção; se ninguém a apanha, o programa para.",
    "try { } catch (const std::exception& e) { } apanha-a e deixa o programa continuar.",
    "e.what() dá a mensagem.",
    ".at() lança std::out_of_range numa posição inválida.",
  ],
  recap: [
    "throw raises an exception; if nothing catches it, the program stops.",
    "try { } catch (const std::exception& e) { } catches it and the program continues.",
    "e.what() gives the message.",
    ".at() throws std::out_of_range for a bad position.",
  ],
};
