/*
 * Module 20 - inline assembly and NASM.
 *
 * Everything here runs for real on an x86-64 Linux PC: the __asm__ blocks
 * are compiled into the course itself, and the NASM example is the real file
 * c/asm/add.asm, assembled and linked by `make asm-demo`, whose output this
 * module runs and prints when it has been built. On any other CPU the
 * __asm__ demonstrations are skipped and the module says so.
 *
 * Printed with puts() rather than printf() because AT&T assembly is full of
 * % signs, which printf would take for format specifiers.
 */

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __linux__
#include <unistd.h>
#endif

#include "lessons.h"
#include "ui.h"

#ifdef __linux__
/* The path of a file that sits next to the running program, such as
 * "asm/add.asm". Returns 1 on success. */
static int beside_program(const char *relative, char *out, size_t size)
{
    char exe[4096];
    ssize_t length = readlink("/proc/self/exe", exe, sizeof exe - 1);
    if (length <= 0) {
        return 0;
    }
    exe[length] = '\0';

    char *slash = strrchr(exe, '/');
    if (slash == NULL) {
        return 0;
    }
    *slash = '\0';

    return snprintf(out, size, "%s/%s", exe, relative) < (int) size;
}

/* Print asm/add.asm as it is on disk, up to its linker note. */
static void show_nasm_source(void)
{
    char path[4200];
    char line[256];

    if (!beside_program("asm/add.asm", path, sizeof path)) {
        puts("  (could not locate asm/add.asm)");
        return;
    }
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        puts("  (asm/add.asm is not next to this program: run the course from");
        puts("  a source checkout to see it)");
        return;
    }
    while (fgets(line, sizeof line, file) != NULL) {
        if (strncmp(line, "; Tells the linker", 18) == 0) {
            break;
        }
        line[strcspn(line, "\n")] = '\0';
        printf("    %s\n", line);
    }
    fclose(file);
}

/* Run asm/use_asm, if `make asm-demo` has built it, and print its output. */
static void run_nasm_demo(void)
{
    char path[4200];
    char line[256];

    if (!beside_program("asm/use_asm", path, sizeof path) || access(path, X_OK) != 0) {
        puts("  asm/use_asm has not been built, so there is no output to show.");
        puts("  Run `make asm-demo` in the c/ folder (it needs nasm) and come");
        puts("  back to this module.");
        return;
    }
    FILE *pipe = popen(path, "r");
    if (pipe == NULL) {
        puts("  asm/use_asm could not be started.");
        return;
    }
    puts("  asm/use_asm is built, and this is what it printed just now:\n");
    while (fgets(line, sizeof line, pipe) != NULL) {
        printf("  Running:  %s", line);
    }
    pclose(pipe);
}
#else
static void show_nasm_source(void)
{
    puts("  (this part needs Linux and the files in asm/)");
}
static void run_nasm_demo(void)
{
    puts("  The NASM example is Linux-only (ELF objects, the System V ABI).");
}
#endif

#if defined(__x86_64__)
static void demo_arithmetic(void)
{
    long first = 40;
    long second = 2;
    long sum;

    /* "=&r": the output is written (by mov) before the inputs are read (by
     * add), so it must not share a register with them. */
    __asm__("mov %1, %0\n\t"
            "add %2, %0"
            : "=&r"(sum)
            : "r"(first), "r"(second));
    printf("  Running:  mov, add            -> %ld\n", sum);

    long doubled = 21;
    __asm__("add %0, %0" : "+r"(doubled));
    printf("  Running:  add %%0, %%0          -> %ld   (21 doubled, in place)\n", doubled);

    long seven = 7;
    long times_five;
    __asm__("lea (%1,%1,4), %0" : "=r"(times_five) : "r"(seven));
    printf("  Running:  lea (x,x,4)         -> %ld   (7 times 5, no multiply)\n", times_five);

    unsigned char data[3] = {10, 20, 30};
    long second_byte;
    __asm__("movzbl %1, %k0" : "=r"(second_byte) : "m"(data[1]));
    printf("  Running:  movzbl from data[1] -> %ld\n", second_byte);
}
#else
static void demo_arithmetic(void)
{
    puts("  This CPU is not x86-64, so the __asm__ examples are skipped.");
}
#endif

