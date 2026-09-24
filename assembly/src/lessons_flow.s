# Modules 2 and 3 - branching, and loops.
#
# Everything this course's own menu does between reading a keystroke and
# running a module is in these two modules: a compare, a conditional jump,
# and a jump back to the top. main.s stays the worked example both of them
# point at, the same way module 1 points at ui.s.

.global lesson_02_branching
.global lesson_03_loops

.section .rodata

# ── module 2 ──────────────────────────────────────────────────────
m2_title:      .asciz "MODULE 2 - COMPARING AND BRANCHING"

m2_part1_hdr:  .asciz "PART 1: cmp sets flags, it does not store anything"
m2_part1_a:    .asciz "  There is no if in assembly. There is a compare, which throws its"
m2_part1_b:    .asciz "  own result away, and a jump that reads what the compare left"
m2_part1_c:    .asciz "  behind:"
m2_part1_d:    .asciz ""
m2_part1_e:    .asciz "    cmp $10, %rax      # computes rax - 10, keeps only the flags"
m2_part1_f:    .asciz "    jge .big           # jump if that subtraction says rax >= 10"
m2_part1_g:    .asciz ""
m2_part1_h:    .asciz "  cmp is a sub whose answer is discarded. What survives is the"
m2_part1_i:    .asciz "  FLAGS register: ZF (the result was zero), SF (it was negative),"
m2_part1_j:    .asciz "  CF (it borrowed), OF (it overflowed as a signed value). Every"
m2_part1_k:    .asciz "  conditional jump is nothing but a named test on those bits."
m2_part1_l:    .asciz ""
m2_part1_m:    .asciz "  AT&T operand order bites hardest right here. cmp $10, %rax is"
m2_part1_n:    .asciz "  rax - 10, not 10 - rax. Read it as \"compare rax against 10\" and"
m2_part1_o:    .asciz "  the jump after it means what it looks like it means."

m2_part2_hdr:  .asciz "PART 2: the jumps, and the signed/unsigned trap"
m2_part2_a:    .asciz "  Equality has one pair, and it is the easy one:"
m2_part2_b:    .asciz ""
m2_part2_c:    .asciz "    je / jz     jump if equal (ZF set)"
m2_part2_d:    .asciz "    jne / jnz   jump if not equal"
m2_part2_e:    .asciz ""
m2_part2_f:    .asciz "  Ordering has two complete sets, and picking the wrong one is a"
m2_part2_g:    .asciz "  bug the assembler will never mention:"
m2_part2_h:    .asciz ""
m2_part2_i:    .asciz "    signed:    jl  jle  jg  jge      (less/greater)"
m2_part2_j:    .asciz "    unsigned:  jb  jbe  ja  jae      (below/above)"
m2_part2_k:    .asciz ""
m2_part2_l:    .asciz "  Same bytes in the register, opposite answers. Put -1 in rax and"
m2_part2_m:    .asciz "  compare it against 1:"
m2_part2_n:    .asciz ""
m2_part2_o:    .asciz "    mov $-1, %rax"
m2_part2_p:    .asciz "    cmp $1, %rax"
m2_part2_q:    .asciz "    jl  ...            # TAKEN: as a signed value, -1 < 1"
m2_part2_r:    .asciz "    ja  ...            # ALSO TAKEN: as unsigned, -1 is huge"
m2_part2_s:    .asciz ""
m2_part2_t:    .asciz "  -1 in a 64-bit register is the bit pattern 0xFFFFFFFFFFFFFFFF."
m2_part2_u:    .asciz "  Read as signed that is -1; read as unsigned it is the largest"
m2_part2_v:    .asciz "  value a register can hold. The register does not remember which"
m2_part2_w:    .asciz "  one you meant -- the jump you pick is where that decision lives."

