// Modules 1 to 5 - the basics.
//
// This course assumes nothing about C. If you came from the C course, the
// point of interest is what changed and what did not -- the traps that
// survive the switch matter more than the ones that got fixed.

#include <iostream>
#include <limits>
#include <string>

#include "lessons.h"
#include "ui.h"

void lesson01Compiling() {
    title("MODULE 1 - COMPILING AND PRINTING");

    heading("PART 1: cout, not printf");

    std::cout << "  C++ compiles the same way C does -- source in, a binary\n";
    std::cout << "  out, nothing runs until then. What is different is how\n";
    std::cout << "  you write to the screen:\n\n";

    std::cout << "    #include <iostream>\n\n";
    std::cout << "    int main()\n";
    std::cout << "    {\n";
    std::cout << "        std::cout << \"Hello world\" << std::endl;\n";
    std::cout << "        return 0;\n";
    std::cout << "    }\n\n";

    std::cout << "  << is not shift here -- it is \"send this to\". You chain\n";
    std::cout << "  as many as you like on one line:\n\n";
    std::cout << "    std::cout << \"age: \" << 25 << \"\\n\";\n\n";

    std::cout << "  No format string, no %d/%s to match up. cout figures out\n";
    std::cout << "  how to print an int or a string from its type -- the\n";
    std::cout << "  first thing that changed for the better.\n";

    waitEnter();
    clearScreen();
    heading("PART 2: std::endl vs \\n");

    std::cout << "    std::cout << \"a\" << std::endl;   // newline, then flush\n";
    std::cout << "    std::cout << \"b\" << \"\\n\";         // just a newline\n\n";

    std::cout << "  std::endl does two things: break the line, and force\n";
    std::cout << "  anything buffered to actually reach the screen right\n";
    std::cout << "  now. Flushing has a real cost if it happens every line\n";
    std::cout << "  in a long loop. Default to \\n; reach for std::endl when\n";
    std::cout << "  you specifically need the output to appear immediately\n";
    std::cout << "  -- before a crash, or before asking for input.\n";

    waitEnter();
    clearScreen();
    heading("PART 3: compiling");

    std::cout << "    g++ -std=c++20 -Wall -Wextra program.cpp -o program\n";
    std::cout << "    ./program\n\n";

    std::cout << "  g++, not gcc -- and the file ends in .cpp, not .c. Using\n";
    std::cout << "  gcc on a .cpp file will not link against the C++\n";
    std::cout << "  standard library correctly. -std=c++20 pins the language\n";
    std::cout << "  version, the same reason the C course pins -std=c11.\n\n";

    std::cout << "  -Wall -Wextra stay on for the same reason as before: a\n";
    std::cout << "  warning here is usually a bug that has not bitten yet.\n";

    waitEnter();
    clearScreen();
    exercise(1);

    question("std::cout << \"a\" << \"b\";  -- one line or two?",
              "one",
              "<< does not add a newline any more than printf did. You get "
              "ab, joined up.");

    question("Which compiler builds a .cpp file: gcc or g++?",
              "g++",
              "gcc can be told to treat a file as C++, but g++ is the one "
              "set up to do it correctly by default.");

    challenge(
        {"Print your name and the current year, each on its own line."},
        {"Jose", "2026"},
        {"#include <iostream>", "", "int main()", "{",
         "    std::cout << \"Jose\" << \"\\n\";", "    std::cout << \"2026\" << \"\\n\";",
         "    return 0;", "}"});

    waitEnter();
    clearScreen();
    heading("SUMMARY");

    std::cout << "   - g++, not gcc; .cpp, not .c\n";
    std::cout << "   - std::cout << value, chained, no format string\n";
    std::cout << "   - \\n by default, std::endl when you need it flushed now\n\n";

    std::cout << "  Module 2: variables, and one trap that did not go away.\n";
    waitEnter();
}