void lesson_20_assembly(void)
{
    title("MODULE 20 - INLINE ASSEMBLY AND NASM");

    heading("PART 1: __asm__, the CPU's own instructions inside C");

    puts("  Assembly is the CPU's instruction set. GCC and Clang let you put");
    puts("  it in the middle of a C function with an extended asm statement.");
    puts("  Honest reasons: an instruction C has no word for, exact control");
    puts("  of what runs, or learning what the compiler produces. It is not");
    puts("  part of the C standard, so it is __asm__ with the underscores,");
    puts("  and it does not compile on a compiler that lacks it.\n");
    puts("    __asm__(\"template\"");
    puts("            : outputs        // \"=r\"(variable)   written");
    puts("            : inputs         // \"r\"(variable)    read");
    puts("            : clobbers);     // what it changes behind the compiler\n");
    puts("  %0, %1, ... in the template are the operands in order. The letter");
    puts("  says what the operand may be: r = any register, m = a memory");
    puts("  location, i = a constant. = means the operand is written, + means");
    puts("  read AND written.\n");
    puts("  The syntax is AT&T by default: SOURCE first, registers written");
    puts("  with a %, constants with a $. So  add %2, %0  means  %0 += %2.\n");

    demo_arithmetic();

    puts("\n  The & in \"=&r\" matters. mov writes %0 before add reads %2; without");
    puts("  the & the compiler may give %0 and %2 the same register, and the");
    puts("  mov destroys an input. That bug depends on the optimiser, so it is");
    puts("  not run here. When an output is written before every input has been");
    puts("  read, mark it early-clobber (&).");

    wait_enter();
    clear_screen();
    heading("PART 2: what you owe the compiler");

    puts("  The compiler cannot read inside the string, so everything it");
    puts("  needs to know about the asm is what you wrote after the colons:\n");
    puts("    - an operand you do not list may sit in a register the asm");
    puts("      overwrites. A register the asm changes beyond its operands goes");
    puts("      in the clobber list, e.g. : \"rcx\", \"memory\".");
    puts("    - \"memory\" tells it the asm reads or writes memory it was not");
    puts("      given, so it must not keep values cached across the statement.");
    puts("    - on x86, GCC treats the flags (condition codes) as changed by");
    puts("      every asm statement, so you need not list them.");
    puts("    - __asm__ volatile stops the statement from being deleted when");
    puts("      its outputs look unused, or moved across other code.\n");
    puts("  And nothing checks the code itself. A wrong register name is an");
    puts("  assembler error; a wrong instruction that assembles is your bug.\n");
    puts("  Compare with Rust (the Rust course, module 8): its asm! uses Intel");
    puts("  syntax by default, names its operands, and states what it avoids");
    puts("  with options(nomem, nostack, pure). Same CPU, the opposite default.");

    wait_enter();
    clear_screen();
    heading("PART 3: NASM, a whole file of assembly");

    puts("  For more than a few instructions, write a separate .asm file,");
    puts("  assemble it with NASM and link it with the C. NASM uses Intel");
    puts("  syntax: DESTINATION first, no % or $, memory in [brackets]. The");
    puts("  same operation in the two:\n");
    puts("    AT&T (GCC default)     Intel (NASM)");
    puts("    mov %rdi, %rax         mov rax, rdi");
    puts("    add $5, %rax           add rax, 5");
    puts("    movzbl (%rdi), %eax    movzx eax, byte [rdi]\n");
    puts("  The file below is the real asm/add.asm of this course:\n");
    show_nasm_source();
    puts("");
    puts("  The registers follow the System V AMD64 ABI, the calling");
    puts("  convention Linux uses: integer arguments in rdi, rsi, rdx, rcx,");
    puts("  r8, r9; the result in rax. C declares the functions and trusts");
    puts("  that the declaration matches:\n");
    puts("    uint64_t add_numbers(uint64_t a, uint64_t b);\n");
    puts("  and the build is two commands (or `make asm-demo`):\n");
    puts("    nasm -f elf64 asm/add.asm -o asm/add.o");
    puts("    cc -std=c11 asm/use_asm.c asm/add.o -o asm/use_asm\n");
    run_nasm_demo();
    puts("\n  Which to choose: __asm__ when the assembly belongs inside one C");
    puts("  function and the compiler should pick registers; a NASM file when");
    puts("  it is a routine on its own, to be read and tested as assembly.");

    wait_enter();
    clear_screen();
    exercise(20);

    question("In GCC's default AT&T syntax, which operand comes first in\n"
             "  an instruction: the source or the destination?",
             "source",
             "AT&T is source first; Intel syntax (NASM) is destination first.");
    question("In the System V x86-64 calling convention, which register\n"
             "  holds the SECOND integer argument?",
             "rsi",
             "rdi, rsi, rdx, rcx, r8, r9 in that order.");

    {
        const char *task[] = {
            "Use __asm__ (x86-64) with a lea instruction to compute 14 * 3",
            "without multiplying:  lea (%1,%1,2), %0.  Print the result.",
        };
        const char *expected[] = {
            "42",
        };
        const char *solution[] = {
            "#include <stdio.h>",
            "",
            "int main(void)",
            "{",
            "    long x = 14;",
            "    long y;",
            "    __asm__(\"lea (%1,%1,2), %0\" : \"=r\"(y) : \"r\"(x));",
            "    printf(\"%ld\\n\", y);",
            "    return 0;",
            "}",
        };
        challenge(task, 2, 0, 0, expected, 1, solution, 10);
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    puts("   - __asm__(template : outputs : inputs : clobbers), AT&T by default");
    puts("   - r / m / i say what an operand may be; = writes, + reads and writes");
    puts("   - an output written early needs &; undeclared changes need clobbers");
    puts("   - NASM files use Intel syntax and link in through the System V ABI\n");
    puts("  The next step would be reading what the compiler writes itself:");
    puts("  gcc -S -O2 file.c, and compare it with what you would have written.");
    wait_enter();
}
