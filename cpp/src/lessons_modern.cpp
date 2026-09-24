// Modules 6 to 12 - the part that sets C++ apart.
//
// Each module closes a gap the C course spent real time on: strings without
// \0, functions that write back without &, arrays that know their own size,
// and finally memory that frees itself at the right time. None of it is
// magic -- each one is shown working, and shown still breakable.
//
// Module 10 goes past std::vector into std::map and std::set -- the STL
// containers module 8 didn't need, and the trap map's own operator[]
// hides (a missing-key read that silently inserts, rather than telling
// you it was missing).
//
// Module 11 names the mechanism that was already underneath std::vector<T>,
// std::map<K, V> and std::string since module 5: templates, and the one
// error they can produce, shown by actually compiling it and printing
// g++'s own error text -- not a hand-written summary of one.
//
// Module 12 closes the course: std::unique_ptr and std::shared_ptr replace
// module 9's own new/delete, std::move is shown changing an actually counted
// copy into an actually counted move (not asserted), and structured bindings
// give module 10's map loop a second, more direct way to write the same
// thing.

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

/* MinGW has no <sys/wait.h>, and needs none: on Windows std::system returns
 * the command's exit code directly, without POSIX's wait-status encoding
 * wrapped around it. Guarding the include alone is not enough -- the two
 * macros are used below, so Windows needs its own definitions of them. */
#ifdef _WIN32
#  define WIFEXITED(status)   1
#  define WEXITSTATUS(status) (status)
#else
#  include <sys/wait.h>
#endif

#include "lessons.h"
#include "ui.h"

void lesson06Strings() {
    title("MODULE 6 - STD::STRING");

    heading("PART 1: == finally means what it looks like");

    std::string a = "hi";
    std::string b = "hi";

    std::cout << "    std::string a = \"hi\";\n";
    std::cout << "    std::string b = \"hi\";\n";
    std::cout << "    a == b   -> " << std::boolalpha << (a == b) << "\n\n";

    std::cout << "  In C this was strcmp(a, b) == 0, because == compared\n";
    std::cout << "  addresses. std::string overloads == to compare\n";
    std::cout << "  contents, so it does what it looks like it does. The\n";
    std::cout << "  single most reported C++ beginner question over a\n";
    std::cout << "  string comparison bug does not exist here.\n\n";

    std::cout << "  + concatenates, and length()/size() are the same thing:\n\n";
    std::cout << "    a + \" there\"   -> " << (a + " there") << "\n";
    std::cout << "    a.length()     -> " << a.length() << "\n";
    std::cout << "    a[0]           -> " << a[0] << "\n";

    waitEnter();
    clearScreen();
    heading("PART 2: substr and find");

    std::string full = "hello world";
    std::cout << "    std::string full = \"hello world\";\n\n";
    std::cout << "    full.substr(6)     -> " << full.substr(6) << "\n";
    std::cout << "    full.substr(0, 5)  -> " << full.substr(0, 5) << "\n";
    std::cout << "    full.find(\"world\") -> " << full.find("world") << "\n\n";

    std::cout << "  substr(start) takes to the end; substr(start, count)\n";
    std::cout << "  takes count characters. find returns the index where a\n";
    std::cout << "  match starts.\n\n";

    std::cout << "  What it returns when there is no match is the one thing\n";
    std::cout << "  worth memorising:\n\n";
    std::cout << "    full.find(\"xyz\")   -> " << full.find("xyz") << "\n\n";

    std::cout << "  Not -1. That number is std::string::npos -- the largest\n";
    std::cout << "  possible size_t, standing in for \"not found\". Compare\n";
    std::cout << "  against it by name, never against -1:\n\n";
    std::cout << "    if (full.find(\"xyz\") == std::string::npos) { ... }\n";

    waitEnter();
    clearScreen();
    heading("PART 3: out-of-range access");

    std::cout << "    a[50]      // no check -- undefined behaviour\n";
    std::cout << "    a.at(50)   // checked -- throws\n\n";

    std::cout << "  a[i] is exactly as unchecked as a C array. a.at(i) is\n";
    std::cout << "  the version that verifies the index and throws\n";
    std::cout << "  std::out_of_range instead of doing whatever happens to\n";
    std::cout << "  be at that address. Running it for real:\n\n";

    try {
        std::cout << a.at(50) << "\n";
    } catch (const std::out_of_range &e) {
        std::cout << "    caught: " << e.what() << "\n\n";
    }

    std::cout << "  try/catch is C++'s error handling -- a block that might\n";
    std::cout << "  throw, and a handler for the specific exception type you\n";
    std::cout << "  expect. Module 9 uses the same shape for something more\n";
    std::cout << "  important than a bounds check.\n";

    waitEnter();
    clearScreen();
    exercise(6);

    question("std::string a = \"cat\", b = \"cat\";  What does a == b give?\n"
             "  (answer: true or false)",
             "true",
             "std::string's == compares contents, unlike C's == on char*, "
             "which compared addresses.");

    question("\"hello\".find(\"z\")  -- what does this return?",
             "npos",
             "Not -1. std::string::npos is the largest possible size_t, "
             "used as the not-found marker.");

    question("s[50] on a short string is unchecked. Which method checks\n"
             "  and throws instead?",
             "at",
             "s.at(50) verifies the index and throws std::out_of_range "
             "rather than reading past the string.");

    challenge(
        {"Read a word and print it reversed, using only string",
         "operations -- no manual indexing loop needed if you look",
         "for a way to build the reverse directly."},
        {"hello"},
        {"Word: hello", "olleh"},
        {"#include <iostream>", "#include <string>", "", "int main()", "{",
         "    std::string word;", "    std::cout << \"Word: \";",
         "    std::cin >> word;", "",
         "    std::string reversed(word.rbegin(), word.rend());",
         "    std::cout << reversed << \"\\n\";", "    return 0;", "}", "",
         "rbegin()/rend() walk the string back to front; building a new",
         "string from that range does the reversal in one line. A loop",
         "indexing from the end, like the C version did, also works."});

    waitEnter();
    clearScreen();
    heading("SUMMARY");

    std::cout << "   - == compares contents; + concatenates; no \\0 to manage\n";
    std::cout << "   - find returns std::string::npos, not -1, when nothing matches\n";
    std::cout << "   - [] is unchecked like C; at() checks and throws\n";
    waitEnter();
}

