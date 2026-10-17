; lessons_basics.asm - module 1: registers, syscalls, and your first real program.
;
; Every string below is printed through ui.asm's print_cstr, itself nothing
; but raw write() syscalls -- the menu this module runs inside of already
; is the demonstration: registers holding arguments, a real stack frame,
; a real syscall. Part 4 says so explicitly and points back at ui.asm.
;
; Each screen of text is one label and a run of ln lines (course.inc) that
; end in a single db 0, printed in one call; the code at the bottom is just
; the order the screens appear in.

%include "course.inc"

default rel

global lesson_01_registers

section .rodata

; ---- module 1 ----------------------------------------------------
m1_title: db "MODULE 1 - REGISTERS, SYSCALLS, YOUR FIRST PROGRAM", 0
m1_p1_hdr: db "PART 1: registers", 0
m1_p2_hdr: db "PART 2: Intel syntax, and the order of the operands", 0
m1_p3_hdr: db "PART 3: your first program, no libc at all", 0
m1_p4_hdr: db "PART 4: this menu is the same thing, at real size", 0
m1_p5_hdr: db "PART 5: reading a compiler's own output", 0

m1_p1:
    ln "  A register is a tiny piece of storage built into the CPU itself --"
    ln "  no memory address, no load from RAM, just there. x86-64 has sixteen"
    ln "  general-purpose ones: rax, rbx, rcx, rdx, rsi, rdi, rbp, rsp, and"
    ln "  r8 through r15. The System V calling convention (the one Linux"
    ln "  uses) gives most of them a job by agreement, not by hardware:"
    ln ""
    ln "    rdi, rsi, rdx, rcx, r8, r9   the first six integer/pointer"
    ln "                                   arguments to a function call"
    ln "    rax                            the return value"
    ln "    rsp                            the stack pointer"
    ln "    rbp                            traditionally, this function's"
    ln "                                   own stack frame base"
    db 0

m1_p2:
    ln "  An instruction is a name followed by its operands, and in the"
    ln "  Intel syntax this course uses, the destination comes first:"
    ln ""
    ln "    mov eax, 42        ; eax = 42"
    ln ""
    ln "  Three rules cover nearly all of it:"
    ln ""
    ln "    - destination first, then source:  mov dst, src"
    ln "    - registers and numbers are written bare -- no $ and no %"
    ln "    - a memory operand goes in square brackets:  [rbx], [rbx+8]"
    ln ""
    ln "  This is the syntax NASM assembles, the one Intel's manuals use, and"
    ln "  the one most OS-development tutorials use. The GNU"
    ln "  tools default to the other one, AT&T, which reverses the operands"
    ln "  and decorates every register and number (mov $42, %eax); gcc -S"
    ln "  prints it unless asked for -masm=intel. Recognizing it is enough:"
    ln "  this course never writes it."
    db 0

m1_p3:
    ln "  A Linux program's most direct interface to the kernel is a syscall:"
    ln "  a number in rax saying which one, arguments in rdi/rsi/rdx/..., then"
    ln "  the syscall instruction itself. No printf, no libc -- this is what"
    ln "  printf eventually calls down into, stripped to the metal:"
    ln ""
    ln "    global _start"
    ln ""
    ln "    section .text"
    ln "    _start:"
    ln "        mov rdi, 42        ; exit code"
    ln "        mov rax, 60        ; syscall number for exit"
    ln "        syscall"
    ln ""
    ln "  This was actually assembled and run while writing this module:"
    ln ""
    ln "    nasm -f elf64 exit42.asm -o exit42.o && ld exit42.o -o exit42"
    ln "    ./exit42; echo $?"
    ln ""
    ln "  prints 42 -- the real exit code, read back from the real process."
    ln "  _start, not main: there is no C runtime here to call main() for"
    ln "  you, so the linker's own default entry point is _start instead."
    db 0

m1_p4:
    ln "  assembly/src/ui.asm -- the code that just drew this screen and will"
    ln "  read your answer below -- is built from exactly the pieces above:"
    ln "  registers carrying arguments, a real stack frame (push rbp; mov"
    ln "  rbp, rsp) for any function whose arguments need to survive more"
    ln "  than one call, and nothing but read/write/exit syscalls for every"
    ln "  byte of I/O. Open it -- it is worked example code the same way"
    ln "  every other course's own source is."
    db 0

