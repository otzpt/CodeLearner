#include "ui.h"

#include <algorithm>
#include <cctype>
#include <iostream>

namespace {

// Inside width of the frame. Matches the C course's box exactly.
constexpr int WIDTH = 54;

void paddedLine(const std::string &text, char border) {
    std::cout << "  " << border << " " << text;
    for (int i = static_cast<int>(text.size()); i < WIDTH - 3; i++) {
        std::cout << ' ';
    }
    std::cout << border << "\n";
}

void frame(char fill) {
    std::cout << "  +";
    for (int i = 0; i < WIDTH - 2; i++) {
        std::cout << fill;
    }
    std::cout << "+\n";
}

// Strip surrounding blanks and lower-case, so "  Two " and "two" compare
// equal. Only used to grade a typed answer against the expected one.
std::string normalise(const std::string &text) {
    size_t start = text.find_first_not_of(" \t");
    if (start == std::string::npos) {
        return "";
    }
    size_t end = text.find_last_not_of(" \t");
    std::string trimmed = text.substr(start, end - start + 1);

    std::transform(trimmed.begin(), trimmed.end(), trimmed.begin(),
                    [](unsigned char c) { return std::tolower(c); });
    return trimmed;
}

}  // namespace

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    // ANSI codes rather than system("clear"): no dependency on TERM being
    // set. Same fix, same reason, as the C course's ui.c.
    std::cout << "\033[H\033[2J\033[3J" << std::flush;
#endif
}

void waitEnter() {
    std::cout << "\n  Press ENTER to continue...";
    std::cout.flush();
    // Consume to the end of the line, the same way the C course does with
    // getchar() in a loop -- cin.get() one character at a time, in case a
    // previous >> left a newline sitting in the buffer.
    int c;
    while ((c = std::cin.get()) != '\n' && c != EOF) {
        // discard
    }
}

void rule() {
    std::cout << "  ";
    for (int i = 0; i < WIDTH; i++) {
        std::cout << '-';
    }
    std::cout << "\n";
}

void title(const std::string &text) {
    std::cout << "\n";
    frame('=');
    paddedLine(text, '|');
    frame('=');
    std::cout << "\n";
}

void heading(const std::string &text) {
    std::cout << "\n  " << text << "\n";
    rule();
}

bool askYes(const std::string &questionText) {
    std::cout << "\n  " << questionText << " (y/N): ";
    std::cout.flush();

    std::string answer;
    std::getline(std::cin, answer);
    return !answer.empty() && (answer[0] == 'y' || answer[0] == 'Y');
}

void exercise(int number) {
    std::cout << "\n  >> EXERCISE - MODULE " << number << "\n";
    rule();
}

bool question(const std::string &text, const std::string &correct, const std::string &why) {
    std::cout << "\n  " << text << "\n";
    std::cout << "  Your answer: ";
    std::cout.flush();

    std::string answer;
    std::getline(std::cin, answer);

    bool right = normalise(answer) == normalise(correct);
    if (right) {
        std::cout << "\n  CORRECT.  " << why << "\n";
    } else {
        std::cout << "\n  NOT QUITE. The answer is: " << correct << "\n";
        std::cout << "             " << why << "\n";
    }
    return right;
}

void challenge(const std::vector<std::string> &task,
               const std::vector<std::string> &input,
               const std::vector<std::string> &expected,
               const std::vector<std::string> &solution) {
    std::cout << "\n  >> WRITE THIS YOURSELF, in a real file\n";
    rule();

    for (const auto &line : task) {
        std::cout << "  " << line << "\n";
    }

    if (!input.empty()) {
        std::cout << "\n  Try it with this input:\n\n";
        for (const auto &line : input) {
            std::cout << "      " << line << "\n";
        }
    }

    if (!expected.empty()) {
        std::cout << "\n  It must print:\n\n";
        for (const auto &line : expected) {
            std::cout << "      " << line << "\n";
        }
        std::cout << "\n  That output is the whole specification. Any code\n";
        std::cout << "  that produces it is correct.\n";
    }

    std::cout << "\n  Try it first. Compile with:\n";
    std::cout << "    g++ -std=c++20 -Wall -Wextra test.cpp -o test && ./test\n";

    if (!askYes("Want to see example code?")) {
        return;
    }

    std::cout << "\n";
    rule();
    for (const auto &line : solution) {
        std::cout << "  " << line << "\n";
    }
    rule();
    std::cout << "  This is EXAMPLE CODE, not the answer. It is one way to\n";
    std::cout << "  get that output; yours may look nothing like it and\n";
    std::cout << "  still be right -- or better. Compare the output, not\n";
    std::cout << "  the code.\n";
}