void lesson07Functions() {
    title("MODULE 7 - FUNCTIONS AND REFERENCES");

    heading("PART 1: still passed by copy, by default");

    std::cout << "    void tryChange(int copy) { copy = 99; }\n\n";
    std::cout << "    int x = 5;\n";
    std::cout << "    tryChange(x);\n";
    std::cout << "    // x is still 5\n\n";

    std::cout << "  No change from C: an int argument is a copy, and\n";
    std::cout << "  changing the copy does not touch the caller's variable.\n";

    waitEnter();
    clearScreen();
    heading("PART 2: a reference removes the need for &");

    std::cout << "    void realChange(int &ref) { ref = 99; }\n\n";
    std::cout << "    int x = 5;\n";
    std::cout << "    realChange(x);      // no &x -- just x\n\n";

    int x = 5;
    auto realChange = [](int &ref) { ref = 99; };
    realChange(x);
    std::cout << "  Running for real: x is now " << x << "\n\n";

    std::cout << "  int &ref means \"ref is another name for whatever was\n";
    std::cout << "  passed in\" -- not a copy, not an address you have to\n";
    std::cout << "  dereference, the actual variable under a second name.\n";
    std::cout << "  This is what a C function needed a pointer and a caller\n";
    std::cout << "  needed & for. The mechanism underneath is the same\n";
    std::cout << "  address-passing; the syntax stopped making you spell it\n";
    std::cout << "  out on both ends.\n";

    waitEnter();
    clearScreen();
    heading("PART 3: const & for the common case");

    std::cout << "    std::string greet(const std::string &name) {\n";
    std::cout << "        return \"hello, \" + name;\n";
    std::cout << "    }\n\n";

    auto greet = [](const std::string &name) { return "hello, " + name; };
    std::cout << "  Running: " << greet("Ana") << "\n\n";

    std::cout << "  A plain std::string parameter would copy the whole\n";
    std::cout << "  string on every call. const &name passes it without\n";
    std::cout << "  copying, and const promises the function will not\n";
    std::cout << "  modify it -- the caller's string is safe by contract,\n";
    std::cout << "  not just by convention.\n\n";

    std::cout << "  Rule of thumb: small types (int, double, bool, char)\n";
    std::cout << "  pass by value; anything bigger -- std::string,\n";
    std::cout << "  std::vector, your own classes -- pass by const & unless\n";
    std::cout << "  the function actually needs to change it, in which case\n";
    std::cout << "  it takes a plain & instead.\n";

    waitEnter();
    clearScreen();
    heading("PART 4: overloading and defaults");

    std::cout << "    int add(int a, int b) { return a + b; }\n";
    std::cout << "    double add(double a, double b) { return a + b; }\n\n";

    std::cout << "  Two functions, same name, different parameter types --\n";
    std::cout << "  illegal in C, ordinary in C++. The compiler picks the\n";
    std::cout << "  right one from the argument types at the call site.\n\n";

    auto addInt = [](int a, int b) { return a + b; };
    auto addDouble = [](double a, double b) { return a + b; };
    std::cout << "    add(2, 3)     -> " << addInt(2, 3) << "\n";
    std::cout << "    add(2.5, 3.5) -> " << addDouble(2.5, 3.5) << "\n\n";

    std::cout << "  Default arguments work exactly as in the C course's\n";
    std::cout << "  power(base, exp = 2) -- nothing new there.\n";

    waitEnter();
    clearScreen();
    exercise(7);

    question("void f(int &n) { n = 99; }   Do you call f(x) or f(&x)?",
             "f(x)",
             "References remove the & at the call site entirely -- that is "
             "their whole point.");

    question("void f(const std::string &s)  -- can f modify the caller's\n"
             "  string? (answer: yes or no)",
             "no",
             "const promises it will not, and the compiler enforces that "
             "promise at compile time.");

    question("int add(int,int) and double add(double,double) in the same\n"
             "  program -- legal in C++?  (answer: yes or no)",
             "yes",
             "Overloading: the compiler chooses based on the argument "
             "types you pass at each call site.");

    challenge(
        {"Write swap(int &a, int &b) that swaps two ints using",
         "references, no pointers. Test it."},
        {},
        {"2 1"},
        {"#include <iostream>", "", "void swap(int &a, int &b)", "{",
         "    int held = a;", "    a = b;", "    b = held;", "}", "",
         "int main()", "{", "    int x = 1, y = 2;", "    swap(x, y);",
         "    std::cout << x << \" \" << y << \"\\n\";", "    return 0;", "}"});

    waitEnter();
    clearScreen();
    heading("SUMMARY");

    std::cout << "   - plain parameters are still copies\n";
    std::cout << "   - & makes a parameter an alias -- no & needed to call\n";
    std::cout << "   - const & passes big types without copying and without risk\n";
    std::cout << "   - overloading picks a function by its argument types\n";
    waitEnter();
}

void lesson08Vectors() {
    title("MODULE 8 - STD::VECTOR");

    heading("PART 1: an array that grows");

    std::cout << "    std::vector<int> v = {3, 1, 2};\n";
    std::cout << "    v.push_back(10);\n\n";

    std::vector<int> v = {3, 1, 2};
    v.push_back(10);
    std::cout << "  Running: size() = " << v.size() << ", contents: ";
    for (int n : v) {
        std::cout << n << " ";
    }
    std::cout << "\n\n";

    std::cout << "  No malloc, no realloc, no capacity to track by hand --\n";
    std::cout << "  module 12 of the C course spent a whole section on\n";
    std::cout << "  exactly this problem. push_back grows the vector as\n";
    std::cout << "  needed, and size() always tells you how many elements\n";
    std::cout << "  are really there.\n\n";

    std::cout << "  <vector> also has to be included -- it is a template,\n";
    std::cout << "  not a built-in type, the same way std::string needs\n";
    std::cout << "  <string>.\n";

    waitEnter();
    clearScreen();
    heading("PART 2: sorting, and [] is still not checked");

    std::cout << "    #include <algorithm>\n";
    std::cout << "    std::sort(v.begin(), v.end());\n\n";

    std::sort(v.begin(), v.end());
    std::cout << "  Running: ";
    for (int n : v) {
        std::cout << n << " ";
    }
    std::cout << "\n\n";

    std::cout << "  begin()/end() mark the range to sort -- the whole\n";
    std::cout << "  vector, here. The same pair is what range-based for\n";
    std::cout << "  from module 5 walks internally.\n\n";

    std::cout << "  v[100] on a 4-element vector is undefined behaviour --\n";
    std::cout << "  the language makes no promise at all about what\n";
    std::cout << "  happens. On this compiler, right now, it aborts with an\n";
    std::cout << "  assertion:\n\n";
    std::cout << "    Assertion '__n < this->size()' failed.\n\n";

    std::cout << "  That is an OBSERVED result on this toolchain, not a\n";
    std::cout << "  language guarantee -- a different compiler, or this one\n";
    std::cout << "  built differently, is allowed to read garbage instead\n";
    std::cout << "  and keep running. Never rely on operator[] to catch a\n";
    std::cout << "  mistake. v.at(100) is the one that is actually\n";
    std::cout << "  guaranteed to throw std::out_of_range, on every\n";
    std::cout << "  standard-conforming compiler, every time.\n";

    waitEnter();
    clearScreen();
    exercise(8);

    question("std::vector<int> v;  v.push_back(1);  -- do you need to\n"
             "  decide a size up front?  (answer: yes or no)",
             "no",
             "push_back grows the vector as needed. There is no capacity to "
             "plan ahead of time.");

    question("v[100] on a small vector -- does the language guarantee\n"
             "  what happens?  (answer: yes or no)",
             "no",
             "It is undefined behaviour. What this specific compiler does "
             "with it is an observation, not a promise.");

    question("Which access always throws std::out_of_range on a bad index,\n"
             "  on every standard-conforming compiler?",
             "at",
             "v.at(i) is checked by the standard itself; v[i] is not "
             "required to check anything.");

    challenge(
        {"Read integers until -1 is entered, storing each in a", "vector.",
         "Print them sorted, then print how many there were."},
        {"1", "2", "5", "-1"},
        {"Enter numbers, -1 to stop:", "1 2 5", "3"},
        {"#include <algorithm>", "#include <iostream>", "#include <vector>", "",
         "int main()", "{", "    std::vector<int> nums;", "    int n;",
         "", "    std::cout << \"Enter numbers, -1 to stop:\\n\";",
         "    while (std::cin >> n && n != -1) {", "        nums.push_back(n);",
         "    }", "", "    std::sort(nums.begin(), nums.end());",
         "    for (int x : nums) {", "        std::cout << x << \" \";", "    }",
         "    std::cout << \"\\n\";",
         "    std::cout << nums.size() << \"\\n\";", "    return 0;", "}"});

    waitEnter();
    clearScreen();
    heading("SUMMARY");

    std::cout << "   - push_back grows the vector; size() is always accurate\n";
    std::cout << "   - std::sort(v.begin(), v.end()) sorts the whole thing\n";
    std::cout << "   - [] is unchecked, at() throws -- same rule as std::string\n\n";

    std::cout << "  Module 9 closes the loop C's module 9 opened: memory\n";
    std::cout << "  that frees itself, correctly, on its own.\n";
    waitEnter();
}