m2_part3_hdr:  .asciz "PART 3: the shape of if/else"
m2_part3_a:    .asciz "  An if in C compiles to a jump that skips the body. The condition"
m2_part3_b:    .asciz "  gets inverted on the way: you jump AWAY when it is false."
m2_part3_c:    .asciz ""
m2_part3_d:    .asciz "    C:                        assembly:"
m2_part3_e:    .asciz ""
m2_part3_f:    .asciz "    if (n >= 10) {                cmp $10, %rax"
m2_part3_g:    .asciz "        big();                    jl .else_part     # inverted"
m2_part3_h:    .asciz "    } else {                      call big"
m2_part3_i:    .asciz "        small();                  jmp .endif"
m2_part3_j:    .asciz "    }                         .else_part:"
m2_part3_k:    .asciz "                                  call small"
m2_part3_l:    .asciz "                              .endif:"
m2_part3_m:    .asciz ""
m2_part3_n:    .asciz "  The unconditional jmp at the end of the if-body is not optional."
m2_part3_o:    .asciz "  Leave it out and the if-body falls straight through into the"
m2_part3_p:    .asciz "  else-body and both run. Nothing warns you; the program just"
m2_part3_q:    .asciz "  does twice what it should do once."

m2_part4_hdr:  .asciz "PART 4: test, and comparing against zero"
m2_part4_a:    .asciz "  cmp $0, %rax works. Nobody writes it:"
m2_part4_b:    .asciz ""
m2_part4_c:    .asciz "    test %rax, %rax"
m2_part4_d:    .asciz "    jz .it_was_zero"
m2_part4_e:    .asciz ""
m2_part4_f:    .asciz "  test ANDs its two operands and throws the result away, keeping"
m2_part4_g:    .asciz "  the flags -- so a register ANDed with itself sets ZF exactly when"
m2_part4_h:    .asciz "  the register is zero. It assembles a byte shorter than the cmp,"
m2_part4_i:    .asciz "  which is why every compiler's output is full of it."
m2_part4_j:    .asciz ""
m2_part4_k:    .asciz "  Worth recognizing for the same reason module 1 read gcc -S output:"
m2_part4_l:    .asciz "  test %eax, %eax followed by a jump is how a compiled if (p != NULL)"
m2_part4_m:    .asciz "  and if (n) both come out the other side."

m2_part5_hdr:  .asciz "PART 5: the menu you are standing in"
m2_part5_a:    .asciz "  assembly/src/main.s reads one line, then decides what to run with"
m2_part5_b:    .asciz "  the same four instructions this module just described:"
m2_part5_c:    .asciz ""
m2_part5_d:    .asciz "    movb choice_buf(%rip), %al   # the first byte you typed"
m2_part5_e:    .asciz "    cmp $'0', %al"
m2_part5_f:    .asciz "    je .quit"
m2_part5_g:    .asciz ""
m2_part5_h:    .asciz "  A character is compared as a number, because it is one: '0' is"
m2_part5_i:    .asciz "  the byte 48. That is also how main.s turns your keystroke into an"
m2_part5_j:    .asciz "  index -- sub $'0', %al gives 0 through 9 from '0' through '9',"
m2_part5_k:    .asciz "  the exact trick module 4 uses to read a whole number."

m2_summary_hdr: .asciz "SUMMARY"
m2_sum_a:      .asciz "   - cmp computes a subtraction and keeps only the flags; the jump"
m2_sum_b:      .asciz "     after it is what reads them"
m2_sum_c:      .asciz "   - cmp $10, %rax is rax - 10, not 10 - rax (AT&T order, again)"
m2_sum_d:      .asciz "   - jl/jg are signed, jb/ja are unsigned, the assembler will not"
m2_sum_e:      .asciz "     tell you which one the value in the register wanted"
m2_sum_f:      .asciz "   - an if is a jump on the INVERTED condition, plus a jmp at the"
m2_sum_g:      .asciz "     end of the if-body so it does not fall into the else-body"
m2_sum_h:      .asciz "   - test %rax, %rax is the idiomatic \"is it zero\""

