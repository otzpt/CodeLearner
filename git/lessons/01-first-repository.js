module.exports = {
  id: "first-repository",
  title: "Your first repository",
  titlePt: "O teu primeiro repositório",
  screens: [
    [
      "Git keeps a history of your files. Every time you are happy with a change, you save a snapshot of the whole project, called a commit. Later you can see what changed, when, and go back.",
      "You give Git commands in a terminal. In this course, each example is a small script: commands that run one after another, in a brand new empty folder. echo prints text, and echo text > file writes it into a file:",
      { code: 'echo "hello" > notes.txt\ncat notes.txt\nls' },
      "cat shows what is inside a file. ls lists the files in the folder.",
    ],
    [
      "git init turns the current folder into a repository: a folder whose history Git keeps. git status says what Git sees:",
      { code: 'git init -q\necho "hello" > notes.txt\ngit status' },
      "-q means quiet. notes.txt is untracked: it is in the folder, but Git is not keeping its history yet.",
    ],
    [
      "git status --short gives one line per file. ?? means untracked:",
      { code: 'git init -q\necho "hello" > notes.txt\necho "bye" > other.txt\ngit status --short' },
      { quiz: "What does git init do?", choices: ["saves a snapshot", "makes the current folder a repository", "downloads Git"], answer: 2, why: "It starts a repository in the folder; nothing is saved yet." },
    ],
    [
      "Git commands only work inside a repository. Run one in a plain folder and Git says so:",
      { code: "git status", error: true },
      { tip: "git init once per project; git status any time to see what Git sees." },
    ],
    [
      {
        exercise: "init",
        task: "Write a script that makes a repository, creates a file todo.txt containing the word milk, and shows git status --short.",
        expected: ["?? todo.txt"],
        hints: ["git init -q", 'echo "milk" > todo.txt, then git status --short'],
        solution: 'git init -q\necho "milk" > todo.txt\ngit status --short',
      },
    ],
  ],
  recapPt: [
    "O Git guarda o histórico dos ficheiros em fotografias chamadas commits.",
    "git init transforma a pasta atual num repositório.",
    "git status mostra o que o Git vê; --short dá uma linha por ficheiro (?? = não seguido).",
    "Fora de um repositório, os comandos do Git dão erro.",
  ],
  recap: [
    "Git keeps your files' history as snapshots called commits.",
    "git init makes the current folder a repository.",
    "git status shows what Git sees; --short gives one line per file (?? = untracked).",
    "Outside a repository, Git commands fail.",
  ],
};