void lesson02Variables() {
    title("MODULE 2 - VARIABLES AND TYPES");

    heading("PART 1: the same types, plus std::string and auto");

    std::cout << "    int    age    = 25;\n";
    std::cout << "    double height = 1.75;      // C++ prefers double to float\n";
    std::cout << "    std::string name = \"Ana\";  // a real string type\n\n";

    std::cout << "  std::string is not char[]. It grows on its own, knows its\n";
    std::cout << "  own length, and does not need a manual \\0 -- module 6 is\n";
    std::cout << "  built entirely around what that removes.\n\n";

    int age = 25;
    double height = 1.75;
    std::string name = "Ana";
    std::cout << "  Running now: " << age << " " << height << " " << name << "\n";

    waitEnter();
    clearScreen();
    heading("PART 2: auto");

    std::cout << "    auto x = 5;      // compiler deduces int\n";
    std::cout << "    auto y = 5.0;    // compiler deduces double\n";
    std::cout << "    auto n = name;   // compiler deduces std::string\n\n";

    std::cout << "  auto is not \"no type\" -- it is \"figure out the type from\n";
    std::cout << "  the right-hand side, once, right here\". The variable is\n";
    std::cout << "  exactly as fixed-type as if you had written it out:\n\n";

    auto x = 5;
    std::cout << "    sizeof(int) = " << sizeof(int) << ", sizeof(x) = " << sizeof(x) << "\n\n";

    std::cout << "  Use it when the type is obvious from the right side, or\n";
    std::cout << "  painful to spell out. Writing auto for everything makes\n";
    std::cout << "  code that reads fast and reviews slow -- you cannot tell\n";
    std::cout << "  what anything is without your editor's help.\n";

    waitEnter();
    clearScreen();
    heading("PART 3: the division that did not go away");

    int a = 5, b = 2;
    std::cout << "    int a = 5, b = 2;\n\n";
    std::cout << "    a / b            -> " << (a / b) << "\n";
    std::cout << "    (double) a / b   -> " << (static_cast<double>(a) / b) << "\n\n";

    std::cout << "  Same rule as C: two ints divide as ints, and the\n";
    std::cout << "  fraction is thrown away, not rounded. C++ changed how\n";
    std::cout << "  you print, how you read, how you store text -- it did\n";
    std::cout << "  not change arithmetic. Cast one side before dividing.\n\n";

    std::cout << "  static_cast<double>(a) is the C++ way to write what C\n";
    std::cout << "  spells (double)a. The old form still compiles; this one\n";
    std::cout << "  is easier to search for and fails to compile instead of\n";
    std::cout << "  silently doing the wrong thing on a handful of trickier\n";
    std::cout << "  conversions. Prefer it going forward.\n";

    waitEnter();
    clearScreen();
    exercise(2);

    question("int x = 7 / 2;   What is x?",
              "3",
              "Still integer division. C++ did not touch this rule.");

    question("What is the C++ way to write (double)a?",
              "static_cast<double>(a)",
              "Same effect as the C-style cast, easier to find in a search "
              "and stricter about what conversions it allows.");

    challenge(
        {"Declare two ints, 7 and 2, and print their division with one",
         "decimal place using static_cast (it should show 3.5)."},
        {"3.5"},
        {"#include <iostream>", "", "int main()", "{", "    int a = 7, b = 2;",
         "    std::cout << static_cast<double>(a) / b << \"\\n\";",
         "    return 0;", "}"});

    waitEnter();
    clearScreen();
    heading("SUMMARY");

    std::cout << "   - std::string and auto are the two real upgrades here\n";
    std::cout << "   - int / int still truncates -- cast one side first\n";
    std::cout << "   - static_cast<T>(x) is the modern spelling of (T)x\n";
    waitEnter();
}

