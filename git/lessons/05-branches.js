module.exports = {
  id: "branches",
  title: "Branches: working on two things",
  titlePt: "Ramos: trabalhar em duas coisas",
  screens: [
    [
      "A branch is a separate line of commits. You can try something on a branch without touching the main line. git switch -c name makes a branch and moves to it; git branch lists them, with * on the current one:",
      { code: 'git init -q\necho "v1" > app.txt\ngit add app.txt\ngit commit -q -m "Start"\ngit switch -q -c feature\ngit branch' },
    ],
    [
      "Commits made on a branch stay on it. Switching back to main brings back main's version of the files:",
      { code: 'git init -q\necho "v1" > app.txt\ngit add app.txt\ngit commit -q -m "Start"\ngit switch -q -c feature\necho "v2" > app.txt\ngit commit -q -am "Try v2"\ngit switch -q main\ncat app.txt\ngit switch -q feature\ncat app.txt' },
      "The folder always shows the branch you are on.",
    ],
    [
      "git merge brings another branch's commits into the current one. When main has not moved since the branch started, Git just moves main forward:",
      { code: 'git init -q\necho "v1" > app.txt\ngit add app.txt\ngit commit -q -m "Start"\ngit switch -q -c feature\necho "v2" > app.txt\ngit commit -q -am "Finish v2"\ngit switch -q main\ngit merge -q feature\ncat app.txt\ngit log --format="%s"' },
      { quiz: "You are on main and run git merge feature. Which branch changes?", choices: ["feature", "main"], answer: 2, why: "merge brings the other branch into the one you are on." },
    ],
    [
      {
        exercise: "branches",
        task: 'Commit readme.txt with "draft" on main. Make a branch called fix, change the file to "final" and commit. Go back to main and print the file, then merge fix and print the file again.',
        expected: ["draft", "final"],
        hints: ["git switch -q -c fix, edit, git commit -q -am \"...\"", "git switch -q main, cat, git merge -q fix, cat."],
        solution: 'git init -q\necho "draft" > readme.txt\ngit add readme.txt\ngit commit -q -m "Draft"\ngit switch -q -c fix\necho "final" > readme.txt\ngit commit -q -am "Final"\ngit switch -q main\ncat readme.txt\ngit merge -q fix\ncat readme.txt',
      },
    ],
  ],
  recapPt: [
    "Um ramo é uma linha de commits separada.",
    "git switch -c nome cria um ramo e muda para ele; git switch nome muda.",
    "A pasta mostra sempre os ficheiros do ramo atual.",
    "git merge outro traz os commits do outro ramo para o atual.",
  ],
  recap: [
    "A branch is a separate line of commits.",
    "git switch -c name makes a branch and moves to it; git switch name moves.",
    "The folder always shows the current branch's files.",
    "git merge other brings the other branch's commits into the current one.",
  ],
};
