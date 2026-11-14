// How the Arduino course runs code: a sketch is compiled with g++ against
// shim/Arduino.h, a stand-in for the Arduino core that runs on a PC. Time
// is simulated (delay moves the clock on), pins are plain variables, and
// Serial prints to the terminal. loop() runs LOOPS times, then it stops.

const path = require("node:path");

const SHIM = path.join(__dirname, "shim");
const WINDOWS = process.platform === "win32";
const LOOPS = 3;

module.exports = {
  id: "arduino",
  title: "ARDUINO FROM ZERO",
  titlePt: "ARDUINO DO ZERO",
  LOOPS,
  file: "sketch.ino",
  starter: "void setup() {\n  Serial.begin(9600);\n  // your code here\n}\n\nvoid loop() {\n}",
  steps: (file) => [
    ["g++", "-std=gnu++17", "-Wall", "-Wextra", "-I", SHIM, "-include", "Arduino.h",
      "-x", "c++", file, "-x", "none", path.join(SHIM, "main.cpp"), "-o", WINDOWS ? "program.exe" : "program"],
    [WINDOWS ? "program.exe" : "./program"],
  ],
  env: { SIM_LOOPS: String(LOOPS) },
  howToRun: (name) => `g++ -I <CodeLearner>/arduino/shim -include Arduino.h -x c++ ${name} -x none <CodeLearner>/arduino/shim/main.cpp -o program && SIM_LOOPS=${LOOPS} ./program`,

  parseError(text) {
    const m = text.match(/\.ino:(\d+):\d+: (?:fatal )?error: (.*)/);
    if (m) {
      return { errorLine: "error: " + m[2], line: Number(m[1]) };
    }
    return { errorLine: text.split("\n").find((l) => /error/.test(l)) || "" };
  },

  warnings: (text) =>
    [...text.matchAll(/\.ino:(\d+):\d+: warning: (.*)/g)].map((m) => `warning (line ${m[1]}): ${m[2]}`),

  errors: [
    [/was not declared|does not name a type/, "a name the compiler does not know: a typo (capitals count), or something used before it was declared.", "um nome que o compilador não conhece: um erro de escrita (as maiúsculas contam), ou algo usado antes de ser declarado."],
    [/expected ';'/, "a missing semicolon. Every statement ends with ;, usually on the line before the one named.", "falta um ponto e vírgula. Cada instrução acaba em ;, normalmente na linha antes da indicada."],
    [/expected/, "something is typed in a way the compiler cannot read: often a missing ; ) or }.", "algo está escrito de uma forma que o compilador não consegue ler: muitas vezes falta ; ) ou }."],
    [/undefined reference to .(setup|loop)/, "every sketch needs both setup() and loop(), even if one is empty.", "todo o sketch precisa de setup() e loop(), mesmo que um esteja vazio."],
  ],
};
