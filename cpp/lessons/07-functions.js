module.exports = {
  id: "functions",
  title: "Functions and references",
  titlePt: "Funções e referências",
  screens: [
    [
      "A function is a block of code with a name. Write it above main. The type before the name is what it gives back with return; void means nothing. Parameters each have a type:",
      { code: '#include <iostream>\n\nint area(int width, int height)\n{\n    return width * height;\n}\n\nint main()\n{\n    std::cout << area(3, 4) << "\\n";\n    std::cout << area(10, 2) + 1 << "\\n";\n    return 0;\n}' },
      "Writing the function does not run it. Only area(3, 4) runs it.",
    ],
    [
      "A function gets copies of the values it is given. Changing a parameter does not change the caller's variable:",
      { code: '#include <iostream>\n\nvoid add_one(int n)\n{\n    n = n + 1;\n}\n\nint main()\n{\n    int x = 5;\n    add_one(x);\n    std::cout << x << "\\n";\n    return 0;\n}' },
      { quiz: "After add_one(x) above, what is x?", choices: ["5", "6"], answer: 1, why: "add_one changed its own copy." },
    ],
    [
      "A reference is a second name for an existing variable, written with & after the type. A reference parameter is not a copy: changing it changes the caller's variable:",
      { code: '#include <iostream>\n\nvoid add_one(int& n)\n{\n    n = n + 1;\n}\n\nint main()\n{\n    int x = 5;\n    add_one(x);\n    std::cout << x << "\\n";\n    return 0;\n}' },
      "const std::string& means \"a reference you may only read\": no copy, and no changes. It is the usual way to pass strings and vectors in.",
      { tip: "Plain parameter: a copy. type& : the caller's own variable. const type& : read it without copying." },
    ],
    [
      {
        exercise: "functions",
        task: "Write int largest(const std::vector<int>& v) that returns the biggest value, and void double_all(std::vector<int>& v) that doubles every value. For {4, 9, 2}: print largest, call double_all, then print the values on one line separated by spaces.",
        expected: ["9", "8 18 4"],
        hints: ["largest: start with v[0], loop, keep the bigger one.", "double_all: for (int& n : v) n *= 2; — the & lets the loop change each value."],
        solution: '#include <iostream>\n#include <vector>\n\nint largest(const std::vector<int>& v)\n{\n    int best = v[0];\n    for (int n : v) {\n        if (n > best) {\n            best = n;\n        }\n    }\n    return best;\n}\n\nvoid double_all(std::vector<int>& v)\n{\n    for (int& n : v) {\n        n *= 2;\n    }\n}\n\nint main()\n{\n    std::vector<int> v = {4, 9, 2};\n    std::cout << largest(v) << "\\n";\n    double_all(v);\n    std::cout << v[0] << " " << v[1] << " " << v[2] << "\\n";\n    return 0;\n}',
      },
    ],
  ],
  recapPt: [
    "tipo nome(parâmetros) { ... return ...; } define uma função; void não devolve nada.",
    "Um parâmetro normal é uma cópia.",
    "tipo& é uma referência: mudá-la muda a variável de quem chama.",
    "const tipo& lê sem copiar e sem mudar.",
  ],
  recap: [
    "type name(params) { ... return ...; } defines a function; void returns nothing.",
    "A plain parameter is a copy.",
    "type& is a reference: changing it changes the caller's variable.",
    "const type& reads without copying and without changing.",
  ],
};
