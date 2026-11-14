module.exports = {
  id: "undo",
  title: "Undoing changes",
  titlePt: "Desfazer alterações",
  screens: [
    [
      "git restore file throws away the unsaved changes to a file, bringing back the last committed version. Careful: those changes are gone for good:",
      { code: 'git init -q\necho "good" > notes.txt\ngit add notes.txt\ngit commit -q -m "Good notes"\necho "oops" > notes.txt\ngit restore notes.txt\ncat notes.txt' },
    ],
    [
      "git restore --staged file takes a change off the stage, but keeps it in the file. Use it when you added something by mistake:",
      { code: 'git init -q\necho "a" > keep.txt\ngit add keep.txt\ngit commit -q -m "Start"\necho "secret" > password.txt\ngit add password.txt\ngit restore --staged password.txt\ngit status --short' },
      "password.txt is back to untracked (??), and still on disk.",
      { quiz: "You staged a file by mistake but want to keep your edits. Which command?", choices: ["git restore file", "git restore --staged file"], answer: 2, why: "--staged only unstages; plain restore throws the edits away." },
    ],
    [
      "git revert makes a new commit that undoes an old one. The history keeps both, so nothing is lost and it is safe on shared projects:",
      { code: 'git init -q\necho "v1" > app.txt\ngit add app.txt\ngit commit -q -m "Version 1"\necho "v2 broken" > app.txt\ngit commit -q -am "Version 2"\ngit revert --no-edit HEAD > /dev/null\ncat app.txt\ngit log --format="%s"' },
      "> /dev/null hides a command's own messages.",
      { tip: "restore drops unsaved edits; restore --staged unstages; revert undoes a commit with a new one." },
    ],
    [
      {
        exercise: "undo",
        task: 'Commit a file plan.txt containing "plan A". Change it to "plan B" and commit with "Switch to B". Then undo that commit with git revert and print the file.',
        expected: ["plan A"],
        hints: ["git revert --no-edit HEAD undoes the newest commit.", "Hide revert's own message with > /dev/null, then cat plan.txt."],
        solution: 'git init -q\necho "plan A" > plan.txt\ngit add plan.txt\ngit commit -q -m "Plan A"\necho "plan B" > plan.txt\ngit commit -q -am "Switch to B"\ngit revert --no-edit HEAD > /dev/null\ncat plan.txt',
      },
    ],
  ],
  recapPt: [
    "git restore ficheiro deita fora as alterações não guardadas (para sempre).",
    "git restore --staged ficheiro tira do palco mas mantém o ficheiro.",
    "git revert cria um commit novo que desfaz um antigo; o histórico fica.",
  ],
  recap: [
    "git restore file throws away unsaved changes (for good).",
    "git restore --staged file unstages but keeps the file.",
    "git revert makes a new commit that undoes an old one; history stays.",
  ],
};
