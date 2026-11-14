// How the C++ course runs code: compile with the system C++ compiler, with
// warnings on, then run the program.

const WINDOWS = process.platform === "win32";
const CXX = process.env.CXX || "g++";
const PROGRAM = WINDOWS ? "program.exe" : "./program";

module.exports = {
  id: "cpp",
  title: "C++ FROM ZERO",
  titlePt: "C++ DO ZERO",
  file: "main.cpp",
  starter: "#include <iostream>\n\nint main()\n{\n    // your code here\n    return 0;\n}",
  steps: (file, dir, opts = {}) => [
    [CXX, "-std=c++20", "-Wall", "-Wextra", ...(opts.flags || []), "-o", WINDOWS ? "program.exe" : "program", file],
    [PROGRAM],
  ],
  howToRun: (name) => `${CXX} -std=c++20 -Wall -Wextra -o program ${name} && ${PROGRAM}`,

  // "file.cpp:LINE:COL: error: message"; an uncaught exception prints
  // "terminate called after throwing an instance of 'T'" and "what():  ...".
  parseError(text) {
    const m = text.match(/:(\d+):\d+: (?:fatal )?error: (.*)/);
    if (m) {
      return { errorLine: "error: " + m[2], line: Number(m[1]) };
    }
    const thrown = text.match(/instance of '([^']+)'/);
    const what = text.match(/what\(\):\s*(.*)/);
    if (thrown) {
      return { errorLine: `uncaught ${thrown[1]}${what ? ": " + what[1] : ""}` };
    }
    const link = text.split("\n").find((l) => /undefined reference|ld returned/.test(l));
    return { errorLine: link ? link.replace(/^.*?: /, "") : text.trim().split("\n")[0] || "" };
  },

  warnings: (text) =>
    [...text.matchAll(/:(\d+):\d+: warning: (.*)/g)].map((m) => `warning (line ${m[1]}): ${m[2]}`),

  errors: [
    [/was not declared|does not name a type|has no member/, "a name the compiler does not know: a typo, something used before it was declared, a missing #include, or a missing std::.", "um nome que o compilador não conhece: um erro de escrita, algo usado antes de ser declarado, um #include em falta ou falta std::."],
    [/expected ';'/, "a missing semicolon. Every statement ends with ;, usually on the line before the one named.", "falta um ponto e vírgula. Cada instrução acaba em ;, normalmente na linha antes da indicada."],
    [/expected/, "something is typed in a way C++ cannot read: often a missing ; ) or }.", "algo está escrito de uma forma que o C++ não consegue ler: muitas vezes falta ; ) ou }."],
    [/out_of_range/, "a position outside the container: .at() checked it and refused.", "uma posição fora do contentor: o .at() verificou e recusou."],
    [/uncaught/, "an exception was thrown and nothing caught it, so the program stopped.", "foi lançada uma exceção e nada a apanhou, por isso o programa parou."],
    [/no match for|cannot convert|invalid conversion/, "a value of one type where another is needed.", "um valor de um tipo onde é preciso outro."],
  ],
};