m2_q1_text:    .asciz "After cmp $10, %rax, which jump runs when rax holds 25 --\n  jl or jg?"
m2_q1_correct: .asciz "jg"
m2_q1_why:     .asciz "cmp $10, %rax computes rax - 10. 25 - 10 is positive, so the\n             greater-than jump is the one taken."
m2_q2_text:    .asciz "rax holds -1. You compare it against 1 and use ja. Is the jump\n  taken? (yes/no)"
m2_q2_correct: .asciz "yes"
m2_q2_why:     .asciz "ja is the UNSIGNED test, and -1 unsigned is 0xFFFFFFFFFFFFFFFF,\n             the largest value there is. jl would have been the signed one."
m2_q3_text:    .asciz "Which instruction is the one-word idiom for \"is this register\n  zero\"? (one word)"
m2_q3_correct: .asciz "test"
m2_q3_why:     .asciz "test %rax, %rax ANDs the register with itself, which sets ZF\n             exactly when every bit is 0, one byte shorter than cmp $0."

m2_chal_hdr:   .asciz "\n  >> WRITE THIS YOURSELF, in a real file"
m2_chal_a:     .asciz "  Put a number in rax with mov, then print \"big\\n\" if it is 10 or"
m2_chal_b:     .asciz "  more and \"small\\n\" if it is not. Exit 0 either way. Syscalls"
m2_chal_c:     .asciz "  only -- write (1) and exit (60), the same two module 1 used."
m2_chal_out:   .asciz "\n  With mov $25, %rax at the top it must print:\n\n      big\n\n  and with mov $3, %rax, exactly:\n\n      small\n"
m2_chal_build: .asciz "\n  Assemble and link with:\n    as prog.s -o prog.o && ld prog.o -o prog\n"
m2_chal_ask:   .asciz "Want to see example code?"
m2_chal_sol:   .asciz ".global _start\n\n.section .rodata\nbig_msg:\n    .ascii \"big\\n\"\n    big_len = . - big_msg\nsmall_msg:\n    .ascii \"small\\n\"\n    small_len = . - small_msg\n\n.section .text\n_start:\n    mov $25, %rax\n\n    cmp $10, %rax\n    jl .small            # inverted: jump away when the if is false\n\n    mov $big_msg, %rsi\n    mov $big_len, %rdx\n    jmp .print\n\n.small:\n    mov $small_msg, %rsi\n    mov $small_len, %rdx\n\n.print:\n    mov $1, %rax         # write\n    mov $1, %rdi         # stdout\n    syscall\n\n    mov $0, %rdi\n    mov $60, %rax        # exit\n    syscall\n"

# ── module 3 ──────────────────────────────────────────────────────
m3_title:      .asciz "MODULE 3 - LOOPS"

m3_part1_hdr:  .asciz "PART 1: a loop is a label and a jump backwards"
m3_part1_a:    .asciz "  There is no while and no for. A loop is a label you can jump back"
m3_part1_b:    .asciz "  to, a body, and a conditional jump that decides whether to go"
m3_part1_c:    .asciz "  round again -- module 2's instructions, pointed upwards:"
m3_part1_d:    .asciz ""
m3_part1_e:    .asciz "    mov $0, %rcx           # i = 0"
m3_part1_f:    .asciz "  .loop:"
m3_part1_g:    .asciz "    cmp $5, %rcx           # i < 5 ?"
m3_part1_h:    .asciz "    jge .done              # inverted, exactly like an if"
m3_part1_i:    .asciz "    <body>"
m3_part1_j:    .asciz "    inc %rcx               # i++"
m3_part1_k:    .asciz "    jmp .loop"
m3_part1_l:    .asciz "  .done:"
m3_part1_m:    .asciz ""
m3_part1_n:    .asciz "  That is a while loop, whole. The three pieces a for loop writes"
m3_part1_o:    .asciz "  on one line -- init, test, step -- are just in three places here."

