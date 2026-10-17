; lessons_memory.asm - modules 5 and 6: memory and addressing, and the stack.
;
; The first modules that run things while the lesson is on screen. Every
; number printed by a live_* line below is read from this running program,
; so the addresses and register values on screen are the machine's, not
; the author's. Text screens are labels followed by ln lines (course.inc).

%include "course.inc"

default rel

global lesson_05_memory
global lesson_06_stack

section .rodata

; ---- module 5 -----------------------------------------------------
m5_title:   db "MODULE 5 - MEMORY AND ADDRESSING", 0
m5_p1_hdr:  db "PART 1: a label is an address, and [label] is what is there", 0
m5_p2_hdr:  db "PART 2: sections, and which of them you may write to", 0
m5_p3_hdr:  db "PART 3: sizes, and the half of a register you did not write", 0
m5_p4_hdr:  db "PART 4: [base + index*scale], and lea", 0
m5_p5_hdr:  db "PART 5: byte order", 0

m5_p1:
    ln "  Every label is a number: the address the linker gave it. The same"
    ln "  name means two different things depending on the brackets:"
    ln ""
    ln "    mov rax, counter       ; rax = the ADDRESS of counter"
    ln "    mov rax, [counter]     ; rax = the 8 bytes stored AT that address"
    ln ""
    ln "  Leaving the brackets off is the commonest first bug: the program"
    ln "  assembles, runs, and does arithmetic on an address instead of a"
    ln "  value. counter below is a variable holding 42; both forms, read"
    ln "  by this lesson while it runs:"
    ln ""
    db 0
m5_p1_a:   db "    mov rax, counter     ->  rax = ", 0
m5_p1_b:   db "    mov rax, [counter]   ->  rax = ", 0
m5_p1_c:
    ln ""
    ln "  The first number is a place; the second is what the place holds."
    ln "  Other assemblers draw this line differently -- GNU's Intel mode"
    ln "  wants the word offset to get the address -- so when you read"
    ln "  someone else's listing, check which one you are looking at."
    db 0

m5_p2:
    ln "  A program's memory is not one flat area. The assembler files every"
    ln "  label under a section, and the linker gives each kind its own"
    ln "  permissions:"
    ln ""
    ln "    section .text      code                      read + execute"
    ln "    section .rodata    constants, strings        read only"
    ln "    section .data      variables with a value    read + write"
    ln "    section .bss       variables that start 0    read + write"
    ln ""
    ln "  .bss is the odd one: it takes no room in the file; the file says"
    ln "  'reserve this many zeroed bytes' and the loader does. Four labels"
    ln "  from this very program, one per section, read now:"
    ln ""
    db 0
m5_p2_a:   db "    .text    lesson_05_memory  ", 0
m5_p2_b:   db "    .rodata  m5_title          ", 0
m5_p2_c:   db "    .data    m5_var            ", 0
m5_p2_d:   db "    .bss     m5_buf            ", 0
m5_p2_e:
    ln ""
    ln "  Writing to .rodata is a crash, not a quiet bug. Assembled and run"
    ln "  while writing this module, with msg: db 'hi' in .rodata and"
    ln "  mov byte [msg], 'H' as the code:"
    ln ""
    ln "    $ ./rodata_write"
    ln "    Segmentation fault"
    ln ""
    ln "  (Exit status 139: 128 plus signal 11, SIGSEGV. A shell that keeps core"
    ln "  dumps adds '(core dumped)'.)"
    db 0

m5_p3:
    ln "  Memory has no types. [x] is just an address; how many bytes the"
    ln "  instruction moves comes from its other operand, or from you:"
    ln ""
    ln "    byte  1 byte   al      db       word   2 bytes  ax    dw"
    ln "    dword 4 bytes  eax     dd       qword  8 bytes  rax   dq"
    ln ""
    ln "  When no register is involved, the size has to be written. NASM 3.01"
    ln "  does not refuse a bare store of a number, it picks ONE byte:"
    ln ""
    ln "    mov [x], 5               ->   mov BYTE PTR [rip+0x0],0x5"
    ln "    mov qword [x], 5         ->   mov QWORD PTR [rip+0x0],0x5"
    ln ""
    ln "  (objdump -d -M intel on the real object files.) x holds -1, every"
    ln "  bit set, before each store below. Read by this lesson, now:"
    ln ""
    db 0