m1_p5:
    ln "  Take a function you already know from the C course:"
    ln ""
    ln "    int add(int a, int b) { return a + b; }"
    ln ""
    ln "  and ask gcc for its assembly instead of an object file, in the"
    ln "  same Intel syntax this course uses:"
    ln ""
    ln "    gcc -O0 -S -masm=intel add.c -o add.s"
    ln ""
    ln "  The real output (-O0 so it maps straight to the source, not"
    ln "  optimized into something unrecognizable), directives left out:"
    ln ""
    ln "    add:"
    ln "        push    rbp"
    ln "        mov     rbp, rsp"
    ln "        mov     DWORD PTR -4[rbp], edi     ; a (1st arg) saved off rdi"
    ln "        mov     DWORD PTR -8[rbp], esi     ; b (2nd arg) saved off rsi"
    ln "        mov     edx, DWORD PTR -4[rbp]"
    ln "        mov     eax, DWORD PTR -8[rbp]"
    ln "        add     eax, edx                   ; eax = a + b"
    ln "        pop     rbp"
    ln "        ret"
    ln ""
    ln "  edi and esi are the 32-bit names for rdi and rsi -- the same"
    ln "  argument registers this module has been using all along. The"
    ln "  return value ends up in eax (rax) before ret, exactly like every"
    ln "  function in ui.asm returns its result there. gcc writes a memory"
    ln "  operand as DWORD PTR -4[rbp]; NASM writes the same thing as"
    ln "  dword [rbp-4]."
    db 0

m1_exercise: db `\n  >> EXERCISE - MODULE 1\n  ------------------------------------------------------\n`, 0

m1_q1_text:   db `In Intel syntax, mov eax, 5 puts 5 into eax. Which operand is the\n  destination -- the first one or the second one?`, 0
m1_q1_answer: db "first", 0
m1_q1_why:    db `Intel order is destination, source -- the reverse of the AT&T\n             order gcc -S prints by default, where the same\n             instruction is written mov $5, %eax.`, 0

m1_q2_text:   db "A function's 2nd integer argument arrives in which register?", 0
m1_q2_answer: db "rsi", 0
m1_q2_why:    db `rdi, rsi, rdx, rcx, r8, r9 in that order for the first six\n             integer/pointer arguments -- rsi is the second one.`, 0

m1_q3_text:   db `What goes in rax right before the syscall instruction runs?\n  (one word)`, 0
m1_q3_answer: db "number", 0
m1_q3_why:    db `exit is 60, write is 1, read is 0 -- the kernel reads rax first\n             to know which syscall is even being requested.`, 0

m1_challenge:
    ln ""
    ln "  >> WRITE THIS YOURSELF, in a real file"
    ln "  Write and assemble a program that writes the single line"
    ln '  "done\n" to stdout, then exits with code 0. No libc, syscalls'
    ln "  only -- write (1) then exit (60)."
    ln ""
    ln "  It must print:"
    ln ""
    ln "      done"
    ln ""
    ln "  Assemble and link with:"
    ln "    nasm -f elf64 prog.asm -o prog.o && ld prog.o -o prog"
    db 0

m1_summary_hdr: db "SUMMARY", 0
m1_summary:
    ln "   - Intel syntax: destination first, then source -- mov dst, src;"
    ln "     the AT&T order gcc -S prints by default is the reverse"
    ln "   - the first six integer/pointer arguments to a call go in rdi,"
    ln "     rsi, rdx, rcx, r8, r9 -- not the stack, not until argument 7"
    ln "   - a syscall is a number in rax plus arguments in the same"
    ln "     registers a function call would use, then the syscall"
    ln "     instruction -- no libc required for any of it"
    ln "   - a compiler's -O0 output is not a black box: push rbp / mov"
    ln "     rbp, rsp is the same stack frame this course's own ui.asm"
    ln "     writes by hand"
    ln ""
    ln "  Not fluency -- enough to recognize what your own compiled C"
    ln "  becomes, and enough that objdump -d on a program you wrote is"
    ln "  readable instead of noise."
    db 0

m1_solution:
    incbin "01.asm"
    db 0

section .text

lesson_01_registers:
    mov rdi, m1_title
    call title

    part m1_p1_hdr, m1_p1
    part m1_p2_hdr, m1_p2
    part m1_p3_hdr, m1_p3
    part m1_p4_hdr, m1_p4
    part m1_p5_hdr, m1_p5

    ; exercise(1)
    mov rdi, m1_exercise
    call print_cstr
    quiz m1_q1_text, m1_q1_answer, m1_q1_why
    quiz m1_q2_text, m1_q2_answer, m1_q2_why
    quiz m1_q3_text, m1_q3_answer, m1_q3_why
    challenge m1_challenge, m1_solution

    ; summary(1)
    summary m1_summary_hdr, m1_summary
    ret

; Tells the linker this object does not need an executable stack.
section .note.GNU-stack noalloc noexec nowrite progbits
