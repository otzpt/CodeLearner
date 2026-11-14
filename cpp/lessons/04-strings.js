module.exports = {
  id: "strings",
  title: "Text: std::string",
  titlePt: "Texto: std::string",
  screens: [
    [
      "std::string holds text. + joins strings, == compares their text, and .size() counts the characters:",
      { code: '#include <iostream>\n#include <string>\n\nint main()\n{\n    std::string first = "Ana";\n    std::string full = first + " Silva";\n    std::cout << full << "\\n";\n    std::cout << full.size() << "\\n";\n    std::cout << (first == "Ana") << "\\n";\n    return 0;\n}' },
      "A comparison prints 1 for true and 0 for false.",
    ],
    [
      "Each character has a position, starting at 0. [ ] reads one, and .substr(start, length) takes a piece:",
      { code: '#include <iostream>\n#include <string>\n\nint main()\n{\n    std::string word = "Hello";\n    std::cout << word[0] << "\\n";\n    std::cout << word[word.size() - 1] << "\\n";\n    std::cout << word.substr(1, 3) << "\\n";\n    return 0;\n}' },
      "So a 5-letter word has positions 0 to 4, and the last one is size() - 1.",
      { quiz: 'In std::string s = "cat"; what is s[1]?', choices: ["c", "a", "t"], answer: 2, why: "c is 0, a is 1, t is 2." },
    ],
    [
      "Numbers and strings convert both ways: std::to_string(number) makes text, std::stoi(text) makes an int:",
      { code: '#include <iostream>\n#include <string>\n\nint main()\n{\n    std::string label = "score " + std::to_string(42);\n    int n = std::stoi("17") + 1;\n    std::cout << label << "\\n" << n << "\\n";\n    return 0;\n}' },
      { tip: "Positions start at 0; the last is size() - 1. to_string and stoi convert." },
    ],
    [
      {
        exercise: "greeting",
        task: 'Create std::string name = "Bruno". Print: Hello, Bruno! Your name has 5 letters. Then print its first letter and its last letter on one line, separated by a space.',
        expected: ["Hello, Bruno! Your name has 5 letters.", "B o"],
        hints: ['"Hello, " + name + ...  or several << pieces.', 'name[0] and name[name.size() - 1], with " " between them.'],
        solution: '#include <iostream>\n#include <string>\n\nint main()\n{\n    std::string name = "Bruno";\n    std::cout << "Hello, " << name << "! Your name has " << name.size() << " letters.\\n";\n    std::cout << name[0] << " " << name[name.size() - 1] << "\\n";\n    return 0;\n}',
      },
    ],
  ],
  recapPt: [
    "std::string guarda texto; + junta, == compara o texto, .size() conta.",
    "As posições começam em 0; .substr(início, tamanho) tira um pedaço.",
    "std::to_string(n) e std::stoi(s) convertem entre números e texto.",
  ],
  recap: [
    "std::string holds text; + joins, == compares the text, .size() counts.",
    "Positions start at 0; .substr(start, length) takes a piece.",
    "std::to_string(n) and std::stoi(s) convert between numbers and text.",
  ],
};
