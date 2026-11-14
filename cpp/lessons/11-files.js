module.exports = {
  id: "files",
  title: "Reading and writing files",
  titlePt: "Ler e escrever ficheiros",
  screens: [
    [
      "<fstream> gives files that work like std::cout and std::cin. std::ofstream writes a file, making it or emptying it first. It closes itself when it goes out of scope, at the closing }:",
      { code: '#include <fstream>\n#include <iostream>\n\nint main()\n{\n    {\n        std::ofstream out("notes.txt");\n        out << "first line\\n";\n        out << "score " << 42 << "\\n";\n    }\n    std::cout << "saved\\n";\n    return 0;\n}' },
    ],
    [
      "std::ifstream reads a file. std::getline in a while loop reads one line at a time, and stops at the end of the file:",
      { code: '#include <fstream>\n#include <iostream>\n#include <string>\n\nint main()\n{\n    {\n        std::ofstream out("list.txt");\n        out << "milk\\nbread\\n";\n    }\n    std::ifstream in("list.txt");\n    std::string line;\n    while (std::getline(in, line)) {\n        std::cout << "- " << line << "\\n";\n    }\n    return 0;\n}' },
      "getline does not keep the \\n, so the cout adds one.",
    ],
    [
      "A file that cannot be opened does not stop the program by itself: the stream just reads nothing. Test it before using it:",
      { code: '#include <fstream>\n#include <iostream>\n\nint main()\n{\n    std::ifstream in("missing.txt");\n    if (!in) {\n        std::cout << "missing.txt could not be opened\\n";\n        return 1;\n    }\n    return 0;\n}', error: true },
      { quiz: "What does std::ofstream out(\"data.txt\"); do to a file that already has text?", choices: ["adds to the end", "empties it first"], answer: 2, why: "Opening for writing starts the file empty; std::ios::app would add to the end." },
    ],
    [
      {
        exercise: "files",
        task: 'Write the lines "red", "green" and "blue" to colours.txt. Then read the file back and print each line with its number: 1 red, 2 green, 3 blue.',
        expected: ["1 red", "2 green", "3 blue"],
        hints: ["Write inside { } with std::ofstream so it closes before you read.", "Read with getline in a while loop and count."],
        solution: '#include <fstream>\n#include <iostream>\n#include <string>\n\nint main()\n{\n    {\n        std::ofstream out("colours.txt");\n        out << "red\\ngreen\\nblue\\n";\n    }\n    std::ifstream in("colours.txt");\n    std::string line;\n    int number = 1;\n    while (std::getline(in, line)) {\n        std::cout << number << " " << line << "\\n";\n        number++;\n    }\n    return 0;\n}',
      },
    ],
  ],
  recapPt: [
    "std::ofstream out(nome); escreve com <<; fecha-se sozinho no } .",
    "std::ifstream in(nome); com while (std::getline(in, line)) lê linha a linha.",
    "Testa if (!in) antes de usar um ficheiro.",
  ],
  recap: [
    "std::ofstream out(name); writes with <<; it closes itself at the } .",
    "std::ifstream in(name); with while (std::getline(in, line)) reads line by line.",
    "Test if (!in) before using a file.",
  ],
};
