#!/usr/bin/env bash
#
# Git course - main menu.
#
# Written in Bash on purpose: git has no compiler and no language of its
# own to write a course "in" the way C or Python do -- the honest choice,
# per docs/writing-a-course.md's own rule 1, is the shell you actually run
# git from. Every command this course shows is a real invocation against a
# real throwaway repository, not a transcript typed from memory.
#
# Run:  ./main.sh   (needs bash and git; nothing else)

set -uo pipefail

# Git's own messages are localized. Pinning this to C keeps every
# transcript in this course in English regardless of the machine it runs
# on -- the same reason scripts that parse git output do it.
export LC_ALL=C LANG=C

SCRIPT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd)

# shellcheck source=ui.sh
source "$SCRIPT_DIR/ui.sh"
# shellcheck source=lessons_basics.sh
source "$SCRIPT_DIR/lessons_basics.sh"
# shellcheck source=lessons_more.sh
source "$SCRIPT_DIR/lessons_more.sh"

MODULE_TITLES=(
    "What is Git?"
    "git clone"
    "git status"
    "git add"
    "git commit"
    "git push"
    "git pull"
    "Branches"
    "Releases"
    "GitHub Actions"
)

MODULE_FUNCS=(
    lesson_01_what_is_git
    lesson_02_clone
    lesson_03_status
    lesson_04_add
    lesson_05_commit
    lesson_06_push
    lesson_07_pull
    lesson_08_branches
    lesson_09_releases
    lesson_10_actions
)

show_menu() {
    ui_clear_screen
    ui_title_centered "GIT COURSE"

    local i
    for i in "${!MODULE_TITLES[@]}"; do
        printf '   [%2d]  %s\n' "$((i + 1))" "${MODULE_TITLES[$i]}"
    done
    printf '\n   [ 0]  Quit\n'
    ui_rule
}

main() {
    if ! command -v git &>/dev/null; then
        echo "git is not installed. Install it first, then run this again." >&2
        exit 1
    fi

    local choice
    while true; do
        show_menu
        printf '\n  Pick a module: '

        if ! read -r choice; then
            break
        fi
        [[ "$choice" == "0" ]] && break

        if [[ "$choice" =~ ^[0-9]+$ ]] && ((choice >= 1 && choice <= ${#MODULE_TITLES[@]})); then
            ui_clear_screen
            "${MODULE_FUNCS[$((choice - 1))]}"
        else
            printf '\n  Not a valid option.\n'
            ui_wait_enter
        fi
    done

    printf '\n  See you next time.\n\n'
}

main "$@"