m5_p3_a:   db "    mov byte  [x], 5   ->  x = ", 0
m5_p3_b:   db "    mov qword [x], 5   ->  x = ", 0
m5_p3_c:
    ln ""
    ln "  Registers have the same trap in a stranger form. Starting from"
    ln "  rax = -1 each time:"
    ln ""
    db 0
m5_p3_d:   db "    mov eax, 5   ->  rax = ", 0
m5_p3_e:   db "    mov ax, 5    ->  rax = ", 0
m5_p3_f:   db "    mov al, 5    ->  rax = ", 0
m5_p3_g:
    ln ""
    ln "  A write to a 32-bit register zeroes the upper half. A write to a"
    ln "  16-bit or 8-bit one leaves the rest alone. The rule is on purpose:"
    ln "  a 32-bit write does not depend on the register's old value. NASM"
    ln "  relies on it for you: it assembles mov rax, 5 as the five-byte"
    ln "  mov eax, 5 (ndisasm shows it), correct only because of this rule."
    db 0

m5_p4:
    ln "  A memory operand can do the address arithmetic itself:"
    ln ""
    ln "    [ base + index*scale + displacement ]"
    ln ""
    ln "  base and index are 64-bit registers, scale is 1, 2, 4 or 8 (the"
    ln "  size of one element), displacement is a constant. Every part is"
    ln "  optional. For an array of dwords starting at table:"
    ln ""
    ln "    mov eax, [table + rcx*4]       ; eax = table[rcx]"
    ln ""
    ln "  One instruction, no multiply, flags untouched. The array below is"
    ln "  1, 2, 3, 4, 5 in dwords, summed by a loop that does exactly that:"
    ln ""
    db 0
m5_p4_a:   db "    sum of the five dwords        = ", 0
m5_p4_b:
    ln ""
    ln "  lea (load effective address) is the same arithmetic WITHOUT the"
    ln "  load: it stores the address the operand works out to, and reads"
    ln "  nothing from memory. Compilers use it as a cheap multiply-and-add:"
    ln ""
    ln "    lea rax, [rbx + rbx*4]         ; rax = rbx * 5, no mul, no flags"
    ln ""
    db 0
m5_p4_c:   db "    with rbx = 7                  = ", 0
m5_p4_d:
    ln ""
    ln "  The brackets in a lea are a lie told for convenience: nothing is"
    ln "  read. Using mov there instead would read memory at address 35."
    db 0

m5_p5:
    ln "  x86 stores a multi-byte number with its LEAST significant byte at"
    ln "  the lowest address: little-endian. The dword 0x11223344, declared"
    ln "  with dd, sits in memory as the four bytes 44 33 22 11. Read by"
    ln "  this lesson one byte at a time, lowest address first:"
    ln ""
    db 0
m5_p5_a:   db "    address + 0   = ", 0
m5_p5_b:   db "    address + 1   = ", 0
m5_p5_c:   db "    address + 2   = ", 0
m5_p5_d:   db "    address + 3   = ", 0
m5_p5_e:
    ln ""
    ln "  You will not notice it in registers, which have no 'order'. You"
    ln "  will the moment you look at memory, a file, a disk sector or a"
    ln "  network packet byte by byte, and again in module 11: a screen"
    ln "  cell is a word, 0x0F41, and the character 'A' (0x41) is the low"
    ln "  byte, so it is the first one in memory and the colour 0x0F the"
    ln "  second."
    db 0

