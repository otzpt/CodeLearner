; lessons_flow.asm - modules 2 and 3: branching, and loops.
;
; Everything this course's own menu does between reading a keystroke and
; running a module is in these two modules: a compare, a conditional jump,
; and a jump back to the top. main.asm stays the worked example both of
; them point at, the same way module 1 points at ui.asm.

%include "course.inc"

default rel

global lesson_02_branching
global lesson_03_loops

section .rodata

; ---- module 2 ----------------------------------------------------
m2_title: db "MODULE 2 - COMPARING AND BRANCHING", 0
m2_p1_hdr: db "PART 1: cmp sets flags, it does not store anything", 0
m2_p2_hdr: db "PART 2: the jumps, and the signed/unsigned trap", 0
m2_p3_hdr: db "PART 3: the shape of if/else", 0
m2_p4_hdr: db "PART 4: test, and comparing against zero", 0
m2_p5_hdr: db "PART 5: the menu you are standing in", 0

m2_p1:
    ln "  There is no if in assembly. There is a compare, which throws its"
    ln "  own result away, and a jump that reads what the compare left"
    ln "  behind:"
    ln ""
    ln "    cmp rax, 10        ; computes rax - 10, keeps only the flags"
    ln "    jge .big           ; jump if that subtraction says rax >= 10"
    ln ""
    ln "  cmp is a sub whose answer is discarded. What survives is the"
    ln "  FLAGS register: ZF (the result was zero), SF (it was negative),"
    ln "  CF (it borrowed), OF (it overflowed as a signed value). Every"
    ln "  conditional jump is nothing but a named test on those bits."
    ln ""
    ln "  The operand order matters most right here. cmp rax, 10 is"
    ln '  rax - 10, not 10 - rax. Read it as "compare rax against 10" and'
    ln "  the jump after it means what it looks like it means."
    db 0

m2_p2:
    ln "  Equality has one pair, and it is the easy one:"
    ln ""
    ln "    je / jz     jump if equal (ZF set)"
    ln "    jne / jnz   jump if not equal"
    ln ""
    ln "  Ordering has two complete sets, and picking the wrong one is a"
    ln "  bug the assembler will never mention:"
    ln ""
    ln "    signed:    jl  jle  jg  jge      (less/greater)"
    ln "    unsigned:  jb  jbe  ja  jae      (below/above)"
    ln ""
    ln "  Same bytes in the register, opposite answers. Put -1 in rax and"
    ln "  compare it against 1:"
    ln ""
    ln "    mov rax, -1"
    ln "    cmp rax, 1"
    ln "    jl  ...            ; TAKEN: as a signed value, -1 < 1"
    ln "    ja  ...            ; ALSO TAKEN: as unsigned, -1 is huge"
    ln ""
    ln "  -1 in a 64-bit register is the bit pattern 0xFFFFFFFFFFFFFFFF."
    ln "  Read as signed that is -1; read as unsigned it is the largest"
    ln "  value a register can hold. The register does not remember which"
    ln "  one you meant -- the jump you pick is where that decision lives."
    db 0

m2_p3:
    ln "  An if in C compiles to a jump that skips the body. The condition"
    ln "  gets inverted on the way: you jump AWAY when it is false."
    ln ""
    ln "    C:                        assembly:"
    ln ""
    ln "    if (n >= 10) {                cmp rax, 10"
    ln "        big();                    jl .else_part     ; inverted"
    ln "    } else {                      call big"
    ln "        small();                  jmp .endif"
    ln "    }                         .else_part:"
    ln "                                  call small"
    ln "                              .endif:"
    ln ""
    ln "  The unconditional jmp at the end of the if-body is not optional."
    ln "  Leave it out and the if-body falls straight through into the"
    ln "  else-body and both run. Nothing warns you; the program just"
    ln "  does twice what it should do once."
    db 0

m2_p4:
    ln "  cmp rax, 0 works. Nobody writes it:"
    ln ""
    ln "    test rax, rax"
    ln "    jz .it_was_zero"
    ln ""
    ln "  test ANDs its two operands and throws the result away, keeping"
    ln "  the flags -- so a register ANDed with itself sets ZF exactly when"
    ln "  the register is zero. It assembles a byte shorter than the cmp,"
    ln "  which is why every compiler's output is full of it."
    ln ""
    ln "  Worth recognizing for the same reason module 1 read gcc -S output:"
    ln "  test eax, eax followed by a jump is how a compiled if (p != NULL)"
    ln "  and if (n) both come out the other side."
    db 0

