# lessons_more.sh - modules 6 through 10: syncing with a remote,
# branches, releases, and GitHub Actions.
#
# Same rule as lessons_basics.sh: every transcript is produced by actually
# running git against a throwaway sandbox, not typed from memory.

lesson_06_push() {
    ui_title "MODULE 6 - GIT PUSH"

    ui_heading "PART 1: sending commits to the remote"
    cat <<'EOF'
  A commit (module 5) only exists on your own disk until you push it.
  `git push <remote> <branch>` sends every commit the remote does not
  already have.
EOF

    local sandbox remote clone
    ui_new_remote_and_clone sandbox remote clone
    (
        cd "$clone" || exit 1
        printf 'print("hi")\n' >hello.py
        git add hello.py
        git -c user.name=Student -c user.email=student@example.com \
            -c commit.gpgsign=false commit --quiet -m "Add hello.py"
        ui_run "git push origin main" git push origin main
    )
    ui_sandbox_cleanup "$sandbox"

    ui_wait_enter
    ui_clear_screen
    ui_heading "PART 2: rejected, someone else pushed first"
    cat <<'EOF'
  Two people cloned the same project. One of them pushes. The other,
  still one commit behind, tries to push their own work.
EOF
    local alice bob
    ui_new_remote sandbox remote
    alice="$sandbox/alice"
    bob="$sandbox/bob"
    git clone --quiet "$remote" "$alice" >/dev/null 2>&1
    git clone --quiet "$remote" "$bob" >/dev/null 2>&1
    (
        cd "$alice" || exit 1
        printf 'alice was here\n' >>README.md
        git -c user.name=Alice -c user.email=alice@example.com \
            -c commit.gpgsign=false commit --quiet -am "Alice's change"
        git push --quiet origin main
    )
    (
        cd "$bob" || exit 1
        printf 'bob was here\n' >>README.md
        git -c user.name=Bob -c user.email=bob@example.com \
            -c commit.gpgsign=false commit --quiet -am "Bob's change"
        ui_run "git push origin main" git push origin main
    )
    ui_sandbox_cleanup "$sandbox"

    cat <<'EOF'

  That "[rejected] ... (fetch first)" means git will not silently overwrite
  commits it has never seen. The fix is module 7's `git pull`, not
  `git push --force`: force-push replaces the remote's history with
  yours, discarding Alice's commit entirely. On a shared branch, that
  means erasing a teammate's work.
EOF

    ui_wait_enter
    ui_clear_screen
    ui_exercise 6

    ui_question \
        "Your push is rejected because someone else pushed first. Should you re-run it with --force? (yes/no)" \
        "no" \
        "force replaces the remote's history with yours, discarding their commit entirely. Module 7 covers the safe way to reconcile first."

    local task=(
        "In a clone of any repository you can push to:"
        "  echo x >> README.md"
        "  git commit -am \"A change\""
        "  git push origin main"
    )
    local expected=(
        "A line like: main -> main"
    )
    local solution=(
        '$ git commit -am "A change"'
        '$ git push origin main'
        "To https://github.com/you/repo.git"
        "   1234abc..5678def  main -> main"
    )
    ui_challenge task expected solution

    ui_wait_enter
    ui_clear_screen
    ui_heading "SUMMARY"
    cat <<'EOF'
  - Commits are local until pushed.
  - push is rejected, not merged, when the remote has commits you don't.
  - The safe response to a rejected push is pull, not --force.
  - --force replaces remote history with yours. It can erase real work.
EOF
    ui_wait_enter
}