m5_summary_hdr: db "SUMMARY", 0
m5_summary:
    ln "   - a label is an address: mov rax, label loads it, mov rax,"
    ln "     [label] loads what is stored there"
    ln "   - .text runs, .rodata is read-only, .data and .bss are writable;"
    ln "     .bss is zero-filled and takes no room in the file"
    ln "   - a memory store of a number needs a size keyword: without one"
    ln "     NASM 3.01 stores a single byte"
    ln "   - writing eax zeroes the top half of rax; writing ax or al does"
    ln "     not touch the rest"
    ln "   - [base + index*scale + displacement] with scale 1, 2, 4 or 8;"
    ln "     lea computes the address and loads nothing"
    ln "   - x86 is little-endian: the low byte is at the lowest address"
    db 0

m5_exercise:
    db `\n  >> EXERCISE - MODULE 5\n  ------------------------------------------------------\n`, 0
m5_q1_text:   db `mov rax, [msg] puts what in rax: the address of msg, or the\n  8 bytes stored there? (one word: address or value)`, 0
m5_q1_answer: db "value", 0
m5_q1_why:    db `The brackets mean 'the contents of'. mov rax, msg, with none,\n             is the one that loads the address.`, 0
m5_q2_text:   db `rax holds -1. After mov eax, 5, what are the top 32 bits of\n  rax? (a number)`, 0
m5_q2_answer: db "0", 0
m5_q2_why:    db `Any write to a 32-bit register clears the upper half. mov ax, 5\n             or mov al, 5 would have left it all ones.`, 0
m5_q3_text:   db `The dword 0x11223344 is in memory. Which byte is at the lowest\n  address? (two hex digits)`, 0
m5_q3_answer: db "44", 0
m5_q3_why:    db `Little-endian: the least significant byte comes first, so memory\n             reads 44 33 22 11.`, 0

m5_challenge:
    ln ""
    ln "  >> WRITE THIS YOURSELF, in a real file"
    ln ""
    ln "  Reverse a string with an index register. Put the eight bytes"
    ln "  'stressed' in .data, then build the reversed string in a .bss"
    ln "  buffer by reading from the end with [label + register], and"
    ln "  print it with a newline. No loop instruction tricks needed: a"
    ln "  counter, one load, one store."
    ln ""
    ln "  It must print:"
    ln ""
    ln "      desserts"
    ln ""
    ln "  Assemble and link with:"
    ln "    nasm -f elf64 prog.asm -o prog.o && ld prog.o -o prog"
    db 0

m5_solution:
    incbin "05.asm"
    db 0

; ---- module 6 -----------------------------------------------------
m6_title:   db "MODULE 6 - THE STACK, AND CALLING A FUNCTION", 0
m6_p1_hdr:  db "PART 1: the stack is memory with a pointer", 0
m6_p2_hdr:  db "PART 2: call and ret", 0
m6_p3_hdr:  db "PART 3: a stack frame, and arguments past the sixth", 0
m6_p4_hdr:  db "PART 4: who has to put a register back", 0
m6_p5_hdr:  db "PART 5: recursion, and how much stack there is", 0

m6_p1:
    ln "  The stack is ordinary memory. What makes it a stack is one register,"
    ln "  rsp, which holds the address of the top item, and two instructions:"
    ln ""
    ln "    push rax       ; rsp = rsp - 8, then store rax at [rsp]"
    ln "    pop  rbx       ; load rbx from [rsp], then rsp = rsp + 8"
    ln ""
    ln "  The stack grows toward LOWER addresses, which surprises everyone"
    ln "  once: pushing makes rsp smaller. Each slot is 8 bytes in 64-bit"
    ln "  code. Read by this lesson while it runs:"
    ln ""
    db 0
m6_p1_a:   db "    rsp before a push             = ", 0
m6_p1_b:   db "    rsp after  push 0x1111        = ", 0
m6_p1_c:   db "    difference (before - after)   = ", 0
m6_p1_d:   db "    [rsp], the value pushed       = ", 0
m6_p1_e:   db "    rsp after  pop rax            = ", 0
m6_p1_f:
    ln ""
    ln "  pop does not erase anything. The 0x1111 is still in memory below"
    ln "  rsp; it is simply no longer on the stack, and the next push will"
    ln "  overwrite it. Stack memory is borrowed, never cleared."
    db 0

