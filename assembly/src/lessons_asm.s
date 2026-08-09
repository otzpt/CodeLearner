# Module 1 - registers, syscalls, and your first real program.
#
# Every string below is printed through ui.s's print_cstr, itself nothing
# but raw write() syscalls -- the menu this module runs inside of already
# is the demonstration: registers holding arguments, a real stack frame,
# a real syscall. Part 4 says so explicitly and points back at ui.s.

.global lesson_01_registers

.section .rodata

l1_part1_hdr:  .asciz "PART 1: registers"
l1_part1_a:    .asciz "  A register is a tiny piece of storage built into the CPU itself --"
l1_part1_b:    .asciz "  no memory address, no load from RAM, just there. x86-64 has sixteen"
l1_part1_c:    .asciz "  general-purpose ones: rax, rbx, rcx, rdx, rsi, rdi, rbp, rsp, and"
l1_part1_d:    .asciz "  r8 through r15. The System V calling convention (the one Linux"
l1_part1_e:    .asciz "  uses) gives most of them a job by agreement, not by hardware:"
l1_part1_f:    .asciz ""
l1_part1_g:    .asciz "    rdi, rsi, rdx, rcx, r8, r9   the first six integer/pointer"
l1_part1_h:    .asciz "                                   arguments to a function call"
l1_part1_i:    .asciz "    rax                            the return value"
l1_part1_j:    .asciz "    rsp                            the stack pointer"
l1_part1_k:    .asciz "    rbp                            traditionally, this function's"
l1_part1_l:    .asciz "                                   own stack frame base"

l1_part2_hdr:  .asciz "PART 2: AT&T syntax, and why it looks backwards"
l1_part2_a:    .asciz "  Most tutorials and textbooks online show Intel syntax:"
l1_part2_b:    .asciz ""
l1_part2_c:    .asciz "    mov eax, 42        ; Intel: destination, source"
l1_part2_d:    .asciz ""
l1_part2_e:    .asciz "  The GNU tools this course uses -- as, and gcc -S's default output --"
l1_part2_f:    .asciz "  use AT&T syntax instead, and the operand order is reversed:"
l1_part2_g:    .asciz ""
l1_part2_h:    .asciz "    mov $42, %eax      # AT&T: source, destination"
l1_part2_i:    .asciz ""
l1_part2_j:    .asciz "  Three differences, all through this course: source comes first, an"
l1_part2_k:    .asciz "  immediate value needs a $, a register name needs a %. Typing Intel"
l1_part2_l:    .asciz "  syntax at a GNU assembler does not warn you it is backwards -- it"
l1_part2_m:    .asciz "  just fails to assemble, or means the opposite of what a tutorial"
l1_part2_n:    .asciz "  written for a different assembler said it would."

l1_part3_hdr:  .asciz "PART 3: your first program, no libc at all"
l1_part3_a:    .asciz "  A Linux program's most direct interface to the kernel is a syscall:"
l1_part3_b:    .asciz "  a number in rax saying which one, arguments in rdi/rsi/rdx/..., then"
l1_part3_c:    .asciz "  the syscall instruction itself. No printf, no libc -- this is what"
l1_part3_d:    .asciz "  printf eventually calls down into, stripped to the metal:"
l1_part3_e:    .asciz ""
l1_part3_f:    .asciz "    .global _start"
l1_part3_g:    .asciz ""
l1_part3_h:    .asciz "    _start:"
l1_part3_i:    .asciz "        mov $42, %rdi      # exit code"
l1_part3_j:    .asciz "        mov $60, %rax      # syscall number for exit"
l1_part3_k:    .asciz "        syscall"
l1_part3_l:    .asciz ""
l1_part3_m:    .asciz "  This was actually assembled and run while writing this module:"
l1_part3_n:    .asciz ""
l1_part3_o:    .asciz "    as exit42.s -o exit42.o && ld exit42.o -o exit42"
l1_part3_p:    .asciz "    ./exit42; echo $?"
l1_part3_q:    .asciz ""
l1_part3_r:    .asciz "  prints 42 -- the real exit code, read back from the real process."
l1_part3_s:    .asciz "  _start, not main: there is no C runtime here to call main() for"
l1_part3_t:    .asciz "  you, so the linker's own default entry point is _start instead."

