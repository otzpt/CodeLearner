module.exports = {
  id: "vectors",
  title: "Lists of things: std::vector",
  titlePt: "Listas de coisas: std::vector",
  screens: [
    [
      "A std::vector (from <vector>) holds many values of one type, in order, and grows as you add to it. The type goes in < >. Positions start at 0, and .size() counts the values:",
      { code: '#include <iostream>\n#include <vector>\n\nint main()\n{\n    std::vector<int> scores = {15, 12, 18};\n    scores.push_back(20);\n    std::cout << scores[0] << " " << scores[3] << "\\n";\n    std::cout << scores.size() << "\\n";\n    return 0;\n}' },
      "push_back adds a value to the end.",
    ],
    [
      "A range for goes through every value, without counting positions yourself:",
      { code: '#include <iostream>\n#include <string>\n#include <vector>\n\nint main()\n{\n    std::vector<std::string> fruits = {"apple", "banana"};\n    for (const std::string& fruit : fruits) {\n        std::cout << "I like " << fruit << "\\n";\n    }\n    return 0;\n}' },
      "Read it as \"for each fruit in fruits\". The const & part means \"look at each one without copying it\"; lesson 7 explains it.",
    ],
    [
      "[ ] does not check the position. Going past the end reads memory that is not part of the vector, and the result is not defined. .at() checks, and stops the program with a clear error instead:",
      { code: '#include <iostream>\n#include <vector>\n\nint main()\n{\n    std::vector<int> scores = {15, 12, 18};\n    std::cout << scores.at(3) << "\\n";\n    return 0;\n}', error: true },
      { quiz: "A vector has 3 values. What is the last valid position?", choices: ["3", "2", "4"], answer: 2, why: "Positions go from 0 to size() - 1." },
    ],
    [
      "std::sort, from <algorithm>, puts a vector in order:",
      { code: '#include <algorithm>\n#include <iostream>\n#include <vector>\n\nint main()\n{\n    std::vector<int> v = {10, 9, 2};\n    std::sort(v.begin(), v.end());\n    for (int n : v) {\n        std::cout << n << " ";\n    }\n    std::cout << "\\n";\n    return 0;\n}' },
      "v.begin() and v.end() mean \"from the first value to the last\".",
      { tip: "push_back adds; .size() counts; .at(i) checks the position, [i] does not." },
    ],
    [
      {
        exercise: "vectors",
        task: 'Make a std::vector<std::string> with "rice", "beans", "oil". Add "salt" to the end. Print how many items there are, then every item on its own line starting with "- ".',
        expected: ["4", "- rice", "- beans", "- oil", "- salt"],
        hints: ["push_back adds salt. .size() gives the count.", 'for (const std::string& item : items) std::cout << "- " << item << "\\n";'],
        solution: '#include <iostream>\n#include <string>\n#include <vector>\n\nint main()\n{\n    std::vector<std::string> items = {"rice", "beans", "oil"};\n    items.push_back("salt");\n    std::cout << items.size() << "\\n";\n    for (const std::string& item : items) {\n        std::cout << "- " << item << "\\n";\n    }\n    return 0;\n}',
      },
    ],
  ],
  recapPt: [
    "std::vector<int> v = {1, 2, 3}; cresce com push_back; .size() conta.",
    "As posições vão de 0 a size() - 1.",
    "for (int n : v) percorre todos os valores.",
    ".at(i) verifica a posição e lança um erro; [i] não verifica.",
    "std::sort(v.begin(), v.end()) ordena.",
  ],
  recap: [
    "std::vector<int> v = {1, 2, 3}; grows with push_back; .size() counts.",
    "Positions go from 0 to size() - 1.",
    "for (int n : v) visits every value.",
    ".at(i) checks the position and throws an error; [i] does not.",
    "std::sort(v.begin(), v.end()) sorts.",
  ],
};