m6_p2:
    ln "  A function call is a jump that remembers where to come back to."
    ln "  call does two things, ret undoes one of them:"
    ln ""
    ln "    call target    ; push the address of the NEXT instruction, jump"
    ln "    ret            ; pop that address into rip"
    ln ""
    ln "  The return address is a plain 8-byte value on the stack, which is"
    ln "  why a stray push or pop in a function is fatal: ret will treat"
    ln "  whatever is on top as the place to go. Here a function reads the"
    ln "  top of the stack the moment it is entered, and hands it back:"
    ln ""
    db 0
m6_p2_a:   db "    return address on the stack   = ", 0
m6_p2_b:   db "    address after the call        = ", 0
m6_p2_c:   db "    rsp mod 16, before the call   = ", 0
m6_p2_d:   db "    rsp mod 16, inside the callee = ", 0
m6_p2_e:
    ln ""
    ln "  The first two are the same number, by construction. The last two"
    ln "  differ by 8: the System V ABI says rsp must be a multiple of 16"
    ln "  at the moment of a call, so inside the callee, with the return"
    ln "  address pushed, it is always 8 more than a multiple of 16. Compiled"
    ln "  code that uses aligned SSE moves on the stack faults when it is not."
    db 0

m6_p3:
    ln "  A function that needs local variables, or that wants rsp to stay"
    ln "  put while it pushes things, builds a frame:"
    ln ""
    ln "    push rbp            ; save the caller's frame pointer"
    ln "    mov  rbp, rsp       ; rbp is now fixed for this call"
    ln "    sub  rsp, 16        ; room for two 8-byte locals"
    ln "    ...                 ; locals at [rbp-8], [rbp-16]"
    ln "    leave               ; mov rsp, rbp  then  pop rbp"
    ln "    ret"
    ln ""
    ln "  Only the first six integer arguments travel in registers. The"
    ln "  seventh is pushed by the caller, and the callee finds it above"
    ln "  the saved rbp and the return address:"
    ln ""
    ln "    [rbp+16]   7th argument         [rbp+8]   return address"
    ln "    [rbp]      the caller's rbp     [rbp-8]   first local"
    ln ""
    ln "  A function sum7(1, 2, 3, 4, 5, 6, 7) that adds them up, called"
    ln "  that way by this lesson, returns:"
    ln ""
    db 0
m6_p3_a:   db "    sum7(1, 2, 3, 4, 5, 6, 7)     = ", 0
m6_p3_b:
    ln ""
    ln "  The caller removes what it pushed (add rsp, 8 after the call); the"
    ln "  callee never does. Whoever pushed it cleans it up."
    db 0

m6_p4:
    ln "  A callee may overwrite some registers and must put others back."
    ln "  That split is the whole calling convention:"
    ln ""
    ln "    caller-saved   rax rcx rdx rsi rdi r8-r11   a call may destroy them"
    ln "    callee-saved   rbx rbp r12-r15              a call must restore them"
    ln ""
    ln "  Two functions here, both adding to rbx. m6_good pushes rbx first and"
    ln "  pops it before ret; m6_bad does not. rbx held 0x1111 before each:"
    ln ""
    db 0
m6_p4_a:   db "    after m6_good, rbx   = ", 0
m6_p4_b:   db "    after m6_bad,  rbx   = ", 0
m6_p4_c:
    ln ""
    ln "  m6_bad broke its caller without a sound. The assembler cannot see"
    ln "  it, and neither can the CPU; the symptom shows up later, somewhere"
    ln "  else. And the syscall instruction has a convention of its own: it"
    ln "  destroys rcx and r11, storing the return address in rcx and the"
    ln "  flags in r11. rcx held 0x1234 before a write() of zero bytes:"
    ln ""
    db 0
m6_p4_d:   db "    rcx after the syscall            = ", 0
m6_p4_e:   db "    address after the syscall        = ", 0
m6_p4_f:
    ln ""
    ln "  (Which is why a value that has to survive a read or a write lives"
    ln "  in rbx or r12-r15, as module 4 said, and never in rcx.)"
    db 0

