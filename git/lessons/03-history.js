module.exports = {
  id: "history",
  title: "Looking at history",
  titlePt: "Ver o histórico",
  screens: [
    [
      "git diff shows what changed and is not staged yet. Lines starting with + were added, lines with - were removed:",
      { code: 'git init -q\nprintf "milk\\nbread\\n" > list.txt\ngit add list.txt\ngit commit -q -m "List"\nprintf "milk\\neggs\\n" > list.txt\ngit diff' },
      "printf writes several lines at once; \\n is a new line.",
    ],
    [
      "Every commit has an id, a long code of letters and digits, called its hash. git log --oneline shows a short form of it with each message. The ids are different every time, so this course prints only the messages:",
      { code: 'git init -q\nfor word in one two three; do\n  echo "$word" > file.txt\n  git add file.txt\n  git commit -q -m "Write $word"\ndone\ngit log --format="%s"' },
      "The for line repeats the indented commands for each word.",
    ],
    [
      "git show HEAD shows the newest commit and what it changed. HEAD means \"where you are now\", and HEAD~1 the commit before it:",
      { code: 'git init -q\necho "v1" > app.txt\ngit add app.txt\ngit commit -q -m "First"\necho "v2" > app.txt\ngit commit -q -am "Second"\ngit show HEAD~1:app.txt\ngit show HEAD:app.txt' },
      "commit -am adds every already-tracked file and commits, in one step. HEAD~1:app.txt is the file as it was one commit ago.",
      { quiz: "In git diff output, what does a line starting with + mean?", choices: ["a line removed", "a line added"], answer: 2, why: "+ is added, - is removed." },
    ],
    [
      {
        exercise: "history",
        task: 'Commit a file version.txt containing 1 with the message "One", then change it to 2 and commit with "Two". Print the file as it was in the first commit, then the number of commits with git rev-list --count HEAD.',
        expected: ["1", "2"],
        hints: ["git show HEAD~1:version.txt prints the old version.", "git rev-list --count HEAD prints how many commits there are."],
        solution: 'git init -q\necho "1" > version.txt\ngit add version.txt\ngit commit -q -m "One"\necho "2" > version.txt\ngit commit -q -am "Two"\ngit show HEAD~1:version.txt\ngit rev-list --count HEAD',
      },
    ],
  ],
  recapPt: [
    "git diff mostra o que mudou e ainda não foi preparado (+ juntado, - tirado).",
    "Cada commit tem um id (hash); git log --oneline mostra-os em forma curta.",
    "HEAD é onde estás agora; HEAD~1 é o commit anterior.",
    "git show HEAD~1:ficheiro mostra o ficheiro como estava.",
  ],
  recap: [
    "git diff shows unstaged changes (+ added, - removed).",
    "Every commit has an id (hash); git log --oneline shows them short.",
    "HEAD is where you are now; HEAD~1 is the commit before.",
    "git show HEAD~1:file shows the file as it was.",
  ],
};