m3_part2_hdr:  .asciz "PART 2: test at the top, or test at the bottom"
m3_part2_a:    .asciz "  The loop above tests first, so a body that should not run at all"
m3_part2_b:    .asciz "  runs zero times. Moving the test to the bottom makes it a"
m3_part2_c:    .asciz "  do-while, and costs one jump less per iteration:"
m3_part2_d:    .asciz ""
m3_part2_e:    .asciz "  .loop:"
m3_part2_f:    .asciz "    <body>"
m3_part2_g:    .asciz "    inc %rcx"
m3_part2_h:    .asciz "    cmp $5, %rcx"
m3_part2_i:    .asciz "    jl .loop               # one jump, not two"
m3_part2_j:    .asciz ""
m3_part2_k:    .asciz "  The top-tested version runs a conditional jump AND an"
m3_part2_l:    .asciz "  unconditional one every lap. This runs one. It is also wrong"
m3_part2_m:    .asciz "  whenever the count can be zero, because the body has already"
m3_part2_n:    .asciz "  happened before anything is checked -- which is why a compiler"
m3_part2_o:    .asciz "  emits the top test once, before the loop, and the bottom-tested"
m3_part2_p:    .asciz "  shape for every lap after it."

m3_part3_hdr:  .asciz "PART 3: counting down is cheaper, and the loop trap"
m3_part3_a:    .asciz "  Counting towards zero drops the cmp entirely, because dec sets"
m3_part3_b:    .asciz "  ZF by itself:"
m3_part3_c:    .asciz ""
m3_part3_d:    .asciz "    mov $5, %rcx"
m3_part3_e:    .asciz "  .loop:"
m3_part3_f:    .asciz "    <body>"
m3_part3_g:    .asciz "    dec %rcx               # sets ZF when rcx reaches 0"
m3_part3_h:    .asciz "    jnz .loop"
m3_part3_i:    .asciz ""
m3_part3_j:    .asciz "  x86 also has an actual instruction called loop, which decrements"
m3_part3_k:    .asciz "  rcx and jumps if it is still non-zero -- the two lines above in"
m3_part3_l:    .asciz "  one. Do not reach for it:"
m3_part3_m:    .asciz ""
m3_part3_n:    .asciz "    - it is microcoded, and slower on every current CPU than the"
m3_part3_o:    .asciz "      dec/jnz pair it replaces"
m3_part3_p:    .asciz "    - it hardcodes rcx, and rcx is caller-saved: any call in the"
m3_part3_q:    .asciz "      body is free to destroy your counter and usually does"
m3_part3_r:    .asciz ""
m3_part3_s:    .asciz "  That second one is the real trap. A counter that has to survive"
m3_part3_t:    .asciz "  calls belongs in a callee-saved register -- rbx, or r12 through"
m3_part3_u:    .asciz "  r15 -- pushed on entry and popped before ret."

m3_part4_hdr:  .asciz "PART 4: walking an array, from this file"
m3_part4_a:    .asciz "  print_lines in ui.s prints every string in a table. It is a real"
m3_part4_b:    .asciz "  loop over a real array, and every screen in this course went"
m3_part4_c:    .asciz "  through it:"
m3_part4_d:    .asciz ""
m3_part4_e:    .asciz "    mov %rdi, %r12         # array base   -- callee-saved, on"
m3_part4_f:    .asciz "    mov %rsi, %r13         # count        -- purpose (PART 3)"
m3_part4_g:    .asciz "  .pl_loop:"
m3_part4_h:    .asciz "    cmp $0, %r13"
m3_part4_i:    .asciz "    je .pl_done"
m3_part4_j:    .asciz "    mov (%r12), %rdi       # load one pointer out of the array"
m3_part4_k:    .asciz "    call print_cstr        # clobbers rax, rcx, rdx, rsi, rdi..."
m3_part4_l:    .asciz "    add $8, %r12           # ...but not r12/r13"
m3_part4_m:    .asciz "    dec %r13"
m3_part4_n:    .asciz "    jmp .pl_loop"
m3_part4_o:    .asciz ""
m3_part4_p:    .asciz "  add $8 because the array holds 8-byte pointers. Stepping by 1"
m3_part4_q:    .asciz "  would land in the middle of one, and the load would produce"
m3_part4_r:    .asciz "  garbage that is still a perfectly valid-looking address."