m6_p5:
    ln "  A function that calls itself is a function like any other: every"
    ln "  call pushes its own return address and gets its own copy of the"
    ln "  saved registers. A factorial that keeps n on the stack across the"
    ln "  recursive call, run by this lesson:"
    ln ""
    ln "    fact:  cmp rdi, 1 / jbe .base"
    ln "           push rdi / dec rdi / call fact / pop rdi"
    ln "           imul rax, rdi / ret"
    ln ""
    db 0
m6_p5_a:   db "    fact(5)                            = ", 0
m6_p5_b:   db "    stack used by fact(5), in bytes    = ", 0
m6_p5_c:   db "    this process's stack limit, bytes  = ", 0
m6_p5_d:
    ln ""
    ln "  72 is four levels that recurse, 16 bytes each (the return address"
    ln "  and the saved n), plus the return address of the last call. The"
    ln "  limit is the kernel's (getrlimit, syscall 97; 'ulimit -s' reports"
    ln "  the same number in kilobytes). A recursion with no base case, or a"
    ln "  huge one, runs off the end of that space and the process dies with"
    ln "  a segmentation fault. The stack does not grow to fit the problem."
    db 0

m6_summary_hdr: db "SUMMARY", 0
m6_summary:
    ln "   - push subtracts 8 from rsp and stores; pop loads and adds 8; the"
    ln "     stack grows toward lower addresses and pop erases nothing"
    ln "   - call pushes the return address and jumps; ret pops it into rip;"
    ln "     a stray push or pop makes ret go somewhere it should not"
    ln "   - rsp is a multiple of 16 at a call, so 8 off inside the callee"
    ln "   - a frame is push rbp / mov rbp, rsp / sub rsp, N ... leave / ret;"
    ln "     locals are [rbp-8].., the 7th argument is [rbp+16]"
    ln "   - rbx, rbp, r12-r15 are callee-saved: put them back. Everything"
    ln "     else a call may destroy, and syscall destroys rcx and r11"
    ln "   - the stack is a fixed size; recursion needs a base case"
    db 0

m6_exercise:
    db `\n  >> EXERCISE - MODULE 6\n  ------------------------------------------------------\n`, 0
m6_q1_text:   db `Does the stack grow toward higher or lower addresses?\n  (one word: higher or lower)`, 0
m6_q1_answer: db "lower", 0
m6_q1_why:    db `push subtracts 8 from rsp. The top of the stack is the lowest\n             address in use.`, 0
m6_q2_text:   db `ret takes the top of the stack and loads it into which\n  register? (two or three letters)`, 0
m6_q2_answer: db "rip", 0
m6_q2_why:    db `call pushed the address of the next instruction; ret pops it\n             into the instruction pointer, rip, and execution resumes there.`, 0
m6_q3_text:   db `Which of rbx and rcx must a function put back before it\n  returns? (one register)`, 0
m6_q3_answer: db "rbx", 0
m6_q3_why:    db `rbx is callee-saved. rcx is caller-saved: the callee may use it\n             freely, so a caller that needs rcx kept must save it itself.`, 0

m6_challenge:
    ln ""
    ln "  >> WRITE THIS YOURSELF, in a real file"
    ln ""
    ln "  Write a recursive fib(n), fib(0) = 0, fib(1) = 1, fib(n) ="
    ln "  fib(n-1) + fib(n-2), and exit with the value of fib(10) as the"
    ln "  exit status. Mind the registers: the result of the first call has"
    ln "  to survive the second."
    ln ""
    ln "  Run it and ask the shell for the exit status. It must print:"
    ln ""
    ln "      $ ./prog; echo $?"
    ln "      55"
    ln ""
    ln "  Assemble and link with:"
    ln "    nasm -f elf64 prog.asm -o prog.o && ld prog.o -o prog"
    db 0

m6_solution:
    incbin "06.asm"
    db 0

section .data
m5_counter: dq 42
m5_var:     dq -1
m5_table:   dd 1, 2, 3, 4, 5
m5_order:   dd 0x11223344

m6_deepest: dq 0

section .bss
m5_buf:     resb 16
m6_rlimit:  resq 2