namespace lesson9detail {

class Student {
public:
    Student(std::string n, double g) : name_(std::move(n)), grade_(g) {
        std::cout << "    constructor: " << name_ << "\n";
    }
    ~Student() {
        std::cout << "    destructor: " << name_ << "\n";
    }
    void describe() const {
        std::cout << name_ << ": " << grade_ << "\n";
    }

private:
    std::string name_;
    double grade_;
};

}  // namespace lesson9detail

void lesson09Classes() {
    title("MODULE 9 - CLASSES AND RAII");

    heading("PART 1: struct's other half");

    std::cout << "  The C course's struct only grouped fields. A class adds\n";
    std::cout << "  functions that belong to those fields, and a way to hide\n";
    std::cout << "  them:\n\n";

    std::cout << "    class Student {\n";
    std::cout << "    public:\n";
    std::cout << "        Student(std::string n, double g) : name_(n), grade_(g) {}\n";
    std::cout << "        void describe() const { std::cout << name_ << \": \" << grade_; }\n";
    std::cout << "    private:\n";
    std::cout << "        std::string name_;\n";
    std::cout << "        double grade_;\n";
    std::cout << "    };\n\n";

    std::cout << "  private means only Student's own functions can touch\n";
    std::cout << "  name_ and grade_. Reaching in from outside is a compile\n";
    std::cout << "  error, not a convention someone might ignore:\n\n";
    std::cout << "    error: 'double Student::grade_' is private within\n";
    std::cout << "           this context\n\n";

    std::cout << "  Verified on this compiler, with exactly that message.\n";

    waitEnter();
    clearScreen();
    heading("PART 2: constructor, destructor, and when each runs");

    std::cout << "  The constructor runs when the object is created; the\n";
    std::cout << "  destructor runs when it goes out of scope -- no new,\n";
    std::cout << "  no delete, no call written anywhere for either one:\n\n";

    std::cout << "  Running for real:\n\n";
    std::cout << "    entering main\n";
    {
        lesson9detail::Student a("Ana", 15.0);
        std::cout << "    ";
        a.describe();
        std::cout << "\n";
    }
    std::cout << "    left the block\n\n";

    std::cout << "  The destructor fired at the closing brace, before \"left\n";
    std::cout << "  the block\" printed -- not at the end of main, not when\n";
    std::cout << "  you remember to clean up. This is RAII: a resource is\n";
    std::cout << "  tied to an object's lifetime, and the object's lifetime\n";
    std::cout << "  is tracked by the compiler, the same way it already\n";
    std::cout << "  tracks when a plain int goes out of scope.\n";

    waitEnter();
    clearScreen();
    heading("PART 3: new/delete still exist, and still leak");

    std::cout << "  Underneath std::string and std::vector is exactly the\n";
    std::cout << "  malloc/free problem from the C course, spelled\n";
    std::cout << "  differently:\n\n";

    std::cout << "    int *v = new int[5];\n";
    std::cout << "    // ... no delete[] v ...\n\n";

    std::cout << "  Real LeakSanitizer output from running that:\n\n";
    std::cout << "    ==12345==ERROR: LeakSanitizer: detected memory leaks\n";
    std::cout << "    Direct leak of 20 byte(s) in 1 object(s) allocated from:\n\n";

    std::cout << "  Twenty bytes: five ints, four bytes each. C++ giving you\n";
    std::cout << "  std::vector does not mean the underlying problem went\n";
    std::cout << "  away -- it means the standard library already wrote the\n";
    std::cout << "  RAII wrapper around new/delete so you do not have to.\n";
    std::cout << "  Reaching for new yourself brings malloc's whole set of\n";
    std::cout << "  rules back with it. Prefer std::vector, std::string and\n";
    std::cout << "  friends; they exist specifically so new/delete rarely\n";
    std::cout << "  has to appear in code you write.\n";

    waitEnter();
    clearScreen();
    exercise(9);

    question("A class field is declared private. Can code outside the\n"
             "  class read it directly?  (answer: yes or no)",
             "no",
             "That is a compile error, not just bad practice -- the "
             "compiler enforces it.");

    question("An object is created inside { }. When does its destructor\n"
             "  run?  (answer: at the closing brace, or at program exit)",
             "at the closing brace",
             "RAII: a destructor runs when its object's scope ends, not "
             "whenever you remember to call something.");

    question("int *p = new int[5]; with no delete[] p; -- does this leak,\n"
             "  the same as C's malloc without free?  (answer: yes or no)",
             "yes",
             "new/delete are malloc/free with different names. std::vector "
             "exists so you rarely have to call either yourself.");

    challenge(
        {"Write a class Counter with a private int starting at 0, a",
         "method increment() that adds 1, and a method value() that",
         "returns the current count. Create one, call increment()",
         "three times, and print value()."},
        {},
        {"3"},
        {"#include <iostream>", "", "class Counter {", "public:",
         "    void increment() { count_++; }", "    int value() const { return count_; }",
         "private:", "    int count_ = 0;", "};", "", "int main()", "{",
         "    Counter c;", "    c.increment();", "    c.increment();",
         "    c.increment();", "    std::cout << c.value() << \"\\n\";",
         "    return 0;", "}"});

    waitEnter();
    clearScreen();
    heading("SUMMARY");

    std::cout << "   - class adds methods and private/public to struct\n";
    std::cout << "   - constructor runs on creation, destructor at scope end\n";
    std::cout << "   - RAII: resources tied to object lifetime, freed automatically\n";
    std::cout << "   - new/delete are still malloc/free -- still leak if unmatched\n\n";

    std::cout << "  That covers the core course. std::string, std::vector and\n";
    std::cout << "  your own classes cover the overwhelming majority of what\n";
    std::cout << "  C needed manual memory management for.\n\n";

    std::cout << "  Module 10 rounds out the STL with the two containers\n";
    std::cout << "  vector doesn't replace: std::map and std::set.\n";
    waitEnter();
}

