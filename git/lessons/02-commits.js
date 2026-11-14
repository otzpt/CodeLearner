module.exports = {
  id: "commits",
  title: "Saving snapshots: add and commit",
  titlePt: "Guardar fotografias: add e commit",
  screens: [
    [
      "Saving takes two steps. git add picks which changes go into the next snapshot; this is called staging. git commit saves the staged changes, with a message saying what you did:",
      { code: 'git init -q\necho "milk" > list.txt\ngit add list.txt\ngit status --short\ngit commit -q -m "Start the shopping list"\ngit status --short' },
      "A means added to the stage. After the commit, status shows nothing: every change is saved.",
    ],
    [
      "Change a saved file and status shows it as modified (M). It must be added again before the next commit:",
      { code: 'git init -q\necho "milk" > list.txt\ngit add list.txt\ngit commit -q -m "Start the list"\necho "bread" >> list.txt\ngit status --short\ngit add list.txt\ngit commit -q -m "Add bread"\ngit log --oneline --format="%s"' },
      ">> adds a line to the end of a file, where > would replace it. git log lists the commits, newest first.",
    ],
    [
      "Two steps exist so you can choose what goes together. Change two files, add only one, and only that one is saved:",
      { code: 'git init -q\necho "a" > one.txt\necho "b" > two.txt\ngit add one.txt\ngit commit -q -m "Only one"\ngit status --short' },
      "git add . stages every change in the folder at once.",
      { quiz: "You changed a file but did not run git add. What does git commit save?", choices: ["the change", "nothing new from that file"], answer: 2, why: "Only staged changes go into a commit." },
    ],
    [
      "A commit with nothing staged is refused:",
      { code: 'git init -q\necho "a" > one.txt\ngit commit -m "Nothing staged"', error: true },
      { tip: "git add to stage, git commit -m \"message\" to save. Write messages that say what changed." },
    ],
    [
      {
        exercise: "commit",
        task: 'In a new repository, make poem.txt with the line "roses", commit it with the message "First line". Then add the line "violets" to it and commit with "Second line". Finally show the messages with git log --format="%s".',
        expected: ["Second line", "First line"],
        hints: ['echo "roses" > poem.txt, git add poem.txt, git commit -q -m "First line"', 'echo "violets" >> poem.txt, then add and commit again.'],
        solution: 'git init -q\necho "roses" > poem.txt\ngit add poem.txt\ngit commit -q -m "First line"\necho "violets" >> poem.txt\ngit add poem.txt\ngit commit -q -m "Second line"\ngit log --format="%s"',
      },
    ],
  ],
  recapPt: [
    "git add prepara alterações; git commit -m \"mensagem\" guarda-as.",
    "Só o que foi preparado entra no commit; git add . prepara tudo.",
    "git status --short: A = adicionado, M = modificado.",
    "git log mostra os commits, do mais recente para o mais antigo.",
  ],
  recap: [
    "git add stages changes; git commit -m \"message\" saves them.",
    "Only staged changes go into a commit; git add . stages everything.",
    "git status --short: A = added, M = modified.",
    "git log shows the commits, newest first.",
  ],
};