lesson_07_pull() {
    ui_title "MODULE 7 - GIT PULL"

    ui_heading "PART 1: catching up, the easy case"
    cat <<'EOF'
  `git pull` is fetch (download what the remote has) plus merge (combine
  it with your own work) in one command. When nobody touched the same
  lines you did, that merge is trivial.
EOF
    local sandbox remote a b
    ui_new_remote sandbox remote
    a="$sandbox/a"
    b="$sandbox/b"
    git clone --quiet "$remote" "$a" >/dev/null 2>&1
    git clone --quiet "$remote" "$b" >/dev/null 2>&1
    (
        cd "$a" || exit 1
        printf 'a change\n' >>README.md
        git -c user.name=A -c user.email=a@example.com \
            -c commit.gpgsign=false commit --quiet -am "A's change"
        git push --quiet origin main
    )
    (
        cd "$b" || exit 1
        ui_run "git pull origin main" git pull origin main
        ui_run "cat README.md" cat README.md
    )
    ui_sandbox_cleanup "$sandbox"

    ui_wait_enter
    ui_clear_screen
    ui_heading "PART 2: the trap, both sides changed the same line"
    cat <<'EOF'
  Same setup, except this time both clones edit line 2 of the same file
  before either one pulls.
EOF
    ui_new_remote sandbox remote
    a="$sandbox/a"
    b="$sandbox/b"
    git clone --quiet "$remote" "$a" >/dev/null 2>&1
    git clone --quiet "$remote" "$b" >/dev/null 2>&1
    (
        cd "$a" || exit 1
        printf 'alice was here\n' >>README.md
        git -c user.name=Alice -c user.email=alice@example.com \
            -c commit.gpgsign=false commit --quiet -am "Alice's change"
        git push --quiet origin main >/dev/null
    )
    (
        cd "$b" || exit 1
        printf 'bob was here\n' >>README.md
        git -c user.name=Bob -c user.email=bob@example.com \
            -c commit.gpgsign=false commit --quiet -am "Bob's change"
        ui_run "git pull --no-rebase origin main" \
            git -c user.name=Bob -c user.email=bob@example.com pull --no-rebase origin main
        ui_run "git status" git status
        ui_run "cat README.md" cat README.md

        printf '\n  Resolving by hand, pick a final version and remove the markers:\n'
        printf '# Demo Project\nboth changes kept\n' >README.md
        ui_run "git add README.md" git add README.md
        ui_run "git commit --no-edit" \
            git -c user.name=Bob -c user.email=bob@example.com \
            -c commit.gpgsign=false commit --no-edit
    )
    ui_sandbox_cleanup "$sandbox"

    cat <<'EOF'

  "<<<<<<<", "=======", ">>>>>>>" are conflict markers git leaves in the
  file itself, not an error message, an edit for you to finish. Delete
  the markers, leave the content you actually want, then `git add` the
  file (marking it resolved, same as staging anything else) and commit.
EOF

    ui_wait_enter
    ui_clear_screen
    ui_exercise 7

    ui_question \
        "After a pull leaves conflict markers in a file, what command tells git the conflict is resolved?" \
        "git add" \
        "add marks the file resolved the same way it stages any other change, commit afterwards finishes the merge."

    local task=(
        "In a clone with a merge conflict already open:"
        "  (edit the file: keep what you want, delete <<<<<<< ======= >>>>>>>)"
        "  git add <file>"
        "  git status"
    )
    local expected=(
        "All conflicts fixed but you are still merging."
    )
    local solution=(
        '$ git add README.md'
        '$ git status'
        "All conflicts fixed but you are still merging."
        "  (use \"git commit\" to conclude merge)"
    )
    ui_challenge task expected solution

    ui_wait_enter
    ui_clear_screen
    ui_heading "SUMMARY"
    cat <<'EOF'
  - pull = fetch + merge in one command.
  - No overlap -> the merge is automatic and silent.
  - Overlapping lines -> git leaves <<<<<<< / ======= / >>>>>>> markers
    in the file and stops for you to decide.
  - Resolve by editing the file, then add, then commit: the same two
    commands as any other change.
EOF
    ui_wait_enter
}