m3_part5_hdr:  .asciz "PART 5: indexed addressing, so you do not have to add"
m3_part5_a:    .asciz "  x86 can do the arithmetic inside the operand:"
m3_part5_b:    .asciz ""
m3_part5_c:    .asciz "    movb (%rdi, %rcx), %al       # al = rdi[rcx]        (bytes)"
m3_part5_d:    .asciz "    mov  (%rdi, %rcx, 8), %rax   # rax = rdi[rcx]  (8-byte slots)"
m3_part5_e:    .asciz ""
m3_part5_f:    .asciz "  The form is (base, index, scale), scale being 1, 2, 4, or 8 --"
m3_part5_g:    .asciz "  one element's size. It is one instruction, it does not touch"
m3_part5_h:    .asciz "  flags, and it leaves the base pointer alone so the loop counter"
m3_part5_i:    .asciz "  stays a plain index instead of a second moving pointer."
m3_part5_j:    .asciz ""
m3_part5_k:    .asciz "  strlen and streq in ui.s both walk their strings this way. Your"
m3_part5_l:    .asciz "  compiled C does too, which is why array[i] costs no instructions"
m3_part5_m:    .asciz "  of its own in an -O0 listing."

m3_summary_hdr: .asciz "SUMMARY"
m3_sum_a:      .asciz "   - a loop is a label, a body, and a conditional jump back to it"
m3_sum_b:      .asciz "   - top-tested runs the body zero times when the count is zero;"
m3_sum_c:      .asciz "     bottom-tested always runs it at least once, for one jump less"
m3_sum_d:      .asciz "   - dec sets ZF, so counting down to zero needs no cmp at all"
m3_sum_e:      .asciz "   - the loop instruction is slower than dec/jnz and hardcodes rcx,"
m3_sum_f:      .asciz "     which any call in the body is allowed to clobber"
m3_sum_g:      .asciz "   - a counter that outlives a call goes in rbx or r12-r15"
m3_sum_h:      .asciz "   - (base, index, scale) indexes an array in one operand"

m3_q1_text:    .asciz "Which register set survives a call -- caller-saved like rcx, or\n  callee-saved like r12? (one word: rcx or r12)"
m3_q1_correct: .asciz "r12"
m3_q1_why:     .asciz "r12-r15, rbx, and rbp are callee-saved: a function that wants\n             them must restore them, so your counter is still there after."
m3_q2_text:    .asciz "Which flag does dec set when the counter reaches zero?\n  (two letters)"
m3_q2_correct: .asciz "zf"
m3_q2_why:     .asciz "dec sets ZF on a zero result, so dec/jnz is a complete loop\n             test with no cmp in front of it."
m3_q3_text:    .asciz "An array of 8-byte pointers: how much do you add to the base to\n  reach the next element? (a number)"
m3_q3_correct: .asciz "8"
m3_q3_why:     .asciz "Stepping by 1 lands inside an element, and the load that follows\n             returns a valid-looking address made of two halves."