void lesson10Containers() {
    title("MODULE 10 - MAPS AND SETS");

    heading("PART 1: std::map -- key to value");

    std::cout << "  A vector holds values in order, found by index. A map\n";
    std::cout << "  holds them found by key instead, kept sorted by that\n";
    std::cout << "  key:\n\n";

    std::cout << "    std::map<std::string, int> ages;\n";
    std::cout << "    ages[\"Ana\"] = 30;\n";
    std::cout << "    ages[\"Bea\"] = 25;\n\n";

    std::map<std::string, int> ages;
    ages["Ana"] = 30;
    ages["Bea"] = 25;

    std::cout << "  Running, iterated in key order (not insertion order):\n\n";
    for (const auto &entry : ages) {
        std::cout << "    " << entry.first << ": " << entry.second << "\n";
    }

    std::cout << "\n  `[]` on a key you already checked reads the value. On a\n";
    std::cout << "  key that is NOT there, it does something C++ beginners\n";
    std::cout << "  routinely get bitten by: it silently INSERTS that key\n";
    std::cout << "  with a default value (0 for int), rather than telling\n";
    std::cout << "  you it was missing:\n\n";

    std::cout << "    std::cout << ages[\"Carl\"];   // reads 0 -- AND inserts \"Carl\": 0\n\n";

    std::cout << "  Running: before, the map has " << ages.size() << " entries.\n";
    std::cout << "  ages[\"Carl\"] gives: " << ages["Carl"] << "\n";
    std::cout << "  After, the map has " << ages.size() << " entries -- \"Carl\" is now in\n";
    std::cout << "  it, whether that was the intent or not.\n\n";

    std::cout << "  The safe check is .count(), not []:\n\n";
    std::cout << "    if (ages.count(\"Dee\") == 0) { ... }\n\n";

    if (ages.count("Dee") == 0) {
        std::cout << "  Running: \"Dee\" is not in the map, and .count() did not add\n";
        std::cout << "  it -- size is still " << ages.size() << ".\n";
    }

    waitEnter();
    clearScreen();
    heading("PART 2: std::set -- unique values, kept sorted");

    std::cout << "  A set holds each value at most once, sorted, with no\n";
    std::cout << "  paired value the way map has -- membership is the whole\n";
    std::cout << "  point:\n\n";

    std::cout << "    std::set<int> seen;\n";
    std::cout << "    seen.insert(5);\n";
    std::cout << "    seen.insert(3);\n";
    std::cout << "    seen.insert(5);   // already there -- ignored, not duplicated\n\n";

    std::set<int> seen;
    seen.insert(5);
    seen.insert(3);
    seen.insert(5);

    std::cout << "  Running: size is " << seen.size() << " (the second 5 changed nothing),\n";
    std::cout << "  iterated in sorted order:\n\n    ";
    for (int v : seen) {
        std::cout << v << " ";
    }
    std::cout << "\n";

    waitEnter();
    clearScreen();
    heading("PART 3: <algorithm> beyond sort");

    std::cout << "  Module 8 already used std::sort. std::find does the same\n";
    std::cout << "  kind of job for a single lookup in something that is not\n";
    std::cout << "  a map or set -- a plain vector, searched by value:\n\n";

    std::cout << "    std::vector<int> nums = {4, 8, 15, 16, 23, 42};\n";
    std::cout << "    auto it = std::find(nums.begin(), nums.end(), 16);\n\n";

    std::vector<int> nums = {4, 8, 15, 16, 23, 42};
    auto it = std::find(nums.begin(), nums.end(), 16);

    std::cout << "  Running: ";
    if (it != nums.end()) {
        std::cout << "found 16 at index " << (it - nums.begin()) << ".\n";
    } else {
        std::cout << "not found.\n";
    }

    std::cout << "\n  it == nums.end() means \"not found\" -- the same role NULL\n";
    std::cout << "  plays for a pointer in C, and .get() returning None plays\n";
    std::cout << "  for a dict in Python.\n";

    waitEnter();
    clearScreen();
    exercise(10);

    question("std::map<std::string, int> m; you write\n"
             "  std::cout << m[\"x\"]; and \"x\" was never inserted. Does\n"
             "  this crash, or silently insert \"x\" with value 0?",
             "silently insert",
             "operator[] on a map always succeeds by inserting a "
             "default-constructed value if the key is missing -- .count() "
             "is the way to check without inserting.");

    question("std::set<int> s; s.insert(7); s.insert(7); What is\n"
             "  s.size() after both inserts?",
             "1",
             "A set holds each value once; inserting an already-present "
             "value changes nothing.");

    question("std::find returns an iterator. What does it equal when\n"
             "  the value was not found?  (one word)",
             "end",
             "container.end() -- one past the last element, used as the "
             "\"not found\" signal the same way NULL is for a pointer.");

    challenge(
        {"Read words until \"stop\". Using a std::set<std::string>,",
         "print how many DISTINCT words were entered (duplicates",
         "should not be counted twice)."},
        {"cat", "dog", "cat", "stop"},
        {"2"},
        {"#include <iostream>", "#include <set>", "#include <string>", "",
         "int main() {", "    std::set<std::string> words;", "    std::string w;",
         "    while (std::cin >> w && w != \"stop\") {", "        words.insert(w);",
         "    }", "    std::cout << words.size() << \"\\n\";", "    return 0;", "}"});

    waitEnter();
    clearScreen();
    heading("SUMMARY");

    std::cout << "   - std::map<K, V>: key to value, sorted by key\n";
    std::cout << "   - map's [] inserts a default value on a missing key --\n";
    std::cout << "     use .count() to check without inserting\n";
    std::cout << "   - std::set<T>: each value at most once, sorted\n";
    std::cout << "   - std::find returns end() on a container when the\n";
    std::cout << "     value is not found\n\n";

    std::cout << "  Module 11 is templates: the mechanism that lets\n";
    std::cout << "  std::vector<int> and std::vector<std::string> both\n";
    std::cout << "  exist from one piece of code, not one written per type.\n";
    waitEnter();
}

namespace lesson11detail {

template<typename T>
T myMax(T a, T b) {
    return (a > b) ? a : b;
}

template<typename T>
class Box {
public:
    explicit Box(T value) : value_(value) {}
    T get() const { return value_; }

private:
    T value_;
};

}  // namespace lesson11detail