lesson_08_branches() {
    ui_title "MODULE 8 - BRANCHES"

    ui_heading "PART 1: working on a copy of history"
    cat <<'EOF'
  A branch is a movable pointer to a commit, nothing heavier. Creating
  one is instant because it copies nothing, it just gives a new name to
  "start from here".
EOF
    local repo
    ui_new_repo repo
    (
        cd "$repo" || exit 1
        ui_run "git branch" git branch
        ui_run "git switch -c feature-x" git switch -c feature-x
        printf 'second line\n' >>notes.txt
        git -c user.name=Student -c user.email=student@example.com \
            -c commit.gpgsign=false commit --quiet -am "Add second line, on feature-x"
        ui_run "git switch main" git switch main
        ui_run "cat notes.txt" cat notes.txt
        printf '\n  main does not yet have the commit from feature-x, merge brings it in:\n'
        ui_run "git merge feature-x" git merge feature-x
        ui_run "cat notes.txt" cat notes.txt
    )
    ui_sandbox_cleanup "$repo"

    ui_wait_enter
    ui_clear_screen
    ui_heading "PART 2: the trap, both branches changed the same line"
    cat <<'EOF'
  That merge was a "fast-forward": main had not moved, so git just slid
  its pointer forward. When both branches gained real, conflicting
  commits since they split, merge cannot just slide a pointer, same
  situation module 7 hit through pull, reached this time through
  `git merge` directly.
EOF
    ui_new_repo repo
    (
        cd "$repo" || exit 1
        git switch -c feature-a >/dev/null 2>&1
        printf 'change from feature-a\n' >>notes.txt
        git -c user.name=Student -c user.email=student@example.com \
            -c commit.gpgsign=false commit --quiet -am "Change from feature-a"
        git switch main >/dev/null 2>&1
        printf 'change from main\n' >>notes.txt
        git -c user.name=Student -c user.email=student@example.com \
            -c commit.gpgsign=false commit --quiet -am "Change from main"

        ui_run "git merge feature-a" git merge feature-a
        ui_run "cat notes.txt" cat notes.txt

        printf 'first line\nmerged: kept both\n' >notes.txt
        ui_run "git add notes.txt" git add notes.txt
        ui_run "git commit --no-edit" \
            git -c user.name=Student -c user.email=student@example.com \
            -c commit.gpgsign=false commit --no-edit
        ui_run "git log --oneline --graph --all" git log --oneline --graph --all
    )
    ui_sandbox_cleanup "$repo"

    ui_wait_enter
    ui_clear_screen
    ui_exercise 8

    ui_question \
        "main has not moved since a branch split off it. Does merging that branch back need a merge commit?" \
        "no" \
        "with no new commits on main to reconcile, git just moves main's pointer forward, a fast-forward, no merge commit created."

    local task=(
        "In any git repository with a commit:"
        "  git switch -c my-branch"
        "  echo x >> notes.txt"
        "  git commit -am \"A change\""
        "  git switch main"
        "  git merge my-branch"
    )
    local expected=(
        "Updating <old>..<new>"
        "Fast-forward"
    )
    local solution=(
        '$ git switch -c my-branch'
        '$ echo x >> notes.txt && git commit -am "A change"'
        '$ git switch main'
        '$ git merge my-branch'
        "Updating 1234abc..5678def"
        "Fast-forward"
    )
    ui_challenge task expected solution

    ui_wait_enter
    ui_clear_screen
    ui_heading "SUMMARY"
    cat <<'EOF'
  - A branch is a movable pointer to a commit, creating one is instant.
  - `git switch -c <name>` creates and moves to a branch in one step.
  - No new commits on the target -> fast-forward, no merge commit.
  - Both sides changed -> a real merge, and possibly the same conflict
    markers and add/commit resolution module 7 already covered.
EOF
    ui_wait_enter
}

