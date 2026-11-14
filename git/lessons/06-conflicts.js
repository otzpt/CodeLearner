module.exports = {
  id: "conflicts",
  title: "Merge conflicts",
  titlePt: "Conflitos ao juntar",
  screens: [
    [
      "If both branches changed the same line, Git cannot know which to keep. The merge stops with a conflict:",
      { code: 'git init -q\necho "colour: red" > config.txt\ngit add config.txt\ngit commit -q -m "Start"\ngit switch -q -c blue\necho "colour: blue" > config.txt\ngit commit -q -am "Blue"\ngit switch -q main\necho "colour: green" > config.txt\ngit commit -q -am "Green"\ngit merge blue', error: true },
    ],
    [
      "Git writes both versions into the file, between markers. The part after <<<<<<< is yours (the current branch), the part after ======= is the other branch's:",
      { code: 'git init -q\necho "colour: red" > config.txt\ngit add config.txt\ngit commit -q -m "Start"\ngit switch -q -c blue\necho "colour: blue" > config.txt\ngit commit -q -am "Blue"\ngit switch -q main\necho "colour: green" > config.txt\ngit commit -q -am "Green"\ngit merge -q blue > /dev/null\ncat config.txt' },
    ],
    [
      "To finish, write the file the way it should end up, without the markers, then add and commit. That commit completes the merge:",
      { code: 'git init -q\necho "colour: red" > config.txt\ngit add config.txt\ngit commit -q -m "Start"\ngit switch -q -c blue\necho "colour: blue" > config.txt\ngit commit -q -am "Blue"\ngit switch -q main\necho "colour: green" > config.txt\ngit commit -q -am "Green"\ngit merge -q blue > /dev/null\necho "colour: teal" > config.txt\ngit add config.txt\ngit commit -q -m "Merge blue, settle on teal"\ngit status --short\ngit log --format="%s" -1' },
      "git merge --abort cancels a merge in progress and puts everything back.",
      { quiz: "What must happen before a conflicted merge can be committed?", choices: ["nothing", "edit the file to its final form and git add it"], answer: 2, why: "You decide the result; git add marks it as resolved." },
    ],
    [
      {
        exercise: "conflicts",
        task: 'Make main and a branch called other both change the single line of size.txt (start "M"; other sets "L"; main sets "S"). Merge other into main, resolve the conflict by writing "XL", commit, and print the file.',
        expected: ["XL"],
        hints: ["Hide the merge's own message: git merge -q other > /dev/null", 'Then echo "XL" > size.txt, git add size.txt, git commit -q -m "Resolve", cat size.txt'],
        solution: 'git init -q\necho "M" > size.txt\ngit add size.txt\ngit commit -q -m "Start"\ngit switch -q -c other\necho "L" > size.txt\ngit commit -q -am "Large"\ngit switch -q main\necho "S" > size.txt\ngit commit -q -am "Small"\ngit merge -q other > /dev/null\necho "XL" > size.txt\ngit add size.txt\ngit commit -q -m "Resolve"\ncat size.txt',
      },
    ],
  ],
  recapPt: [
    "Se os dois ramos mudaram a mesma linha, o merge para com um conflito.",
    "O ficheiro fica com as duas versões entre marcadores <<<<<<< ======= >>>>>>>.",
    "Escreve o resultado final, git add e git commit para acabar.",
    "git merge --abort cancela o merge.",
  ],
  recap: [
    "If both branches changed the same line, the merge stops with a conflict.",
    "The file holds both versions between <<<<<<< ======= >>>>>>> markers.",
    "Write the final result, then git add and git commit to finish.",
    "git merge --abort cancels the merge.",
  ],
};