section .text

lesson_05_memory:
    push rbx
    mov rdi, m5_title
    call title

    part_begin m5_p1_hdr, m5_p1
    live_hex m5_p1_a, m5_counter        ; no brackets: the address
    live_dec m5_p1_b, [m5_counter]      ; brackets: the value
    mov rdi, m5_p1_c
    call print_cstr
    part_end

    part_begin m5_p2_hdr, m5_p2
    live_hexw m5_p2_a, lesson_05_memory, 6
    live_hexw m5_p2_b, m5_title, 6
    live_hexw m5_p2_c, m5_var, 6
    live_hexw m5_p2_d, m5_buf, 6
    mov rdi, m5_p2_e
    call print_cstr
    part_end

    part_begin m5_p3_hdr, m5_p3
    mov qword [m5_var], -1
    mov byte [m5_var], 5
    live_hexw m5_p3_a, [m5_var], 16
    mov qword [m5_var], -1
    mov qword [m5_var], 5
    live_hexw m5_p3_b, [m5_var], 16
    mov rdi, m5_p3_c
    call print_cstr
    mov rax, -1
    mov eax, 5
    live_hexw m5_p3_d, rax, 16
    mov rax, -1
    mov ax, 5
    live_hexw m5_p3_e, rax, 16
    mov rax, -1
    mov al, 5
    live_hexw m5_p3_f, rax, 16
    mov rdi, m5_p3_g
    call print_cstr
    part_end

    part_begin m5_p4_hdr, m5_p4
    xor eax, eax                        ; the sum
    xor ecx, ecx                        ; the index
.sum:
    cmp rcx, 5
    jge .summed
    add eax, [m5_table + rcx*4]         ; one element, no multiply
    inc rcx
    jmp .sum
.summed:
    live_dec m5_p4_a, rax
    mov rdi, m5_p4_b
    call print_cstr
    mov rbx, 7
    lea rax, [rbx + rbx*4]
    live_dec m5_p4_c, rax
    mov rdi, m5_p4_d
    call print_cstr
    part_end

    part_begin m5_p5_hdr, m5_p5
    movzx eax, byte [m5_order + 0]
    live_hexw m5_p5_a, rax, 2
    movzx eax, byte [m5_order + 1]
    live_hexw m5_p5_b, rax, 2
    movzx eax, byte [m5_order + 2]
    live_hexw m5_p5_c, rax, 2
    movzx eax, byte [m5_order + 3]
    live_hexw m5_p5_d, rax, 2
    mov rdi, m5_p5_e
    call print_cstr
    part_end

    ; exercise(5)
    mov rdi, m5_exercise
    call print_cstr
    quiz m5_q1_text, m5_q1_answer, m5_q1_why
    quiz m5_q2_text, m5_q2_answer, m5_q2_why
    quiz m5_q3_text, m5_q3_answer, m5_q3_why
    challenge m5_challenge, m5_solution

    ; summary(5)
    summary m5_summary_hdr, m5_summary
    pop rbx
    ret

; ---- module 6 code --------------------------------------------------

; m6_probe() -> rax = the return address on the stack, rdx = rsp inside.
m6_probe:
    mov rax, [rsp]              ; the return address call just pushed
    mov rdx, rsp
    ret

; m6_sum7(a, b, c, d, e, f, g) -> rax = a + b + ... + g. The first six
; arrive in registers, the seventh on the stack, above the frame.
m6_sum7:
    push rbp
    mov rbp, rsp
    sub rsp, 16                 ; room for locals (not needed here, shown anyway)
    mov rax, rdi
    add rax, rsi
    add rax, rdx
    add rax, rcx
    add rax, r8
    add rax, r9
    add rax, [rbp+16]           ; saved rbp (8) + return address (8) above rbp
    leave
    ret

m6_good:
    push rbx                    ; callee-saved: put it back before returning
    add rbx, 1
    pop rbx
    ret

m6_bad:
    add rbx, 1                  ; callee-saved, and not put back
    ret

