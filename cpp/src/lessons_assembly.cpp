// Module 13 - inline assembly and NASM.
//
// Everything here runs for real on an x86-64 Linux PC: the asm blocks are
// compiled into the course itself, and the NASM example is the real file
// cpp/asm/add.asm, assembled and linked by `make asm-demo`, whose output this
// module runs and prints once it has been built. On any other CPU the asm
// demonstrations are skipped and the module says so.
//
// Compiler messages quoted here were copied from running g++ and the linker
// on the lines shown.

#include <bit>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "lessons.h"
#include "ui.h"

namespace {

void say(const std::string &text) {
    std::cout << text << "\n";
}

#ifdef __linux__
// The path of a file that sits next to the running program.
std::filesystem::path besideProgram(const std::string &relative) {
    return std::filesystem::read_symlink("/proc/self/exe").parent_path() / relative;
}

void showNasmSource() {
    std::ifstream file(besideProgram("asm/add.asm"));
    if (!file) {
        say("  (asm/add.asm is not next to this program: run the course from");
        say("  a source checkout to see it)");
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        if (line.rfind("; Tells the linker", 0) == 0) {
            break;
        }
        std::cout << "    " << line << "\n";
    }
}

void runNasmDemo() {
    const std::filesystem::path demo = besideProgram("asm/use_asm");
    if (!std::filesystem::exists(demo)) {
        say("  asm/use_asm has not been built, so there is no output to show.");
        say("  Run `make asm-demo` in the cpp/ folder (it needs nasm) and come");
        say("  back to this module.");
        return;
    }
    std::FILE *pipe = popen(demo.c_str(), "r");
    if (pipe == nullptr) {
        say("  asm/use_asm could not be started.");
        return;
    }
    say("  asm/use_asm is built, and this is what it printed just now:\n");
    char buffer[256];
    while (std::fgets(buffer, sizeof buffer, pipe) != nullptr) {
        std::cout << "  Running:  " << buffer;
    }
    pclose(pipe);
}
#else
void showNasmSource() {
    say("  (this part needs Linux and the files in asm/)");
}
void runNasmDemo() {
    say("  The NASM example is Linux-only (ELF objects, the System V ABI).");
}
#endif

#if defined(__x86_64__)
void demoArithmetic() {
    long doubled = 21;
    asm("add %0, %0" : "+r"(doubled));
    std::cout << "  Running:  add %0, %0     -> " << doubled << "   (21 doubled, in place)\n";

    long seven = 7;
    long timesFive;
    asm("lea (%1,%1,4), %0" : "=r"(timesFive) : "r"(seven));
    std::cout << "  Running:  lea (x,x,4)    -> " << timesFive << "   (7 times 5, no multiply)\n";

    unsigned long long wide = 255;
    unsigned long long counted;
    asm("popcnt %1, %0" : "=r"(counted) : "r"(wide));
    std::cout << "  Running:  popcnt 255     -> " << counted << "   (the 1 bits in 255)\n";
}
#else
void demoArithmetic() {
    say("  This CPU is not x86-64, so the asm examples are skipped.");
}
#endif

}  // namespace