l1_part4_hdr:  .asciz "PART 4: this menu is the same thing, at real size"
l1_part4_a:    .asciz "  assembly/src/ui.s -- the code that just drew this screen and will"
l1_part4_b:    .asciz "  read your answer below -- is built from exactly the pieces above:"
l1_part4_c:    .asciz "  registers carrying arguments, a real stack frame (push %rbp; mov"
l1_part4_d:    .asciz "  %rsp, %rbp) for any function whose arguments need to survive more"
l1_part4_e:    .asciz "  than one call, and nothing but read/write/exit syscalls for every"
l1_part4_f:    .asciz "  byte of I/O. Open it -- it is worked example code the same way"
l1_part4_g:    .asciz "  every other course's own source is."

l1_part5_hdr:  .asciz "PART 5: reading a compiler's own output"
l1_part5_a:    .asciz "  Take a function you already know from the C course:"
l1_part5_b:    .asciz ""
l1_part5_c:    .asciz "    int add(int a, int b) { return a + b; }"
l1_part5_d:    .asciz ""
l1_part5_e:    .asciz "  and ask gcc for its assembly instead of an object file:"
l1_part5_f:    .asciz ""
l1_part5_g:    .asciz "    gcc -O0 -S add.c -o add.s"
l1_part5_h:    .asciz ""
l1_part5_i:    .asciz "  The real output (-O0 so it maps straight to the source, not"
l1_part5_j:    .asciz "  optimized into something unrecognizable):"
l1_part5_k:    .asciz ""
l1_part5_l:    .asciz "    add:"
l1_part5_m:    .asciz "        pushq   %rbp"
l1_part5_n:    .asciz "        movq    %rsp, %rbp"
l1_part5_o:    .asciz "        movl    %edi, -4(%rbp)     # a (1st arg) saved off rdi"
l1_part5_p:    .asciz "        movl    %esi, -8(%rbp)     # b (2nd arg) saved off rsi"
l1_part5_q:    .asciz "        movl    -4(%rbp), %edx"
l1_part5_r:    .asciz "        movl    -8(%rbp), %eax"
l1_part5_s:    .asciz "        addl    %edx, %eax          # eax = a + b"
l1_part5_t:    .asciz "        popq    %rbp"
l1_part5_u:    .asciz "        ret"
l1_part5_v:    .asciz ""
l1_part5_w:    .asciz "  edi and esi are the 32-bit names for rdi and rsi -- the same"
l1_part5_x:    .asciz "  argument registers this module has been using all along. The"
l1_part5_y:    .asciz "  return value ends up in eax (rax) before ret, exactly like every"
l1_part5_z:    .asciz "  function in ui.s returns its result there."

l1_summary_hdr: .asciz "SUMMARY"
l1_summary_a:  .asciz "   - AT&T syntax: source, destination -- the opposite of Intel, and"
l1_summary_b:  .asciz "     the assembler will not warn you if you get it backwards"
l1_summary_c:  .asciz "   - the first six integer/pointer arguments to a call go in rdi,"
l1_summary_d:  .asciz "     rsi, rdx, rcx, r8, r9 -- not the stack, not until argument 7"
l1_summary_e:  .asciz "   - a syscall is a number in rax plus arguments in the same"
l1_summary_f:  .asciz "     registers a function call would use, then the syscall"
l1_summary_g:  .asciz "     instruction -- no libc required for any of it"
l1_summary_h:  .asciz "   - a compiler's -O0 output is not a black box: pushq %rbp / movq"
l1_summary_i:  .asciz "     %rsp, %rbp is the same stack frame this course's own ui.s"
l1_summary_j:  .asciz "     writes by hand"
l1_summary_k:  .asciz ""
l1_summary_l:  .asciz "  Not fluency -- enough to recognize what your own compiled C"
l1_summary_m:  .asciz "  becomes, and enough that objdump -d on a program you wrote is"
l1_summary_n:  .asciz "  readable instead of noise."

# ── exercise ──────────────────────────────────────────────────────
# Expected answers stay one word, matching docs/writing-a-course.md's own
# rule for question(): short and unambiguous, since streq (see ui.s) is an
# exact match with no partial-credit wording tolerance.
l1_q2_text:    .asciz "A function's 2nd integer argument arrives in which register?"
l1_q2_correct: .asciz "rsi"
l1_q3_text:    .asciz "What goes in rax right before the syscall instruction runs?\n  (one word)"
l1_q3_correct: .asciz "number"