; m6_fact(n in rdi) -> rax = n!. Records how deep the stack got.
m6_fact:
    cmp rdi, 1
    jbe .base
    push rdi                    ; n has to survive the recursive call
    dec rdi
    call m6_fact
    pop rdi
    imul rax, rdi
    ret
.base:
    mov rax, 1
    mov [m6_deepest], rsp       ; the lowest rsp any level reaches
    ret

lesson_06_stack:
    push rbx
    push r12
    push r13
    push r14
    sub rsp, 8                  ; four pushes left rsp 8 off a multiple of 16
    mov rdi, m6_title
    call title

    part_begin m6_p1_hdr, m6_p1
    mov r12, rsp                ; rsp before
    push 0x1111
    mov r13, rsp                ; rsp after the push
    mov rbx, [rsp]
    pop rax
    mov r14, rsp                ; rsp after the pop
    live_hex m6_p1_a, r12
    live_hex m6_p1_b, r13
    sub r12, r13
    live_dec m6_p1_c, r12
    live_hex m6_p1_d, rbx
    live_hex m6_p1_e, r14
    mov rdi, m6_p1_f
    call print_cstr
    part_end

    part_begin m6_p2_hdr, m6_p2
    mov r12, rsp                ; rsp as it is just before the call
    call m6_probe
.after_call:
    mov r13, rdx                ; rsp inside the callee
    mov rbx, rax
    live_hexw m6_p2_a, rbx, 6
    live_hexw m6_p2_b, .after_call, 6
    and r12, 15
    and r13, 15
    live_dec m6_p2_c, r12
    live_dec m6_p2_d, r13
    mov rdi, m6_p2_e
    call print_cstr
    part_end

    part_begin m6_p3_hdr, m6_p3
    mov edi, 1
    mov esi, 2
    mov edx, 3
    mov ecx, 4
    mov r8d, 5
    mov r9d, 6
    sub rsp, 8                  ; keep rsp a multiple of 16 at the call
    push 7                      ; the 7th argument
    call m6_sum7
    add rsp, 16                 ; the caller removes what it pushed
    live_dec m6_p3_a, rax
    mov rdi, m6_p3_b
    call print_cstr
    part_end

    part_begin m6_p4_hdr, m6_p4
    mov rbx, 0x1111
    call m6_good
    live_hex m6_p4_a, rbx
    mov rbx, 0x1111
    call m6_bad
    live_hex m6_p4_b, rbx
    mov rdi, m6_p4_c
    call print_cstr
    mov rcx, 0x1234
    mov rax, 1                  ; write
    mov rdi, 1
    mov rsi, m6_p4_f
    mov rdx, 0                  ; ...of zero bytes
    syscall
.after_syscall:
    mov r12, rcx
    live_hexw m6_p4_d, r12, 6
    live_hexw m6_p4_e, .after_syscall, 6
    mov rdi, m6_p4_f
    call print_cstr
    part_end

    part_begin m6_p5_hdr, m6_p5
    mov rbx, rsp
    mov rdi, 5
    call m6_fact
    mov r12, rax
    sub rbx, [m6_deepest]       ; how far below the start rsp went
    mov rax, 97                 ; getrlimit(RLIMIT_STACK, &m6_rlimit)
    mov rdi, 3
    mov rsi, m6_rlimit
    syscall
    live_dec m6_p5_a, r12
    live_dec m6_p5_b, rbx
    live_dec m6_p5_c, [m6_rlimit]
    mov rdi, m6_p5_d
    call print_cstr
    part_end

    ; exercise(6)
    mov rdi, m6_exercise
    call print_cstr
    quiz m6_q1_text, m6_q1_answer, m6_q1_why
    quiz m6_q2_text, m6_q2_answer, m6_q2_why
    quiz m6_q3_text, m6_q3_answer, m6_q3_why
    challenge m6_challenge, m6_solution

    ; summary(6)
    summary m6_summary_hdr, m6_summary
    add rsp, 8
    pop r14
    pop r13
    pop r12
    pop rbx
    ret

; Tells the linker this object does not need an executable stack.
section .note.GNU-stack noalloc noexec nowrite progbits
