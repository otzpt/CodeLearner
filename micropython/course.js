// How the MicroPython course runs code: in ordinary Python, with sim.py
// standing in for the board's machine module and a simulated clock in place
// of time (sim.install()). No board and no micropython binary needed. The
// parts of the language used here behave the same in MicroPython.

const path = require("node:path");
const python = require("../python/course.js");

const PYTHON = process.platform === "win32" ? "python" : "python3";
const SIM_DIR = __dirname;

// Install the stand-in, then run the student's file as the main program,
// keeping its own file name so tracebacks point at the right lines.
const RUN = [
  "import sys",
  `sys.path.insert(0, ${JSON.stringify(SIM_DIR)})`,
  "import sim",
  "sim.install()",
  "path = sys.argv[1]",
  "sys.argv = sys.argv[1:]",
  "code = compile(open(path, encoding='utf-8').read(), path, 'exec')",
  "exec(code, {'__name__': '__main__'})",
].join("\n");

module.exports = {
  ...python,
  id: "micropython",
  title: "MICROPYTHON FROM ZERO",
  titlePt: "MICROPYTHON DO ZERO",
  file: "main.py",
  steps: (file) => [[PYTHON, "-c", RUN, path.resolve(file)]],
  howToRun: (name) => `on a board: copy ${name} to it as main.py (for example with Thonny); on this PC, the course runs it for you`,
  errors: [
    [/ValueError: invalid pin|invalid/, "that pin or value is not valid for this board.", "esse pino ou valor não é válido para esta placa."],
    ...python.errors,
  ],
};