void lesson11Templates() {
    title("MODULE 11 - TEMPLATES");

    heading("PART 1: the duplication templates solve");

    std::cout << "  A function to find the larger of two ints:\n\n";
    std::cout << "    int myMax(int a, int b) { return (a > b) ? a : b; }\n\n";

    auto myMaxInt = [](int a, int b) { return (a > b) ? a : b; };
    std::cout << "  Running: myMax(3, 7) -> " << myMaxInt(3, 7) << "\n\n";

    std::cout << "  Now the same thing is needed for double -- module 7's\n";
    std::cout << "  overloading lets both exist, but the body has to be\n";
    std::cout << "  retyped, word for word, with only the type name changed:\n\n";
    std::cout << "    double myMax(double a, double b) { return (a > b) ? a : b; }\n\n";

    auto myMaxDouble = [](double a, double b) { return (a > b) ? a : b; };
    std::cout << "  Running: myMax(2.5, 8.1) -> " << myMaxDouble(2.5, 8.1) << "\n\n";

    std::cout << "  Add float next, or std::string, and the count of\n";
    std::cout << "  identical bodies keeps growing. Every one of them has\n";
    std::cout << "  to be found and fixed together if the logic ever\n";
    std::cout << "  changes -- exactly the job a compiler, not a person,\n";
    std::cout << "  should be doing.\n";

    waitEnter();
    clearScreen();
    heading("PART 2: one function template, any type");

    std::cout << "    template<typename T>\n";
    std::cout << "    T myMax(T a, T b) { return (a > b) ? a : b; }\n\n";

    std::cout << "  typename T declares T as a placeholder for whatever\n";
    std::cout << "  type the call site uses. One definition now covers\n";
    std::cout << "  every type that supports >:\n\n";

    std::cout << "    myMax(3, 7)     -> " << lesson11detail::myMax(3, 7) << "\n";
    std::cout << "    myMax(2.5, 8.1) -> " << lesson11detail::myMax(2.5, 8.1) << "\n\n";

    std::cout << "  Neither call wrote myMax<int> or myMax<double>. The\n";
    std::cout << "  compiler looked at the arguments' types and worked out\n";
    std::cout << "  T on its own -- this is template argument deduction,\n";
    std::cout << "  and it is why calling a template usually looks exactly\n";
    std::cout << "  like calling an ordinary function.\n";

    waitEnter();
    clearScreen();
    heading("PART 3: a class template");

    std::cout << "  Module 9's Student held one fixed pair of types. A\n";
    std::cout << "  container that needs to hold an int in one place and a\n";
    std::cout << "  double somewhere else needs the same parameterizing a\n";
    std::cout << "  function template gets:\n\n";

    std::cout << "    template<typename T>\n";
    std::cout << "    class Box {\n";
    std::cout << "    public:\n";
    std::cout << "        explicit Box(T value) : value_(value) {}\n";
    std::cout << "        T get() const { return value_; }\n";
    std::cout << "    private:\n";
    std::cout << "        T value_;\n";
    std::cout << "    };\n\n";

    lesson11detail::Box<int> intBox(42);
    lesson11detail::Box<double> doubleBox(3.14);

    std::cout << "  Running:\n\n";
    std::cout << "    Box<int> intBox(42);         intBox.get()    -> "
              << intBox.get() << "\n";
    std::cout << "    Box<double> doubleBox(3.14); doubleBox.get() -> "
              << doubleBox.get() << "\n\n";

    std::cout << "  Box<int> and Box<double> are two separate classes, each\n";
    std::cout << "  generated by the compiler from the same template at\n";
    std::cout << "  compile time -- there is no single Box that stores\n";
    std::cout << "  \"any type\" at run time the way a void* would; T is\n";
    std::cout << "  resolved before the program ever runs, and every use\n";
    std::cout << "  of Box<int> shares that one generated class.\n";

    waitEnter();
    clearScreen();
    heading("PART 4: a real compile error");

    std::cout << "  A template only checks that a type can do what its\n";
    std::cout << "  body demands at the moment it is actually instantiated\n";
    std::cout << "  with that type -- not before:\n\n";

    std::cout << "    template<typename T>\n";
    std::cout << "    T sum(T a, T b) { return a + b; }\n\n";
    std::cout << "    struct Point { int x, y; };   // no operator+\n";
    std::cout << "    sum(Point{1, 2}, Point{3, 4});\n\n";

    std::cout << "  sum<int> works because int has +. Point does not\n";
    std::cout << "  define one. Writing exactly that to a real file and\n";
    std::cout << "  compiling it for real, right now:\n\n";

    const std::string tmpDir = std::filesystem::temp_directory_path().string();
    const std::string srcPath = tmpDir + "/codelearner_m11_test.cpp";
    const std::string binPath = tmpDir + "/codelearner_m11_test";

    bool wroteSource = false;
    {
        std::ofstream out(srcPath);
        if (out) {
            out << "template<typename T>\n"
                   "T sum(T a, T b) { return a + b; }\n\n"
                   "struct Point { int x, y; };\n\n"
                   "int main() {\n"
                   "    Point p = sum(Point{1, 2}, Point{3, 4});\n"
                   "    (void)p;\n"
                   "    return 0;\n"
                   "}\n";
            wroteSource = static_cast<bool>(out);
        }
    }

    std::cout << "    $ g++ -std=c++20 -Wall -Wextra codelearner_m11_test.cpp "
                 "-o codelearner_m11_test\n\n";
    std::cout.flush();

    // Compile only -- this instantiation is EXPECTED to fail, and a compile
    // error is the compiler's normal, defined response to a bad program, not
    // undefined behaviour. The broken program is never executed.
    int rawStatus = -1;
    if (wroteSource) {
        const std::string compileCmd = "g++ -std=c++20 -Wall -Wextra " +
                                        srcPath + " -o " + binPath + " 2>&1";
        rawStatus = std::system(compileCmd.c_str());
    }

    std::remove(srcPath.c_str());
    std::remove(binPath.c_str());

    // g++ actually running and rejecting Point's missing operator+ exits 1.
    // Anything else -- no source written, g++ missing from PATH (the shell's
    // "command not found" exits 127), a crash, a signal -- is not that error,
    // and must not be narrated as if it were.
#ifdef _WIN32
    const int exitStatus = wroteSource ? rawStatus : -1;
    const bool compilerRanAndFailedAsExpected =
        wroteSource && exitStatus == 1;
#else
    const bool exited = wroteSource && WIFEXITED(rawStatus);
    const int exitStatus = exited ? WEXITSTATUS(rawStatus) : -1;
    const bool compilerRanAndFailedAsExpected = exited && exitStatus == 1;
#endif

    if (compilerRanAndFailedAsExpected) {
        std::cout << "\n  That is g++'s real error text, produced by this\n";
        std::cout << "  compiler right now, not a summary written by hand. It\n";
        std::cout << "  generated sum<Point> the moment it saw the call, tried\n";
        std::cout << "  to compile the body against Point's actual members,\n";
        std::cout << "  and failed there -- at compile time, before the\n";
        std::cout << "  program ever ran.\n";
    } else if (!wroteSource) {
        std::cout << "\n  Couldn't write the test source file to " << tmpDir
                  << " to compile it,\n";
        std::cout << "  so there is no real compiler output to show here.\n";
        std::cout << "  Skipping ahead.\n";
    } else {
        std::cout << "\n  Couldn't get a real compile error to show (the\n";
        std::cout << "  command exited with status " << exitStatus
                  << ", not a normal\n";
        std::cout << "  compiler failure) -- a C++ compiler is expected on\n";
        std::cout << "  PATH for this part. Skipping ahead.\n";
    }

    waitEnter();
    clearScreen();
    exercise(11);

    question("template<typename T> T myMax(T a, T b) -- is this compiled\n"
             "  once and reused for every type, or separately for each\n"
             "  type actually used?",
             "separately",
             "Each type used with a template gets its own compiled "
             "version, generated at compile time -- Box<int> and "
             "Box<double> are two different classes, not one flexible "
             "one.");

    question("template<typename T> T myMax(T a, T b);  Calling myMax(3, 7)\n"
             "  -- do you have to write myMax<int>(3, 7) instead?\n"
             "  (answer: yes or no)",
             "no",
             "Template argument deduction reads T from the arguments' own "
             "types. Writing myMax<int>(...) explicitly still works, but "
             "is rarely needed.");

    question("A template is instantiated with a type missing an operation\n"
             "  its body needs, like Point with sum's +. Is that caught\n"
             "  at compile time or run time?",
             "compile time",
             "The compiler tries to generate that specific version and "
             "fails right there -- the program never gets to run with the "
             "bad instantiation.");

    challenge(
        {"Write a template function mySwap(T &a, T &b) that swaps two",
         "values of any type, using references the way module 7's swap",
         "did but written once for every type. Test it by swapping two",
         "ints and printing them."},
        {},
        {"2 1"},
        {"#include <iostream>", "", "template<typename T>",
         "void mySwap(T &a, T &b)", "{", "    T held = a;", "    a = b;",
         "    b = held;", "}", "", "int main()", "{",
         "    int x = 1, y = 2;", "    mySwap(x, y);",
         "    std::cout << x << \" \" << y << \"\\n\";", "    return 0;",
         "}"});

    waitEnter();
    clearScreen();
    heading("SUMMARY");

    std::cout << "   - a template is one definition, compiled separately per type used\n";
    std::cout << "   - template<typename T> works for both functions and classes\n";
    std::cout << "   - template argument deduction reads T from the arguments, no <T> needed\n";
    std::cout << "   - a type missing an operation a template needs fails at compile time\n\n";

    std::cout << "  std::vector<T>, std::map<K, V> and std::string were all\n";
    std::cout << "  templates from module 5 onward -- this is the mechanism\n";
    std::cout << "  that was working underneath the whole time.\n\n";

    std::cout << "  Module 12 is the last one: the idioms that make the\n";
    std::cout << "  rest of modern C++ look the way it does, starting with\n";
    std::cout << "  module 9's own new/delete leak, fixed for good.\n";
    waitEnter();
}