m2_p5:
    ln "  assembly/src/main.asm reads one line, turns it into the number r15,"
    ln "  then decides what to do with the same instructions this module just"
    ln "  described:"
    ln ""
    ln "    cmp r15, 0"
    ln "    je .quit"
    ln "    cmp r15, module_count"
    ln "    jg .invalid"
    ln ""
    ln "  0 quits, anything past the last module is not a module, and both are"
    ln "  a compare followed by a jump on its flags. Turning the typed digits"
    ln "  into r15 is the loop module 4 teaches. One piece of that loop is the"
    ln "  unsigned half of PART 2:"
    ln ""
    ln "    sub eax, '0'"
    ln "    cmp eax, 9"
    ln "    ja .invalid"
    ln ""
    ln "  A byte below '0' wraps to a huge unsigned number, so a single ja"
    ln '  rejects both "too low" and "too high".'
    db 0

m2_exercise: db `\n  >> EXERCISE - MODULE 2\n  ------------------------------------------------------\n`, 0

m2_q1_text:   db `After cmp rax, 10, which jump runs when rax holds 25 --\n  jl or jg?`, 0
m2_q1_answer: db "jg", 0
m2_q1_why:    db `cmp rax, 10 computes rax - 10. 25 - 10 is positive, so the\n             greater-than jump is the one taken.`, 0

m2_q2_text:   db `rax holds -1. You compare it against 1 and use ja. Is the jump\n  taken? (yes/no)`, 0
m2_q2_answer: db "yes", 0
m2_q2_why:    db `ja is the UNSIGNED test, and -1 unsigned is 0xFFFFFFFFFFFFFFFF,\n             the largest value there is. jl would have been the signed one.`, 0

m2_q3_text:   db `Which instruction is the one-word idiom for "is this register\n  zero"? (one word)`, 0
m2_q3_answer: db "test", 0
m2_q3_why:    db `test rax, rax ANDs the register with itself, which sets ZF\n             exactly when every bit is 0, one byte shorter than cmp rax, 0.`, 0

m2_challenge:
    ln ""
    ln "  >> WRITE THIS YOURSELF, in a real file"
    ln '  Put a number in rax with mov, then print "big\n" if it is 10 or'
    ln '  more and "small\n" if it is not. Exit 0 either way. Syscalls'
    ln "  only -- write (1) and exit (60), the same two module 1 used."
    ln ""
    ln "  With mov rax, 25 at the top it must print:"
    ln ""
    ln "      big"
    ln ""
    ln "  and with mov rax, 3, exactly:"
    ln ""
    ln "      small"
    ln ""
    ln "  Assemble and link with:"
    ln "    nasm -f elf64 prog.asm -o prog.o && ld prog.o -o prog"
    db 0

m2_summary_hdr: db "SUMMARY", 0
m2_summary:
    ln "   - cmp computes a subtraction and keeps only the flags; the jump"
    ln "     after it is what reads them"
    ln "   - cmp rax, 10 is rax - 10, not 10 - rax: the first operand is the one compared"
    ln "   - jl/jg are signed, jb/ja are unsigned, the assembler will not"
    ln "     tell you which one the value in the register wanted"
    ln "   - an if is a jump on the INVERTED condition, plus a jmp at the"
    ln "     end of the if-body so it does not fall into the else-body"
    ln '   - test rax, rax is the idiomatic "is it zero"'
    db 0

m2_solution:
    incbin "02.asm"
    db 0

; ---- module 3 ----------------------------------------------------
m3_title: db "MODULE 3 - LOOPS", 0
m3_p1_hdr: db "PART 1: a loop is a label and a jump backwards", 0
m3_p2_hdr: db "PART 2: test at the top, or test at the bottom", 0
m3_p3_hdr: db "PART 3: counting down is cheaper, and the loop trap", 0
m3_p4_hdr: db "PART 4: walking an array, from this course's menu", 0
m3_p5_hdr: db "PART 5: indexed addressing, so you do not have to add", 0

