module.exports = {
  id: "input",
  title: "Reading what the user types",
  titlePt: "Ler o que o utilizador escreve",
  screens: [
    [
      "std::cin is the keyboard, and >> takes a value from it into a variable. It reads what fits the variable's type:",
      { code: '#include <iostream>\n\nint main()\n{\n    int age;\n    std::cout << "Your age? ";\n    std::cin >> age;\n    std::cout << "Next year you will be " << age + 1 << "\\n";\n    return 0;\n}', output: ["Your age? {20}", "Next year you will be 21"] },
      "The bold part is what was typed.",
    ],
    [
      "If the user types something that does not fit, the read fails and the variable is set to 0. The read itself can be tested like a condition:",
      { code: '#include <iostream>\n\nint main()\n{\n    int n;\n    std::cout << "Number: ";\n    if (std::cin >> n) {\n        std::cout << "got " << n << "\\n";\n    } else {\n        std::cout << "that was not a number\\n";\n    }\n    return 0;\n}', output: ["Number: {hello}", "that was not a number"] },
    ],
    [
      ">> stops at the first space, so it reads one word. std::getline reads a whole line into a std::string (from <string>), spaces included:",
      { code: '#include <iostream>\n#include <string>\n\nint main()\n{\n    std::string name;\n    std::cout << "Full name? ";\n    std::getline(std::cin, name);\n    std::cout << "Hello, " << name << "!\\n";\n    return 0;\n}', output: ["Full name? {Ana Silva}", "Hello, Ana Silva!"] },
      { quiz: 'The user types "Ana Silva" and the program does std::cin >> name. What is in name?', choices: ["Ana Silva", "Ana", "nothing"], answer: 2, why: ">> stops at the first space. getline reads the whole line." },
    ],
    [
      {
        exercise: "ask",
        task: 'Ask "Width? " and "Height? " (whole numbers) and print: Area: W*H',
        expected: ["Width? {4}", "Height? {5}", "Area: 20"],
        hints: ["Two int variables, each with a cout prompt and a cin >> read.", 'std::cout << "Area: " << width * height << "\\n";'],
        solution: '#include <iostream>\n\nint main()\n{\n    int width, height;\n    std::cout << "Width? ";\n    std::cin >> width;\n    std::cout << "Height? ";\n    std::cin >> height;\n    std::cout << "Area: " << width * height << "\\n";\n    return 0;\n}',
      },
    ],
  ],
  recapPt: [
    "std::cin >> x; lê um valor do tipo de x.",
    "Uma leitura falhada pode ser testada: if (std::cin >> n).",
    ">> lê uma palavra; std::getline(std::cin, s) lê a linha toda.",
  ],
  recap: [
    "std::cin >> x; reads a value of x's type.",
    "A failed read can be tested: if (std::cin >> n).",
    ">> reads one word; std::getline(std::cin, s) reads the whole line.",
  ],
};