lesson_09_releases() {
    ui_title "MODULE 9 - RELEASES"

    ui_heading "PART 1: tags, naming one commit permanently"
    cat <<'EOF'
  A branch name moves as you commit. A tag does not, it names one exact
  commit, forever, which is what makes it the right tool for "this is
  what shipped as v1.0.0" and the wrong one for ongoing work.
EOF
    local repo
    ui_new_repo repo
    (
        cd "$repo" || exit 1
        ui_run "git tag v0.1-notes" git tag v0.1-notes
        ui_run "git tag -a v1.0.0 -m 'First release'" \
            git -c user.name=Student -c user.email=student@example.com \
            -c commit.gpgsign=false tag -a v1.0.0 -m "First release"
        ui_run "git tag -n" git tag -n

        printf '\n  The difference is not cosmetic, ask git what kind of object each is:\n'
        ui_run "git cat-file -t v0.1-notes" git cat-file -t v0.1-notes
        ui_run "git cat-file -t v1.0.0" git cat-file -t v1.0.0
    )
    ui_sandbox_cleanup "$repo"

    cat <<'EOF'

  A plain ("lightweight") tag is just a pointer straight at the commit.
  cat-file says so itself: "commit". An annotated tag (-a, with -m) is
  its own real object, with a message and a tagger recorded permanently,
  which is why every release should use -a: the message is otherwise
  gone from history the moment you forget why you tagged it.
EOF

    ui_wait_enter
    ui_clear_screen
    ui_heading "PART 2: pushing a tag, and what a GitHub Release adds on top"
    cat <<'EOF'
  Tags are local until pushed, exactly like commits, but push does not
  send them along by default, so pushing a tag is its own explicit
  command.
EOF
    local sandbox remote clone
    ui_new_remote_and_clone sandbox remote clone
    (
        cd "$clone" || exit 1
        git -c user.name=Student -c user.email=student@example.com \
            -c commit.gpgsign=false tag -a v1.0.0 -m "First release" >/dev/null
        ui_run "git push origin v1.0.0" git push origin v1.0.0
    )
    ui_sandbox_cleanup "$sandbox"

    cat <<'EOF'

  On GitHub (or GitLab, or any forge), a "Release" is a tag plus a page:
  the notes you write, and any files you attach, installers, binaries,
  a changelog. The tag is the only part git itself knows about; the notes
  and attachments are the forge's own layer on top, per GitHub's own
  documentation, this course has no network access to a real GitHub
  release to run against, so that part is stated, not demonstrated.
EOF
    if command -v gh &>/dev/null; then
        cat <<'EOF'

  This machine has the gh CLI installed, so here is a real one, from this
  very project's own public repository:
EOF
        ui_run "gh release list --repo otzpt/CodeLearner --limit 3" \
            gh release list --repo otzpt/CodeLearner --limit 3
    fi

    ui_wait_enter
    ui_clear_screen
    ui_exercise 9

    ui_question \
        "You want a release tag to permanently remember why it was cut. Lightweight or annotated tag?" \
        "annotated" \
        "only an annotated tag (-a, with -m) is its own object carrying a message and tagger, a lightweight tag is just a pointer."

    local task=(
        "In any git repository with a commit:"
        "  git tag -a v0.0.1 -m \"First tag\""
        "  git tag -n"
    )
    local expected=(
        "v0.0.1          First tag"
    )
    local solution=(
        '$ git tag -a v0.0.1 -m "First tag"'
        '$ git tag -n'
        "v0.0.1          First tag"
    )
    ui_challenge task expected solution

    ui_wait_enter
    ui_clear_screen
    ui_heading "SUMMARY"
    cat <<'EOF'
  - A tag names one exact commit, permanently; a branch keeps moving.
  - Lightweight = a bare pointer. Annotated (-a -m) = a real object with
    a message and tagger, use annotated for releases.
  - Tags are local until pushed, same as commits, and need their own
    `git push origin <tag>`.
  - A GitHub Release is a tag plus notes and attachments layered on top
    by GitHub, not something git itself stores.
EOF
    ui_wait_enter
}