void lesson13Assembly() {
    title("MODULE 13 - INLINE ASSEMBLY AND NASM");

    heading("PART 1: asm in C++, and the instruction you did not need");

    say("  GCC and Clang accept the same extended asm in C++ as in C. The");
    say("  full syntax, with AT&T order (source first), the r / m / i operand");
    say("  letters, early-clobber & and the clobber list, is module 20 of the");
    say("  C course, and it behaves identically here:\n");
    say("    asm(\"lea (%1,%1,4), %0\" : \"=r\"(result) : \"r\"(x));\n");
    demoArithmetic();
    say("");
    say("  Look at the last line. popcnt counts the 1 bits in a number: an");
    say("  instruction C and C++ have no operator for. Before reaching for");
    say("  asm, ask whether the language already exposes it. C++20 does:\n");
    say("    #include <bit>");
    say("    std::popcount(255u)\n");
    std::cout << "  Running:  std::popcount(255u) = " << std::popcount(255u) << "\n\n";
    say("  Compiled with -mpopcnt (or an -march that has it), that call IS the");
    say("  popcnt instruction. Compiled without, g++ -O2 emits a call to a");
    say("  portable library routine (__popcountdi2), so the same source runs");
    say("  on any CPU. The inline asm has no such fallback: it faults on a CPU");
    say("  without popcnt. Read <bit> and the compiler's intrinsics");
    say("  (__builtin_* and <immintrin.h>) before writing any asm.");

    waitEnter();
    clearScreen();
    heading("PART 2: what C++ adds to the picture");

    say("  An asm statement is opaque, so C++ machinery that needs to look");
    say("  inside a function cannot use one. constexpr is the example:\n");
    say("    constexpr long twice(long x) {");
    say("        asm(\"add %0, %0\" : \"+r\"(x));");
    say("        return x;");
    say("    }\n");
    say("    warning: inline assembly is not a constant expression");
    say("    [-Winvalid-constexpr]");
    say("    note: only unevaluated inline assembly is allowed in a");
    say("    'constexpr' function in C++20\n");
    say("  A function containing asm cannot be evaluated at compile time. If");
    say("  the function must be constexpr, the asm cannot be in the path the");
    say("  compiler evaluates; the usual answer is the standard-library or");
    say("  builtin version from Part 1.\n");
    say("  In a template, the asm is copied into every instantiation, so a");
    say("  large block in a template is code size multiplied. Put it in a");
    say("  small non-template function the template calls.\n");
    say("  And RAII, module 9, does not reach inside an asm block: if the asm");
    say("  changes a register or memory the compiler is tracking, the clobber");
    say("  list has to say so, because no destructor is going to notice.");

    waitEnter();
    clearScreen();
    heading("PART 3: NASM, and extern \"C\"");

    say("  For more than a few instructions, write a separate .asm file,");
    say("  assemble it with NASM and link it. NASM uses Intel syntax:");
    say("  DESTINATION first, no % or $, memory in [brackets]. The file below");
    say("  is the real asm/add.asm of this course:\n");
    showNasmSource();
    say("");
    say("  The registers follow the System V AMD64 ABI, the convention Linux");
    say("  uses: integer arguments in rdi, rsi, rdx, rcx, r8, r9; the result");
    say("  in rax.\n");
    say("  C++ has a trap the C course does not. Declare the function the");
    say("  obvious way:\n");
    say("    std::uint64_t add_numbers(std::uint64_t a, std::uint64_t b);\n");
    say("  and the link fails:\n");
    say("    undefined reference to `add_numbers(unsigned long, unsigned long)'\n");
    say("  C++ encodes the parameter types into every function name (name");
    say("  mangling) so overloads can coexist, and looks for that mangled");
    say("  name. The assembly defines the plain label add_numbers. The");
    say("  declaration must say it follows C's rules:\n");
    say("    extern \"C\" {");
    say("    std::uint64_t add_numbers(std::uint64_t a, std::uint64_t b);");
    say("    }\n");
    say("  The build is two commands (or `make asm-demo`):\n");
    say("    nasm -f elf64 asm/add.asm -o asm/add.o");
    say("    g++ -std=c++20 asm/use_asm.cpp asm/add.o -o asm/use_asm\n");
    runNasmDemo();

    waitEnter();
    clearScreen();
    exercise(13);

    question("Without extern \"C\", C++ looks for a mangled name that encodes\n"
             "  the parameter types. true or false?",
             "true",
             "Mangling lets overloads coexist; the assembly's plain label never "
             "matches it.");

    question("Which header provides std::popcount in C++20?",
             "bit",
             "<bit> holds popcount and the other bit operations, which compile "
             "to the instruction where one exists.");

    challenge(
        {"Print std::popcount(255u) on one line, then the same count computed",
         "with an inline asm popcnt instruction on the next:",
         "asm(\"popcnt %1, %0\" : \"=r\"(result) : \"r\"(value));",
         "Use unsigned long long for the asm operands."},
        {},
        {"8", "8"},
        {"#include <bit>", "#include <iostream>", "",
         "int main()", "{", "    unsigned x = 255;",
         "    unsigned long long wide = x;",
         "    unsigned long long counted;",
         "    asm(\"popcnt %1, %0\" : \"=r\"(counted) : \"r\"(wide));",
         "    std::cout << std::popcount(x) << \"\\n\" << counted << \"\\n\";",
         "    return 0;", "}"});

    waitEnter();
    clearScreen();
    heading("SUMMARY");

    std::cout << "   - extended asm is identical to C's; look for std:: or a builtin first\n";
    std::cout << "   - an asm statement cannot be evaluated in a constexpr function\n";
    std::cout << "   - NASM functions need extern \"C\" or the linker looks for a mangled name\n";
    std::cout << "   - the System V ABI: arguments in rdi, rsi, rdx, rcx, r8, r9; result in rax\n\n";
    std::cout << "  That is the whole course, down to the instruction.\n";
    waitEnter();
}
