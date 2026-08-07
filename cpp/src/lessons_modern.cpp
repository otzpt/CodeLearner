// Modules 6 to 9 - the part that sets C++ apart.
//
// Each module closes a gap the C course spent real time on: strings without
// \0, functions that write back without &, arrays that know their own size,
// and finally memory that frees itself at the right time. None of it is
// magic -- each one is shown working, and shown still breakable.

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

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

    std::cout << "  That is the core course. std::string, std::vector and\n";
    std::cout << "  your own classes cover the overwhelming majority of what\n";
    std::cout << "  C needed manual memory management for.\n";
    waitEnter();
}