m3_p1:
    ln "  There is no while and no for. A loop is a label you can jump back"
    ln "  to, a body, and a conditional jump that decides whether to go"
    ln "  round again -- module 2's instructions, pointed upwards:"
    ln ""
    ln "    mov rcx, 0             ; i = 0"
    ln "  .loop:"
    ln "    cmp rcx, 5             ; i < 5 ?"
    ln "    jge .done              ; inverted, exactly like an if"
    ln "    <body>"
    ln "    inc rcx                ; i++"
    ln "    jmp .loop"
    ln "  .done:"
    ln ""
    ln "  That is a while loop, whole. The three pieces a for loop writes"
    ln "  on one line -- init, test, step -- are just in three places here."
    db 0

m3_p2:
    ln "  The loop above tests first, so a body that should not run at all"
    ln "  runs zero times. Moving the test to the bottom makes it a"
    ln "  do-while, and costs one jump less per iteration:"
    ln ""
    ln "  .loop:"
    ln "    <body>"
    ln "    inc rcx"
    ln "    cmp rcx, 5"
    ln "    jl .loop               ; one jump, not two"
    ln ""
    ln "  The top-tested version runs a conditional jump AND an"
    ln "  unconditional one every lap. This runs one. It is also wrong"
    ln "  whenever the count can be zero, because the body has already"
    ln "  happened before anything is checked -- which is why a compiler"
    ln "  emits the top test once, before the loop, and the bottom-tested"
    ln "  shape for every lap after it."
    db 0

m3_p3:
    ln "  Counting towards zero drops the cmp entirely, because dec sets"
    ln "  ZF by itself:"
    ln ""
    ln "    mov rcx, 5"
    ln "  .loop:"
    ln "    <body>"
    ln "    dec rcx                ; sets ZF when rcx reaches 0"
    ln "    jnz .loop"
    ln ""
    ln "  x86 also has an actual instruction called loop, which decrements"
    ln "  rcx and jumps if it is still non-zero -- the two lines above in"
    ln "  one. Do not reach for it:"
    ln ""
    ln "    - it is microcoded, and slower on every current CPU than the"
    ln "      dec/jnz pair it replaces"
    ln "    - it hardcodes rcx, and rcx is caller-saved: any call in the"
    ln "      body is free to destroy your counter and usually does"
    ln ""
    ln "  That second one is the real trap. A counter that has to survive"
    ln "  calls belongs in a callee-saved register -- rbx, or r12 through"
    ln "  r15 -- pushed on entry and popped before ret."
    db 0

m3_p4:
    ln "  The menu in main.asm walks a table of modules -- one entry per"
    ln "  lesson, three 8-byte values each. It is a real loop over a real"
    ln "  array, here cut down to its skeleton:"
    ln ""
    ln "    mov r12, modules       ; array base   -- callee-saved, on"
    ln "    mov r13, 1             ; entry number -- purpose (PART 3)"
    ln "  .item_loop:"
    ln "    cmp r13, module_count"
    ln "    jg .items_done"
    ln "    mov rdi, [r12]         ; load one pointer out of the entry"
    ln "    call print_cstr        ; clobbers rax, rcx, rdx, rsi, rdi..."
    ln "    add r12, ENTRY_SIZE    ; ...but not r12/r13"
    ln "    inc r13"
    ln "    jmp .item_loop"
    ln ""
    ln "  ENTRY_SIZE is 24: three 8-byte values. Stepping by 1 would land in"
    ln "  the middle of one, and the load would produce garbage that is still"
    ln "  a perfectly valid-looking address."
    db 0

m3_p5:
    ln "  x86 can do the arithmetic inside the operand:"
    ln ""
    ln "    mov al, [rdi+rcx]            ; al = rdi[rcx]        (bytes)"
    ln "    mov rax, [rdi+rcx*8]         ; rax = rdi[rcx]  (8-byte slots)"
    ln ""
    ln "  The form is [base + index*scale], scale being 1, 2, 4, or 8 --"
    ln "  one element's size. It is one instruction, it does not touch"
    ln "  flags, and it leaves the base pointer alone so the loop counter"
    ln "  stays a plain index instead of a second moving pointer."
    ln ""
    ln "  strlen and streq in ui.asm both walk their strings this way. Your"
    ln "  compiled C does too, which is why array[i] costs no instructions"
    ln "  of its own in an -O0 listing."
    db 0

