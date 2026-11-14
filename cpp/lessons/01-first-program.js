module.exports = {
  id: "first-program",
  title: "Your first program",
  titlePt: "O teu primeiro programa",
  screens: [
    [
      "A program is a list of instructions for the computer, written in a text file. C++ is one language for writing them.",
      "The computer cannot read C++ directly. A compiler first translates your file into a program the computer can run, then you run that. In a terminal, it is two steps:",
      { code: "g++ -std=c++20 -Wall -Wextra -o hello hello.cpp\n./hello", run: false },
      "-Wall -Wextra ask the compiler to warn you about likely mistakes. Every example in this course is compiled and run while you read it.",
    ],
    [
      "Here is a whole C++ program. Every C++ program has this shape:",
      { code: '#include <iostream>\n\nint main()\n{\n    std::cout << "Hello!\\n";\n    return 0;\n}' },
      "#include <iostream> brings in the input and output tools. main is where the program starts. Everything between { and } is its body. return 0 says \"finished fine\".",
    ],
    [
      "std::cout is the screen, and << sends things to it, one after another. Numbers need no quotes, and a sum is worked out first. \\n starts a new line:",
      { code: '#include <iostream>\n\nint main()\n{\n    std::cout << "two plus three is " << 2 + 3 << "\\n";\n    std::cout << "2 + 3" << "\\n";\n    return 0;\n}' },
      "std:: means \"from the standard library\", the tools that come with C++. In quotes, 2 + 3 is just text.",
      { quiz: 'What does std::cout << "10 + 5"; show?', choices: ["15", "10 + 5", "an error"], answer: 2, why: "In quotes, it is text, shown exactly as written." },
    ],
    [
      "Every statement ends with a semicolon. Without it, the compiler refuses and the program never runs:",
      { code: '#include <iostream>\n\nint main()\n{\n    std::cout << "hi\\n"\n    return 0;\n}', error: true },
      "The compiler names the line where it noticed, often the line after the real mistake. // starts a comment that the compiler ignores.",
      { tip: "Every statement ends with ;. Text in double quotes. \\n for a new line." },
    ],
    [
      {
        exercise: "hello",
        task: "Write a program that prints your name on one line and the number 2026 on the next. For the checker, use the name Ana.",
        expected: ["Ana", "2026"],
        hints: ["The file already has #include and main. Write inside main.", 'std::cout << "Ana\\n";'],
        solution: '#include <iostream>\n\nint main()\n{\n    std::cout << "Ana\\n";\n    std::cout << 2026 << "\\n";\n    return 0;\n}',
      },
    ],
  ],
  recapPt: [
    "Um compilador traduz o ficheiro .cpp num programa; depois corres esse programa.",
    "Todo o programa em C++ tem #include <iostream> e int main() { ... return 0; }.",
    "std::cout << ... mostra valores; \\n muda de linha.",
    "Cada instrução acaba em ;.",
  ],
  recap: [
    "A compiler turns your .cpp file into a program; then you run that program.",
    "Every C++ program has #include <iostream> and int main() { ... return 0; }.",
    "std::cout << ... shows values; \\n starts a new line.",
    "Every statement ends with ;.",
  ],
};