void lesson03Input() {
    title("MODULE 3 - READING INPUT");

    heading("PART 1: std::cin");

    std::cout << "    int age;\n";
    std::cout << "    std::cin >> age;\n\n";

    std::cout << "  >> reads into age directly -- no & needed, because age\n";
    std::cout << "  itself is not passed by value the way it would be to an\n";
    std::cout << "  ordinary function. cin >> uses references, which module\n";
    std::cout << "  7 covers properly; for now, just: no &, unlike scanf.\n\n";

    std::cout << "  For text, std::string plus getline removes the size cap\n";
    std::cout << "  scanf(\"%49s\", ...) needed:\n\n";
    std::cout << "    std::string name;\n";
    std::cout << "    std::getline(std::cin, name);\n\n";
    std::cout << "  A string that grows on its own has nothing to overflow.\n";

    waitEnter();
    clearScreen();
    heading("PART 2: the >> then getline bug");

    std::cout << "  This is the C++ version of the scanf-then-fgets problem,\n";
    std::cout << "  and it is just as easy to hit by accident:\n\n";

    std::cout << "    int age;\n";
    std::cout << "    std::cin >> age;\n";
    std::cout << "    std::getline(std::cin, name);   // reads an empty line\n\n";

    std::cout << "  With input \"25\\nJose\\n\", running this for real:\n\n";
    std::cout << "    Age: 25\n";
    std::cout << "    Name: []\n\n";

    std::cout << "  >> stopped at the digits and left the newline sitting in\n";
    std::cout << "  the buffer. getline immediately finds that leftover\n";
    std::cout << "  newline and returns an empty string -- no error, no\n";
    std::cout << "  warning, just the wrong answer.\n\n";

    std::cout << "  The fix discards everything up to and including that\n";
    std::cout << "  newline before the getline runs:\n\n";
    std::cout << "    #include <limits>\n";
    std::cout << "    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\\n');\n\n";

    std::cout << "  Verified: with the ignore() call added, the same input\n";
    std::cout << "  gives Name: [Jose].\n";

    waitEnter();
    clearScreen();
    heading("PART 3: cin can fail");

    std::cout << "    int n;\n";
    std::cout << "    if (!(std::cin >> n)) {\n";
    std::cout << "        std::cout << \"That is not a number.\\n\";\n";
    std::cout << "        return 1;\n";
    std::cout << "    }\n\n";

    std::cout << "  std::cin >> n evaluates to the stream itself, and a\n";
    std::cout << "  stream that just failed to read converts to false. Typing\n";
    std::cout << "  \"abc\" where a number was expected takes this branch --\n";
    std::cout << "  verified, exit status 1, without n ever being touched.\n";

    waitEnter();
    clearScreen();
    exercise(3);

    question("std::cin >> age;  -- does this need &age, like scanf did?\n"
             "  (answer: yes or no)",
             "no",
             "cin >> takes age through a reference. Module 7 explains why "
             "that removes the need for &.");

    question("cin >> age; then getline(cin, name);  -- what ends up in name?\n"
             "  (answer: empty, or the typed text)",
             "empty",
             ">> leaves the newline behind; getline reads it immediately and "
             "stops.");

    question("What removes the size limit fgets needed in C?",
             "std::string",
             "A std::string grows to fit whatever getline reads. There is no "
             "fixed buffer to overflow.");

    challenge(
        {"Ask for the user's name and greet them."},
        {"What is your name? Jose", "Hello, Jose"},
        {"#include <iostream>", "#include <string>", "", "int main()", "{",
         "    std::string name;", "", "    std::cout << \"What is your name? \";",
         "    std::getline(std::cin, name);", "",
         "    std::cout << \"Hello, \" << name << \"\\n\";",
         "    return 0;", "}"});

    waitEnter();
    clearScreen();
    heading("SUMMARY");

    std::cout << "   - cin >> x needs no &; getline(cin, str) has no size cap\n";
    std::cout << "   - mixing >> and getline leaves a newline behind -- ignore() it\n";
    std::cout << "   - !(cin >> x) is true when the read failed\n";
    waitEnter();
}