namespace lesson12detail {

// Wraps one std::string just to print when it is actually alive, so a
// unique_ptr/shared_ptr demo can show a real destructor firing rather than
// asserting that it did.
class Resource {
public:
    explicit Resource(std::string name) : name_(std::move(name)) {
        std::cout << "    acquired: " << name_ << "\n";
    }
    ~Resource() {
        std::cout << "    released: " << name_ << "\n";
    }

private:
    std::string name_;
};

// Counts real constructor/copy/move calls -- module 12's PART 4 prints
// these counters after running real code, rather than asserting what a
// copy or a move "should" do.
class Tracked {
public:
    Tracked() {
        ++constructed;
    }
    Tracked(const Tracked &) {
        ++copied;
    }
    Tracked(Tracked &&) noexcept {
        ++moved;
    }

    static int constructed;
    static int copied;
    static int moved;
};

int Tracked::constructed = 0;
int Tracked::copied = 0;
int Tracked::moved = 0;

// Structurally identical to Tracked, with its own counters -- used only for
// the noexcept-vs-not comparison, so that demo starts from zero instead of
// whatever Tracked's counters already hold from PART 4's first demo.
class TrackedNoexceptMove {
public:
    TrackedNoexceptMove() {
        ++constructed;
    }
    TrackedNoexceptMove(const TrackedNoexceptMove &) {
        ++copied;
    }
    TrackedNoexceptMove(TrackedNoexceptMove &&) noexcept {
        ++moved;
    }

    static int constructed;
    static int copied;
    static int moved;
};

int TrackedNoexceptMove::constructed = 0;
int TrackedNoexceptMove::copied = 0;
int TrackedNoexceptMove::moved = 0;

// Same idea, but the move constructor is NOT noexcept -- used once, to show
// the one place that omission actually changes what a real run does.
class TrackedThrowingMove {
public:
    TrackedThrowingMove() {
        ++constructed;
    }
    TrackedThrowingMove(const TrackedThrowingMove &) {
        ++copied;
    }
    TrackedThrowingMove(TrackedThrowingMove &&) {
        ++moved;
    }

    static int constructed;
    static int copied;
    static int moved;
};

int TrackedThrowingMove::constructed = 0;
int TrackedThrowingMove::copied = 0;
int TrackedThrowingMove::moved = 0;

}  // namespace lesson12detail

