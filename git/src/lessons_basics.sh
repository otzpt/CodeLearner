# lessons_basics.sh - modules 1 through 5: what git is, cloning, and the
# working-directory / staging / repository cycle.
#
# Every command shown runs for real against a throwaway sandbox created by
# this module and destroyed before it returns -- see ui.sh's ui_run and
# ui_sandbox_* and docs/writing-a-course.md rule 2. Nothing here is typed
# from memory.

lesson_01_what_is_git() {
    ui_title "MODULE 1 - WHAT IS GIT?"

    ui_heading "PART 1: version control"
    cat <<'EOF'
  Git tracks changes to files over time. Every saved snapshot is a commit;
  the history of commits is the point, since you can see what changed, when,
  and get back to any of them.

  This course assumes git is already installed. Here is the version this
  machine actually has:
EOF
    ui_run "git --version" git --version

    ui_wait_enter
    ui_clear_screen
    ui_heading "PART 2: three areas, not one 'save'"
    cat <<'EOF'
  A change passes through three places before it is permanent history:

    working directory  ->  staging area (the "index")  ->  repository

  Editing a file changes the working directory only. `git add` copies a
  snapshot of it into the staging area. `git commit` is what actually
  writes that snapshot into the repository's history. Three separate
  steps, three separate commands, and modules 4 and 5 are `add` and `commit`
  for exactly that reason, not one combined "save" module.
EOF

    local repo
    repo=$(ui_sandbox_new)
    (
        cd "$repo" || exit 1
        ui_run "git init -b main" git init --quiet -b main
        ui_run "git status" git status
    )
    ui_sandbox_cleanup "$repo"

    cat <<'EOF'

  "No commits yet", since init only creates the empty .git/ folder. It does
  not start tracking anything by itself.

  This course always passes -b main to git init. Left to itself, `git
  init` names the first branch after your own init.defaultBranch config,
  which is "master" on some machines and "main" on others, and pinning it
  is what keeps every transcript in this course identical on any machine.
EOF

    ui_wait_enter
    ui_clear_screen
    ui_exercise 1

    ui_question \
        "Which of the three areas does 'git add' move a change into? (working / staging / repository)" \
        "staging" \
        "add copies a snapshot into the index. The repository does not get it until commit."

    local task=(
        "In an empty folder:"
        "  git init -b main"
        "  git status"
    )
    local expected=(
        "On branch main"
        ""
        "No commits yet"
        ""
        'nothing to commit (create/copy files and use "git add" to track)'
    )
    local solution=(
        '$ git init -b main'
        "Initialized empty Git repository in .../.git/"
        '$ git status'
        "On branch main"
        "No commits yet"
        'nothing to commit (create/copy files and use "git add" to track)'
    )
    ui_challenge task expected solution

    ui_wait_enter
    ui_clear_screen
    ui_heading "SUMMARY"
    cat <<'EOF'
  - A change moves working directory -> staging -> repository.
  - `git init` creates .git/. It does not track anything by itself.
  - `git status` tells you which of the three areas everything is in.
  - This course pins -b main everywhere, since the real default depends
    on your git version and config, not on git's own behavior.
EOF
    ui_wait_enter
}