l1_challenge_hdr:   .asciz "\n  >> WRITE THIS YOURSELF, in a real file"
l1_challenge_task1: .asciz "  Write and assemble a program that writes the single line"
l1_challenge_task2: .asciz "  \"done\\n\" to stdout, then exits with code 0. No libc, syscalls"
l1_challenge_task3: .asciz "  only -- write (1) then exit (60)."
l1_challenge_out:   .asciz "\n  It must print:\n\n      done\n"
l1_challenge_build: .asciz "\n  Assemble and link with:\n    as prog.s -o prog.o && ld prog.o -o prog\n"
l1_challenge_ask:   .asciz "Want to see example code?"

.section .text

# print_lines(rdi=array_of_ptrs, rsi=count) -> prints each pointer's string
# followed by a newline. Every module prints a couple dozen fixed lines;
# looping over a table here beats writing "mov $x, %rdi; call print_cstr;
# call print_newline" fifty times over.
print_lines:
    push %rbp
    mov %rsp, %rbp
    push %r12
    push %r13
    mov %rdi, %r12       # array base
    mov %rsi, %r13        # remaining count
.pl_loop:
    cmp $0, %r13
    je .pl_done
    mov (%r12), %rdi
    call print_cstr
    mov $nl_const, %rdi
    call print_cstr
    add $8, %r12
    dec %r13
    jmp .pl_loop
.pl_done:
    pop %r13
    pop %r12
    leave
    ret

.section .rodata
nl_const: .asciz "\n"

part1_lines:
    .quad l1_part1_a, l1_part1_b, l1_part1_c, l1_part1_d, l1_part1_e
    .quad l1_part1_f
    .quad l1_part1_g, l1_part1_h, l1_part1_i, l1_part1_j, l1_part1_k, l1_part1_l
part1_count = (. - part1_lines) / 8

part2_lines:
    .quad l1_part2_a, l1_part2_b, l1_part2_c, l1_part2_d, l1_part2_e
    .quad l1_part2_f, l1_part2_g, l1_part2_h, l1_part2_i, l1_part2_j
    .quad l1_part2_k, l1_part2_l, l1_part2_m, l1_part2_n
part2_count = (. - part2_lines) / 8

part3_lines:
    .quad l1_part3_a, l1_part3_b, l1_part3_c, l1_part3_d, l1_part3_e
    .quad l1_part3_f, l1_part3_g, l1_part3_h, l1_part3_i, l1_part3_j
    .quad l1_part3_k, l1_part3_l, l1_part3_m, l1_part3_n, l1_part3_o
    .quad l1_part3_p, l1_part3_q, l1_part3_r, l1_part3_s, l1_part3_t
part3_count = (. - part3_lines) / 8

part4_lines:
    .quad l1_part4_a, l1_part4_b, l1_part4_c, l1_part4_d, l1_part4_e
    .quad l1_part4_f, l1_part4_g
part4_count = (. - part4_lines) / 8

part5_lines:
    .quad l1_part5_a, l1_part5_b, l1_part5_c, l1_part5_d, l1_part5_e
    .quad l1_part5_f, l1_part5_g, l1_part5_h, l1_part5_i, l1_part5_j
    .quad l1_part5_k, l1_part5_l, l1_part5_m, l1_part5_n, l1_part5_o
    .quad l1_part5_p, l1_part5_q, l1_part5_r, l1_part5_s, l1_part5_t
    .quad l1_part5_u, l1_part5_v, l1_part5_w, l1_part5_x, l1_part5_y
    .quad l1_part5_z
part5_count = (. - part5_lines) / 8

summary_lines:
    .quad l1_summary_a, l1_summary_b, l1_summary_c, l1_summary_d
    .quad l1_summary_e, l1_summary_f, l1_summary_g, l1_summary_h
    .quad l1_summary_i, l1_summary_j, l1_summary_k, l1_summary_l
    .quad l1_summary_m, l1_summary_n
summary_count = (. - summary_lines) / 8