m3_chal_hdr:   .asciz "\n  >> WRITE THIS YOURSELF, in a real file"
m3_chal_a:     .asciz "  Print the digits 1 to 5, one per line, with a loop. No libc, so"
m3_chal_b:     .asciz "  turn the counter into a character yourself: add $'0' to it, the"
m3_chal_c:     .asciz "  reverse of the sub $'0' main.s does to your keystroke."
m3_chal_out:   .asciz "\n  It must print, exactly:\n\n      1\n      2\n      3\n      4\n      5\n"
m3_chal_build: .asciz "\n  Assemble and link with:\n    as prog.s -o prog.o && ld prog.o -o prog\n"
m3_chal_ask:   .asciz "Want to see example code?"
m3_chal_sol:   .asciz ".global _start\n\n.section .bss\n    .lcomm out, 2          # one digit plus the newline\n\n.section .text\n_start:\n    mov $1, %r12           # callee-saved: write() will not touch it\n\n.loop:\n    cmp $5, %r12\n    jg .done\n\n    mov %r12, %rax\n    add $'0', %al          # 1 -> '1'\n    movb %al, out(%rip)\n    movb $10, out+1(%rip)  # '\\n'\n\n    mov $1, %rax           # write\n    mov $1, %rdi           # stdout\n    mov $out, %rsi\n    mov $2, %rdx\n    syscall\n\n    inc %r12\n    jmp .loop\n\n.done:\n    mov $0, %rdi\n    mov $60, %rax          # exit\n    syscall\n"

nl2: .asciz "\n"

# ── line tables ───────────────────────────────────────────────────
m2_p1_lines:
    .quad m2_part1_a, m2_part1_b, m2_part1_c, m2_part1_d, m2_part1_e
    .quad m2_part1_f, m2_part1_g, m2_part1_h, m2_part1_i, m2_part1_j
    .quad m2_part1_k, m2_part1_l, m2_part1_m, m2_part1_n, m2_part1_o
m2_p1_count = (. - m2_p1_lines) / 8

m2_p2_lines:
    .quad m2_part2_a, m2_part2_b, m2_part2_c, m2_part2_d, m2_part2_e
    .quad m2_part2_f, m2_part2_g, m2_part2_h, m2_part2_i, m2_part2_j
    .quad m2_part2_k, m2_part2_l, m2_part2_m, m2_part2_n, m2_part2_o
    .quad m2_part2_p, m2_part2_q, m2_part2_r, m2_part2_s, m2_part2_t
    .quad m2_part2_u, m2_part2_v, m2_part2_w
m2_p2_count = (. - m2_p2_lines) / 8

m2_p3_lines:
    .quad m2_part3_a, m2_part3_b, m2_part3_c, m2_part3_d, m2_part3_e
    .quad m2_part3_f, m2_part3_g, m2_part3_h, m2_part3_i, m2_part3_j
    .quad m2_part3_k, m2_part3_l, m2_part3_m, m2_part3_n, m2_part3_o
    .quad m2_part3_p, m2_part3_q
m2_p3_count = (. - m2_p3_lines) / 8

m2_p4_lines:
    .quad m2_part4_a, m2_part4_b, m2_part4_c, m2_part4_d, m2_part4_e
    .quad m2_part4_f, m2_part4_g, m2_part4_h, m2_part4_i, m2_part4_j
    .quad m2_part4_k, m2_part4_l, m2_part4_m
m2_p4_count = (. - m2_p4_lines) / 8

m2_p5_lines:
    .quad m2_part5_a, m2_part5_b, m2_part5_c, m2_part5_d, m2_part5_e
    .quad m2_part5_f, m2_part5_g, m2_part5_h, m2_part5_i, m2_part5_j
    .quad m2_part5_k
m2_p5_count = (. - m2_p5_lines) / 8

m2_sum_lines:
    .quad m2_sum_a, m2_sum_b, m2_sum_c, m2_sum_d, m2_sum_e, m2_sum_f
    .quad m2_sum_g, m2_sum_h
m2_sum_count = (. - m2_sum_lines) / 8

m2_chal_lines:
    .quad m2_chal_a, m2_chal_b, m2_chal_c
m2_chal_count = (. - m2_chal_lines) / 8

m3_p1_lines:
    .quad m3_part1_a, m3_part1_b, m3_part1_c, m3_part1_d, m3_part1_e
    .quad m3_part1_f, m3_part1_g, m3_part1_h, m3_part1_i, m3_part1_j
    .quad m3_part1_k, m3_part1_l, m3_part1_m, m3_part1_n, m3_part1_o