lesson_10_actions() {
    ui_title "MODULE 10 - GITHUB ACTIONS"

    ui_heading "PART 1: a workflow is a file in your repo"
    cat <<'EOF'
  GitHub Actions runs on GitHub's own servers, not on this machine, this
  module is where the honesty rule in this course's own docs matters: it
  states GitHub's documented behavior and shows a real, correctly
  structured workflow file, but it cannot execute one for you the way
  every earlier module actually ran its own git commands.

  What it CAN show for real: a workflow file is nothing special to git.
  It is just a .yml file at .github/workflows/, added and committed like
  any other file.
EOF
    local repo
    ui_new_repo repo
    (
        cd "$repo" || exit 1
        mkdir -p .github/workflows
        cat >.github/workflows/tests.yml <<'YAML'
name: Tests

on:
  push:
    branches: [main]
  pull_request:

jobs:
  test:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      - name: Run the test suite
        run: ./run-tests.sh
YAML
        ui_run "git add .github/workflows/tests.yml" \
            git add .github/workflows/tests.yml
        ui_run "git commit -m 'Add CI workflow'" \
            git -c user.name=Student -c user.email=student@example.com \
            -c commit.gpgsign=false commit -m "Add CI workflow"
        ui_run "cat .github/workflows/tests.yml" cat .github/workflows/tests.yml
    )
    ui_sandbox_cleanup "$repo"

    ui_wait_enter
    ui_clear_screen
    ui_heading "PART 2: reading it, and what fires it"
    cat <<'EOF'
  Four things matter in that file:

    on:        what triggers a run, here, a push to main, or any pull
               request. A release workflow commonly triggers on a tag
               push instead: `on: push: tags: ["v*"]`, tying it straight
               to module 9's `git push origin v1.0.0`.
    jobs:      one or more named units of work; each runs on a fresh
               virtual machine (runs-on).
    steps:     run in order, top to bottom, inside that job.
    uses:      runs someone else's pre-built step (actions/checkout@v4
               fetches your repo's code onto that fresh machine, without
               it, the runner starts with nothing).
    run:       runs a real shell command, same as typing it locally.

  Push a commit matching `on:`, and GitHub schedules that job automatically
 , no separate trigger command, no polling. Every run and its full log
  appears under the repository's own Actions tab.
EOF

    ui_wait_enter
    ui_clear_screen
    ui_exercise 10

    ui_question \
        "A workflow's 'on:' section lists 'push: tags: [\"v*\"]'. What makes it run?" \
        "pushing a tag matching v*" \
        "on: is the trigger list, this one ignores ordinary commits and fires only when a matching tag is pushed, e.g. v1.0.0 from module 9."

    local task=(
        "In any GitHub repository you can push to, add"
        ".github/workflows/hello.yml with:"
        "  name: Hello"
        "  on: [push]"
        "  jobs:"
        "    say-hello:"
        "      runs-on: ubuntu-latest"
        "      steps:"
        "        - run: echo \"Hello from Actions\""
        "Commit and push it."
    )
    local expected=(
        "A new run named 'Hello' appears under the repository's Actions tab,"
        "with 'Hello from Actions' in its log."
    )
    local solution=(
        '$ mkdir -p .github/workflows'
        '$ git add .github/workflows/hello.yml'
        '$ git commit -m "Add hello workflow"'
        '$ git push origin main'
    )
    ui_challenge task expected solution

    ui_wait_enter
    ui_clear_screen
    ui_heading "SUMMARY"
    cat <<'EOF'
  - A workflow is just a .yml file under .github/workflows/, committed
    and pushed like any other file, nothing git-specific about it.
  - on: decides what triggers it: a push, a pull request, a tag matching
    a pattern.
  - jobs: -> steps:, top to bottom, on a fresh machine each run.
  - It runs on GitHub's servers the moment a matching push lands, there
    is no separate command to fire it.
EOF
    ui_wait_enter
}