void lesson04Conditions() {
    title("MODULE 4 - CONDITIONS");

    heading("PART 1: if / else if / else, and a real bool");

    std::cout << "    int score = 85;\n\n";
    std::cout << "    if (score >= 90) {\n";
    std::cout << "        std::cout << \"A\\n\";\n";
    std::cout << "    } else if (score >= 80) {\n";
    std::cout << "        std::cout << \"B\\n\";\n";
    std::cout << "    } else {\n";
    std::cout << "        std::cout << \"F\\n\";\n";
    std::cout << "    }\n\n";

    int score = 85;
    std::cout << "  Running with score = " << score << ":  ";
    if (score >= 90) {
        std::cout << "A\n\n";
    } else if (score >= 80) {
        std::cout << "B\n\n";
    } else {
        std::cout << "F\n\n";
    }

    std::cout << "  Same rule as C: the first true branch runs, order\n";
    std::cout << "  matters. What is new is bool -- a real type, not an int\n";
    std::cout << "  standing in for one:\n\n";

    bool passed = score >= 70;
    std::cout << "    bool passed = score >= 70;\n";
    std::cout << "    std::cout << passed;                    -> " << passed << "\n";
    std::cout << "    std::cout << std::boolalpha << passed;  -> " << std::boolalpha << passed
              << "\n\n";

    std::cout << "  Printed raw, a bool is still 1 or 0 -- the type is real,\n";
    std::cout << "  but cout's default formatting was not changed to match.\n";
    std::cout << "  std::boolalpha switches it to true/false, and that\n";
    std::cout << "  setting sticks on the stream until you turn it off.\n";

    waitEnter();
    clearScreen();
    heading("PART 2: = is still not ==");

    std::cout << "    int x = 3;\n";
    std::cout << "    if (x = 5) { ... }\n\n";

    std::cout << "  This still compiles, still assigns, still evaluates to\n";
    std::cout << "  true, and the block still runs every time. g++ -Wall\n";
    std::cout << "  catches it with the same warning gcc gives:\n\n";
    std::cout << "    warning: suggest parentheses around assignment used\n";
    std::cout << "             as truth value [-Wparentheses]\n\n";

    std::cout << "  Verified on this compiler. The language changed a great\n";
    std::cout << "  deal between C and C++; this particular mistake did not\n";
    std::cout << "  get fixed, because = and == are still two different,\n";
    std::cout << "  both perfectly legal, operators.\n";

    waitEnter();
    clearScreen();
    exercise(4);

    question("bool ok = true;  std::cout << ok;  -- what prints, without\n"
             "  boolalpha? (answer: true or 1)",
             "1",
             "bool is a real type, but cout's default formatting for it is "
             "still 1/0. std::boolalpha changes that.");

    question("int x = 3;  if (x = 5) { ... }   Does the block run?\n"
             "  (answer: yes, no or depends)",
             "yes",
             "= assigns and evaluates to 5, which is true. It runs every "
             "time -- g++ warns, but still compiles it.");

    challenge(
        {"Read a number and say whether it is positive, negative or zero."},
        {"Number: -5", "negative"},
        {"#include <iostream>", "", "int main()", "{", "    int n;",
         "    std::cout << \"Number: \";", "    std::cin >> n;", "",
         "    if (n > 0) {", "        std::cout << \"positive\\n\";",
         "    } else if (n < 0) {", "        std::cout << \"negative\\n\";",
         "    } else {", "        std::cout << \"zero\\n\";", "    }",
         "    return 0;", "}"});

    waitEnter();
    clearScreen();
    heading("SUMMARY");

    std::cout << "   - the first true branch runs; order matters, as before\n";
    std::cout << "   - bool is a real type; cout still prints it as 1/0 unless\n";
    std::cout << "     boolalpha is set\n";
    std::cout << "   - = still is not ==, and the compiler still only warns\n";
    waitEnter();
}