m3_p1_count = (. - m3_p1_lines) / 8

m3_p2_lines:
    .quad m3_part2_a, m3_part2_b, m3_part2_c, m3_part2_d, m3_part2_e
    .quad m3_part2_f, m3_part2_g, m3_part2_h, m3_part2_i, m3_part2_j
    .quad m3_part2_k, m3_part2_l, m3_part2_m, m3_part2_n, m3_part2_o
    .quad m3_part2_p
m3_p2_count = (. - m3_p2_lines) / 8

m3_p3_lines:
    .quad m3_part3_a, m3_part3_b, m3_part3_c, m3_part3_d, m3_part3_e
    .quad m3_part3_f, m3_part3_g, m3_part3_h, m3_part3_i, m3_part3_j
    .quad m3_part3_k, m3_part3_l, m3_part3_m, m3_part3_n, m3_part3_o
    .quad m3_part3_p, m3_part3_q, m3_part3_r, m3_part3_s, m3_part3_t
    .quad m3_part3_u
m3_p3_count = (. - m3_p3_lines) / 8

m3_p4_lines:
    .quad m3_part4_a, m3_part4_b, m3_part4_c, m3_part4_d, m3_part4_e
    .quad m3_part4_f, m3_part4_g, m3_part4_h, m3_part4_i, m3_part4_j
    .quad m3_part4_k, m3_part4_l, m3_part4_m, m3_part4_n, m3_part4_o
    .quad m3_part4_p, m3_part4_q, m3_part4_r
m3_p4_count = (. - m3_p4_lines) / 8

m3_p5_lines:
    .quad m3_part5_a, m3_part5_b, m3_part5_c, m3_part5_d, m3_part5_e
    .quad m3_part5_f, m3_part5_g, m3_part5_h, m3_part5_i, m3_part5_j
    .quad m3_part5_k, m3_part5_l, m3_part5_m
m3_p5_count = (. - m3_p5_lines) / 8

m3_sum_lines:
    .quad m3_sum_a, m3_sum_b, m3_sum_c, m3_sum_d, m3_sum_e, m3_sum_f
    .quad m3_sum_g, m3_sum_h
m3_sum_count = (. - m3_sum_lines) / 8

m3_chal_lines:
    .quad m3_chal_a, m3_chal_b, m3_chal_c
m3_chal_count = (. - m3_chal_lines) / 8

m2_ex_marker: .asciz "\n  >> EXERCISE - MODULE 2\n  ------------------------------------------------------\n"
m3_ex_marker: .asciz "\n  >> EXERCISE - MODULE 3\n  ------------------------------------------------------\n"

.section .text

lesson_02_branching:
    push %rbp
    mov %rsp, %rbp

    mov $m2_title, %rdi
    call title

    mov $m2_part1_hdr, %rdi
    call heading
    mov $m2_p1_lines, %rdi
    mov $m2_p1_count, %rsi
    call print_lines
    call wait_enter
    call clear_screen

    mov $m2_part2_hdr, %rdi
    call heading
    mov $m2_p2_lines, %rdi
    mov $m2_p2_count, %rsi
    call print_lines
    call wait_enter
    call clear_screen

    mov $m2_part3_hdr, %rdi
    call heading
    mov $m2_p3_lines, %rdi
    mov $m2_p3_count, %rsi
    call print_lines
    call wait_enter
    call clear_screen

    mov $m2_part4_hdr, %rdi
    call heading
    mov $m2_p4_lines, %rdi
    mov $m2_p4_count, %rsi
    call print_lines
    call wait_enter
    call clear_screen

    mov $m2_part5_hdr, %rdi
    call heading
    mov $m2_p5_lines, %rdi
    mov $m2_p5_count, %rsi
    call print_lines
    call wait_enter
    call clear_screen

    # exercise(2)
    mov $m2_ex_marker, %rdi
    call print_cstr

    mov $m2_q1_text, %rdi
    mov $m2_q1_correct, %rsi
    mov $m2_q1_why, %rdx
    call question

    mov $m2_q2_text, %rdi
    mov $m2_q2_correct, %rsi
    mov $m2_q2_why, %rdx
    call question

    mov $m2_q3_text, %rdi
    mov $m2_q3_correct, %rsi
    mov $m2_q3_why, %rdx
    call question

    mov $m2_chal_hdr, %rdi
    call print_cstr
    mov $nl2, %rdi
    call print_cstr
    mov $m2_chal_lines, %rdi
    mov $m2_chal_count, %rsi
    call print_lines
    mov $m2_chal_out, %rdi
    call print_cstr
    mov $m2_chal_build, %rdi
    call print_cstr

    mov $m2_chal_ask, %rdi
    call ask_yes
    cmp $1, %rax
    jne .m2_no_solution
    mov $nl2, %rdi
    call print_cstr
    call rule
    mov $m2_chal_sol, %rdi
    call print_cstr
    call rule
