; lessons_c.asm - module 9: assembly and C together.
;
; The first ADVANCED module, and the first whose examples are files outside
; this one: examples/09/*. The lesson prints those real files (incbin), so
; what is on screen is what was built and run, and check-course.py builds
; them again and compares their output with the text below.

%include "course.inc"

default rel

global lesson_09_c

section .rodata

m9_title:   db "MODULE 9 - ASSEMBLY AND C TOGETHER", 0
m9_p1_hdr:  db "PART 1: the contract, and an assembly function called from C", 0
m9_p1b_hdr: db "PART 1 (continued): examples/09/max.asm", 0
m9_p1c_hdr: db "PART 1 (continued): examples/09/main.c", 0
m9_p2_hdr:  db "PART 2: assembly calling C", 0
m9_p2b_hdr: db "PART 2 (continued): examples/09/hello.asm", 0
m9_p3_hdr:  db "PART 3: C with no libc, and an assembly front door", 0
m9_p3b_hdr: db "PART 3 (continued): examples/09/start.asm", 0
m9_p3c_hdr: db "PART 3 (continued): examples/09/free.c", 0
m9_p4_hdr:  db "PART 4: what the compiler adds on its own", 0
m9_p4b_hdr: db "PART 4 (continued): the red zone", 0
m9_p5_hdr:  db "PART 5: layout is part of the contract", 0

m9_p1:
    ln "  C and assembly meet in exactly one place: the System V AMD64 calling"
    ln "  convention, which modules 1 and 6 have been using all along. Both"
    ln "  sides keep the same promises:"
    ln ""
    ln "    arguments    rdi rsi rdx rcx r8 r9, then the stack"
    ln "                 (floating point goes in xmm0-xmm7; not covered here)"
    ln "    result       rax"
    ln "    kept intact  rbx rbp r12-r15, and the direction flag clear"
    ln "    at a call    rsp is a multiple of 16"
    ln "    names        on Linux a C function f is the symbol f"
    ln ""
    ln "  So an assembly function can be called from C if it is global, reads"
    ln "  its arguments from those registers, and leaves its answer in eax or"
    ln "  rax. int is 32 bits, so an int argument is in edi, not rdi: the top"
    ln "  half of rdi is not defined to hold anything."
    ln ""
    ln "  The next two screens are a function and the C that calls it, the"
    ln "  real files, as built and run while this module was written."
    db 0

m9_p1b:
    ln ""
    ln "  global makes the label visible to the linker; that is all it takes"
    ln "  for gcc's output to find it."
    db 0

m9_p1c:
    ln ""
    ln "    $ nasm -f elf64 max.asm -o max.o"
    ln "    $ gcc main.c max.o -o demo"
    ln "    $ ./demo"
    ln "    asm_max(3, 9)   = 9"
    ln "    asm_max(-5, -2) = -2"
    ln ""
    ln "  The declaration int asm_max(int, int); in the C file is the only"
    ln "  description gcc has of the function. If it said long, or if it"
    ln "  left out an argument, gcc would call it that way and the assembly"
    ln "  would neither know nor complain."
    db 0

m9_p2:
    ln "  The other direction: assembly calls a C library function. printf"
    ln "  shows every rule at once, because it is variadic:"
    ln ""
    ln "    - rsp must be a multiple of 16 at the call. Inside main the"
    ln "      return address makes it 8 off, so sub rsp, 8 first."
    ln "    - a variadic function reads al as the number of vector registers"
    ln "      used for arguments. xor eax, eax says none."
    ln "    - gcc builds position-independent executables by default, so the"
    ln "      call goes through the procedure linkage table: call printf wrt"
    ln "      ..plt, and data is addressed relative to rip (default rel)."
    ln ""
    ln "  Leave out the last one and the link fails. This is the real output"
    ln "  for a printf call written the plain way, with absolute addresses:"
    ln ""
    ln "    relocation R_X86_64_PC32 against symbol `printf@@GLIBC_2.2.5'"
    ln "    can not be used when making a PIE object; recompile with -fPIE"
    ln ""
    ln "  Two fixes. Write it position-independent, as the next screen does,"
    ln "  or ask gcc not to build a PIE: gcc -no-pie hello.o -o hello, which"
    ln "  was verified to link and print the same line."
    db 0

m9_p2b:
    ln ""
    ln "    $ ./hello"
    ln "    the answer is 42"
    ln ""
    ln "  main is a plain label the C runtime calls; it still does the work"
    ln "  before and after."
    db 0

m9_p3:
    ln "  An operating system has no C runtime and no libc: nothing has run"
    ln "  before the first instruction. C can still be used, with three"
    ln "  changes to how it is built:"
    ln ""
    ln "    -ffreestanding   do not assume the standard library exists"
    ln "    -nostdlib        do not link it"
    ln "    -static -no-pie  one fixed address, no loader to fix things up"
    ln ""
    ln "  And someone has to provide what the runtime did: a first"
    ln "  instruction. That is a few lines of assembly that set the stack,"
    ln "  call a C function, and deal with what comes back. Here the process"
    ln "  is a Linux program, so 'dealing with it' is the exit system call;"
    ln "  in module 15 the same shape boots a kernel."
    ln ""
    ln "  Built and run for this module (the files follow):"
    ln ""
    ln "    $ nasm -f elf64 start.asm -o start.o"
    ln "    $ gcc -ffreestanding -nostdlib -static -fno-pie -no-pie -O2 \"
    ln "          -c free.c -o free.o"
    ln "    $ ld start.o free.o -o free"
    ln "    $ ./free; echo $?"
    ln "    hello from C, with no libc"
    ln "    7"
    db 0

m9_p3b:
    ln ""
    ln "  and rsp, -16 forces the alignment instead of assuming it: the"
    ln "  kernel starts a process with a good rsp, but an OS entry point"
    ln "  can not assume what its loader left."
    db 0

m9_p3c:
    ln ""
    ln "  The system call is inline assembly, the same instruction as in"
    ln "  module 1, with the registers named in the constraints. The"
    ln "  finished program is 9128 bytes."
    db 0

m9_p4:
    ln "  A freestanding compiler still writes some code you did not ask for,"
    ln "  and two cases bite an operating system. The first: it calls"
    ln "  functions you have not got. A struct copy this large:"
    ln ""
    ln "    struct big { char b[65536]; };"
    ln "    void copy(struct big *d, const struct big *s) { *d = *s; }"
    ln ""
    ln "  comes out of gcc -O2 -ffreestanding -fno-pie -S -masm=intel as"
    ln ""
    ln "    copy:"
    ln "        sub     rsp, 8"
    ln "        mov     edx, 65536"
    ln "        call    memcpy"
    ln "        add     rsp, 8"
    ln "        ret"
    ln ""
    ln "  and ld, with no libc to find memcpy in, says:"
    ln ""
    ln "    undefined reference to `memcpy'"
    ln ""
    ln "  A 4096-byte copy was written inline as rep movsq, module 8's own"
    ln "  instruction; where the line falls depends on the gcc version and"
    ln "  flags. Whatever your code looks like, a kernel has to supply"
    ln "  memcpy, memset and memcmp itself."
    db 0

m9_p4b:
    ln "  The second case: the red zone. The ABI lets a leaf function use the"
    ln "  128 bytes below rsp without moving rsp. Real gcc -O2 output (shortened"
    ln "  to the interesting lines) for a function with two locals:"
    ln ""
    ln "    default                           -mno-red-zone"
    ln "    mov DWORD PTR -8[rsp], edi        sub rsp, 16"
    ln "    add edi, edi                      mov DWORD PTR 8[rsp], edi"
    ln "    mov DWORD PTR -4[rsp], edi        add edi, edi"
    ln "    ...                               mov DWORD PTR 12[rsp], edi"
    ln "                                      ...  add rsp, 16"
    ln ""
    ln "  The default version keeps its locals at rsp-8 and rsp-4, below the"
    ln "  stack pointer. A user program is safe: the ABI forbids signal and"
    ln "  interrupt handlers from touching that area. Your own interrupt handler"
    ln "  runs on the same stack and does not honour it: the CPU pushes"
    ln "  straight through those locals, and the function wakes up to find its"
    ln "  variables overwritten. It is the kind of bug that appears once an"
    ln "  hour. Kernel code is built with -mno-red-zone, so locals live above"
    ln "  rsp where an interrupt cannot reach them (module 16)."
    db 0

m9_p5:
    ln "  The hardware defines layouts, and C struct layout is its own thing."
    ln "  lgdt (module 13) is given the address of 10 bytes: a 2-byte limit,"
    ln "  immediately followed by an 8-byte base. Two ways to declare it, and"
    ln "  the sizes gcc really gave, from a program run for this module:"
    ln ""
    ln "    struct gdtr        { uint16_t limit; uint64_t base; };"
    ln "    struct gdtr_packed { uint16_t limit; uint64_t base; }"
    ln "                                         __attribute__((packed));"
    ln ""
    ln "    sizeof(struct gdtr)        = 16, base at offset 8"
    ln "    sizeof(struct gdtr_packed) = 10, base at offset 2"
    ln ""
    ln "  The plain struct pads six bytes after limit so base is 8-aligned."
    ln "  Hand lgdt that and it reads the base from the wrong place: no"
    ln "  error, a GDT somewhere random. When hardware fixes the layout,"
    ln "  declare the struct packed and check its size where the compiler"
    ln "  can see it:"
    ln ""
    ln '    _Static_assert(sizeof(struct gdtr_packed) == 10, "lgdt wants 10");'
    ln ""
    ln "  The same rule covers every table in the next modules: descriptors,"
    ln "  page-table entries, interrupt gates. And memory-mapped hardware,"
    ln "  like the screen at 0xB8000, is accessed through a volatile pointer,"
    ln "  or the optimizer is free to drop stores nothing seems to read."
    db 0

m9_summary_hdr: db "SUMMARY", 0
m9_summary:
    ln "   - the System V convention is the whole interface: registers in,"
    ln "     rax out, rbx/rbp/r12-r15 kept, rsp a multiple of 16 at a call"
    ln "   - asm called from C needs global and a matching declaration;"
    ln "     C called from asm needs alignment, al set for varargs, and"
    ln "     wrt ..plt (or -no-pie) under gcc's default PIE"
    ln "   - freestanding C needs an assembly entry point and supplies its own"
    ln "     memcpy/memset: the compiler calls them without being asked"
    ln "   - kernel code is compiled with -mno-red-zone because interrupts use"
    ln "     the stack below rsp"
    ln "   - hardware layouts need packed structs and a size assert; screen"
    ln "     memory needs volatile"
    db 0

m9_exercise:
    db `\n  >> EXERCISE - MODULE 9\n  ------------------------------------------------------\n`, 0
m9_q1_text:   db `At the moment of a call, rsp must be a multiple of what?\n  (a number)`, 0
m9_q1_answer: db "16", 0
m9_q1_why:    db `The ABI requires 16-byte alignment at the call, so inside the\n             callee it is 8 more, because call pushed the return address.`, 0
m9_q2_text:   db `struct { uint16_t limit; uint64_t base; } with no packing: how\n  many bytes does gcc make it? (a number)`, 0
m9_q2_answer: db "16", 0
m9_q2_why:    db `base needs 8-byte alignment, so six padding bytes follow limit.\n             lgdt wants 10 bytes with base at offset 2: packed.`, 0
m9_q3_text:   db `Which gcc flag stops it using the 128 bytes below rsp for\n  locals? (the whole flag)`, 0
m9_q3_answer: db "-mno-red-zone", 0
m9_q3_why:    db `An interrupt pushes onto the stack right through the red zone,\n             so kernel code must not keep anything there.`, 0

m9_challenge:
    ln ""
    ln "  >> WRITE THIS YOURSELF, in a real file"
    ln ""
    ln "  Write int asm_clamp(int x, int lo, int hi) in NASM: it returns lo"
    ln "  if x is below lo, hi if x is above hi, and x otherwise. This is"
    ln "  the C that calls it (save it as clamp_main.c):"
    ln ""
    incbin "09/clamp_main.c"
    ln ""
    ln "  Build and run:"
    ln "    nasm -f elf64 prog.asm -o prog.o && gcc clamp_main.c prog.o -o prog"
    ln "    ./prog"
    ln ""
    ln "  It must print:"
    ln ""
    ln "      5"
    ln "      0"
    ln "      10"
    db 0

m9_solution:
    incbin "09.asm"
    db 0

m9_max_asm:
    incbin "09/max.asm"
    db 0
m9_main_c:
    incbin "09/main.c"
    db 0
m9_hello_asm:
    incbin "09/hello.asm"
    db 0
m9_start_asm:
    incbin "09/start.asm"
    db 0
m9_free_c:
    incbin "09/free.c"
    db 0

section .text

lesson_09_c:
    mov rdi, m9_title
    call title

    part m9_p1_hdr, m9_p1
    file_screen m9_p1b_hdr, m9_max_asm, m9_p1b
    file_screen m9_p1c_hdr, m9_main_c, m9_p1c
    part m9_p2_hdr, m9_p2
    file_screen m9_p2b_hdr, m9_hello_asm, m9_p2b
    part m9_p3_hdr, m9_p3
    file_screen m9_p3b_hdr, m9_start_asm, m9_p3b
    file_screen m9_p3c_hdr, m9_free_c, m9_p3c
    part m9_p4_hdr, m9_p4
    part m9_p4b_hdr, m9_p4b
    part m9_p5_hdr, m9_p5

    ; exercise(9)
    mov rdi, m9_exercise
    call print_cstr
    quiz m9_q1_text, m9_q1_answer, m9_q1_why
    quiz m9_q2_text, m9_q2_answer, m9_q2_why
    quiz m9_q3_text, m9_q3_answer, m9_q3_why
    challenge m9_challenge, m9_solution

    ; summary(9)
    summary m9_summary_hdr, m9_summary
    ret

; Tells the linker this object does not need an executable stack.
section .note.GNU-stack noalloc noexec nowrite progbits