lesson_02_clone() {
    ui_title "MODULE 2 - GIT CLONE"

    ui_heading "PART 1: joining a project that already exists"
    cat <<'EOF'
  Most of the time you do not start a project with `git init`. Instead you
  join one that already has history, on a server somewhere (GitHub, GitLab, a
  coworker's machine). `git clone` copies that whole history to your own
  disk: every commit, every branch, not just the latest snapshot.

  This demo's "server" is a plain local folder standing in for a GitHub
  remote, since clone does not care whether the source is a URL or a
  path, the mechanics are identical either way.
EOF

    local sandbox remote
    ui_new_remote sandbox remote
    (
        cd "$sandbox" || exit 1
        ui_run "git clone $remote clone" git clone "$remote" clone
    )

    ui_wait_enter
    ui_clear_screen
    ui_heading "PART 2: what you actually got"
    cat <<'EOF'
  Clone also sets up a "remote" for you automatically, named origin, and
  that name is where "origin/main" and "git push origin main" come from
  in every later module.
EOF
    (
        cd "$sandbox/clone" || exit 1
        ui_run "git remote -v" git remote -v
        ui_run "git log --oneline" git log --oneline
    )
    ui_sandbox_cleanup "$sandbox"

    cat <<'EOF'

  Trap: cloning into a folder that already exists and already has content
  fails with "destination path ... already exists and is not an empty
  directory", since clone always creates a fresh folder and never merges
  into one.
EOF

    ui_wait_enter
    ui_clear_screen
    ui_exercise 2

    ui_question \
        "What is the default name git gives the remote a clone came from?" \
        "origin" \
        "It is only a convention, not special syntax, you could rename it, but almost nobody does."

    local task=(
        "Clone any public repository you have a URL for, for example:"
        "  git clone https://github.com/otzpt/CodeLearner.git"
        "  cd CodeLearner"
        "  git remote -v"
    )
    local expected=(
        "origin  https://github.com/otzpt/CodeLearner.git (fetch)"
        "origin  https://github.com/otzpt/CodeLearner.git (push)"
    )
    local solution=(
        '$ git clone https://github.com/otzpt/CodeLearner.git'
        "Cloning into 'CodeLearner'..."
        '$ cd CodeLearner && git remote -v'
        "origin  https://github.com/otzpt/CodeLearner.git (fetch)"
        "origin  https://github.com/otzpt/CodeLearner.git (push)"
    )
    ui_challenge task expected solution

    ui_wait_enter
    ui_clear_screen
    ui_heading "SUMMARY"
    cat <<'EOF'
  - clone copies the whole history, not just the latest files.
  - It always creates a new folder; it never merges into an existing one.
  - The remote it came from is named origin, by convention, automatically.
  - `git remote -v` shows what origin actually points at.
EOF
    ui_wait_enter
}

lesson_03_status() {
    ui_title "MODULE 3 - GIT STATUS"

    ui_heading "PART 1: what status actually answers"
    cat <<'EOF'
  `git status` answers one question: right now, what is different between
  the working directory, the staging area, and the last commit? It is the
  command you run before every add, every commit, and every push, not
  because it changes anything, but because it changes nothing and tells
  you the truth.
EOF

    local sandbox remote clone
    ui_new_remote_and_clone sandbox remote clone
    (
        cd "$clone" || exit 1
        ui_run "git status" git status
    )

    ui_wait_enter
    ui_clear_screen
    ui_heading "PART 2: status after each kind of change"
    (
        cd "$clone" || exit 1
        printf 'print("hello")\n' >hello.py
        printf '\n  A brand-new file, not yet added:\n'
        ui_run "git status" git status

        git add hello.py
        printf '\n  The same file, after "git add":\n'
        ui_run "git status" git status

        printf 'print("hello world")\n' >>hello.py
        printf '\n  Edited again after adding, staged and unstaged at once:\n'
        ui_run "git status" git status
    )
    ui_sandbox_cleanup "$sandbox"

    cat <<'EOF'

  That last one is the trap: the file appears under BOTH "Changes to be
  committed" and "Changes not staged for commit" at the same time. `git
  add` staged a snapshot of the file as it was at that moment, and editing
  it again afterwards does not update what is already staged. Committing
  now would save the first version, not the one currently on screen.
EOF

    ui_wait_enter
    ui_clear_screen
    ui_exercise 3

    ui_question \
        "A file shows under both staged and unstaged changes at once. Which version does 'git commit' save?" \
        "the staged version" \
        "add staged a snapshot at the moment you ran it, and editing the file again afterwards does not update what is already staged."

    local task=(
        "In any git repository with at least one commit:"
        "  echo x > new.txt"
        "  git status"
        "  git add new.txt"
        "  git status"
    )
    local expected=(
        "First status: new.txt listed under 'Untracked files'."
        "Second status: new.txt listed under 'Changes to be committed'."
    )
    local solution=(
        '$ echo x > new.txt && git status'
        "Untracked files:"
        "	new.txt"
        '$ git add new.txt && git status'
        "Changes to be committed:"
        "	new file:   new.txt"
    )
    ui_challenge task expected solution

    ui_wait_enter
    ui_clear_screen
    ui_heading "SUMMARY"
    cat <<'EOF'
  - status never changes anything, it only reports.
  - An untracked file, a staged change, and an unstaged change are three
    different sections of the same output. Read the section, not just
    the filename.
  - Staging is a snapshot, taken once, at the moment you run `git add`.
EOF
    ui_wait_enter
}

lesson_04_add() {
    ui_title "MODULE 4 - GIT ADD"

    ui_heading "PART 1: staging is a snapshot, not a bookmark"
    cat <<'EOF'
  `git add <file>` copies the file's current content into the staging
  area. It is not "mark this file as one I'm working on", it is "the
  exact bytes in this file right now are what the next commit will save",
  which module 3 already showed can drift out of sync with what is on
  screen.
EOF

    local sandbox remote clone
    ui_new_remote_and_clone sandbox remote clone
    (
        cd "$clone" || exit 1
        printf 'one\n' >a.txt
        printf 'two\n' >b.txt
        ui_run "git status" git status

        printf '\n  Adding everything at once:\n'
        ui_run "git add ." git add .
        ui_run "git status" git status
    )
    ui_sandbox_cleanup "$sandbox"

    ui_wait_enter
    ui_clear_screen
    ui_heading "PART 2: unstaging"
    cat <<'EOF'
  git add is not permanent and does not touch the working directory.
  Taking a file back out of the staging area, before it is committed,
  leaves the file itself exactly as it was.
EOF
    ui_new_remote_and_clone sandbox remote clone
    (
        cd "$clone" || exit 1
        printf 'oops\n' >a.txt
        git add a.txt >/dev/null
        ui_run "git restore --staged a.txt" git restore --staged a.txt
        ui_run "git status" git status
    )
    ui_sandbox_cleanup "$sandbox"

    cat <<'EOF'

  Trap: "git add ." stages every change under the current directory,
  including files you did not mean to, like a stray build artifact, a local
  config file. `git status` before every commit is how that gets caught;
  a .gitignore (not covered by this course's ten modules) is how it stops
  showing up as "untracked" in the first place.
EOF

    ui_wait_enter
    ui_clear_screen
    ui_exercise 4

    ui_question \
        "You ran 'git add' on a file by mistake, before committing. Does 'git restore --staged' change the file's content?" \
        "no" \
        "Unstaging only removes it from the index; the working directory copy is untouched either way."

    local task=(
        "In any git repository:"
        "  echo x > a.txt && echo y > b.txt"
        "  git add ."
        "  git status"
    )
    local expected=(
        "Changes to be committed:"
        "	new file:   a.txt"
        "	new file:   b.txt"
    )
    local solution=(
        '$ echo x > a.txt && echo y > b.txt'
        '$ git add . && git status'
        "Changes to be committed:"
        "	new file:   a.txt"
        "	new file:   b.txt"
    )
    ui_challenge task expected solution

    ui_wait_enter
    ui_clear_screen
    ui_heading "SUMMARY"
    cat <<'EOF'
  - `git add <path>` stages one file or folder; `git add .` stages
    everything under the current directory.
  - Staging copies content at that instant. Editing after adding does
    not update the staged snapshot.
  - `git restore --staged <file>` unstages without touching the file.
EOF
    ui_wait_enter
}

lesson_05_commit() {
    ui_title "MODULE 5 - GIT COMMIT"

    ui_heading "PART 1: what a commit actually is"
    cat <<'EOF'
  A commit is a permanent, named snapshot of everything currently staged,
  plus a message saying why. Unlike staging, a commit is not silently
  overwritten by editing a file afterwards. It exists in history from
  this point on.
EOF

    local sandbox remote clone
    ui_new_remote_and_clone sandbox remote clone
    (
        cd "$clone" || exit 1
        printf 'print("hi")\n' >hello.py
        git add hello.py >/dev/null
        ui_run "git commit -m 'Add hello.py'" \
            git -c user.name=Student -c user.email=student@example.com \
            -c commit.gpgsign=false commit -m "Add hello.py"
        ui_run "git log --oneline" git log --oneline
    )

    ui_wait_enter
    ui_clear_screen
    ui_heading "PART 2: committing with nothing staged"
    (
        cd "$clone" || exit 1
        ui_run "git commit -m 'nothing changed'" \
            git -c user.name=Student -c user.email=student@example.com \
            commit -m "nothing changed"
    )
    ui_sandbox_cleanup "$sandbox"

    cat <<'EOF'

  Trap: "nothing to commit, working tree clean" is not an error. It is
  git correctly refusing to create an empty, meaningless snapshot. If a
  change genuinely is not showing up here, the fix is `git add` (module
  4), not forcing the commit.

  Also worth knowing: `git commit -a` skips staging for files git already
  tracks, but it still will not pick up a brand-new file. New files
  always need an explicit `git add` at least once.
EOF

    ui_wait_enter
    ui_clear_screen
    ui_exercise 5

    ui_question \
        "You run 'git commit' with nothing staged. Does git create an empty commit by default?" \
        "no" \
        "That message means git refuses to record a snapshot with no actual change in it."

    local task=(
        "In any git repository:"
        "  echo x > file.txt"
        "  git add file.txt"
        "  git commit -m \"Add file.txt\""
        "  git log --oneline -1"
    )
    local expected=(
        "One line, ending in: Add file.txt"
    )
    local solution=(
        '$ git add file.txt'
        '$ git commit -m "Add file.txt"'
        "[main abc1234] Add file.txt"
        '$ git log --oneline -1'
        "abc1234 Add file.txt"
    )
    ui_challenge task expected solution

    ui_wait_enter
    ui_clear_screen
    ui_heading "SUMMARY"
    cat <<'EOF'
  - A commit permanently saves whatever is currently staged.
  - git refuses to commit when nothing is staged. That is not a bug.
  - `-a` auto-stages changes to already-tracked files only, never new ones.
  - `git log --oneline` is the fast way to see the history you're building.
EOF
    ui_wait_enter
}
