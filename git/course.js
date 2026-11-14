// How the Git course runs code: each script runs with bash in a fresh, empty
// folder that is deleted afterwards, with a fixed name, email and branch
// name, so its output is the same on every machine.

const path = require("node:path");

// Runs the script ($1) in a new temporary folder and passes on its exit code.
const IN_FRESH_FOLDER = 'd=$(mktemp -d) && cd "$d" && bash "$1"; s=$?; cd / && rm -rf "$d"; exit $s';

const SETTINGS = [
  ["user.name", "Ana"],
  ["user.email", "ana@example.com"],
  ["init.defaultBranch", "main"],
  ["advice.detachedHead", "false"],
  ["core.pager", "cat"],
  ["color.ui", "false"],
];

const env = {
  GIT_CONFIG_NOSYSTEM: "1",
  GIT_CONFIG_GLOBAL: process.platform === "win32" ? "NUL" : "/dev/null",
  GIT_CONFIG_COUNT: String(SETTINGS.length),
  LC_ALL: "C",
};
SETTINGS.forEach(([key, value], i) => {
  env[`GIT_CONFIG_KEY_${i}`] = key;
  env[`GIT_CONFIG_VALUE_${i}`] = value;
});

module.exports = {
  id: "git",
  title: "GIT FROM ZERO",
  titlePt: "GIT DO ZERO",
  file: "main.sh",
  starter: "# Each line is a command, run one after another in an empty folder.\n",
  steps: (file) => [["bash", "-c", IN_FRESH_FOLDER, "bash", path.resolve(file)]],
  env,
  // git merge reports a conflict on stdout.
  errorsOnStdout: true,
  howToRun: (name) => `mkdir try && cd try && bash ../${name}`,

  // bash: "main.sh: line N: message"; git: "fatal: ..." or "error: ...".
  parseError(text) {
    const b = text.match(/line (\d+): (.*)/);
    const g = text.split("\n").find((l) => /^(fatal|error):|^CONFLICT/.test(l));
    if (g) {
      return { errorLine: g, line: b ? Number(b[1]) : null };
    }
    if (b) {
      return { errorLine: b[2], line: Number(b[1]) };
    }
    return { errorLine: text.trim().split("\n").pop() || "" };
  },

  errors: [
    [/not a git repository/, "this folder is not a Git repository yet. Run git init first, or cd into the repository.", "esta pasta ainda não é um repositório Git. Corre git init primeiro, ou entra no repositório com cd."],
    [/nothing to commit|nothing added to commit/, "there are no staged changes to save. git add the files first.", "não há alterações preparadas para guardar. Faz git add aos ficheiros primeiro."],
    [/pathspec .* did not match/, "a file or branch name that Git does not know. Check the spelling.", "um nome de ficheiro ou ramo que o Git não conhece. Verifica a ortografia."],
    [/CONFLICT|Merge conflict|fix conflicts/, "both branches changed the same lines, and Git needs you to choose.", "os dois ramos mudaram as mesmas linhas e o Git precisa que escolhas."],
    [/command not found/, "a misspelled command.", "um comando mal escrito."],
  ],
};