.m2_no_solution:

    call wait_enter
    call clear_screen
    # summary(2)

    mov $m2_summary_hdr, %rdi
    call heading
    mov $m2_sum_lines, %rdi
    mov $m2_sum_count, %rsi
    call print_lines
    call wait_enter

    leave
    ret

lesson_03_loops:
    push %rbp
    mov %rsp, %rbp

    mov $m3_title, %rdi
    call title

    mov $m3_part1_hdr, %rdi
    call heading
    mov $m3_p1_lines, %rdi
    mov $m3_p1_count, %rsi
    call print_lines
    call wait_enter
    call clear_screen

    mov $m3_part2_hdr, %rdi
    call heading
    mov $m3_p2_lines, %rdi
    mov $m3_p2_count, %rsi
    call print_lines
    call wait_enter
    call clear_screen

    mov $m3_part3_hdr, %rdi
    call heading
    mov $m3_p3_lines, %rdi
    mov $m3_p3_count, %rsi
    call print_lines
    call wait_enter
    call clear_screen

    mov $m3_part4_hdr, %rdi
    call heading
    mov $m3_p4_lines, %rdi
    mov $m3_p4_count, %rsi
    call print_lines
    call wait_enter
    call clear_screen

    mov $m3_part5_hdr, %rdi
    call heading
    mov $m3_p5_lines, %rdi
    mov $m3_p5_count, %rsi
    call print_lines
    call wait_enter
    call clear_screen

    # exercise(3)
    mov $m3_ex_marker, %rdi
    call print_cstr

    mov $m3_q1_text, %rdi
    mov $m3_q1_correct, %rsi
    mov $m3_q1_why, %rdx
    call question

    mov $m3_q2_text, %rdi
    mov $m3_q2_correct, %rsi
    mov $m3_q2_why, %rdx
    call question

    mov $m3_q3_text, %rdi
    mov $m3_q3_correct, %rsi
    mov $m3_q3_why, %rdx
    call question

    mov $m3_chal_hdr, %rdi
    call print_cstr
    mov $nl2, %rdi
    call print_cstr
    mov $m3_chal_lines, %rdi
    mov $m3_chal_count, %rsi
    call print_lines
    mov $m3_chal_out, %rdi
    call print_cstr
    mov $m3_chal_build, %rdi
    call print_cstr

    mov $m3_chal_ask, %rdi
    call ask_yes
    cmp $1, %rax
    jne .m3_no_solution
    mov $nl2, %rdi
    call print_cstr
    call rule
    mov $m3_chal_sol, %rdi
    call print_cstr
    call rule
.m3_no_solution:

    call wait_enter
    call clear_screen
    # summary(3)

    mov $m3_summary_hdr, %rdi
    call heading
    mov $m3_sum_lines, %rdi
    mov $m3_sum_count, %rsi
    call print_lines
    call wait_enter

    leave
    ret