challenge_task_lines:
    .quad l1_challenge_task1, l1_challenge_task2, l1_challenge_task3
challenge_task_count = (. - challenge_task_lines) / 8

challenge_solution:
    .asciz ".global _start\n\n.section .rodata\nmsg:\n    .ascii \"done\\n\"\n    msg_len = . - msg\n\n.section .text\n_start:\n    mov $1, %rax\n    mov $1, %rdi\n    mov $msg, %rsi\n    mov $msg_len, %rdx\n    syscall\n\n    mov $0, %rdi\n    mov $60, %rax\n    syscall\n"

.section .text

lesson_01_registers:
    push %rbp
    mov %rsp, %rbp

    mov $l1_title, %rdi
    call title

    mov $l1_part1_hdr, %rdi
    call heading
    mov $part1_lines, %rdi
    mov $part1_count, %rsi
    call print_lines
    call wait_enter
    call clear_screen

    mov $l1_part2_hdr, %rdi
    call heading
    mov $part2_lines, %rdi
    mov $part2_count, %rsi
    call print_lines
    call wait_enter
    call clear_screen

    mov $l1_part3_hdr, %rdi
    call heading
    mov $part3_lines, %rdi
    mov $part3_count, %rsi
    call print_lines
    call wait_enter
    call clear_screen

    mov $l1_part4_hdr, %rdi
    call heading
    mov $part4_lines, %rdi
    mov $part4_count, %rsi
    call print_lines
    call wait_enter
    call clear_screen

    mov $l1_part5_hdr, %rdi
    call heading
    mov $part5_lines, %rdi
    mov $part5_count, %rsi
    call print_lines
    call wait_enter
    call clear_screen

    # exercise(1) -- literal comment tools/check-teaching-order.py's regex
    # looks for, matching every other course's exercise_marker convention.
    # Assembly has no exercise()-shaped call the way C's exercise(1); does,
    # so the marker is a comment here instead of code.
    mov $exercise_marker, %rdi
    call print_cstr

    mov $l1_q1_full, %rdi
    mov $l1_q1_correct, %rsi
    mov $l1_q1_why_full, %rdx
    call question

    mov $l1_q2_text, %rdi
    mov $l1_q2_correct, %rsi
    mov $l1_q2_why_full, %rdx
    call question

    mov $l1_q3_text, %rdi
    mov $l1_q3_correct, %rsi
    mov $l1_q3_why_full, %rdx
    call question

    mov $l1_challenge_hdr, %rdi
    call print_cstr
    mov $nl_const, %rdi
    call print_cstr
    mov $challenge_task_lines, %rdi
    mov $challenge_task_count, %rsi
    call print_lines
    mov $l1_challenge_out, %rdi
    call print_cstr
    mov $l1_challenge_build, %rdi
    call print_cstr

    mov $l1_challenge_ask, %rdi
    call ask_yes
    cmp $1, %rax
    jne .l1_no_solution
    mov $nl_const, %rdi
    call print_cstr
    call rule
    mov $challenge_solution, %rdi
    call print_cstr
    call rule
.l1_no_solution:

    call wait_enter
    call clear_screen
    # summary(1) -- marks where the exercise section ends, for the same tool.

    mov $l1_summary_hdr, %rdi
    call heading
    mov $summary_lines, %rdi
    mov $summary_count, %rsi
    call print_lines
    call wait_enter

    leave
    ret

.section .rodata
l1_title: .asciz "MODULE 1 - REGISTERS, SYSCALLS, YOUR FIRST PROGRAM"
exercise_marker: .asciz "\n  >> EXERCISE - MODULE 1\n  ------------------------------------------------------\n"
l1_q1_full: .asciz "In AT&T syntax, mov $5, %eax puts 5 into eax. Which operand is the\n  destination -- the first one or the second one?"
l1_q1_correct: .asciz "second"
l1_q1_why_full: .asciz "AT&T order is source, destination -- the reverse of Intel, where\n             the same instruction would be written mov eax, 5."
l1_q2_why_full: .asciz "rdi, rsi, rdx, rcx, r8, r9 in that order for the first six\n             integer/pointer arguments -- rsi is the second one."
l1_q3_why_full: .asciz "exit is 60, write is 1, read is 0 -- the kernel reads rax first\n             to know which syscall is even being requested."
