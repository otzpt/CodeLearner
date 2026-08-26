# ui.sh - the pieces every lesson uses to draw the screen.
#
# Same purpose and the same visual style as the other courses' ui modules,
# kept as its own implementation rather than shared code -- see
# docs/writing-a-course.md for why. Functions are prefixed ui_ because Bash
# has one flat function namespace with no module system to separate them.

UI_WIDTH=54

ui_clear_screen() {
    printf '\033[H\033[2J\033[3J'
}

ui_wait_enter() {
    printf '\n  Press ENTER to continue...'
    read -r _
}

ui_rule() {
    printf '  '
    printf -- '-%.0s' $(seq 1 "$UI_WIDTH")
    printf '\n'
}

_ui_frame() {
    local fill=$1
    printf '  +'
    printf -- "${fill}%.0s" $(seq 1 $((UI_WIDTH - 2)))
    printf '+\n'
}

_ui_padded_line() {
    printf '  %s %-*s%s\n' "$2" $((UI_WIDTH - 3)) "$1" "$2"
}

ui_title() {
    printf '\n'
    _ui_frame '='
    _ui_padded_line "$1" '|'
    _ui_frame '='
    printf '\n'
}

# Same frame as ui_title, but the text is centered instead of left-aligned
# -- for the one-off main menu banner, not module titles (which stay
# left-aligned, matching every other course's ui module).
ui_title_centered() {
    local text=$1 field_width pad
    field_width=$((UI_WIDTH - 3))
    pad=$(( (field_width - ${#text}) / 2 ))
    ((pad < 0)) && pad=0

    printf '\n'
    _ui_frame '='
    _ui_padded_line "$(printf '%*s%s' "$pad" '' "$text")" '|'
    _ui_frame '='
    printf '\n'
}

ui_heading() {
    printf '\n  %s\n' "$1"
    ui_rule
}

ui_ask_yes() {
    local answer
    printf '\n  %s (y/N): ' "$1"
    read -r answer
    [[ "${answer,,}" == y* ]]
}

ui_exercise() {
    printf '\n  >> EXERCISE - MODULE %s\n' "$1"
    ui_rule
}

# question(text, correct, why). Compares ignoring case and surrounding
# spaces, same as every other course's `question`. Never blocks: the point
# is to show the right answer, not to fail anyone.
ui_question() {
    local text=$1 correct=$2 why=$3 answer norm_answer norm_correct
    printf '\n  %s\n  Your answer: ' "$text"
    read -r answer
    norm_answer=$(echo "$answer" | tr '[:upper:]' '[:lower:]' | xargs)
    norm_correct=$(echo "$correct" | tr '[:upper:]' '[:lower:]' | xargs)
    if [[ "$norm_answer" == "$norm_correct" ]]; then
        printf '\n  CORRECT.  %s\n' "$why"
    else
        printf '\n  NOT QUITE. The answer is: %s\n' "$correct"
        printf '             %s\n' "$why"
    fi
}

# challenge(task_array_name, expected_array_name, solution_array_name)
#
# The git-course equivalent of every other course's "write this yourself,
# in a real file": here it's "run these yourself, in a real directory".
# `expected` is the output that sequence must produce -- the whole
# specification, same rule as every other course: a different sequence of
# commands landing on the same result is correct.
ui_challenge() {
    local -n _task=$1 _expected=$2 _solution=$3
    local line

    printf '\n  >> TRY THIS YOURSELF, in a real directory\n'
    ui_rule
    for line in "${_task[@]}"; do
        printf '  %s\n' "$line"
    done

    if [[ ${#_expected[@]} -gt 0 ]]; then
        printf '\n  You should see:\n\n'
        for line in "${_expected[@]}"; do
            printf '      %s\n' "$line"
        done
        printf '\n  That output is the whole specification. Any commands that\n'
        printf '  produce it are correct.\n'
    fi

    if ! ui_ask_yes 'Want to see example commands?'; then
        return
    fi

    printf '\n'
    ui_rule
    for line in "${_solution[@]}"; do
        printf '  %s\n' "$line"
    done
    ui_rule
    printf '  This is EXAMPLE, not the answer. It is one way to reach that\n'
    printf '  result; a different sequence of commands landing on the same\n'
    printf '  state is correct too.\n'
}

# A fresh, empty scratch directory. Every module builds and tears down its
# own -- modules do not share state, since the menu lets you jump to any of
# them in any order.
ui_sandbox_new() {
    mktemp -d "${TMPDIR:-/tmp}/codelearner-git.XXXXXX"
}

ui_sandbox_cleanup() {
    rm -rf -- "$1"
}

# Prints a command the way a shell prompt would show it, then actually
# runs it and lets the real output follow. Every transcript in this course
# is produced this way, live, not typed from memory -- rule 2 in
# docs/writing-a-course.md. `display` is shown as typed; the remaining
# arguments are what actually runs, via "$@" so quoting behaves the same
# as typing it at a real prompt.
ui_run() {
    local display=$1
    shift
    printf '\n  $ %s\n' "$display"
    "$@"
}

# One pre-seeded local repo, no remote. For demos that need an existing
# commit but no networked remote (branches, tags shown purely locally).
ui_new_repo() {
    local -n _repo=$1
    local sandbox
    sandbox=$(ui_sandbox_new)
    _repo="$sandbox/project"

    git init --quiet -b main "$_repo" >/dev/null
    printf 'first line\n' >"$_repo/notes.txt"
    git -C "$_repo" add notes.txt
    git -C "$_repo" -c user.name=Student -c user.email=student@example.com \
        -c commit.gpgsign=false commit --quiet -m "Initial commit"
}

# A bare "remote" pre-populated with one commit -- module 2's own starting
# point, before that module's demo runs `git clone` itself.
ui_new_remote() {
    local -n _sandbox=$1 _remote=$2
    local seed
    _sandbox=$(ui_sandbox_new)
    _remote="$_sandbox/project.git"
    seed="$_sandbox/seed"

    git init --quiet --bare -b main "$_remote" >/dev/null
    git init --quiet -b main "$seed" >/dev/null
    printf '# Demo Project\n' >"$seed/README.md"
    git -C "$seed" add README.md
    git -C "$seed" -c user.name=CodeLearner -c user.email=course@codelearner.local \
        -c commit.gpgsign=false commit --quiet -m "Initial commit"
    git -C "$seed" remote add origin "$_remote"
    git -C "$seed" push --quiet origin main
    rm -rf "$seed"
}

# Same starting point as ui_new_remote, plus a clone already sitting on
# disk -- what every module after clone (status, add, commit, push, pull)
# assumes you already have. The clone itself is not shown; module 2 is
# where that command is the point.
ui_new_remote_and_clone() {
    local -n _sandbox=$1 _remote=$2 _clone=$3
    # Forward the *names* ui_new_remote's own namerefs should bind to --
    # $1/$2 here are already those names one level up. Passing the literal
    # strings "_sandbox"/"_remote" instead would make ui_new_remote's
    # namerefs point at these local namerefs themselves, not the caller's
    # variables (a self-referential chain bash rejects at nameref depth 8).
    ui_new_remote "$1" "$2"
    _clone="$_sandbox/clone"
    git clone --quiet "$_remote" "$_clone" >/dev/null 2>&1
}
