// C++ course - main menu.
//
// Written in C++, the same way the C course is written in C: the source is
// itself worked example code. This file already uses a std::vector of
// structs and a function pointer stored inside one, both taught properly
// in modules 7-9.
//
// Build:  make
// Run:    ./cpp-course

#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "lessons.h"
#include "ui.h"

struct Module {
    std::string title;
    std::function<void()> run;
};

int main() {
    const std::vector<Module> modules = {
        {"Compiling and printing", lesson01Compiling},
        {"Variables and types", lesson02Variables},
        {"Reading input", lesson03Input},
        {"Conditions", lesson04Conditions},
        {"Loops", lesson05Loops},
        {"std::string", lesson06Strings},
        {"Functions and references", lesson07Functions},
        {"std::vector", lesson08Vectors},
        {"Classes and RAII", lesson09Classes},
    };

    for (;;) {
        clearScreen();
        title("C++ COURSE - FROM C TO CLASSES");

        for (size_t i = 0; i < modules.size(); i++) {
            std::cout << "   [" << (i + 1 < 10 ? " " : "") << (i + 1) << "]  "
                      << modules[i].title << "\n";
        }
        std::cout << "\n   [ 0]  Quit\n";
        rule();

        std::cout << "\n  Pick a module: ";
        std::cout.flush();

        std::string choice;
        if (!std::getline(std::cin, choice)) {
            break;
        }
        if (choice == "0") {
            break;
        }

        int n;
        try {
            n = std::stoi(choice);
        } catch (...) {
            n = 0;
        }

        if (n >= 1 && n <= static_cast<int>(modules.size())) {
            clearScreen();
            modules[n - 1].run();
        } else {
            std::cout << "\n  Not a valid option.\n";
            waitEnter();
        }
    }

    std::cout << "\n  See you next time.\n\n";
    return 0;
}