m3_exercise: db `\n  >> EXERCISE - MODULE 3\n  ------------------------------------------------------\n`, 0

m3_q1_text:   db `Which register set survives a call -- caller-saved like rcx, or\n  callee-saved like r12? (one word: rcx or r12)`, 0
m3_q1_answer: db "r12", 0
m3_q1_why:    db `r12-r15, rbx, and rbp are callee-saved: a function that wants\n             them must restore them, so your counter is still there after.`, 0

m3_q2_text:   db `Which flag does dec set when the counter reaches zero?\n  (two letters)`, 0
m3_q2_answer: db "zf", 0
m3_q2_why:    db `dec sets ZF on a zero result, so dec/jnz is a complete loop\n             test with no cmp in front of it.`, 0

m3_q3_text:   db `An array of 8-byte pointers: how much do you add to the base to\n  reach the next element? (a number)`, 0
m3_q3_answer: db "8", 0
m3_q3_why:    db `Stepping by 1 lands inside an element, and the load that follows\n             returns a valid-looking address made of two halves.`, 0

m3_challenge:
    ln ""
    ln "  >> WRITE THIS YOURSELF, in a real file"
    ln "  Print the digits 1 to 5, one per line, with a loop. No libc, so"
    ln "  turn the counter into a character yourself: add the character '0'"
    ln "  to it, the reverse of the subtraction main.asm does to each digit"
    ln ""
    ln "  It must print, exactly:"
    ln ""
    ln "      1"
    ln "      2"
    ln "      3"
    ln "      4"
    ln "      5"
    ln ""
    ln "  Assemble and link with:"
    ln "    nasm -f elf64 prog.asm -o prog.o && ld prog.o -o prog"
    db 0

m3_summary_hdr: db "SUMMARY", 0
m3_summary:
    ln "   - a loop is a label, a body, and a conditional jump back to it"
    ln "   - top-tested runs the body zero times when the count is zero;"
    ln "     bottom-tested always runs it at least once, for one jump less"
    ln "   - dec sets ZF, so counting down to zero needs no cmp at all"
    ln "   - the loop instruction is slower than dec/jnz and hardcodes rcx,"
    ln "     which any call in the body is allowed to clobber"
    ln "   - a counter that outlives a call goes in rbx or r12-r15"
    ln "   - (base, index, scale) indexes an array in one operand"
    db 0

m3_solution:
    incbin "03.asm"
    db 0

section .text

lesson_02_branching:
    mov rdi, m2_title
    call title

    part m2_p1_hdr, m2_p1
    part m2_p2_hdr, m2_p2
    part m2_p3_hdr, m2_p3
    part m2_p4_hdr, m2_p4
    part m2_p5_hdr, m2_p5

    ; exercise(2)
    mov rdi, m2_exercise
    call print_cstr
    quiz m2_q1_text, m2_q1_answer, m2_q1_why
    quiz m2_q2_text, m2_q2_answer, m2_q2_why
    quiz m2_q3_text, m2_q3_answer, m2_q3_why
    challenge m2_challenge, m2_solution

    ; summary(2)
    summary m2_summary_hdr, m2_summary
    ret

lesson_03_loops:
    mov rdi, m3_title
    call title

    part m3_p1_hdr, m3_p1
    part m3_p2_hdr, m3_p2
    part m3_p3_hdr, m3_p3
    part m3_p4_hdr, m3_p4
    part m3_p5_hdr, m3_p5

    ; exercise(3)
    mov rdi, m3_exercise
    call print_cstr
    quiz m3_q1_text, m3_q1_answer, m3_q1_why
    quiz m3_q2_text, m3_q2_answer, m3_q2_why
    quiz m3_q3_text, m3_q3_answer, m3_q3_why
    challenge m3_challenge, m3_solution

    ; summary(3)
    summary m3_summary_hdr, m3_summary
    ret

; Tells the linker this object does not need an executable stack.
section .note.GNU-stack noalloc noexec nowrite progbits