void lesson12Modern() {
    title("MODULE 12 - MODERN C++ IDIOMS");

    heading("PART 1: the leak module 9 already showed you");

    std::cout << "  Module 9 put new/delete underneath std::vector and\n";
    std::cout << "  std::string, and showed the failure mode when a new\n";
    std::cout << "  has no matching delete:\n\n";

    std::cout << "    int *v = new int[5];\n";
    std::cout << "    // ... no delete[] v ...\n\n";

    std::cout << "  That is the same real LeakSanitizer output from module\n";
    std::cout << "  9, quoted again rather than re-run -- running it again\n";
    std::cout << "  would leak inside this very process, the one this\n";
    std::cout << "  project's own verification checks for a clean run:\n\n";

    std::cout << "    ==12345==ERROR: LeakSanitizer: detected memory leaks\n";
    std::cout << "    Direct leak of 20 byte(s) in 1 object(s) allocated from:\n\n";

    std::cout << "  Forgetting one delete[] is the entire bug. The rest of\n";
    std::cout << "  this module is the standard library's answer: wrap the\n";
    std::cout << "  pointer in an object whose destructor calls delete for\n";
    std::cout << "  you, so forgetting it stops being possible.\n";

    waitEnter();
    clearScreen();
    heading("PART 2: std::unique_ptr -- ownership that cleans up itself");

    std::cout << "    class Resource {\n";
    std::cout << "    public:\n";
    std::cout << "        explicit Resource(std::string name) : name_(std::move(name)) {\n";
    std::cout << "            std::cout << \"acquired: \" << name_ << \"\\n\";\n";
    std::cout << "        }\n";
    std::cout << "        ~Resource() { std::cout << \"released: \" << name_ << \"\\n\"; }\n";
    std::cout << "    private:\n";
    std::cout << "        std::string name_;\n";
    std::cout << "    };\n\n";

    std::cout << "    std::unique_ptr<Resource> r =\n";
    std::cout << "        std::make_unique<Resource>(\"file handle\");\n\n";

    std::cout << "  Running for real:\n\n";
    std::cout << "    entering scope\n";
    {
        std::unique_ptr<lesson12detail::Resource> r =
            std::make_unique<lesson12detail::Resource>("file handle");
    }
    std::cout << "    left scope\n\n";

    std::cout << "  The destructor fired at the closing brace -- no delete\n";
    std::cout << "  written anywhere. make_unique<T>(args...) constructs a\n";
    std::cout << "  T and hands back a unique_ptr owning it; that unique_ptr\n";
    std::cout << "  is the only owner allowed to exist. Copying one is a\n";
    std::cout << "  compile error, not a runtime risk -- the header itself\n";
    std::cout << "  says so, in the real declaration g++ points at:\n\n";

    std::cout << "    unique_ptr(const unique_ptr&) = delete;\n\n";

    std::cout << "  Verified on this compiler: assigning one unique_ptr from\n";
    std::cout << "  another fails right there, before the program can ever\n";
    std::cout << "  run with two owners of the same pointer.\n";

    waitEnter();
    clearScreen();
    heading("PART 3: std::shared_ptr -- ownership that is counted");

    std::cout << "  Sometimes more than one part of a program legitimately\n";
    std::cout << "  needs to keep the same object alive together. That is\n";
    std::cout << "  what std::shared_ptr is for: it counts how many\n";
    std::cout << "  shared_ptrs point at the same object right now, and only\n";
    std::cout << "  destroys it once that count reaches zero.\n\n";

    std::cout << "    std::shared_ptr<Resource> a =\n";
    std::cout << "        std::make_shared<Resource>(\"shared file\");\n";
    std::cout << "    std::shared_ptr<Resource> b = a;   // legal -- copies allowed\n\n";

    {
        std::shared_ptr<lesson12detail::Resource> a =
            std::make_shared<lesson12detail::Resource>("shared file");
        std::cout << "  Running:\n\n";
        std::cout << "    use_count() right after creation:      " << a.use_count() << "\n";
        {
            std::shared_ptr<lesson12detail::Resource> b = a;
            std::cout << "    use_count() after b = a (a copy):      " << a.use_count() << "\n";
        }
        std::cout << "    use_count() after b went out of scope: " << a.use_count() << "\n";
    }

    std::cout << "\n  The count rose to 2 while both a and b were alive, and\n";
    std::cout << "  fell back to 1 the instant b's scope ended -- the object\n";
    std::cout << "  itself was released only once a's own scope ended too,\n";
    std::cout << "  the \"released\" line printed just above.\n\n";

    std::cout << "  unique_ptr's copy constructor does not exist at all;\n";
    std::cout << "  shared_ptr's does, and it is real work on every copy and\n";
    std::cout << "  every destruction, not a free operation. Default to\n";
    std::cout << "  unique_ptr; reach for shared_ptr only once more than one\n";
    std::cout << "  genuine owner is a real requirement, not a convenience.\n";

    waitEnter();
    clearScreen();
    heading("PART 4: move semantics -- an observed copy, and an observed move");

    std::cout << "  A class that counts its own constructor calls turns\n";
    std::cout << "  \"a move avoids a copy\" from a claim into something a\n";
    std::cout << "  program actually counts:\n\n";

    std::cout << "    class Tracked {\n";
    std::cout << "    public:\n";
    std::cout << "        Tracked() { ++constructed; }\n";
    std::cout << "        Tracked(const Tracked &) { ++copied; }\n";
    std::cout << "        Tracked(Tracked &&) noexcept { ++moved; }\n";
    std::cout << "        static int constructed, copied, moved;\n";
    std::cout << "    };\n\n";

    std::cout << "    std::vector<Tracked> v;\n";
    std::cout << "    v.reserve(2);           // no reallocation below, on purpose\n";
    std::cout << "    Tracked x, y;\n";
    std::cout << "    v.push_back(x);              // x is an lvalue -- copies\n";
    std::cout << "    v.push_back(std::move(y));   // y is cast to an rvalue -- moves\n\n";

    // Reset the counters before this demo -- module 12 can be revisited in
    // the same run, and the narration below is hardcoded to the numbers
    // this fresh run produces, not whatever earlier visits left behind.
    lesson12detail::Tracked::constructed = 0;
    lesson12detail::Tracked::copied = 0;
    lesson12detail::Tracked::moved = 0;

    {
        std::vector<lesson12detail::Tracked> v;
        v.reserve(2);
        lesson12detail::Tracked x, y;
        v.push_back(x);
        v.push_back(std::move(y));

        std::cout << "  Running for real:\n\n";
        std::cout << "    constructed = " << lesson12detail::Tracked::constructed << "\n";
        std::cout << "    copied      = " << lesson12detail::Tracked::copied << "\n";
        std::cout << "    moved       = " << lesson12detail::Tracked::moved << "\n\n";
    }

    std::cout << "  Two real objects, x and y (constructed = 2). push_back(x)\n";
    std::cout << "  had to leave x usable afterward, so it copied (copied =\n";
    std::cout << "  1). std::move(y) does not move anything by itself -- it\n";
    std::cout << "  only casts y to an rvalue reference, telling push_back it\n";
    std::cout << "  is allowed to steal from y instead of copying it, which\n";
    std::cout << "  is exactly what the move constructor being called once\n";
    std::cout << "  (moved = 1) shows actually happened.\n\n";

    std::cout << "  y itself still exists after being moved from -- it is\n";
    std::cout << "  not destroyed, and touching it afterward is not\n";
    std::cout << "  undefined behaviour. It is left in a valid but\n";
    std::cout << "  unspecified state: safe to assign a new value to or let\n";
    std::cout << "  go out of scope, not safe to assume still holds what it\n";
    std::cout << "  held before.\n";

    waitEnter();
    clearScreen();
    heading("PART 4, continued: why the move constructor says noexcept");

    std::cout << "  reserve(2) above hid a real trap: std::vector must give a\n";
    std::cout << "  strong exception guarantee when it reallocates -- if\n";
    std::cout << "  moving an element partway through could throw and leave\n";
    std::cout << "  the vector corrupted, the standard library is not\n";
    std::cout << "  allowed to risk it, and falls back to copying instead.\n";
    std::cout << "  Two structurally identical classes, one with a\n";
    std::cout << "  noexcept move constructor and one without, each growing\n";
    std::cout << "  past its starting capacity for real:\n\n";

    // Same reason as the reset above: this comparison can run more than
    // once per session, and the numbers below are hardcoded to a fresh run.
    lesson12detail::TrackedNoexceptMove::constructed = 0;
    lesson12detail::TrackedNoexceptMove::copied = 0;
    lesson12detail::TrackedNoexceptMove::moved = 0;
    lesson12detail::TrackedThrowingMove::constructed = 0;
    lesson12detail::TrackedThrowingMove::copied = 0;
    lesson12detail::TrackedThrowingMove::moved = 0;

    {
        std::vector<lesson12detail::TrackedNoexceptMove> fast;
        for (int i = 0; i < 5; i++) {
            fast.push_back(lesson12detail::TrackedNoexceptMove{});
        }
        std::vector<lesson12detail::TrackedThrowingMove> slow;
        for (int i = 0; i < 5; i++) {
            slow.push_back(lesson12detail::TrackedThrowingMove{});
        }

        std::cout << "  Running for real, 5 push_backs into an empty vector,\n";
        std::cout << "  each one a temporary (so every copied count below\n";
        std::cout << "  comes only from vector reallocation, not the push_back\n";
        std::cout << "  call itself):\n\n";
        std::cout << "    move constructor is noexcept:     copied = "
                  << lesson12detail::TrackedNoexceptMove::copied << ", moved = "
                  << lesson12detail::TrackedNoexceptMove::moved << "\n";
        std::cout << "    move constructor is NOT noexcept: copied = "
                  << lesson12detail::TrackedThrowingMove::copied << ", moved = "
                  << lesson12detail::TrackedThrowingMove::moved << "\n\n";
    }

    std::cout << "  Same five elements, same reallocations -- the only\n";
    std::cout << "  difference is one noexcept, and it is the difference\n";
    std::cout << "  between reallocation moving elements and reallocation\n";
    std::cout << "  copying them instead. A move constructor that can throw\n";
    std::cout << "  is a move constructor vector will often quietly decline\n";
    std::cout << "  to use.\n";

    waitEnter();
    clearScreen();
    heading("PART 5: structured bindings -- unpacking a pair or map entry");

    std::cout << "  Module 10's map loop pulled a key and a value out of\n";
    std::cout << "  each entry through .first and .second:\n\n";

    std::cout << "    for (const auto &entry : ages) {\n";
    std::cout << "        std::cout << entry.first << \": \" << entry.second << \"\\n\";\n";
    std::cout << "    }\n\n";

    std::map<std::string, int> ages;
    ages["Ana"] = 30;
    ages["Bea"] = 25;

    std::cout << "  Running that exact loop again, unchanged:\n\n";
    for (const auto &entry : ages) {
        std::cout << "    " << entry.first << ": " << entry.second << "\n";
    }

    std::cout << "\n  A structured binding names both pieces directly instead\n";
    std::cout << "  of reaching through .first and .second:\n\n";

    std::cout << "    for (const auto &[name, age] : ages) {\n";
    std::cout << "        std::cout << name << \": \" << age << \"\\n\";\n";
    std::cout << "    }\n\n";

    std::cout << "  Running -- identical output, from code that says what\n";
    std::cout << "  each piece actually is:\n\n";
    for (const auto &[name, age] : ages) {
        std::cout << "    " << name << ": " << age << "\n";
    }

    std::cout << "\n  This is not module 2's auto again -- auto there deduced\n";
    std::cout << "  one type for one name. A structured binding deduces one\n";
    std::cout << "  type, a pair here, and splits it into several names at\n";
    std::cout << "  once. It works on any std::pair the same way:\n\n";

    std::cout << "    auto [first, second] = std::pair{1, 2};\n\n";

    auto [first, second] = std::pair{1, 2};
    std::cout << "  Running: first = " << first << ", second = " << second << "\n";

    waitEnter();
    clearScreen();
    exercise(12);

    question("A std::unique_ptr<T> goes out of scope. Does its target get\n"
             "  destroyed automatically, with no delete written anywhere?\n"
             "  (answer: yes or no)",
             "yes",
             "That is the whole idiom: the destructor runs at scope end, "
             "the same RAII rule module 9 already taught, applied to a "
             "raw pointer someone else wrapped for you.");

    question("In well-designed C++, which is reached for by default:\n"
             "  unique_ptr or shared_ptr?",
             "unique_ptr",
             "shared_ptr's reference counting is real work on every copy "
             "and destruction. unique_ptr is free of that cost and is the "
             "default; shared_ptr is for when more than one genuine owner "
             "is required, not a convenience.");

    question("Tracked b = std::move(a);  Can code afterward still refer to\n"
             "  a, and is doing so undefined behaviour?  (answer: yes or no,\n"
             "  for \"can still refer to it\")",
             "yes",
             "a still exists in a valid but unspecified state -- safe to "
             "assign to or let go out of scope, not safe to assume it "
             "still holds its old value.");

    challenge(
        {"Write a class Box with a private std::string name, a",
         "constructor Box(std::string name) that prints \"created: \"",
         "followed by the name, and a destructor that prints",
         "\"destroyed: \" followed by the name. Write a function",
         "makeBox(std::string name) returning std::unique_ptr<Box>,",
         "built with std::make_unique. In main, call makeBox(\"gift\"),",
         "store the result, print \"holding it\", then let main end."},
        {},
        {"created: gift", "holding it", "destroyed: gift"},
        {"#include <iostream>", "#include <memory>", "#include <string>", "",
         "class Box {", "public:",
         "    explicit Box(std::string name) : name_(std::move(name)) {",
         "        std::cout << \"created: \" << name_ << \"\\n\";", "    }",
         "    ~Box() { std::cout << \"destroyed: \" << name_ << \"\\n\"; }",
         "private:", "    std::string name_;", "};", "",
         "std::unique_ptr<Box> makeBox(std::string name)", "{",
         "    return std::make_unique<Box>(std::move(name));", "}", "",
         "int main()", "{", "    std::unique_ptr<Box> b = makeBox(\"gift\");",
         "    std::cout << \"holding it\\n\";", "    return 0;", "}"});

    waitEnter();
    clearScreen();
    heading("SUMMARY");

    std::cout << "   - std::unique_ptr<T>: single ownership, destroyed automatically,\n";
    std::cout << "     copying it does not compile\n";
    std::cout << "   - std::shared_ptr<T>: shared ownership, counted with use_count(),\n";
    std::cout << "     costs real work per copy -- default to unique_ptr instead\n";
    std::cout << "   - std::move casts to an rvalue reference; it does not move\n";
    std::cout << "     anything by itself, it only permits the move constructor to run\n";
    std::cout << "   - a moved-from object is valid but unspecified, not destroyed\n";
    std::cout << "     and not undefined to touch\n";
    std::cout << "   - auto [a, b] = pair; unpacks a pair/map entry into named pieces,\n";
    std::cout << "     instead of .first and .second\n\n";

    std::cout << "  That is the whole course: from a compiler that only knew\n";
    std::cout << "  printf, to memory that manages itself and ownership the\n";
    std::cout << "  type system can enforce.\n";
    waitEnter();
}