void lesson05Loops() {
    title("MODULE 5 - LOOPS");

    heading("PART 1: for and while, unchanged");

    std::cout << "    for (int i = 1; i <= 5; i++) {\n";
    std::cout << "        std::cout << i << \" \";\n";
    std::cout << "    }\n\n";

    std::cout << "  Running:  ";
    for (int i = 1; i <= 5; i++) {
        std::cout << i << " ";
    }
    std::cout << "\n\n";

    std::cout << "  Same three parts, same off-by-one risk if the condition\n";
    std::cout << "  is <= where it should be <. while works exactly as it\n";
    std::cout << "  did in C too. What is new is a second kind of for.\n";

    waitEnter();
    clearScreen();
    heading("PART 2: range-based for");

    std::cout << "    std::vector<std::string> fruits = {\"apple\", \"banana\", \"cherry\"};\n\n";
    std::cout << "    for (const auto &f : fruits) {\n";
    std::cout << "        std::cout << \"- \" << f << \"\\n\";\n";
    std::cout << "    }\n\n";

    std::cout << "  \"for each f in fruits\" -- no index, no length to get\n";
    std::cout << "  wrong, no chance of the off-by-one from Part 1. Full\n";
    std::cout << "  vector coverage is module 8; for now, this is how you\n";
    std::cout << "  loop over anything with a beginning and an end.\n\n";

    std::vector<std::string> fruits = {"apple", "banana", "cherry"};
    for (const auto &f : fruits) {
        std::cout << "    - " << f << "\n";
    }

    waitEnter();
    clearScreen();
    heading("PART 3: copy or reference?");

    std::cout << "    for (auto n : nums)  { n *= 10; }   // changes a COPY\n";
    std::cout << "    for (auto &n : nums) { n *= 10; }   // changes the real one\n\n";

    std::cout << "  Without the &, each n is a fresh copy of that element --\n";
    std::cout << "  the same pass-by-value rule from C's functions, just\n";
    std::cout << "  appearing inside a loop instead. g++ even flags the\n";
    std::cout << "  pointless version on its own:\n\n";
    std::cout << "    warning: variable 'n' set but not used\n";
    std::cout << "             [-Wunused-but-set-variable=]\n\n";

    std::vector<int> nums = {1, 2, 3};
    for (auto &n : nums) {
        n *= 10;
    }
    std::cout << "  With the &, running for real:  ";
    for (auto n : nums) {
        std::cout << n << " ";
    }
    std::cout << "\n";

    waitEnter();
    clearScreen();
    exercise(5);

    question("for (auto n : items) { n = 0; }  -- does this clear items?\n"
             "  (answer: yes or no)",
             "no",
             "Without &, n is a copy. Use `auto &n` to reach the real "
             "elements.");

    question("Which loop form removes the risk of an off-by-one entirely?",
             "range-based for",
             "There is no index to get wrong when there is no index at all.");

    challenge(
        {"Print 1 through 5, one per line. Then print each of",
         "\"apple\", \"banana\", \"cherry\" prefixed with \"- \", using a",
         "range-based for loop."},
        {"1", "2", "3", "4", "5", "- apple", "- banana", "- cherry"},
        {"#include <iostream>", "#include <string>", "#include <vector>", "",
         "int main()", "{", "    for (int i = 1; i <= 5; i++) {",
         "        std::cout << i << \"\\n\";", "    }", "",
         "    std::vector<std::string> fruits = {\"apple\", \"banana\", \"cherry\"};",
         "    for (const auto &f : fruits) {", "        std::cout << \"- \" << f << \"\\n\";",
         "    }", "    return 0;", "}"});

    waitEnter();
    clearScreen();
    heading("SUMMARY");

    std::cout << "   - for/while carry over unchanged, off-by-one included\n";
    std::cout << "   - range-based for removes the index entirely\n";
    std::cout << "   - `for (auto x : ...)` copies; `for (auto &x : ...)` does not\n\n";

    std::cout << "  Module 6: std::string, and everything strcmp made painful.\n";
    waitEnter();
}
